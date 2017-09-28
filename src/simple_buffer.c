#include "simple_buffer.h"

/*******************************************************************************
* PRIVATE DATA
*******************************************************************************/
simple_buffer_t _sb[SB_MAX_NUM_BUFFERS];
uint8_t _sb_index = 0;
uint8_t max_num_elem;

/*******************************************************************************
* PRIVATE FUNCTION DECLARATIONS
*******************************************************************************/
sb_error_t _validate_construct_params(simple_buffer_attr_t *attr, sbd_t *sbd);

/*******************************************************************************
* PUBLIC FUNCTION DEFINITIONS
*******************************************************************************/
sb_error_t simple_buffer_construct(simple_buffer_attr_t *attr, sbd_t *sbd) {
  sb_error_t err = _validate_construct_params(attr, sbd);

  if (err == SB_ERR_NONE) {
    _sb[_sb_index].buf_mem = (*attr).buf_mem;
    _sb[_sb_index].head = 0;
    _sb[_sb_index].tail = 0;
    _sb[_sb_index].num_elem = 0;
    _sb[_sb_index].max_num_elem = (*attr).num_elem;
    _sb[_sb_index].status = SB_STATUS_ACTIVE;
    *sbd = _sb_index++;
  }

  return err;
}

sb_error_t simple_buffer_destruct(sbd_t *sbd) {
  _sb[--_sb_index].status = SB_STATUS_INACTIVE;

  return SB_ERR_NONE;
}

sb_error_t simple_buffer_put(sbd_t sbd, uint8_t data) {
  sb_error_t err = SB_ERR_NONE;

  if (_sb[sbd].num_elem < _sb[sbd].max_num_elem) {
    (*_sb[sbd].buf_mem)[_sb[sbd].head++] = data;
    _sb[sbd].head = ((_sb[sbd].head == _sb[sbd].max_num_elem) ? 0:_sb[sbd].head);
    if (_sb[sbd].head == _sb[sbd].max_num_elem) {
      _sb[sbd].head = 0;
    }
    _sb[sbd].num_elem++;
  } else {
    err = SB_ERR_BUF_FULL;
  }

  return err;
}

sb_error_t simple_buffer_get(sbd_t sbd, uint8_t *data) {
  sb_error_t err = SB_ERR_NONE;

  if (_sb[sbd].num_elem > 0) {
    *data = (*_sb[sbd].buf_mem)[_sb[sbd].tail++];
    _sb[sbd].tail = ((_sb[sbd].tail == _sb[sbd].max_num_elem) ? 0:_sb[sbd].tail);
    _sb[sbd].num_elem--;
  } else {
    err = SB_ERR_BUF_EMPTY;
  }

  return err;
}

/*******************************************************************************
* PRIVATE FUNCTION DEFINITIONS
*******************************************************************************/
sb_error_t _validate_construct_params(simple_buffer_attr_t *attr, sbd_t *sbd) {
  sb_error_t err = SB_ERR_NONE;
  uint8_t i;

  if (_sb_index < (SB_MAX_NUM_BUFFERS)) {
    for (i=0;i<=_sb_index;i++) {
      if (_sb[i].buf_mem == (*attr).buf_mem
        && _sb[i].status == SB_STATUS_ACTIVE
      ) {
        err = SB_ERR_BUF_IN_USE;
        break;
      }
    }

  } else {
    err = SB_ERR_BUF_LIMIT;
  }
  return err;
}
