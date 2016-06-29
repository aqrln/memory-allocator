#include <stdio.h>

#include "allocator.h"

int main() {
  printf("Initialize heap\n");
  mem_init(100 * 1024 * 1024);

  printf("Allocate an array\n");
  int *array = mem_alloc(3 * sizeof(int));
  array[0] = 1;
  array[1] = 2;
  array[2] = 3;

  printf("%d\n%d\n%d\n", array[0], array[1], array[2]);

  printf("Free memory\n");
  mem_free(array);

  printf("Destroy heap\n");
  mem_destroy();
  return 0;
}
