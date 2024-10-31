#include "mymalloc.h"

void* heap_head = NULL;
size_t heap_size = 0;
size_t heap_max_size = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // lock to handle concurrency. 

//Function thar initializes the heap
void mymallocinit(void) {
    pthread_mutex_lock(&lock);

    size_t size = 1024;

    heap_head = sbrk(size);
    heap_max_size += size;

    pthread_mutex_unlock(&lock);
}

//Function that request the OS for more memory in case of heap exhaustion
void* requestMemory(size_t size) {
    pthread_mutex_lock(&lock);

    void* link_chunk_ptr = heap_head + heap_size;
    size_t link_chunk_size = heap_max_size - heap_size;
    Chunk* link_chunk = create_new_chunk(link_chunk_ptr, link_chunk_size);
    link_chunk->free = true;
 
    void* ptr = sbrk(1024 - size);
    if (ptr == (void*) -1) {
        return NULL; 
    }

    heap_max_size += 1024 - size;
    heap_size += size;

    link_chunk->next = ptr;
    pthread_mutex_unlock(&lock);

    return ptr;
}

//Function that allocet memory
void* mymalloc(size_t size) {
    pthread_mutex_lock(&lock);
    Chunk* free_chunk = find_free_chunk(size); // Corrigido para declarar free_chunk

    if (free_chunk == NULL) {    
        if (heap_size + size < heap_max_size) {
            void* ptr = (void*)((char*)heap_head + heap_size + size); // Corrigido o tipo
            create_new_chunk(ptr, size);
            pthread_mutex_unlock(&lock);

            return ptr;
        } else {
            void* ptr = requestMemory(size);
            create_new_chunk(ptr, size);

            pthread_mutex_unlock(&lock);

            return ptr;
        }
    } else {
        free_chunk->free = false;
        void* ptr = free_chunk->init; // Corrigido para declarar ptr
        pthread_mutex_unlock(&lock);

        return ptr;
    }
}

//Function that free memory
void myfree(void* ptr) {
    pthread_mutex_lock(&lock);

    Chunk* chunk = chunk_list_head;

    while (chunk != NULL) {
        if (chunk->init == ptr) {
            chunk->free = true;
            fuse_free_chunks();

            pthread_mutex_unlock(&lock);

            return;
        }
        chunk = chunk->next;
    }
    pthread_mutex_unlock(&lock);

    return;
}
