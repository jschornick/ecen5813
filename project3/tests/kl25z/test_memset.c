#include "memory.h"
#include "memory_dma.h"
#include "dma.h"
#include "unity_fixture.h"

TEST_GROUP(memset_dma);

#define MEMLEN 256
extern uint8_t *mem1;
extern uint8_t *mem2;

uint8_t *mem1;
uint8_t *mem2;
//This is run before EACH TEST
TEST_SETUP(memset_dma)
{

  mem1 = malloc(MEMLEN);
  mem2 = malloc(MEMLEN);

  for(size_t i=0; i<MEMLEN; i++) {
    mem1[i] = (uint8_t) i;
    mem2[i] = (uint8_t) i;
  }
}

TEST_TEAR_DOWN(memset_dma)
{
  free(mem1);
  free(mem2);
}


/* A null for the src should fail gracefully. */
TEST(memset_dma, memset_invalid_pointer)
{
  uint8_t len = 3;
  uint8_t val = 42;
  TEST_ASSERT_EQUAL_INT( MEM_FAIL, memset_dma(NULL,len,val) );
}

/* A memset should return src pointer when successful */
TEST(memset_dma, memset_returns_src)
{
  uint8_t x[10];
  /* Verify return value is src */
  TEST_ASSERT_EQUAL_INT( x, memset_dma(x,1,1) );
}


#include "io.h"

/* Verify that memset actually sets the memory region correctly. */
TEST(memset_dma, memset_sets_values)
{

  /* First set a region to a known value */
  my_memset(mem1, MEMLEN, 0x23);
  dma_wait(DMA_MEM_CHAN);
  for(size_t i=0; i<MEMLEN; i++) {
    TEST_ASSERT_EQUAL_INT( 0x23, mem1[i] );
  }

  /* Change half the values */
  memset_dma(mem1, MEMLEN/2, 0x42);
  dma_wait(DMA_MEM_CHAN);

  /* Verify only half of values were set, others unchanged */
  for(size_t i=0; i<MEMLEN/2; i++) {
    TEST_ASSERT_EQUAL_INT( 0x42, mem1[i] );
  }
  for(size_t i=MEMLEN/2+1; i<MEMLEN; i++) {
    TEST_ASSERT_EQUAL_INT( 0x23, mem1[i] );
  }
}

TEST(memset_dma, memset_unaligned_addr_01)
{
  size_t offset = 0x11;
  size_t len = MEMLEN/2;
  uint8_t old_val = 0x23;
  uint8_t new_val = 0x42;

  /* First set entire region to a known value */
  my_memset(mem1, MEMLEN, old_val);
  for(size_t i=0; i<MEMLEN; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }

  /* Change the values starting at different alignments */

  memset_dma(mem1+offset, len, new_val);
  dma_wait(DMA_MEM_CHAN);

  /* Verify exactly half of values were set, others unchanged */
  for(size_t i=offset; i<len+offset; i++) {
    TEST_ASSERT_EQUAL_INT( new_val, mem1[i] );
  }
  // initial bytes unchanged
  for(size_t i=0; i<offset; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }
  // end bytes unchanged
  for(size_t i=len+offset+1; i<MEMLEN; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }
}

TEST(memset_dma, memset_unaligned_addr_02)
{
  size_t offset = 0x12;
  size_t len = MEMLEN/2;
  uint8_t old_val = 0x23;
  uint8_t new_val = 0x42;

  /* First set entire region to a known value */
  my_memset(mem1, MEMLEN, old_val);
  for(size_t i=0; i<MEMLEN; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }

  /* Change the values starting at different alignments */

  memset_dma(mem1+offset, len, new_val);
  dma_wait(DMA_MEM_CHAN);

  /* Verify exactly half of values were set, others unchanged */
  for(size_t i=offset; i<len+offset; i++) {
    TEST_ASSERT_EQUAL_INT( new_val, mem1[i] );
  }
  // initial bytes unchanged
  for(size_t i=0; i<offset; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }
  // end bytes unchanged
  for(size_t i=len+offset+1; i<MEMLEN; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }
}

TEST(memset_dma, memset_unaligned_addr_03)
{
  size_t offset = 0x13;
  size_t len = MEMLEN/2;
  uint8_t old_val = 0x23;
  uint8_t new_val = 0x42;

  /* First set entire region to a known value */
  my_memset(mem1, MEMLEN, old_val);
  for(size_t i=0; i<MEMLEN; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }

  /* Change the values starting at different alignments */

  memset_dma(mem1+offset, len, new_val);
  dma_wait(DMA_MEM_CHAN);

  /* Verify exactly half of values were set, others unchanged */
  for(size_t i=offset; i<len+offset; i++) {
    TEST_ASSERT_EQUAL_INT( new_val, mem1[i] );
  }
  // initial bytes unchanged
  for(size_t i=0; i<offset; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }
  // end bytes unchanged
  for(size_t i=len+offset+1; i<MEMLEN; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }
}

