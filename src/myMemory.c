#include "../includes/myMemory.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
mymemory_t* mymemory_init(size_t size){
  mymemory_t *memory = (mymemory_t*)malloc(sizeof(mymemory_t));

  if(!memory) return NULL;

  memory->total_size = size;
  memory->head = NULL;
  memory->pool = malloc(size);

  if(!memory->pool) {
    free(memory);
    return NULL;
  }

  return memory;

}


void* mymemory_alloc(mymemory_t *memory, size_t size){
  printf("Novo Bloco\n");
  allocation_t *current = memory->head;
  allocation_t *prev= NULL;

  char *endBlock = (char*)memory->pool;
  void *startNewAlloc = NULL;
  // #ifdef DEBUG
  //   printf("current Allocated memory at : %p\n with Fisrt fit", current);
  //   printf("endOfBlock Allocated memory at: %p\n", endOfBlock);
  // #endif // DEBUG

  while(current != NULL){
    ptrdiff_t diff = (char*)current->start - endBlock;
    
    if(diff >= (ptrdiff_t)size){

      #ifdef DEBUG
        printf("Found free block at: %p\n", current->start);
      #endif // DEBUG

      startNewAlloc = (void*)endBlock;
      break;
    }
    prev = current;
    endBlock = endBlock + current->size;
    current = current->next;
    printf("foi uma \n");
  }

  if(startNewAlloc == NULL){
    if(prev == NULL)
      startNewAlloc = memory->pool;
    else{
      ptrdiff_t diff = ((char*)memory->pool + memory->total_size) - (char*)prev->start;
      if(diff >= (ptrdiff_t)size){
        #ifdef DEBUG
          printf("Found free block at: %p\n", current->start);
        #endif // DEBUG

        startNewAlloc = (void*)endBlock;
      }
    }
  }

  printf("startNewAlloc %p \n", startNewAlloc);
  #ifdef DEBUG
    printf("Allocated memory at: %p\n", startNewAlloc);
  #endif // DEBUG

  if(startNewAlloc == NULL) return NULL;

  allocation_t *newNodo = malloc(sizeof(allocation_t));
  newNodo->size = size;
  newNodo->start = startNewAlloc;
  newNodo->next = NULL;

  if (memory->head == NULL) {
      memory->head = newNodo;
  } else {

    if(prev == NULL) {
      newNodo->next = memory->head;
      memory->head = newNodo;
    }else{
      newNodo->next = prev->next;
      prev->next = newNodo;
    }
  }
  return startNewAlloc;
  
}

void mymemory_free(mymemory_t *mymemory, void *ptr){
  if(ptr == NULL || mymemory->head == NULL) return;

  if(ptr == mymemory->head->start){
    printf("Freeing memory at: %p\n", ptr);
    allocation_t *aux = mymemory->head;
    mymemory->head = mymemory->head->next;
    free(aux);
    return;
  }

  allocation_t *current = mymemory->head;

  while(current != NULL){
    if(current->next != NULL && current->next->start == ptr){
      allocation_t *aux = current->next;
      current->next = current->next->next ;
      free(aux);
      return;
    }
    current = current->next;
  }
  return;
}

void mymemory_display(mymemory_t *memory){
  if(memory->head == NULL){
    printf("Nenhum bloco alocado\n");
    return;
  }
  allocation_t *current = memory->head;
  int index = 1;
  while (current != NULL){
    printf("Bloco %d: Inicio: %p, Tamanho: %zu bytes\n", index++, current->start, current->size);
    current = current->next;
  }
}

void mymemory_stats(mymemory_t *memory){
  if(memory->head == NULL){
    printf("Nenhum bloco alocado\n");
    return;
  }
  allocation_t *current = memory->head;
  char *endOfBlock = (char*)memory->pool;
  size_t biggestBlockFreeMemory = 0;
  size_t totaUsedMemory = 0;

  int countFragmetsBlocks = 0;
  int totalAllocated = 0;

  while(current!= NULL){
    size_t freeMemory = (char*)current->start - endOfBlock;
    if(freeMemory > 0){
      // printf("Bloco livre: Inicio: %p, Tamanho: %zu bytes\n", endOfBlock, freeMemory);
      countFragmetsBlocks++;
      if(freeMemory > biggestBlockFreeMemory) biggestBlockFreeMemory = freeMemory;
    }
    totalAllocated++;
    totaUsedMemory += current->size;
    endOfBlock += current->size;
    current = current->next;
    // printf("endOfBlock: %p\n", endOfBlock);
    // printf("nextttt: %p\n", current);
  }

    size_t freeMemory = (char*)memory->pool + memory->total_size - endOfBlock;
    if (freeMemory > 0) {
        // if(freeMemory > biggestBlockFreeMemory) biggestBlockFreeMemory = freeMemory;
    }
    printf("\nEstatísticas:\n");
    printf("Total de alocações: %d\n", totalAllocated);
    printf("Memória total alocada: %zu bytes\n", totaUsedMemory);
    printf("Memória total livre: %zu bytes\n", memory->total_size - totaUsedMemory);
    printf("Maior bloco contíguo de memória livre: %zu bytes\n", biggestBlockFreeMemory);
    printf("Número de fragmentos de memória livre: %d\n", countFragmetsBlocks);
    perror("Fim do programa\n\n");
}

void mymemory_cleanup(mymemory_t *memory){
    allocation_t *current = memory->head;
    while (current) {
        allocation_t *to_free = current;
        current = current->next;
        free(to_free);
    }
    free(memory->pool);
    free(memory);
}