#include "simple_buffer.h"

/*******************************************************************************
* PRIVATE DATA
*******************************************************************************/
uint8_t (*buf_mem)[];
uint8_t tail;
uint8_t head;
uint8_t max_num_elem;
uint8_t num_elem;

/*******************************************************************************
* PUBLIC FUNCTION DEFINITIONS
*******************************************************************************/
sb_error_t simple_buffer_construct(simple_buffer_attr_t *attr, sbd_t *sbd) {
  buf_mem = (*attr).buf_mem;
  head = 0;
  tail = 0;
  num_elem = 0;
  max_num_elem = (*attr).num_elem;
  return SB_ERR_NONE;
}

sb_error_t simple_buffer_destruct(sbd_t *sbd) {
  return SB_ERR_NONE;
}

sb_error_t simple_buffer_put(sbd_t sbd, uint8_t data) {
  sb_error_t err = SB_ERR_NONE;

  if (num_elem++ < max_num_elem) {
    (*buf_mem)[head++] = data;
  } else {
    err = SB_ERR_BUF_FULL;
  }

  return err;
}

sb_error_t simple_buffer_get(sbd_t sbd, uint8_t *data) {
  sb_error_t err = SB_ERR_NONE;

  if (num_elem-- > 0) {
    *data = (*buf_mem)[tail++];
  } else {
    num_elem = 0;
    err = SB_ERR_BUF_EMPTY;
  }

  return err;
}
