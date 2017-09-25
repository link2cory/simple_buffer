#ifndef _SIMPLE_BUFFER_H
#define _SIMPLE_BUFFER_H

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <stdint.h>

/*******************************************************************************
* TYPEDEFS
*******************************************************************************/
typedef uint8_t sbd_t;
typedef struct simple_buffer_attr_t {
  uint8_t num_elem;
  uint8_t (*buf_mem)[];
} simple_buffer_attr_t;

typedef enum sb_error_t {
  SB_ERR_NONE,
  SB_ERR_BUF_FULL,
  SB_ERR_BUF_EMPTY
} sb_error_t;

typedef struct simple_buffer_t {
  uint8_t num_elem;
  uint8_t (*buf_mem)[];
  uint8_t head;
  uint8_t tail;
} simple_buffer_t;

/*******************************************************************************
* FUNCTION DECLARATIONS
*******************************************************************************/
sb_error_t simple_buffer_construct(
  simple_buffer_attr_t *attr,
  sbd_t *sbd
);
sb_error_t simple_buffer_destruct(sbd_t *sbd);
sb_error_t simple_buffer_put(sbd_t sbd, uint8_t data);
sb_error_t simple_buffer_get(sbd_t sbd, uint8_t *data);

#endif // _SIMPLE_BUFFER_H
