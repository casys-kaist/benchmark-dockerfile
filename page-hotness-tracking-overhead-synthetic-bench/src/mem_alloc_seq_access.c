/*
 * mem_alloc_seq_access.c
 *
 *  Created on: September. 7, 2020
 *      Author: Taekyung Heo <tkheo@casys.kaist.ac.kr>
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    const unsigned long long MB = 1024ULL*1024ULL;
    const unsigned long long GB = 1024ULL*1024ULL*1024ULL;
    const unsigned long long align = 2ULL*MB;
    const unsigned long long page_size = 4096ULL;
    char *p;
    unsigned long long memory_size;
    void *base_ptr, *ptr;

    memory_size = strtoul(argv[1], &p, 10) * GB;

    base_ptr = ptr = aligned_alloc(align, memory_size);
    while (1) {
        memset(ptr, page_size, 1);
        ptr += page_size;
        if (ptr == (base_ptr + memory_size))
            ptr = base_ptr;
    }

    return 0;
}
