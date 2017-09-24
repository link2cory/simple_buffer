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
  SB_ERR_BUF_FULL
} sb_error_t;

/*******************************************************************************
* FUNCTION DECLARATIONS
*******************************************************************************/
sb_error_t simple_buffer_construct(
  simple_buffer_attr_t *attr,
  sbd_t *sbd
);
sb_error_t simple_buffer_destruct(sbd_t *sbd);
sb_error_t simple_buffer_put(sbd_t sbd, uint8_t data);

#endif // _SIMPLE_BUFFER_H