TEST(memset_dma, memset_short_length)
{
  size_t offset = 0x10;
  size_t len = 3;
  uint8_t old_val = 0x23;
  uint8_t new_val = 0x42;

  /* First set entire region to a known value */
  my_memset(mem1, MEMLEN, old_val);
  for(size_t i=0; i<MEMLEN; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }

  /* Change the values starting at different alignments */

  memset_dma(mem1+offset, len, new_val);
  dma_wait(DMA_MEM_CHAN);

  /* Verify exactly half of values were set, others unchanged */
  for(size_t i=offset; i<len+offset; i++) {
    TEST_ASSERT_EQUAL_INT( new_val, mem1[i] );
  }
  // initial bytes unchanged
  for(size_t i=0; i<offset; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }
  // end bytes unchanged
  for(size_t i=len+offset+1; i<MEMLEN; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }
}

TEST(memset_dma, memset_unaligned_length_01)
{
  size_t offset = 0x10;
  size_t len = 32 + 1;
  uint8_t old_val = 0x23;
  uint8_t new_val = 0x42;

  /* First set entire region to a known value */
  my_memset(mem1, MEMLEN, old_val);
  for(size_t i=0; i<MEMLEN; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }

  /* Change the values starting at different alignments */

  memset_dma(mem1+offset, len, new_val);
  dma_wait(DMA_MEM_CHAN);

  /* Verify exactly half of values were set, others unchanged */
  for(size_t i=offset; i<len+offset; i++) {
    TEST_ASSERT_EQUAL_INT( new_val, mem1[i] );
  }
  // initial bytes unchanged
  for(size_t i=0; i<offset; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }
  // end bytes unchanged
  for(size_t i=len+offset+1; i<MEMLEN; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }
}


TEST(memset_dma, memset_unaligned_length_02)
{
  size_t offset = 0x10;
  size_t len = 32 + 2;
  uint8_t old_val = 0x23;
  uint8_t new_val = 0x42;

  /* First set entire region to a known value */
  my_memset(mem1, MEMLEN, old_val);
  for(size_t i=0; i<MEMLEN; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }

  /* Change the values starting at different alignments */

  memset_dma(mem1+offset, len, new_val);
  dma_wait(DMA_MEM_CHAN);

  /* Verify exactly half of values were set, others unchanged */
  for(size_t i=offset; i<len+offset; i++) {
    TEST_ASSERT_EQUAL_INT( new_val, mem1[i] );
  }
  // initial bytes unchanged
  for(size_t i=0; i<offset; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }
  // end bytes unchanged
  for(size_t i=len+offset+1; i<MEMLEN; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }
}


TEST(memset_dma, memset_unaligned_length_03)
{
  size_t offset = 0x10;
  size_t len = 32 + 3;
  uint8_t old_val = 0x23;
  uint8_t new_val = 0x42;

  /* First set entire region to a known value */
  my_memset(mem1, MEMLEN, old_val);
  for(size_t i=0; i<MEMLEN; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }

  /* Change the values starting at different alignments */

  memset_dma(mem1+offset, len, new_val);
  dma_wait(DMA_MEM_CHAN);

  /* Verify exactly half of values were set, others unchanged */
  for(size_t i=offset; i<len+offset; i++) {
    TEST_ASSERT_EQUAL_INT( new_val, mem1[i] );
  }
  // initial bytes unchanged
  for(size_t i=0; i<offset; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }
  // end bytes unchanged
  for(size_t i=len+offset+1; i<MEMLEN; i++) {
    TEST_ASSERT_EQUAL_INT( old_val, mem1[i] );
  }
}

/* Could be auto-generated using Unity helper scripts */
TEST_GROUP_RUNNER(memset_dma)
{
  RUN_TEST_CASE(memset_dma, memset_invalid_pointer);
  RUN_TEST_CASE(memset_dma, memset_returns_src);
  RUN_TEST_CASE(memset_dma, memset_sets_values);
  RUN_TEST_CASE(memset_dma, memset_short_length);
  RUN_TEST_CASE(memset_dma, memset_unaligned_addr_01);
  RUN_TEST_CASE(memset_dma, memset_unaligned_addr_02);
  RUN_TEST_CASE(memset_dma, memset_unaligned_addr_03);
  RUN_TEST_CASE(memset_dma, memset_unaligned_length_01);
  RUN_TEST_CASE(memset_dma, memset_unaligned_length_02);
  RUN_TEST_CASE(memset_dma, memset_unaligned_length_03);
}
