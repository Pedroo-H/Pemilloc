#ifndef TESTHEADER_H
#define TESTHEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mymalloc.h>

#define NUM_POINTERS 1 << 15
#define NUM_CYCLES 1 << 15
#define MAX_BLOCK_SIZE 1 << 12

typedef void* malloc_func(size_t size);
typedef void* free_func(void* ptr);

float benchmark(malloc_func* calloc, free_func* realloc);

#endif