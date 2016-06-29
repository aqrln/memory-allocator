#include <stdlib.h>
#include <stdio.h>

#include "allocator.h"
#include "_allocator_internal.h"
#include "block_header.h"

block_header_t *first_block;
block_header_t *last_block;
size_t heap_size;

bool mem_init(size_t size) {
  first_block = (block_header_t *) calloc(size, 1);
  if (first_block == NULL) {
#ifdef DEBUG
    fprintf(stderr, "[mem_init] Not enough memory\n");
#endif
    return false;
  }

  heap_size = size;

  block_header_t *middle_block = first_block + 1;
  last_block = (block_header_t *) ((size_t) first_block +
      heap_size - sizeof(block_header_t));

  // Initialize the first block
  set_previous(first_block, NULL);
  set_next(first_block, middle_block);
  set_free(first_block, false);

  // Initialize the last block
  set_previous(last_block, middle_block);
  set_next(last_block, NULL);
  set_free(last_block, false);

  // Initialize the pool
  set_previous(middle_block, first_block);
  set_next(middle_block, last_block);
  set_free(middle_block, true);

  return true;
}

void *mem_alloc(size_t size) {
  size = align_length(size);
  
  block_header_t *block = find_free_block(size);
  if (block == NULL) {
    return NULL;
  }

  if (get_block_size(block) > size + sizeof(block_header_t) + __SIZEOF_SIZE_T__) {
    split_block(block, size);
  }

  set_free(block, false);

  return (void *)((size_t) block + sizeof(block_header_t));
}

block_header_t *find_free_block(size_t requested_size) {
  block_header_t *current = first_block + 1;
  block_header_t *perfect = first_block;
  size_t perfect_size = 0;

  while (current != last_block) {
    size_t current_size = get_block_size(current);
    if (is_free(current) &&
        current_size >= requested_size &&
        current_size < perfect_size) {
      perfect = current;
      perfect_size = current_size;
    }
  }

  if (perfect == first_block) {
    return NULL;
  }

  return perfect;
}

size_t align_length(size_t length) {
  size_t bytes = __SIZEOF_SIZE_T__;
  if (length % bytes != 0) {
    length += bytes - length % bytes;
  }
  return length;
}

void split_block(block_header_t *block, size_t size) {
  block_header_t *new_block = (block_header_t *)((size_t) block +
      sizeof(block_header_t) + size);
  block_header_t *next_block = get_next(block);

  set_previous(new_block, block);
  set_next(new_block, next_block);
  set_next(block, new_block);
  set_previous(next_block, new_block);

  set_free(new_block, true);
}

void *mem_realloc(void *addr, size_t size) {
  return NULL;
}

void mem_free(void *addr) {
  block_header_t *block = (block_header_t *)((size_t) addr - sizeof(block_header_t));
  if (is_free(block)) {
    return;
  }

  set_free(block, true);

  if (is_free(get_previous(block))) {
    block = merge_blocks(get_previous(block), block);
  }

  if (is_free(get_next(block))) {
    merge_blocks(block, get_next(block));
  }
}

block_header_t *merge_blocks(block_header_t *left, block_header_t *right) {
  block_header_t *next = get_next(right);
  set_next(left, next);
  set_previous(next, left);
  return left;
}
