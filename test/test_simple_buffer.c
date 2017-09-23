#include "unity.h"
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
void test_simple_buffer_NeedToImplement(void)
{
    TEST_IGNORE_MESSAGE("Need to Implement simple_buffer");
}
