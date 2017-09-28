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

typedef struct sb_attr_t {
  uint8_t num_elem;
  uint8_t (*buf_mem)[];
} sb_attr_t;

typedef enum sb_err_t {
  SB_ERR_NONE,
  SB_ERR_BUF_FULL,
  SB_ERR_BUF_EMPTY,
  SB_ERR_BUF_LIMIT,
  SB_ERR_BUF_IN_USE
} sb_err_t;

/*******************************************************************************
* PUBLIC FUNCTION DECLARATIONS
*******************************************************************************/
sb_err_t sb_construct(sb_attr_t *attr, sbd_t *sbd);
sb_err_t sb_destruct(sbd_t *sbd);
sb_err_t sb_put(sbd_t sbd, uint8_t data);
sb_err_t sb_get(sbd_t sbd, uint8_t *data);

#endif // _SIMPLE_BUFFER_H
