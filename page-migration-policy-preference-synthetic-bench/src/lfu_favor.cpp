/*
 * lfu_favor.cpp
 *
 *  Created on: Dec. 18, 2019
 *      Author: Taekyung Heo <tkheo@casys.kaist.ac.kr>
 */

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <random>
#include <atomic>
#include <chrono>
#include <thread>

#include "cxxopts.hpp"

using namespace std;

atomic<bool> is_running(true);

int hot_idx = 0;
int cold_working_set_idx = 0;
int num_cold_working_set_partitions = 0;
int num_pages_in_partition = 0;

// scan cold pages to degrade the performance of non-LFU replacement policies
void cold_page_accessing_thread(int **pages, int page_size)
{
    random_device rd;
    mt19937 gen(rd());
    while (is_running.load()) {
        int begin_idx = cold_working_set_idx * num_pages_in_partition;
        int end_idx = (cold_working_set_idx+1) * num_pages_in_partition - 1;
        for (int i = begin_idx; i < end_idx; i++)
            memset(pages[i], 0, page_size);
        this_thread::sleep_for(chrono::milliseconds(2000));
        cold_working_set_idx++;
        cold_working_set_idx %= num_cold_working_set_partitions;
    }
}

int main(int argc, char* argv[])
{
    int num_pages, num_hot_pages, page_size;
    unsigned long long num_iterations;
    float hot_working_set_ratio;

    // parse options
    cxxopts::Options options("lfu-favor", "lfu-favor synthetic benchmark");
    options.add_options()
        ("num_pages", "number of pages to allocate", cxxopts::value<int>()
         ->default_value("51200"))
        ("hot_working_set_ratio", "ratio of hot working set", cxxopts::value<float>()
         ->default_value("0.5"))
        ("num_cold_working_set_partitions", "number of cold working set partitions", cxxopts::value<int>()
         ->default_value("16"))
        ("num_iterations", "number of iterations", cxxopts::value<unsigned long long>()
         ->default_value("100000"))
        ;
    auto parsed_opts = options.parse(argc, argv);
    num_pages = parsed_opts["num_pages"].as<int>();
    page_size = getpagesize();
    hot_working_set_ratio = parsed_opts["hot_working_set_ratio"].as<float>();
    num_hot_pages = num_pages * hot_working_set_ratio;
    num_cold_working_set_partitions = parsed_opts["num_cold_working_set_partitions"].as<int>();
    num_iterations = parsed_opts["num_iterations"].as<unsigned long long>();
    num_pages_in_partition = num_pages / num_cold_working_set_partitions;

    // access pages based on the state
    int *pages[num_pages];
    for (int i = 0; i < num_pages; i++) {
        pages[i] = (int *)mmap(NULL, page_size, PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (pages[i] == MAP_FAILED)
            exit(EXIT_FAILURE);
        memset(pages[i], 0, page_size);
    }

    // cold pages: 0~(num_cold_pages-1), hot pages: (num_cold_pages)~(num_pages-1)
    random_device rd;
    mt19937 gen(rd());
    thread t1(cold_page_accessing_thread, (int **)pages, page_size);
    uniform_int_distribution<> dist((num_pages-num_hot_pages), num_pages-1);
    for (unsigned long long i = 0; i < num_iterations; i++) {
        memset(pages[dist(gen)], 0, page_size);
    }
    is_running = false;
    t1.join();

    // deallocate pages
    for (int i = 0; i < num_pages; i++)
        munmap(pages[i], page_size);

    return 0;
}
