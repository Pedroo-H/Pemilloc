#include "chunksconfig.h"


//Function that creates a chunk
void create_new_chunk(void *ptr, size_t size) {
    Chunk* chunk = (Chunk*)sbrk(sizeof(Chunk));

    if (chunk == (void*) -1) {
        return;
    }

    chunk->init = ptr;
    chunk->next = NULL;
    chunk->last = chunk_list_tail;
    chunk->size = size;
    chunk->free = false;

    if (chunk_list_head == NULL) {
        chunk_list_head = chunk;
    } else {
        chunk_list_tail->next = chunk;
    }

    chunk_list_tail = chunk;
}


//Function that divides a chunk if necessary
void divide_chunk(Chunk* chunk, size_t size) {
    Chunk* new_chunk = (Chunk*)sbrk(sizeof(Chunk));

    if (new_chunk == (void*) -1) {
        return;
    }

    new_chunk->init = (void*)((char*)chunk->init + size);
    new_chunk->size = chunk->size - size;
    new_chunk->last = chunk;
    new_chunk->next = chunk->next;
    new_chunk->free = true;

    chunk->size = size;
    chunk->next = new_chunk;
}


//Function that finds free chunk that fits in the requisits
Chunk* find_free_chunk(size_t size) {
    Chunk* chunk = chunk_list_head;
    Chunk* free_chunk = NULL;

    while (chunk != NULL) {
        if (chunk->free && chunk->size == size) {
            free_chunk = chunk;
            break;
        } else if (chunk->free && chunk->size > size) {
            divide_chunk(chunk, size);
            free_chunk = chunk;
            break;
        }
        chunk = chunk->next;
    }

    return free_chunk;
}


//Function that fuses one or more free chunks in sequence
void fuse_free_chunks(void) {
    Chunk* chunk = chunk_list_head;

    while (chunk != NULL) {
        Chunk* next_chunk = chunk->next;
        while (next_chunk != NULL && next_chunk->free) {
            chunk->size += next_chunk->size;
            chunk->next = next_chunk->next;
            next_chunk = next_chunk->next;
        }
        chunk = chunk->next;
    }
}
