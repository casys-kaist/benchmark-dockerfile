/*
 * random_favor.cpp
 *
 *  Created on: Dec. 18, 2019
 *      Author: Taekyung Heo <tkheo@casys.kaist.ac.kr>
 */

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <random>

#include "cxxopts.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    int num_pages, page_size;
    unsigned long long num_iterations;

    // parse options
    cxxopts::Options options("random-favor", "random-favor synthetic benchmark");
    options.add_options()
        ("num_pages", "number of pages to allocate", cxxopts::value<int>()
         ->default_value("51200"))
        ("num_iterations", "number of iterations", cxxopts::value<unsigned long long>()
         ->default_value("100000"))
        ;
    auto parsed_opts = options.parse(argc, argv);
    num_pages = parsed_opts["num_pages"].as<int>();
    num_iterations = parsed_opts["num_iterations"].as<unsigned long long>();
    page_size = getpagesize();

    // random number generator
    random_device rd;  //Will be used to obtain a seed for the random number engine
    mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    uniform_int_distribution<> dist(0, num_pages-1);

    // access pages based on the state
    int *pages[num_pages];
    for (int i = 0; i < num_pages; i++) {
        pages[i] = (int *)mmap(NULL, page_size, PROT_READ | PROT_WRITE,
                MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (pages[i] == MAP_FAILED)
            exit(EXIT_FAILURE);
        memset(pages[i], 0, page_size);
    }

    // access random pages
    for (unsigned long long i = 0; i < num_iterations; i++)
        memset(pages[dist(gen)], 0, page_size);

    // deallocate pages
    for (int i = 0; i < num_pages; i++)
        munmap(pages[i], page_size);

    return 0;
}
