#ifndef MYMALLOC_H
#define MYMALLOC_H

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include "chunksconfig.h"

extern void* heap_head;
extern size_t heap_size;
extern size_t heap_max_size;

extern pthread_mutex_t lock;

void mymallocinit(void);
void* requestMemory(size_t size);
void* mymalloc(size_t size);
void myfree(void* ptr);

#endif