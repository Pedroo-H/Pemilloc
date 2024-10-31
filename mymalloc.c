#include "mymalloc.h"
#include <pthread.h>

void* heap_head = NULL;
size_t heap_size = 0;
size_t heap_max_size = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Function that initializes the heap
void mymallocinit(void) {
    pthread_mutex_lock(&lock);
    size_t size = 1024;
    heap_head = sbrk(size);
    heap_max_size += size;
    pthread_mutex_unlock(&lock);
}

// Function that requests the OS for more memory in case of heap exhaustion
void* requestMemory(size_t size) {
    void* ptr;
    
    pthread_mutex_lock(&lock);
    
    void* link_chunk_ptr = (char*)heap_head + heap_size;
    size_t link_chunk_size = heap_max_size - heap_size;
    Chunk* link_chunk = create_new_chunk(link_chunk_ptr, link_chunk_size);
    link_chunk->free = true;

    ptr = sbrk(1024 - size);
    if (ptr == (void*) -1) {
        pthread_mutex_unlock(&lock);
        return NULL; 
    }

    heap_max_size += 1024 - size;
    heap_size += size;

    link_chunk->next = ptr;
    pthread_mutex_unlock(&lock);

    return ptr;
}

// Function that allocates memory
void* mymalloc(size_t size) {
    if (size == 0) return NULL; 

    size = (size + sizeof(void*) - 1) & ~(sizeof(void*) - 1);
    
    Chunk* free_chunk;

    // Lock only when checking and modifying the shared state
    pthread_mutex_lock(&lock);
    free_chunk = find_free_chunk(size);
    
    if (free_chunk != NULL) {
        free_chunk->free = false;
        void* ptr = free_chunk->init;
        pthread_mutex_unlock(&lock);
        return ptr;
    }

    if (heap_size + size <= heap_max_size) {
        void* ptr = (char*)heap_head + heap_size;
        create_new_chunk(ptr, size);
        heap_size += size; 
        pthread_mutex_unlock(&lock);
        return ptr;
    }
    
    pthread_mutex_unlock(&lock);
    
    // Request memory outside the lock
    void* ptr = requestMemory(size);
    if (ptr != NULL) {
        // Lock again to update the chunk list
        pthread_mutex_lock(&lock);
        create_new_chunk(ptr, size);
        pthread_mutex_unlock(&lock);
    }

    return ptr;
}

// Function that frees memory
void myfree(void* ptr) {
    pthread_mutex_lock(&lock);
    
    Chunk* chunk = chunk_list_head;
    while (chunk != NULL) {
        if (chunk->init == ptr) {
            chunk->free = true;
            fuse_free_chunks(chunk);
            pthread_mutex_unlock(&lock);
            return;
        }
        chunk = chunk->next;
    }

    pthread_mutex_unlock(&lock);
}
