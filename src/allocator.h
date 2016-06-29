#pragma once

#include <stddef.h>

/**
 * Allocate a block of memory with specified size. If the allocation is
 * successful, return the address of the beginning of the block, NULL
 * otherwise.
 */
void *mem_alloc(size_t size);

/**
 * Change the size of the block of memory at address addr to size bytes.
 * The block may be moved to another location, so the function returns the
 * new address (which may or may not be equal to initial). If there isn't
 * enough memory for reallocation, the block remains untouched and the
 * function returns NULL. If addr is NULL, the invokation has the same
 * effect as mem_alloc(size).
 */
void *mem_realloc(void *addr, size_t size);

/**
 * Free a previously allocated block of memory.
 */
void mem_free(void *addr);
