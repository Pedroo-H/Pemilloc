#ifndef CHUNKSCONFIG_H
#define CHUNKSCONFIG_H

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

Chunk* chunk_list_head = NULL;
Chunk* chunk_list_tail = NULL;

typedef struct Chunk {
    void* init;
    struct Chunk* next;
    struct Chunk* last;
    size_t size;
    bool free;
} Chunk;

void create_new_chunk(void *ptr, size_t size);
void divide_chunk(Chunk* chunk, size_t size);
Chunk* find_free_chunk(size_t size);
void fuse_free_chunks(void);

#endif