#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

#include "allocator.h"
#include "_allocator_internal.h"
#include "block_header.h"

block_header_t *first_block = NULL;
block_header_t *last_block = NULL;
size_t heap_size = 0;

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

void mem_destroy(void) {
  free(first_block);
  first_block = NULL;
  last_block = NULL;
  heap_size = 0;
}

void *mem_alloc(size_t size) {
  size = align_length(size);
  
  block_header_t *block = find_free_block(size);
  if (block == NULL) {
    return NULL;
  }

  shrink_block(block, size);
  set_free(block, false);

  return header_to_addr(block);
}

void *header_to_addr(block_header_t *block) {
  return (void *)((size_t) block + sizeof(block_header_t));
}

block_header_t *find_free_block(size_t requested_size) {
  block_header_t *current = first_block + 1;
  block_header_t *perfect = first_block;
  size_t perfect_size = SIZE_MAX;

  while (current != last_block) {
    size_t current_size = get_block_size(current);
    if (is_free(current) &&
        current_size >= requested_size &&
        current_size < perfect_size) {
      perfect = current;
      perfect_size = current_size;
    }
    current = get_next(current);
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
  if (addr == NULL) {
    return mem_alloc(size);
  }

  size = align_length(size);

  block_header_t *block = addr_to_header(addr);
  size_t block_size = get_block_size(block);

  if (size == block_size) {
    return addr;
  }

  if (size < block_size) {
    shrink_block(block, size);
    return addr;
  }

  block_header_t *new_block = expand_block(block, size);
  if (new_block == NULL) {
    return NULL;
  }

  return header_to_addr(new_block);
}

void shrink_block(block_header_t *block, size_t size) {
  if (get_block_size(block) > size + sizeof(block_header_t) + __SIZEOF_SIZE_T__) {
    split_block(block, size);
  }
}

block_header_t *expand_block(block_header_t *block, size_t size) {
  void *data = header_to_addr(block);

  block_header_t *prev = get_previous(block);
  block_header_t *next = get_next(block);

  size_t block_size = get_block_size(block);
  size_t prev_size  = get_block_size(prev);
  size_t next_size  = get_block_size(next);

  size_t size_with_prev = block_size + prev_size + sizeof(block_header_t);
  size_t size_with_next = block_size + next_size + sizeof(block_header_t);
  size_t size_with_prev_and_next = size_with_prev + size_with_next - block_size;

  if (is_free(prev) && size_with_prev >= size) {
    merge_blocks(prev, block);
    set_free(prev, false);
    memmove(header_to_addr(prev), data, block_size);
    shrink_block(prev, size);
    return prev;
  }

  if (is_free(next) && size_with_next >= size) {
    merge_blocks(block, next);
    shrink_block(block, size);
    return block;
  }

  if (is_free(prev) &&
      is_free(next) &&
      size_with_prev_and_next >= size) {
    merge_blocks(prev, block);
    merge_blocks(prev, next);
    set_free(prev, false);
    memmove(header_to_addr(prev), data, block_size);
    shrink_block(prev, size);
    return prev;
  }

  void *new_area = mem_alloc(size);
  if (new_area == NULL) {
    return NULL;
  }

  memcpy(new_area, data, block_size);
  mem_free(data);

  return addr_to_header(new_area);
}

void mem_free(void *addr) {
  block_header_t *block = addr_to_header(addr);
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

block_header_t *addr_to_header(void *addr) {
  return (block_header_t *)((size_t) addr - sizeof(block_header_t));
}

block_header_t *merge_blocks(block_header_t *left, block_header_t *right) {
  block_header_t *next = get_next(right);
  set_next(left, next);
  set_previous(next, left);
  return left;
}
