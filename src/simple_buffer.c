#include "simple_buffer.h"

/*******************************************************************************
* PRIVATE DATA
*******************************************************************************/
char (*buf_mem)[];
char buf_index;
char num_elem;


/*******************************************************************************
* PUBLIC FUNCTION DEFINITIONS
*******************************************************************************/
char simple_buffer_construct(simple_buffer_attr_t *attr, sbd_t *sbd) {
  buf_mem = (*attr).buf_mem;
  buf_index = 0;
  num_elem = (*attr).num_elem;
  return 0;
}

char simple_buffer_destruct(sbd_t *sbd) {
  return 0;
}

char simple_buffer_put(sbd_t sbd, char data) {
  char err = 0;

  if (buf_index < num_elem) {
    (*buf_mem)[buf_index++] = data;
  } else {
    err = -1;
  }
  return err;
}
