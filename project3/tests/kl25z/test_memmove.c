#include "memory.h"
#include "memory_dma.h"
#include "dma.h"
#include "unity_fixture.h"

TEST_GROUP(memmove_dma);

#define MEMLEN 256
extern uint8_t *mem1;
extern uint8_t *mem2;

uint8_t *mem1;
uint8_t *mem2;
//This is run before EACH TEST
TEST_SETUP(memmove_dma)
{

  mem1 = malloc(MEMLEN);
  mem2 = malloc(MEMLEN);

  for(size_t i=0; i<MEMLEN; i++) {
    mem1[i] = (uint8_t) i;
    mem2[i] = (uint8_t) i;
  }
}

TEST_TEAR_DOWN(memmove_dma)
{
  free(mem1);
  free(mem2);
}


TEST(memmove_dma, memmove_invalid_pointer)
{
  uint8_t x[10];
  /* Any null pointer parameter should return a MEM_FAIL. */
  TEST_ASSERT_EQUAL_INT( MEM_FAIL, memmove_dma(x,NULL,10) );
  TEST_ASSERT_EQUAL_INT( MEM_FAIL, memmove_dma(NULL,x,10) );
}

/* A successful memmove should return dst pointer when successful */
TEST(memmove_dma, memmove_returns_dst)
{
  uint8_t x[10];
  TEST_ASSERT_EQUAL_INT( x+1, memmove_dma(x,x+1,1) );
}

/* Verify that a non-overlapping src/dst move the expected memory from
   src to dst */
TEST(memmove_dma, memmove_dst_after_src_no_overlap)
{
  size_t src = 20;
  size_t dst = 40;
  size_t len = 20;

  memmove_dma(mem1 + src, mem1 + dst, len);

  /* Check the moved bytes in the new location */
  //TEST_ASSERT_EQUAL_MEMORY(mem1 + dst, mem2 + src, len);
  TEST_ASSERT_EQUAL_MEMORY(mem2 + src, mem1 + dst, len);

  /* No other bytes after dst should have changed. */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst+len, mem2 + dst+len, MEMLEN - (dst+len));
}

TEST(memmove_dma, memmove_src_after_dst_no_overlap)
{
  size_t src = 40;
  size_t dst = 20;
  size_t len = 20;

  memmove_dma(mem1 + src, mem1 + dst, len);

  /* Check the moved bytes in the new location */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst, mem2 + src, len);

  /* No other bytes after dst should have changed. */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst+len, mem2 + dst+len, MEMLEN - (dst+len));
}

/* Verify that a non-overlapping src/dst move the expected memory from
   src to dst */
TEST(memmove_dma, memmove_src_in_dst)
{
  size_t src = 50;
  size_t dst = 40;
  size_t len = 20;

  /* Move len bytes somewhere just behind where our source begins */
  memmove_dma(mem1 + src, mem1 + dst, len);

  /* Verify the moved bytes correct in the new location */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst, mem2 + src, len);

  /* Verify no other bytes have changed that shouldn't have */
  TEST_ASSERT_EQUAL_MEMORY(mem1, mem2, dst); /* early bytes */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + src+len, mem2 + src+len, MEMLEN - (src+len)); /* end bytes */
}

TEST(memmove_dma, memmove_src_in_dst_1_byte_overlap)
{
  size_t src = 41;
  size_t dst = 40;
  size_t len = 20;

  /* Move len bytes somewhere just behind where our source begins */
  memmove_dma(mem1 + src, mem1 + dst, len);

  /* Verify the moved bytes correct in the new location */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst, mem2 + src, len);

  /* Verify no other bytes have changed that shouldn't have */
  TEST_ASSERT_EQUAL_MEMORY(mem1, mem2, dst); /* early bytes */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + src+len, mem2 + src+len, MEMLEN - (src+len)); /* end bytes */
}

TEST(memmove_dma, memmove_dst_in_src)
{
  size_t src = 40;
  size_t dst = 49;
  size_t len = 20;

  /* Move len bytes somewhere just after where our source begins */
  memmove_dma(mem1 + src, mem1 + dst, len);

  /* Verify the moved bytes in the new location */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst, mem2 + src, len);

  /* Verify no other bytes have changed that shouldn't have */
  TEST_ASSERT_EQUAL_MEMORY(mem1, mem2, src); /* early bytes */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst+len, mem2 + dst+len, MEMLEN - (dst+len)); /* end bytes */
}

