/**
 * @file test_memory.c
 * @brief CMocka unittests for memory functions
 *
 * @author Jeff Schornick
 * @date 2017/07/14
 **/

#include <stdarg.h>  /* for cmocka */
#include <stddef.h>  /* for cmocka */
#include <setjmp.h>  /* for cmocka */
#include <cmocka.h>
#include <stdlib.h>
#include "memory.h"

typedef struct {
  uint8_t *mem1;
  uint8_t *mem2;
} mem_t;


#define MEMLEN 256

static int setup(void **state)
{
  size_t size = MEMLEN;
  mem_t *mems = malloc(sizeof(mem_t));
  mems->mem1 = malloc(size);
  mems->mem2 = malloc(size);

  for(size_t i=0; i<size; i++) {
    mems->mem1[i] = (uint8_t) i;
    mems->mem2[i] = (uint8_t) i;
  }
  *state = mems;
  return 0;
}

static int teardown(void **state)
{
  mem_t *mems = *state;
  free(mems->mem1);
  free(mems->mem2);
  free(*state);
  return 0;
}

/*** memmove ***/

/* A null for either src or dst should fail gracefully. */
void test_memmove_invalid_pointer(void **state)
{
  uint8_t x[10];
  /* Any null pointer parameter should return a MEM_FAIL. */
  assert_int_equal( my_memmove(x,NULL,10), MEM_FAIL );
  assert_int_equal( my_memmove(NULL,x,10), MEM_FAIL );
}

/* A successful memmove should return dst pointer when successful */
void test_memmove_returns_dst(void **state)
{
  uint8_t x[10];
  assert_ptr_equal( my_memmove(x,x+1,1), x+1 );
}

/* Verify that a non-overlapping src/dst move the expected memory from
   src to dst */
void test_memmove_no_overlap(void **state)
{
  size_t src = 0;
  size_t dst = 50;
  size_t len = 20;
  mem_t *mems = *state;

  /* Verify src and dst initally fails test, since all bytes are different. */
  assert_memory_not_equal(mems->mem1 + dst, mems->mem2 + src, len);

  my_memmove(mems->mem1 + src, mems->mem1 + dst, len);

  /* Check the moved bytes in the new location */
  assert_memory_equal(mems->mem1 + dst, mems->mem2 + src, len);

  /* No other bytes after dst should have changed. */
  assert_memory_equal(mems->mem1 + dst+len, mems->mem2 + dst+len, MEMLEN - (dst+len));

}

/* Verify that when source begins somewhere in the destination range, the memory
   is still successfully copied to dst. */
void test_memmove_src_in_dst(void **state)
{
  size_t src = 50;
  size_t dst = 40;
  size_t len = 20;
  mem_t *mems = *state;

  /* Check prior state fails test, since all bytes are different. */
  assert_memory_not_equal(mems->mem1 + dst, mems->mem2 + src, len);

  /* Move len bytes somewhere just behind where our source begins */
  my_memmove(mems->mem1 + src, mems->mem1 + dst, len);

  /* Verify the moved bytes correct in the new location */
  assert_memory_equal(mems->mem1 + dst, mems->mem2 + src, len);

  /* Verify no other bytes have changed that shouldn't have */
  assert_memory_equal(mems->mem1, mems->mem2, dst); /* early bytes */
  assert_memory_equal(mems->mem1 + src+len, mems->mem2 + src+len, MEMLEN - (src+len)); /* end bytes */
}

/* Verify that when dst begins somewhere in the source range, the memory is
   still successfully copied to dst. */
void test_memmove_dst_in_src(void **state)
{
  size_t src = 40;
  size_t dst = 50;
  size_t len = 20;
  mem_t *mems = *state;

  /* Check prior state fails test, since all bytes are different. */
  assert_memory_not_equal(mems->mem1 + dst, mems->mem2 + src, len);

  /* Move len bytes somewhere just after where our source begins */
  my_memmove(mems->mem1 + src, mems->mem1 + dst, len);

  /* Verify the moved bytes in the new location */
  assert_memory_equal(mems->mem1 + dst, mems->mem2 + src, len);

  /* Verify no other bytes have changed that shouldn't have */
  assert_memory_equal(mems->mem1, mems->mem2, src); /* early bytes */
  assert_memory_equal(mems->mem1 + dst+len, mems->mem2 + dst+len, MEMLEN - (dst+len)); /* end bytes */
}

/*** memset ***/

/* A null for the src should fail gracefully. */
void test_memset_invalid_pointer(void **state)
{
  uint8_t len = 3;
  uint8_t val = 42;
  assert_int_equal( my_memset(NULL,len,val), MEM_FAIL );
}

