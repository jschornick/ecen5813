#include "memory.h"
//#include "unity.h"
#include "unity_fixture.h"

TEST_GROUP(Memory);

#define MEMLEN 256
extern uint8_t *mem1;
extern uint8_t *mem2;

uint8_t *mem1;
uint8_t *mem2;
//This is run before EACH TEST
TEST_SETUP(Memory)
{
  mem1 = malloc(MEMLEN);
  mem2 = malloc(MEMLEN);

  for(size_t i=0; i<MEMLEN; i++) {
    mem1[i] = (uint8_t) i;
    mem2[i] = (uint8_t) i;
  }
}

TEST_TEAR_DOWN(Memory)
{
  free(mem1);
  free(mem2);
}


TEST(Memory, memmove_invalid_pointer)
{
  uint8_t x[10];
  /* Any null pointer parameter should return a MEM_FAIL. */
  TEST_ASSERT_EQUAL_INT( my_memmove(x,NULL,10), MEM_FAIL );
  TEST_ASSERT_EQUAL_INT( my_memmove(NULL,x,10), MEM_FAIL );
}

/* A successful memmove should return dst pointer when successful */
TEST(Memory, memmove_returns_dst)
{
  uint8_t x[10];
  TEST_ASSERT_EQUAL_INT( my_memmove(x,x+1,1), x+1 );
}

/* Verify that a non-overlapping src/dst move the expected memory from
   src to dst */
TEST(Memory, memmove_no_overlap)
{
  size_t src = 0;
  size_t dst = 50;
  size_t len = 20;

  my_memmove(mem1 + src, mem1 + dst, len);

  /* Check the moved bytes in the new location */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst, mem2 + src, len);

  /* No other bytes after dst should have changed. */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst+len, mem2 + dst+len, MEMLEN - (dst+len));
}

/* Verify that a non-overlapping src/dst move the expected memory from
   src to dst */
TEST(Memory, memmove_src_in_dst)
{
  size_t src = 50;
  size_t dst = 40;
  size_t len = 20;

  /* Move len bytes somewhere just behind where our source begins */
  my_memmove(mem1 + src, mem1 + dst, len);

  /* Verify the moved bytes correct in the new location */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst, mem2 + src, len);

  /* Verify no other bytes have changed that shouldn't have */
  TEST_ASSERT_EQUAL_MEMORY(mem1, mem2, dst); /* early bytes */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + src+len, mem2 + src+len, MEMLEN - (src+len)); /* end bytes */
}

TEST(Memory, memmove_dst_in_src)
{
  size_t src = 40;
  size_t dst = 50;
  size_t len = 20;

  /* Move len bytes somewhere just after where our source begins */
  my_memmove(mem1 + src, mem1 + dst, len);

  /* Verify the moved bytes in the new location */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst, mem2 + src, len);

  /* Verify no other bytes have changed that shouldn't have */
  TEST_ASSERT_EQUAL_MEMORY(mem1, mem2, src); /* early bytes */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst+len, mem2 + dst+len, MEMLEN - (dst+len)); /* end bytes */
}

/* A null for the src should fail gracefully. */
TEST(Memory, memset_invalid_pointer)
{
  uint8_t len = 3;
  uint8_t val = 42;
  TEST_ASSERT_EQUAL_INT( my_memset(NULL,len,val), MEM_FAIL );
}

/* A memset should return src pointer when successful */
TEST(Memory, memset_returns_src)
{
  uint8_t x[10];
  /* Verify return value is src */
  TEST_ASSERT_EQUAL_INT( my_memset(x,1,1), x );
}

/* Verify that memset actually sets the memory region correctly. */
TEST(Memory, memset_sets_values)
{
  size_t len = 20;

  /* First set a region to a known value */
  my_memset(mem1, len, 0x23);
  for(size_t i=0; i<len; i++) {
    TEST_ASSERT_EQUAL_INT(mem1[i], 0x23);
  }

  /* Verify that memset changed the value correctly */
  my_memset(mem1, len, 0x42);
  for(size_t i=0; i<len; i++) {
    TEST_ASSERT_EQUAL_INT(mem1[i], 0x42);
  }
}


/* Could be auto-generated using Unity helper scripts */
TEST_GROUP_RUNNER(Memory)
{
  RUN_TEST_CASE(Memory, memmove_invalid_pointer);
  RUN_TEST_CASE(Memory, memmove_returns_dst);
  RUN_TEST_CASE(Memory, memmove_no_overlap);
  RUN_TEST_CASE(Memory, memmove_src_in_dst);
  RUN_TEST_CASE(Memory, memmove_dst_in_src);
  RUN_TEST_CASE(Memory, memset_invalid_pointer);
  RUN_TEST_CASE(Memory, memset_returns_src);
  RUN_TEST_CASE(Memory, memset_sets_values);
}
