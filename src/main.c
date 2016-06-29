#include <stdio.h>

#include "allocator.h"
#include "block_header.h"

int main() {
  block_header_t header;
  set_previous(&header, (block_header_t *) 0x1122334455667788);
  set_next(&header, (block_header_t *) 0x1122334455667788);
  set_free(&header, true);

  printf("%p %p %d\n", get_previous(&header), get_next(&header), is_free(&header));

  return 0;
}
