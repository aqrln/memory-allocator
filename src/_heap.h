#pragma once

#include <stddef.h>
#include <stdint.h>

/**
 * This header is private to allocator.c
 */

typedef struct block {
  struct block *prev;
  union {
    struct block *next;
    struct {
      uint64_t _unused : 63;
      unsigned free : 1;
    };
  };
} block_t;
