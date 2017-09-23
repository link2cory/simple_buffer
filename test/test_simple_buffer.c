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
char buf_mem[BUF_MEM_SIZE];

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
  char data = (char)rand();

  TEST_ASSERT(simple_buffer_put(sbd, data) != -1);
  TEST_ASSERT(buf_mem[0] == data);
}
