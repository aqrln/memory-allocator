#include "block_header.h"

block_header_t *get_previous(block_header_t *header) {
  return header->_prev;
}

block_header_t *get_next(block_header_t *header) {
  size_t addr = (size_t) header->_next;
  addr &= ~1;
  return (block_header_t *) addr;
}

bool is_free(block_header_t *header) {
  return header->_free;
}

void set_previous(block_header_t *header, block_header_t *prev) {
  header->_prev = prev;
}

void set_next(block_header_t *header, block_header_t *next) {
  bool free = is_free(header);
  header->_next = next;
  set_free(header, free);
}

void set_free(block_header_t *header, bool free) {
  header->_free = free;
}

size_t get_block_size(block_header_t *header) {
  if (get_next(header) == NULL) {
    return 0;
  }
  return (size_t) get_next(header) - (size_t) header - sizeof(block_header_t);
}
