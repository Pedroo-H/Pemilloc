#include "tests.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mymalloc.h"

#define NUM_POINTERS 1 << 15
#define NUM_CYCLES 1 << 15
#define MAX_BLOCK_SIZE 1 << 12

static float benchmark(calloc_func* calloc, realloc_func* realloc, free_func* free);

typedef void* malloc_func(size_t size);
typedef void* free_func(void* ptr);

float benchmark(malloc_func* malloc, free_func* free) {
    void* ptrs[NUM_POINTERS];
    clock_t start = clock();

     for (unsigned short i = 0; i < NUM_CYCLES; i++) {
        for (unsigned short j = 0; j < NUM_POINTERS; j++) {
            pointers[j] = malloc(rand() % MAX_BLOCK_SIZE);
        }
        for (unsigned short j = 0; j < NUM_POINTERS; j++) {
            free(pointers[j]);
        }
    }

    printf("Operação realizada em %f segundos\n", (float) (clock() - start) / CLOCKS_PER_SEC);

    return (clock() - start) / CLOCKS_PER_SEC;
}

int main(void)
{
    printf("%s\n", "*** Fazendo benchmark das funções do GNU ***");
    time_malloc = benchmark(&malloc, &free);
    puts("");

    printf("%s\n", "*** Fazendo o benchmark das funções proprias ***");
    time_mymalloc = benchmark(&mymalloc, &myfree);

    float efficiency = (time_malloc / time_mymalloc) * 100;
    printf("A eficiência de mymalloc em relação a malloc é: %.2f%%\n", efficiency);

    return EXIT_SUCCESS;
}
