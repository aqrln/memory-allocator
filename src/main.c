#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "allocator.h"
#include "block_header.h"
#include "colors.h"

#define HEAP_SIZE (100 * 1024 * 1024)
#define OPERATIONS_COUNT 10000

typedef enum {
  ALLOC, FREE, REALLOC
} operation_t;

typedef struct {
  void *addr;
  size_t size;
} alloc_stack_entry_t;

alloc_stack_entry_t alloc_stack[OPERATIONS_COUNT];

size_t alloc_stack_pointer = 0;

void alloc_stack_push(void *addr, size_t size) {
  alloc_stack[alloc_stack_pointer].addr = addr;
  alloc_stack[alloc_stack_pointer].size = size;
  alloc_stack_pointer++;
}

alloc_stack_entry_t alloc_stack_pop(void) {
  return alloc_stack[--alloc_stack_pointer];
}

int main() {
  printf("Initialize heap\n");
  mem_init(HEAP_SIZE);

  srand(time(NULL));

  size_t total_allocs = 0;
  size_t total_reallocs = 0;
  size_t total_frees = 0;

  void *addr;
  size_t size;
  size_t new_size;
  size_t min_size;
 
  for (int i = 0; i < OPERATIONS_COUNT; i++) {
    operation_t operation = rand() % 3;
    switch (operation) {
      case ALLOC:
        size = rand() % (1024 * 1024 * 2);
        addr = mem_alloc(size);
        if (addr != NULL) {
          alloc_stack_push(addr, size);
          total_allocs++;
        } else {
          printf(YELLOW("Couldn't alloc — not enough memory\n"));
        }
        break;

      case FREE:
        if (alloc_stack_pointer < 1) {
          continue;
        }
        alloc_stack_entry_t block_info = alloc_stack_pop();
        mem_free(block_info.addr);
        total_frees++;
        break;

      case REALLOC:
        if (alloc_stack_pointer < 1) {
          continue;
        }

        block_info = alloc_stack_pop();
        addr = block_info.addr;
        size = block_info.size;

        new_size = rand() % (1024 * 1024 * 2);
        min_size = new_size < size ? new_size : size;

        for (size_t j = 0; j < min_size / sizeof(size_t); j++) {
          ((size_t *) addr)[j] = j;
        }

        addr = mem_realloc(addr, new_size);
        if (addr == NULL) {
          printf(YELLOW("Couldn't realloc — not enough memory\n"));
          continue;
        }

        bool corrupt = false;
        for (size_t j = 0; j < min_size / sizeof(size_t); j++) {
          if (((size_t *) addr)[j] != j) {
            corrupt = true;
            //printf("%ld instead of %ld\n", ((size_t *) addr)[j], j);
          }
        }

        if (corrupt) {
          printf(RED("Corrupt data\n"));
        }

        alloc_stack_push(addr, new_size);
        
        total_reallocs++;
        break;
    }
  }

  printf("Total allocs: %ld\nTotal frees: %ld\nTotal reallocs: %ld\n",
      total_allocs, total_frees, total_reallocs);

  printf("Destroy heap\n");
  mem_destroy();
  return 0;
}