TEST(memmove_dma, memmove_dst_in_src_1_byte_overlap)
{
  size_t src = 40;
  size_t dst = 41;
  size_t len = 20;

  /* Move len bytes somewhere just after where our source begins */
  memmove_dma(mem1 + src, mem1 + dst, len);

  /* Verify the moved bytes in the new location */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst, mem2 + src, len);

  /* Verify no other bytes have changed that shouldn't have */
  TEST_ASSERT_EQUAL_MEMORY(mem1, mem2, src); /* early bytes */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst+len, mem2 + dst+len, MEMLEN - (dst+len)); /* end bytes */
}

TEST(memmove_dma, memmove_dst_in_src_2_byte_overlap)
{
  size_t src = 40;
  size_t dst = 42;
  size_t len = 20;

  /* Move len bytes somewhere just after where our source begins */
  memmove_dma(mem1 + src, mem1 + dst, len);

  /* Verify the moved bytes in the new location */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst, mem2 + src, len);

  /* Verify no other bytes have changed that shouldn't have */
  TEST_ASSERT_EQUAL_MEMORY(mem1, mem2, src); /* early bytes */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst+len, mem2 + dst+len, MEMLEN - (dst+len)); /* end bytes */
}

TEST(memmove_dma, memmove_src_dst_same)
{
  size_t src = 40;
  size_t dst = 40;
  size_t len = 20;

  /* Move len bytes somewhere just after where our source begins */
  memmove_dma(mem1 + src, mem1 + dst, len);

  /* Verify the moved bytes in the new location */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst, mem2 + src, len);

  /* Verify no other bytes have changed that shouldn't have */
  TEST_ASSERT_EQUAL_MEMORY(mem1, mem2, src); /* early bytes */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst+len, mem2 + dst+len, MEMLEN - (dst+len)); /* end bytes */
}

TEST(memmove_dma, memmove_dst_in_src_chunk32)
{
  size_t src = 1;
  size_t dst = src + 16;  // dst exact words away
  size_t len = 30;

  /* Move len bytes somewhere just after where our source begins */
  memmove_dma(mem1 + src, mem1 + dst, len);

  /* Verify the moved bytes in the new location */
  //TEST_ASSERT_EQUAL_MEMORY(mem1 + dst, mem2 + src, len);
  TEST_ASSERT_EQUAL_MEMORY(mem2 + src, mem1 + dst, len);

  /* Verify no other bytes have changed that shouldn't have */
  TEST_ASSERT_EQUAL_MEMORY(mem1, mem2, src); /* early bytes */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst+len, mem2 + dst+len, MEMLEN - (dst+len)); /* end bytes */
}

TEST(memmove_dma, memmove_dst_in_src_chunk16)
{
  size_t src = 11;
  size_t dst = src + 18 ;  // dst is halfwords away, but not even words away
  size_t len = 30;

  /* Move len bytes somewhere just after where our source begins */
  memmove_dma(mem1 + src, mem1 + dst, len);

  /* Verify the moved bytes in the new location */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst, mem2 + src, len);

  /* Verify no other bytes have changed that shouldn't have */
  TEST_ASSERT_EQUAL_MEMORY(mem1, mem2, src); /* early bytes */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst+len, mem2 + dst+len, MEMLEN - (dst+len)); /* end bytes */
}

TEST(memmove_dma, memmove_dst_in_src_chunk8)
{
  size_t src = 11;
  size_t dst = src + 19;  // dest is not a word or halfword multiple away
  size_t len = 30;

  /* Move len bytes somewhere just after where our source begins */
  memmove_dma(mem1 + src, mem1 + dst, len);

  /* Verify the moved bytes in the new location */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst, mem2 + src, len);

  /* Verify no other bytes have changed that shouldn't have */
  TEST_ASSERT_EQUAL_MEMORY(mem1, mem2, src); /* early bytes */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst+len, mem2 + dst+len, MEMLEN - (dst+len)); /* end bytes */
}

// start and end points of src/dst not word aligned
TEST(memmove_dma, memmove_dst_in_src_unaligned)
{
  size_t offset = (uint32_t) mem1 - MEM_ALIGN32((uint32_t) mem1);
  size_t src = 20 + (offset == 0) ? 1 : 0;  // guaranteed unaligned
  size_t dst = src + 16;  // 32-bit transfers
  size_t len = 30;

  /* Move len bytes somewhere just after where our source begins */
  memmove_dma(mem1 + src, mem1 + dst, len);

  /* Verify the moved bytes in the new location */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst, mem2 + src, len);

  /* Verify no other bytes have changed that shouldn't have */
  TEST_ASSERT_EQUAL_MEMORY(mem1, mem2, src); /* early bytes */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst+len, mem2 + dst+len, MEMLEN - (dst+len)); /* end bytes */
}

