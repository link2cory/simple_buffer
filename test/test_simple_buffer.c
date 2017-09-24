#include "unity.h"
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
static char buf_mem[BUF_MEM_SIZE];

/*******************************************************************************
* Setup and Teardown
*******************************************************************************/
void setUp(void)
{
  srand(time(NULL));

  simple_buffer_attr_t attr;
  attr.num_elem = BUF_MEM_SIZE;
  attr.buf_mem = &buf_mem;

  TEST_ASSERT(simple_buffer_construct(&attr, &sbd) != -1);
}

void tearDown(void)
{
  TEST_ASSERT(simple_buffer_destruct(&sbd) != -1);
}


/*******************************************************************************
* Tests
*******************************************************************************/
void test_simple_buffer_put_writes_to_buffer(void)
{
  char i;
  char data;

  for (i=0;i<BUF_MEM_SIZE;i++) {
    data = (char)rand();

    TEST_ASSERT(simple_buffer_put(sbd, data) != -1);
    TEST_ASSERT(buf_mem[i] == data);
  }
}

void test_simple_buffer_put_returns_error_when_buffer_full(void)
{
  char i;
  char data = 0;

  for (i=0;i<BUF_MEM_SIZE;i++) {
    simple_buffer_put(sbd, data);
  }
  TEST_ASSERT(simple_buffer_put(sbd, data) == -1);
}
