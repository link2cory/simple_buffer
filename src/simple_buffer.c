#include "simple_buffer.h"

/*******************************************************************************
* PRIVATE DATA
*******************************************************************************/
uint8_t (*buf_mem)[];
uint8_t buf_index;
uint8_t num_elem;


/*******************************************************************************
* PUBLIC FUNCTION DEFINITIONS
*******************************************************************************/
sb_error_t simple_buffer_construct(simple_buffer_attr_t *attr, sbd_t *sbd) {
  buf_mem = (*attr).buf_mem;
  buf_index = 0;
  num_elem = (*attr).num_elem;
  return SB_ERR_NONE;
}

sb_error_t simple_buffer_destruct(sbd_t *sbd) {
  return SB_ERR_NONE;
}

sb_error_t simple_buffer_put(sbd_t sbd, uint8_t data) {
  sb_error_t err = SB_ERR_NONE;

  if (buf_index < num_elem) {
    (*buf_mem)[buf_index++] = data;
  } else {
    err = SB_ERR_BUF_FULL;
  }
  return err;
}
