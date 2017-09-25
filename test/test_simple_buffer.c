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

    TEST_ASSERT(simple_buffer_get(sbd, &returned_data) == SB_ERR_NONE);
    TEST_ASSERT(returned_data == data);
  }
}

void test_simple_buffer_get_writes_previously_put_data_fifo(void)
{
  uint8_t i;
  uint8_t buffered_data[BUF_MEM_SIZE];
  uint8_t returned_data;

  for (i=0;i<BUF_MEM_SIZE;i++) {
    buffered_data[i] = (uint8_t)rand();
    simple_buffer_put(sbd, buffered_data[i]);
  }

  for (i=0;i<BUF_MEM_SIZE;i++) {
    TEST_ASSERT(simple_buffer_get(sbd, &returned_data) == SB_ERR_NONE);
    TEST_ASSERT(returned_data == buffered_data[i]);
  }
}

void test_simple_buffer_get_returns_error_when_buffer_empty(void)
{
  uint8_t returned_data;

  TEST_ASSERT(simple_buffer_get(sbd, &returned_data) == SB_ERR_BUF_EMPTY);
}

void test_simple_buffer_state_unchanged_after_get_called_on_empty_buffer(void)
{
  uint8_t i;
  uint8_t returned_data;
  uint8_t buffered_data = (uint8_t)rand();

  TEST_ASSERT(simple_buffer_get(sbd, &returned_data) == SB_ERR_BUF_EMPTY);
  simple_buffer_put(sbd, buffered_data);
  TEST_ASSERT(simple_buffer_get(sbd, &returned_data) == SB_ERR_NONE);
  TEST_ASSERT(returned_data == buffered_data);
}

void test_simple_buffer_state_unchanged_after_put_called_on_full_buffer(void)
{
  uint8_t i;
  uint8_t returned_data;
  uint8_t buffered_data[BUF_MEM_SIZE];
  uint8_t extra_buffered_data;

  for (i=0;i<BUF_MEM_SIZE;i++) {
    buffered_data[i] = (uint8_t)rand();
    simple_buffer_put(sbd, buffered_data[i]);
  }

  // attempted put into full buffer
  simple_buffer_put(sbd, buffered_data[i]);

  // get still functions properly
  simple_buffer_get(sbd, &returned_data);
  TEST_ASSERT(returned_data == buffered_data[0]);

  // put still functions properly
  extra_buffered_data = (uint8_t)rand();
  simple_buffer_put(sbd, extra_buffered_data);
  TEST_ASSERT(buf_mem[0] == extra_buffered_data);
}

void test_simple_buffer_operates_properly_if_filled_and_emptied_many_times(void)
{
  uint8_t i;
  uint8_t cycle_index;
  uint8_t returned_data;
  uint8_t buffered_data[BUF_MEM_SIZE];

  for (cycle_index=0;cycle_index<10;cycle_index++) {
    for (i=0;i<BUF_MEM_SIZE;i++) {
      buffered_data[i] = (uint8_t)rand();
      simple_buffer_put(sbd, buffered_data[i]);
    }

    for (i=0;i<BUF_MEM_SIZE;i++) {
      TEST_ASSERT(simple_buffer_get(sbd, &returned_data) == SB_ERR_NONE);
      TEST_ASSERT(returned_data == buffered_data[i]);
    }
  }
}

void test_simple_buffer_can_construct_multiple_unique_instances(void)
{
  static uint8_t buf_mem_1[BUF_MEM_SIZE];
  uint8_t i;
  sbd_t sbd_1;
  uint8_t buffered_data_0[BUF_MEM_SIZE];
  uint8_t buffered_data_1[BUF_MEM_SIZE];
  uint8_t returned_data;

  simple_buffer_attr_t attr;
  attr.num_elem = BUF_MEM_SIZE;
  attr.buf_mem = &buf_mem_1;

  TEST_ASSERT(simple_buffer_construct(&attr, &sbd_1) == SB_ERR_NONE);
  TEST_ASSERT(sbd_1 != sbd);

  // test if the data maintained by each instance is unique
  for (i=0;i<BUF_MEM_SIZE;i++) {
    buffered_data_0[i] = (uint8_t)rand();
    simple_buffer_put(sbd, buffered_data_0[i]);
    buffered_data_1[i] = (uint8_t)rand();
    simple_buffer_put(sbd_1, buffered_data_1[i]);
  }

  for (i=0;i<BUF_MEM_SIZE;i++) {
    TEST_ASSERT(simple_buffer_get(sbd, &returned_data) == SB_ERR_NONE);
    TEST_ASSERT(returned_data == buffered_data_0[i]);
    TEST_ASSERT(simple_buffer_get(sbd_1, &returned_data) == SB_ERR_NONE);
    TEST_ASSERT(returned_data == buffered_data_1[i]);
  }
}
