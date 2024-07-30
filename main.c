#include <stdio.h>
#include <stdlib.h>

#include "includes/myMemory.h"

void mymemory_visual_display(mymemory_t *memory) {
    if (memory == NULL || memory->pool == NULL) {
        printf("Memória não inicializada.\n");
        return;
    }

    char *pool_start = (char *)memory->pool;
    char *pool_end = pool_start + memory->total_size;
    allocation_t *current = memory->head;

    printf("Visualização da memória:\n");
    while (pool_start < pool_end) {
        if (current != NULL && pool_start == current->start) {
            // Imprime bloco alocado
            for (size_t i = 0; i < current->size; i++) {
                printf("#");
            }
            pool_start += current->size;
            current = current->next;
        } else {
            // Imprime espaço livre
            printf("-");
            pool_start++;
        }
    }
    printf("\nLegenda:\n");
    printf("#: Bloco alocado\n");
    printf("-: Espaço livre\n");
}

void fit_array(int *arr, int size){
    int i;
    for (i = 0; i < size; i++) {
        arr[i] = rand() % 100;
    }
}

void print_array(int *arr, int size){
    fit_array(arr, size);
    printf("[");
    for (int i = 0; i <size ; i++) {
        if (i != 0) {
            printf(",");
        }
        printf("%d", arr[i]);
    }
    printf("]\n");
}

void statisticsMemory(mymemory_t *memory) {
    // mymemory_visual_display(memory);
    mymemory_display(memory);
    mymemory_stats(memory);
}

int main()
{

    mymemory_t *m = mymemory_init(10000);

    printf("\n\nINFOS ABOUT MEMORY:\n");
    printf("STRUCT Memory alloc in %p\n", m);
    printf("inicio do pool: %p\n", m->pool);
    printf("pool size: %zu\n", m->total_size);
    printf("--------------------------------------------------\n\n");

    int *v = mymemory_alloc(m, sizeof(int)*100);
    for (int i = 0; i < 100; i++)
        v[i] = i + 1;
    printf("Memory1 allocated %p\n", v);
    printf("--------------------------------------------------\n\n");

    int *v1 = mymemory_alloc(m, sizeof(int)*100);
    for (int i = 0; i < 100; i++)
        v1[i] = i + 1;
    printf("Memory1 allocated %p\n", v1);
    mymemory_free(m, v1);
    printf("\n\nMemory free on %p\n", v1);
    printf("--------------------------------------------------\n\n");

    int *v2 = mymemory_alloc(m, sizeof(int)*100);
    for (int i = 0; i < 100; i++)
        v2[i] = i + 1;
    printf("Memory1 allocated %p\n", v2);
    printf("--------------------------------------------------\n\n");

    statisticsMemory(m);
    return 0;
}