#include "mymalloc.h"

void* heap_head = NULL;
size_t heap_size = 0;
size_t heap_max_size = 0;

//Function thar initializes the heap
void mymallocinit(void) {

    size_t size = 1024;

    heap_head = sbrk(size);
    heap_max_size += size;

}

//Function that request the OS for more memory in case of heap exhaustion
void* requestMemory(size_t size) {

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

    return ptr;
}

//Function that allocet memory
void* mymalloc(size_t size) {
    if (size == 0) return NULL; // Handle zero allocation request

    size = (size + sizeof(void*) - 1) & ~(sizeof(void*) - 1); // Align size

    Chunk* free_chunk = find_free_chunk(size);

    if (free_chunk != NULL) {
        free_chunk->free = false;
        void* ptr = free_chunk->init;
        return ptr;
    }

    if (heap_size + size <= heap_max_size) {
        void* ptr = (char*)heap_head + heap_size; // Correctly calculate pointer
        create_new_chunk(ptr, size);
        heap_size += size; // Update heap size
        return ptr;
    } else {
        void* ptr = requestMemory(size);
        if (ptr == NULL) {
            return NULL; // Handle allocation failure
        }
        create_new_chunk(ptr, size);
        return ptr;
    }
}

//Function that free memory
void myfree(void* ptr) {

    Chunk* chunk = chunk_list_head;

    while (chunk != NULL) {
        if (chunk->init == ptr) {
            chunk->free = true;
            fuse_free_chunks(chunk);

            return;
        }
        chunk = chunk->next;
    }

    return;
}
