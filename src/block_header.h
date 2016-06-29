#pragma once

#include <stdlib.h>
#include <stdbool.h>

/**
 * Header of a block of memory.
 *
 * Essentially it is two pointers to previous and next blocks, but since
 * addresses are aligned, the last bit is used to indicate whether the
 * block is free.
 */
typedef struct block_header {
  struct block_header *_prev;
  union {
    struct block_header *_next;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    bool _free : 1;
#else
    struct {
      size_t _unused : __SIZEOF_SIZE_T__ * 8 - 1;
      bool _free : 1;
    };
#endif
  };
} block_header_t;

/**
 * Get the previous block address
 */
block_header_t *get_previous(block_header_t *header);

/**
 * Set the previous block address
 */
void set_previous(block_header_t *header, block_header_t *prev);

/**
 * Get the next block address
 */
block_header_t *get_next(block_header_t *header);

/**
 * Set the next block address
 */
void set_next(block_header_t *header, block_header_t *next);

/**
 * Get the size of a block
 */
size_t get_block_size(block_header_t *header);

/**
 * Check if the block is free
 */
bool is_free(block_header_t *header);

/**
 * Set whether the block is free
 */
void set_free(block_header_t *header, bool free);
