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
static uint8_t buf_mem[BUF_MEM_SIZE];
static sbd_t buf_handles[SB_MAX_NUM_BUFFERS];
static uint8_t buf_list_index = 0;
static uint8_t buf_list[SB_MAX_NUM_BUFFERS][BUF_MEM_SIZE];

/*******************************************************************************
* Setup and Teardown
*******************************************************************************/
void setUp(void)
{
  srand(time(NULL));

  simple_buffer_attr_t attr;
  attr.num_elem = BUF_MEM_SIZE;
  attr.buf_mem = &buf_list[buf_list_index];

  TEST_ASSERT(
    simple_buffer_construct(&attr, &buf_handles[buf_list_index++]) == SB_ERR_NONE
  );
}

void tearDown(void)
{
  while (buf_list_index > 0) {
    TEST_ASSERT(
      simple_buffer_destruct(&buf_handles[--buf_list_index]) == SB_ERR_NONE
    );
  }
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

    TEST_ASSERT(
      simple_buffer_put(buf_handles[buf_list_index-1], data) == SB_ERR_NONE
    );
    TEST_ASSERT(buf_list[buf_list_index-1][i] == data);
  }
}

void test_simple_buffer_put_returns_error_when_buffer_full(void)
{
  uint8_t i;
  uint8_t data = 0;

  for (i=0;i<BUF_MEM_SIZE;i++) {
    simple_buffer_put(buf_handles[buf_list_index-1], data);
  }
  TEST_ASSERT(
    simple_buffer_put(buf_handles[buf_list_index-1], data) == SB_ERR_BUF_FULL
  );
}

void test_simple_buffer_get_returns_previously_put_data(void)
{
  uint8_t i;
  uint8_t data;
  uint8_t returned_data;

  for (i=0;i<BUF_MEM_SIZE;i++) {
    data = (uint8_t)rand();
    simple_buffer_put(buf_handles[buf_list_index-1], data);

    TEST_ASSERT(
      simple_buffer_get(buf_handles[buf_list_index-1], &returned_data) == SB_ERR_NONE
    );
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
    simple_buffer_put(buf_handles[buf_list_index-1], buffered_data[i]);
  }

  for (i=0;i<BUF_MEM_SIZE;i++) {
    TEST_ASSERT(
      simple_buffer_get(buf_handles[buf_list_index-1], &returned_data) == SB_ERR_NONE
    );
    TEST_ASSERT(returned_data == buffered_data[i]);
  }
}

void test_simple_buffer_get_returns_error_when_buffer_empty(void)
{
  uint8_t returned_data;

  TEST_ASSERT(
    simple_buffer_get(buf_handles[buf_list_index-1], &returned_data) == SB_ERR_BUF_EMPTY
  );
}