/* A memset should return src pointer when successful */
void test_memset_returns_src(void **state)
{
  uint8_t x[10];
  /* Verify return value is src */
  assert_ptr_equal( my_memset(x,1,1), x );
}

/* Verify that memset actually sets the memory region correctly. */
void test_memset_values(void **state)
{
  size_t len = 20;
  uint8_t *mem = ((mem_t *)*state)->mem1;

  /* First set a region to a known value */
  my_memset(mem, len, 0x23);
  for(size_t i=0; i<len; i++) {
    assert_int_not_equal(mem[i], 0x42);
  }

  /* Verify that memset changed the value correctly */
  my_memset(mem, len, 0x42);
  for(size_t i=0; i<len; i++) {
    assert_int_equal(mem[i], 0x42);
  }
}

/*** memzero ***/

/* A null for the src should fail gracefully. */
void test_memzero_invalid_pointer(void **state)
{
  uint8_t len = 3;
  assert_int_equal( my_memzero(NULL,len), MEM_FAIL );
}

/* A memzero should return src pointer when successful */
void test_memzero_returns_src(void **state)
{
  uint8_t x[10];
  assert_ptr_equal( my_memzero(x,1), x );
}

/* Verify that memzero actually zeroes bytes in the memory region */
void test_memzero_zeroes(void **state)
{
  size_t len = 20;
  uint8_t *mem = ((mem_t *)*state)->mem1 + 50;

  /* start out non-zero */
  for(size_t i=0; i<len; i++) {
    assert_int_not_equal(mem[i], 0x0);
  }

  my_memzero(mem, len);

  /* Memory region should now be all zeros */
  for(size_t i=0; i<len; i++) {
    assert_int_equal(mem[i], 0x0);
  }
}


/*** reverse ***/

/* A null for the src should fail gracefully. */
void test_reverse_invalid_pointer(void **state)
{
  uint8_t len=20;
  assert_int_equal( my_reverse(NULL,len), MEM_FAIL );
}

/* A reverse should return src pointer when successful */
void test_reverse_returns_src(void **state)
{
  uint8_t x[10];
  assert_ptr_equal( my_reverse(x,1), x );
}

/* A range with an odd length should reverse correctly */
void test_reverse_odd_len(void **state)
{
  size_t memlen = 7;
  uint8_t input[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };
  uint8_t expected[] = { 0x01, 0x06, 0x05, 0x4, 0x03, 0x02, 0x07 };
  uint8_t *start = input+1;
  size_t revlen = memlen-2;

  my_reverse(start,revlen);
  /* Check that all bytes are the expected value */
  assert_memory_equal( input, expected, memlen );
}

/* A range with an even length should reverse correctly */
void test_reverse_even_len(void **state)
{
  size_t memlen = 6;
  uint8_t input[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };
  uint8_t expected[] = { 0x01, 0x05, 0x4, 0x03, 0x02, 0x06 };
  uint8_t *start = input+1;
  size_t revlen = memlen-2;

  assert_ptr_equal( my_reverse(start,revlen), start);
  /* Check that all bytes are the expected value */
  assert_memory_equal( input, expected, memlen );
}


/* Any value byte (0-255) should be able to be reversed */
void test_reverse_all_chars(void **state)
{
  uint8_t mem[256];
  uint8_t rev[256];
  /* Create a manually reversed copy of the fixture */
  for(size_t i=0; i<256; i++) {
    mem[i] = i;
    rev[i] = 255-i;
  }

  /* Check that we are not reversed before reversing */
  assert_memory_not_equal(mem, rev, 20);
  my_reverse(mem,256);
  /* Now all the bytes should be reversed */
  assert_memory_equal(mem, rev, 256);
}

int main(void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_memmove_invalid_pointer),
    cmocka_unit_test(test_memmove_returns_dst),
    cmocka_unit_test_setup_teardown(test_memmove_no_overlap, setup, teardown),
    cmocka_unit_test_setup_teardown(test_memmove_src_in_dst, setup, teardown),
    cmocka_unit_test_setup_teardown(test_memmove_dst_in_src, setup, teardown),

    cmocka_unit_test(test_memset_invalid_pointer),
    cmocka_unit_test(test_memset_returns_src),
    cmocka_unit_test_setup_teardown(test_memset_values, setup, teardown),

    cmocka_unit_test(test_memzero_invalid_pointer),
    cmocka_unit_test(test_memzero_returns_src),
    cmocka_unit_test_setup_teardown(test_memzero_zeroes, setup, teardown),

    cmocka_unit_test(test_reverse_invalid_pointer),
    cmocka_unit_test(test_reverse_returns_src),
    cmocka_unit_test(test_reverse_odd_len),
    cmocka_unit_test(test_reverse_even_len),
    cmocka_unit_test_setup_teardown(test_reverse_all_chars, setup, teardown)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}

