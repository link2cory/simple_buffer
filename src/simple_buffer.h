#ifndef _SIMPLE_BUFFER_H
#define _SIMPLE_BUFFER_H

/*******************************************************************************
* INCLUDES
*******************************************************************************/
#include <stdint.h>

/*******************************************************************************
* PUBLIC DEFINES
*******************************************************************************/
#define _DEFAULT_MAX_BUFFERS 10

#ifndef SB_MAX_NUM_BUFFERS
#define SB_MAX_NUM_BUFFERS _DEFAULT_MAX_BUFFERS
#endif
/*******************************************************************************
* PUBLIC TYPEDEFS
*******************************************************************************/
typedef uint8_t sbd_t;

typedef struct simple_buffer_attr_t {
  uint8_t num_elem;
  uint8_t (*buf_mem)[];
} simple_buffer_attr_t;

typedef enum sb_error_t {
  SB_ERR_NONE,
  SB_ERR_BUF_FULL,
  SB_ERR_BUF_EMPTY,
  SB_ERR_BUF_LIMIT,
  SB_ERR_BUF_IN_USE
} sb_error_t;

/*******************************************************************************
* PUBLIC FUNCTION DECLARATIONS
*******************************************************************************/
sb_error_t simple_buffer_construct(simple_buffer_attr_t *attr, sbd_t *sbd);
sb_error_t simple_buffer_destruct(sbd_t *sbd);
sb_error_t simple_buffer_put(sbd_t sbd, uint8_t data);
sb_error_t simple_buffer_get(sbd_t sbd, uint8_t *data);

#endif // _SIMPLE_BUFFER_H