TEST(memmove_dma, memmove_src_in_dst_chunk32)
{
  //size_t dst = 40;
  size_t dst = 40;
  size_t src = dst + 16;  // moving 16-46  into 00
  size_t len = 30;  // at least two bytes not in dma

  /* Move len bytes somewhere just behind where our source begins */
  memmove_dma(mem1 + src, mem1 + dst, len);

  /* Verify the moved bytes correct in the new location */
  //TEST_ASSERT_EQUAL_MEMORY(mem1 + dst, mem2 + src, len);
  //                       expected     actual
  TEST_ASSERT_EQUAL_MEMORY(mem2 + src, mem1 + dst, len);

  /* Verify no other bytes have changed that shouldn't have */
  TEST_ASSERT_EQUAL_MEMORY(mem1, mem2, dst); /* early bytes */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + src+len, mem2 + src+len, MEMLEN - (src+len)); /* end bytes */
}

TEST(memmove_dma, memmove_src_in_dst_chunk16)
{
  size_t dst = 40;
  size_t src = dst + 18;
  size_t len = 30;

  /* Move len bytes somewhere just behind where our source begins */
  memmove_dma(mem1 + src, mem1 + dst, len);

  /* Verify the moved bytes correct in the new location */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst, mem2 + src, len);

  /* Verify no other bytes have changed that shouldn't have */
  TEST_ASSERT_EQUAL_MEMORY(mem1, mem2, dst); /* early bytes */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + src+len, mem2 + src+len, MEMLEN - (src+len)); /* end bytes */
}

TEST(memmove_dma, memmove_src_in_dst_chunk8)
{
  size_t dst = 40;
  size_t src = dst + 19;
  size_t len = 30;

  /* Move len bytes somewhere just behind where our source begins */
  memmove_dma(mem1 + src, mem1 + dst, len);

  /* Verify the moved bytes correct in the new location */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst, mem2 + src, len);

  /* Verify no other bytes have changed that shouldn't have */
  TEST_ASSERT_EQUAL_MEMORY(mem1, mem2, dst); /* early bytes */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + src+len, mem2 + src+len, MEMLEN - (src+len)); /* end bytes */
}

// start and end points of src/dst not word aligned
TEST(memmove_dma, memmove_src_in_dst_unaligned)
{
  size_t offset = (uint32_t) mem1 - MEM_ALIGN32((uint32_t) mem1);
  size_t dst = 20 + (offset == 0) ? 1 : 0;  // guaranteed unaligned
  size_t src = dst + 16;  //  32-bit transfers
  size_t len = 30;

  /* Move len bytes somewhere just behind where our source begins */
  memmove_dma(mem1 + src, mem1 + dst, len);

  /* Verify the moved bytes correct in the new location */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + dst, mem2 + src, len);

  /* Verify no other bytes have changed that shouldn't have */
  TEST_ASSERT_EQUAL_MEMORY(mem1, mem2, dst); /* early bytes */
  TEST_ASSERT_EQUAL_MEMORY(mem1 + src+len, mem2 + src+len, MEMLEN - (src+len)); /* end bytes */
}
/* Could be auto-generated using Unity helper scripts */
TEST_GROUP_RUNNER(memmove_dma)
{
  RUN_TEST_CASE(memmove_dma, memmove_invalid_pointer);
  RUN_TEST_CASE(memmove_dma, memmove_returns_dst);
  RUN_TEST_CASE(memmove_dma, memmove_src_dst_same);

  RUN_TEST_CASE(memmove_dma, memmove_src_after_dst_no_overlap);
  RUN_TEST_CASE(memmove_dma, memmove_src_in_dst);
  RUN_TEST_CASE(memmove_dma, memmove_src_in_dst_1_byte_overlap);
  RUN_TEST_CASE(memmove_dma, memmove_src_in_dst_chunk32);
  RUN_TEST_CASE(memmove_dma, memmove_src_in_dst_chunk16);
  RUN_TEST_CASE(memmove_dma, memmove_src_in_dst_chunk8);
  RUN_TEST_CASE(memmove_dma, memmove_src_in_dst_unaligned);

  RUN_TEST_CASE(memmove_dma, memmove_dst_after_src_no_overlap);
  RUN_TEST_CASE(memmove_dma, memmove_dst_in_src);
  RUN_TEST_CASE(memmove_dma, memmove_dst_in_src_1_byte_overlap);
  RUN_TEST_CASE(memmove_dma, memmove_dst_in_src_2_byte_overlap);
  RUN_TEST_CASE(memmove_dma, memmove_dst_in_src_chunk32);
  RUN_TEST_CASE(memmove_dma, memmove_dst_in_src_chunk16);
  RUN_TEST_CASE(memmove_dma, memmove_dst_in_src_chunk8);
  RUN_TEST_CASE(memmove_dma, memmove_dst_in_src_unaligned);
}