void test_simple_buffer_state_unchanged_after_get_called_on_empty_buffer(void)
{
  uint8_t i;
  uint8_t returned_data;
  uint8_t buffered_data = (uint8_t)rand();

  TEST_ASSERT(
    simple_buffer_get(buf_handles[buf_list_index-1], &returned_data) == SB_ERR_BUF_EMPTY
  );
  simple_buffer_put(buf_handles[buf_list_index-1], buffered_data);
  TEST_ASSERT(
    simple_buffer_get(buf_handles[buf_list_index-1], &returned_data) == SB_ERR_NONE
  );
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
    simple_buffer_put(buf_handles[buf_list_index-1], buffered_data[i]);
  }

  // attempted put into full buffer
  simple_buffer_put(buf_handles[buf_list_index-1], buffered_data[i]);

  // get still functions properly
  simple_buffer_get(buf_handles[buf_list_index-1], &returned_data);
  TEST_ASSERT(returned_data == buffered_data[0]);

  // put still functions properly
  extra_buffered_data = (uint8_t)rand();
  simple_buffer_put(buf_handles[buf_list_index-1], extra_buffered_data);
  TEST_ASSERT(buf_list[buf_list_index-1][0] == extra_buffered_data);
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
      simple_buffer_put(buf_handles[buf_list_index-1], buffered_data[i]);
    }

    for (i=0;i<BUF_MEM_SIZE;i++) {
      TEST_ASSERT(
        simple_buffer_get(buf_handles[buf_list_index-1], &returned_data) == SB_ERR_NONE
      );
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

  TEST_ASSERT(
    simple_buffer_construct(&attr, &buf_handles[buf_list_index++]) == SB_ERR_NONE
  );
  TEST_ASSERT(buf_handles[buf_list_index-1] != buf_handles[0]);

  // test if the data maintained by each instance is unique
  for (i=0;i<BUF_MEM_SIZE;i++) {
    buffered_data_0[i] = (uint8_t)rand();
    simple_buffer_put(buf_handles[0], buffered_data_0[i]);
    buffered_data_1[i] = (uint8_t)rand();
    simple_buffer_put(buf_handles[buf_list_index-1], buffered_data_1[i]);
  }

  for (i=0;i<BUF_MEM_SIZE;i++) {
    TEST_ASSERT(
      simple_buffer_get(buf_handles[0], &returned_data) == SB_ERR_NONE
    );
    TEST_ASSERT(returned_data == buffered_data_0[i]);
    TEST_ASSERT(simple_buffer_get(buf_handles[buf_list_index-1], &returned_data) == SB_ERR_NONE);
    TEST_ASSERT(returned_data == buffered_data_1[i]);
  }

}

void test_simple_buffer_constructor_returns_error_when_buf_limit_reached(void)
{
  uint8_t i;
  uint8_t extra_buffer[BUF_MEM_SIZE];
  sbd_t extra_sbd;
  simple_buffer_attr_t attr;

  attr.num_elem = BUF_MEM_SIZE;

  while(buf_list_index < SB_MAX_NUM_BUFFERS) {
    attr.buf_mem = &(buf_list[buf_list_index]);
    TEST_ASSERT(
      simple_buffer_construct(&attr, &buf_handles[buf_list_index++]) == SB_ERR_NONE
    );

    TEST_ASSERT(buf_handles[buf_list_index-1] == buf_list_index-1);
  }

  // now that all the buffers are allocated, attempting to construct one more
  // should return an error code
  attr.buf_mem = &extra_buffer;
  TEST_ASSERT(simple_buffer_construct(&attr, &extra_sbd) == SB_ERR_BUF_LIMIT);
}

void test_simple_buffer_instances_can_be_different_sizes(void)
{
  uint8_t data;
  uint8_t i;
  uint8_t second_buf_max_size = 5;
  simple_buffer_attr_t attr;

  attr.num_elem = second_buf_max_size;
  attr.buf_mem = &(buf_list[buf_list_index]);
  simple_buffer_construct(&attr, &buf_handles[buf_list_index++]);

  // check that the error is thrown only after BUF_MEM_SIZE for buffer 0
  for (i=0;i<BUF_MEM_SIZE;i++) {
    data = (uint8_t)rand();

    TEST_ASSERT(simple_buffer_put(buf_handles[0], data) == SB_ERR_NONE);
  }

  TEST_ASSERT(simple_buffer_put(buf_handles[0], data) == SB_ERR_BUF_FULL);

  // check that error is thrown after second_buf_max_size for buffer 1
  for (i=0;i<second_buf_max_size;i++) {
    data = (uint8_t)rand();

    TEST_ASSERT(
      simple_buffer_put(buf_handles[buf_list_index-1], data) == SB_ERR_NONE
    );
  }

  TEST_ASSERT(
    simple_buffer_put(buf_handles[buf_list_index-1], data) == SB_ERR_BUF_FULL
  );
}

void test_simple_buffer_constructor_returns_error_passed_used_buf_mem(void)
{
  simple_buffer_attr_t attr;
  attr.num_elem = BUF_MEM_SIZE;
  attr.buf_mem = &buf_list[0];

  TEST_ASSERT(
    simple_buffer_construct(&attr, &buf_handles[buf_list_index-1]) == SB_ERR_BUF_IN_USE
  );
}
