#ifndef _SIMPLE_BUFFER_H
#define _SIMPLE_BUFFER_H

/*******************************************************************************
* TYPEDEFS
*******************************************************************************/
typedef char sbd_t;
typedef struct simple_buffer_attr_t {
  char num_elem;
  char (*buf_mem)[];
} simple_buffer_attr_t;

/*******************************************************************************
* FUNCTION DECLARATIONS
*******************************************************************************/
char simple_buffer_construct(simple_buffer_attr_t *attr, sbd_t *sbd);
char simple_buffer_destruct(sbd_t *sbd);

#endif // _SIMPLE_BUFFER_H
