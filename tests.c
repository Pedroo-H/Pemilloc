#include "tests.h"

float benchmark(malloc_func* malloc, free_func* free) {
    void* ptrs[NUM_POINTERS];
    clock_t start = clock();

     for (unsigned short i = 0; i < NUM_CYCLES; i++) {
        for (unsigned short j = 0; j < NUM_POINTERS; j++) {
            ptrs[j] = malloc(rand() % MAX_BLOCK_SIZE);
        }
        for (unsigned short j = 0; j < NUM_POINTERS; j++) {
            free(ptrs[j]);
        }
    }

    printf("Operação realizada em %f segundos\n", (float) (clock() - start) / CLOCKS_PER_SEC);

    return (clock() - start) / CLOCKS_PER_SEC;
}

int main(void)
{
    printf("%s\n", "*** Fazendo benchmark das funções do GNU ***");
    float time_malloc = benchmark(&malloc, &free);
    puts("");

    printf("%s\n", "*** Fazendo o benchmark das funções proprias ***");
    float time_mymalloc = benchmark(&mymalloc, &mymalloc);

    float efficiency = (time_malloc / time_mymalloc) * 100;
    printf("A eficiência de mymalloc em relação a malloc é: %.2f%%\n", efficiency);

    return EXIT_SUCCESS;
}
