#include "unity.h"
#include <stdint.h>
#include <time.h>
#include <stdlib.h>

/*******************************************************************************
* MODULE UNDER TEST
*******************************************************************************/
#include "simple_buffer.h"

/*******************************************************************************
* Defines
*******************************************************************************/
#define BUF_MEM_SIZE 10
/*******************************************************************************
* Local Data
*******************************************************************************/
sbd_t sbd;
static uint8_t buf_mem[BUF_MEM_SIZE];

/*******************************************************************************
* Setup and Teardown
*******************************************************************************/
void setUp(void)
{
  srand(time(NULL));

  simple_buffer_attr_t attr;
  attr.num_elem = BUF_MEM_SIZE;
  attr.buf_mem = &buf_mem;

  TEST_ASSERT(simple_buffer_construct(&attr, &sbd) == SB_ERR_NONE);
}

void tearDown(void)
{
  TEST_ASSERT(simple_buffer_destruct(&sbd) == SB_ERR_NONE);
}


/*******************************************************************************
* Tests
*******************************************************************************/
void test_simple_buffer_put_writes_to_buffer(void)
{
  uint8_t i;
  uint8_t data;

  for (i=0;i<BUF_MEM_SIZE;i++) {
    data = (uint8_t)rand();

    TEST_ASSERT(simple_buffer_put(sbd, data) == SB_ERR_NONE);
    TEST_ASSERT(buf_mem[i] == data);
  }
}

void test_simple_buffer_put_returns_error_when_buffer_full(void)
{
  uint8_t i;
  uint8_t data = 0;

  for (i=0;i<BUF_MEM_SIZE;i++) {
    simple_buffer_put(sbd, data);
  }
  TEST_ASSERT(simple_buffer_put(sbd, data) == SB_ERR_BUF_FULL);
}

void test_simple_buffer_get_returns_previously_put_data(void)
{
  uint8_t i;
  uint8_t data;
  uint8_t returned_data;

  for (i=0;i<BUF_MEM_SIZE;i++) {
    data = (uint8_t)rand();
    simple_buffer_put(sbd, data);
    returned_data = simple_buffer_get(sbd);

    TEST_ASSERT(returned_data == data);
  }
}
