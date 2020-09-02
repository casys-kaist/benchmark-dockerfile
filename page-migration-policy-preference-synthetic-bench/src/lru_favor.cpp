/*
 * lru_favor.cpp
 *
 *  Created on: Dec. 18, 2019
 *      Author: Taekyung Heo <tkheo@casys.kaist.ac.kr>
 */

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <random>
#include <chrono>
#include <thread>
#include <functional>
#include <vector>

#include "cxxopts.hpp"

using namespace std;

int num_working_set_partitions = 0;
int working_set_idx = 0;
vector<uniform_int_distribution<>> dist_vec;

void switch_working_set_periodically(int working_set_access_duration)
{
    std::thread([working_set_access_duration]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(
                        working_set_access_duration*1000));
            working_set_idx++;
            working_set_idx %= num_working_set_partitions;
        }
    }).detach();
}

int main(int argc, char* argv[])
{
    int num_pages, page_size;
    int working_set_access_duration;
    unsigned long long num_iterations;

    // parse options
    cxxopts::Options options("lru-favor", "lru-favor synthetic benchmark");
    options.add_options()
        ("num_pages", "number of pages to allocate", cxxopts::value<int>()
         ->default_value("51200"))
        ("num_working_set_partitions", "number of working set partitions", cxxopts::value<int>()
         ->default_value("4"))
        ("working_set_access_duration", "working set access duration in seconds", cxxopts::value<int>()
         ->default_value("20"))
        ("num_iterations", "number of iterations", cxxopts::value<unsigned long long>()
         ->default_value("100000"))
        ;
    auto parsed_opts = options.parse(argc, argv);
    num_pages = parsed_opts["num_pages"].as<int>();
    page_size = getpagesize();
    num_working_set_partitions = parsed_opts["num_working_set_partitions"].as<int>();
    working_set_access_duration = parsed_opts["working_set_access_duration"].as<int>();
    num_iterations = parsed_opts["num_iterations"].as<unsigned long long>();

    // create random number generators
    random_device rd;
    mt19937 gen(rd());
    int num_pages_in_partition = num_pages / num_working_set_partitions;
    for (int i = 0; i < num_working_set_partitions; i++) {
        int begin_idx, end_idx;
        begin_idx = i * num_pages_in_partition;
        end_idx = (i+1) * num_pages_in_partition - 1;
        uniform_int_distribution<> rand_dist(begin_idx, end_idx);
        dist_vec.push_back(rand_dist);
    }

    // allocate pages
    int *pages[num_pages];
    for (int i = 0; i < num_pages; i++) {
        pages[i] = (int *)mmap(NULL, page_size, PROT_READ | PROT_WRITE,
            MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (pages[i] == MAP_FAILED)
            exit(EXIT_FAILURE);
        memset(pages[i], 0, page_size);
    }

    // access pages
    switch_working_set_periodically(working_set_access_duration);
    for (unsigned long long i = 0; i < num_iterations; i++) {
        uniform_int_distribution<> dist = dist_vec[working_set_idx];
        memset(pages[dist(gen)], 0, page_size);
    }

    // deallocate pages
    for (int i = 0; i < num_pages; i++)
        munmap(pages[i], page_size);

    return 0;
}
