#pragma once

#include "block_header.h"

/**
 * Find a free block of minimal size suitable for allocation
 */
block_header_t *find_free_block(size_t size);

/**
 * Align an address or size
 */
size_t align_length(size_t length);

/**
 * Split a block into two blocks
 */
void split_block(block_header_t *block, size_t size);

/**
 * Merge two blocks and return the conglomerated one
 */
block_header_t *merge_blocks(block_header_t *left, block_header_t *right);

/**
 * Get the block header by data address
 */
block_header_t *addr_to_header(void *addr);

/**
 * Get data address of the block
 */
void *header_to_addr(block_header_t *block);

/**
 * Shrink a block
 */
void shrink_block(block_header_t *block, size_t size);

/**
 * Expand a block
 */
block_header_t *expand_block(block_header_t *block, size_t size);
