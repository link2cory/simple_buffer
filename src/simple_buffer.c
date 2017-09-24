#include "simple_buffer.h"

/*******************************************************************************
* PRIVATE DATA
*******************************************************************************/
char (*buf_mem)[];
char buf_index;


/*******************************************************************************
* PUBLIC FUNCTION DEFINITIONS
*******************************************************************************/
char simple_buffer_construct(simple_buffer_attr_t *attr, sbd_t *sbd) {
  buf_mem = (*attr).buf_mem;
  buf_index = 0;
  return 0;
}

char simple_buffer_destruct(sbd_t *sbd) {
  return 0;
}

char simple_buffer_put(sbd_t sbd, char data) {
  (*buf_mem)[buf_index++] = data;
  return 0;
}
