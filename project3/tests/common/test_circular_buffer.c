/**
 * @file test_circular_buffer.c
 * @brief CMocka unittests for circular buffer functions
 *
 * @author Jeff Schornick
 * @date 2017/07/14
 **/

#include <stdarg.h>  /* for cmocka */
#include <stddef.h>  /* for cmocka */
#include <setjmp.h>  /* for cmocka */
#include <cmocka.h>
#include <stdlib.h>
#include "circular_buffer.h"

/* Verify that CB_init and CB_destory handle dynamic allocation/deallocation of
 * the data buffer. */
void test_circbuf_allocate_free(void **state)
{
  CircBuf_t cb1, cb2;

  /* Force the buffer pointer to null to ensure allocation occurs */
  cb1.buffer = NULL;
  cb2.buffer = NULL;

  /* No data buffers should be NULL after allocation */
  CB_init(&cb1, 10);
  CB_init(&cb2, 10);

  /* Different heap memory should be allocated for each */
  assert_non_null(cb1.buffer);
  assert_non_null(cb2.buffer);
  assert_ptr_not_equal(cb1.buffer, cb2.buffer);

  /* Ensure destroy completes succesfully */
  assert_int_equal(CB_destroy(&cb1), CB_OK);
  assert_int_equal(CB_destroy(&cb2), CB_OK);
}

/* Verify CB_init initizlizes the CB record members properly. */
void test_circbuf_initialized(void **state)
{
  CircBuf_t cb;
  size_t size = 10;

  CB_init(&cb, size);

  /* After CB_init, all members should be set to the default values. */
  assert_non_null(cb.buffer);
  assert_int_equal(cb.count, 0);
  assert_int_equal(cb.size, size);
  assert_ptr_equal(cb.head, cb.buffer);
  assert_ptr_equal(cb.tail, cb.buffer);
}

/* Do all the CB functions handle null pointers gracefully? */
void test_circbuf_handles_null(void **state)
{
  CircBuf_t cb;
  uint8_t x = 42;

  /* Check that we can handle NULL CircBuf_t pointers. Any null parameter should
     result in a status of CB_NULL. */
  assert_int_equal( CB_init(NULL, 1), CB_NULL );
  assert_int_equal( CB_destroy(NULL), CB_NULL );
  assert_int_equal( CB_add_item(NULL, x), CB_NULL );
  assert_int_equal( CB_remove_item(NULL, &x), CB_NULL );
  assert_int_equal( CB_is_full(NULL), CB_NULL );
  assert_int_equal( CB_is_empty(NULL), CB_NULL );
  assert_int_equal( CB_peek(NULL, 1, &x), CB_NULL );

  /* Check that we can handle NULL item pointers as well. */
  CB_init(&cb, 10);
  assert_int_equal( CB_remove_item(&cb, NULL), CB_NULL );
  assert_int_equal( CB_peek(&cb, 1, NULL), CB_NULL );
}

/* Ensure we can add and remove the same items for the entire length of the
   buffer, receiving OK status each time. */
void test_circbuf_add_remove(void **state)
{
  CircBuf_t cb;
  size_t size = 5;
  uint8_t item;

  CB_init(&cb, size);

  /* Push the values 1-5 into the circular buffer, verify status is CB_OK. */
  for( uint8_t i=0; i<size; i++) {
    assert_int_equal( CB_add_item(&cb, i), CB_OK );
  }

  /* Verify that we get the 5 items back in the same order, each with CB_OK
     status. */
  for( uint8_t i=0; i<size; i++) {
    assert_int_equal( CB_remove_item(&cb, &item), CB_OK );
    assert_int_equal(item, i);
  }
}

/* Check that a filled circular buffer reports full, but before! */
void test_circbuf_reports_full(void **state)
{
  CircBuf_t cb;
  size_t size = 10;
  uint8_t item;

  CB_init(&cb, size);

  /* Push all but the last item onto the buffer */
  for( uint8_t i=0; i<size-1; i++) {
    CB_add_item(&cb, i);
    assert_false(CB_is_full(&cb));
  }
  /* Add the last item, which should fill the buffer. Fullness check shoudl
     return true. */
  CB_add_item(&cb, 42);
  assert_true(CB_is_full(&cb));

  /* Remove an item to make space. */
  CB_remove_item(&cb, &item);
  assert_false(CB_is_full(&cb));

  /* Put it back to check that the buffer is full again. */
  CB_add_item(&cb, 23);
  assert_true(CB_is_full(&cb));

}

/* Check that an empty circular buffer reports empty, but not when it has items. */
void test_circbuf_reports_empty(void **state)
{
  CircBuf_t cb;
  size_t size = 5;
  uint8_t item;

  CB_init(&cb, size);

  /* New buffer should be empty. Empty status should be true. */
  assert_true(CB_is_empty(&cb));

  /* Push some items onto the buffer, should no longer report empty. */
  for( uint8_t i=0; i<size; i++) {
    CB_add_item(&cb, i);
    assert_false(CB_is_empty(&cb));
  }
  /* Remove all but one item */
  for( uint8_t i=0; i<size-1; i++) {
    CB_remove_item(&cb, &item);
    assert_false(CB_is_empty(&cb));
  }
  /* Remove last item, and verify that status is now empty. */
  CB_remove_item(&cb, &item);
  assert_true(CB_is_empty(&cb));
}

/* Make sure the circular buffer can wrap past the end when adding, which should
   add additional items starting at the beginning of the buffer. */
void test_circbuf_wraps_on_add(void **state)
{
  CircBuf_t cb;
  size_t size = 3;
  uint8_t item;
  uint8_t *head_ptr;

  CB_init(&cb, size);

  /* Put the first item on and record the head pointer for eventual test. */
  CB_add_item(&cb, 1);
  head_ptr = (uint8_t *) cb.head;

  /* Fill the rest of the buffer to full */
  CB_add_item(&cb, 2);
  CB_add_item(&cb, 3);

  /* Remove the first item to make space */
  CB_remove_item(&cb, &item);

  /* Put one more item at head, which should have wrapped around. The original
     head_ptr should match the new head_ptr. */
  CB_add_item(&cb, 23);
  assert_ptr_equal(cb.head, head_ptr);

  /* Make sure we can wrap many times in a small buffer. */
  CB_destroy(&cb);
  CB_init(&cb, size);
  for(int i=1; i<100; i++){
    CB_add_item(&cb, i);
    if(i>=size) {
      CB_remove_item(&cb, &item);
      assert_int_equal(item, i-(size-1));
    }
  }
}

/* Make sure the cicular buffer removes can wrap past the beginning of the
   buffer, which should remove from the end of the buffer instead. */
void test_circbuf_wraps_on_remove(void **state)
{
  CircBuf_t cb;
  size_t size = 3;
  uint8_t *tail_ptr;
  uint8_t item;

  CB_init(&cb, size);

  /* Put the first item on and record the tail pointer */
  CB_add_item(&cb, 1);
  tail_ptr = (uint8_t *) cb.tail;

  /* Add and remove "size" times to guarantee wrap around  */
  CB_add_item(&cb, 2);
  CB_remove_item(&cb, &item);
  CB_add_item(&cb, 3);
  CB_remove_item(&cb, &item);
  CB_add_item(&cb, 4);
  CB_remove_item(&cb, &item);

  /* Verify that the new tail_ptr is back where we started */
  assert_ptr_equal(cb.tail, tail_ptr);
}

/* Make sure we fail gracefully when adding to full buffer. */
void test_circbuf_handles_overfull(void **state)
{
  CircBuf_t cb;
  size_t size = 10;

  CB_init(&cb, size);

  /* Push all but the last item onto the buffer */
  for( uint8_t i=0; i<size-1; i++) {
    CB_add_item(&cb, i);
    assert_false(CB_is_full(&cb));
  }
  /* Add the last item, which should fill the buffer without error */
  assert_int_equal( CB_add_item(&cb, 42), CB_OK);
  assert_true(CB_is_full(&cb));

  /* Try to add past full, should report FULL instead of OK. */
  assert_int_equal( CB_add_item(&cb, 23), CB_FULL);
}

/* Make sure we fail gracefully when removing from empty buffer */
void test_circbuf_handles_overempty(void **state)
{
  CircBuf_t cb;
  size_t size = 10;
  uint8_t item;

  CB_init(&cb, size);
  CB_add_item(&cb, 42);

  /* Remove of single item should work, leaving buffer empty */
  assert_int_equal( CB_remove_item(&cb, &item), CB_OK);
  assert_true(CB_is_empty(&cb));

  /* Trying to remove from empty should report EMPTY instead of OK*/
  assert_int_equal( CB_remove_item(&cb, &item), CB_EMPTY );
}

/* Make sure CB_peek returns the correct value. */
void test_circbuf_peek_returns_value(void **state)
{
  CircBuf_t cb;
  size_t size = 10;
  uint8_t item;

  CB_init(&cb, size);

  /* Push some items onto the buffer */
  for( uint8_t i=0; i<=5; i++) {
    CB_add_item(&cb, i);
  }

  /* Look back 2 items, make sure we get the right item. */
  CB_peek(&cb, 2, &item);
  assert_int_equal(item, 3);
}

/* Make sure peek can wrap around from the beginning to end and retreive the
   correct value. */
void test_circbuf_peek_wraps_around(void **state)
{
  CircBuf_t cb;
  size_t size = 10;
  uint8_t item;

  CB_init(&cb, size);

  /* Fill up the buffer */
  for( uint8_t i=0; i<size; i++) {
    CB_add_item(&cb, 99);
  }

  /* Remove/add to guarantee wrap around */
  for( uint8_t i=42; i<42+size; i++) {
    CB_remove_item(&cb, &item);
    CB_add_item(&cb, i);
  }
  /* Look back "size-1" items, make we wrap around to first item */
  assert_int_equal( CB_peek(&cb, size-1, &item), CB_OK);
  assert_int_equal(item, 42);

}

/* Check that peek returns an error if we look back too far. */
void test_circbuf_peek_checks_size(void **state)
{
  CircBuf_t cb;
  size_t size = 10;
  uint8_t item;

  CB_init(&cb, size);

  /* Push some items onto the buffer */
  for( uint8_t i=0; i<=5; i++) {
    CB_add_item(&cb, i);
  }

  /* A valid peek position should return CB_OK. */
  assert_int_equal( CB_peek(&cb, 3, &item), CB_OK);
  /* A position too large for our data should return CB_SIZE_ERR */
  assert_int_equal( CB_peek(&cb, 7, &item), CB_SIZE_ERR);

}

int main(void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(test_circbuf_allocate_free),
    cmocka_unit_test(test_circbuf_initialized),
    cmocka_unit_test(test_circbuf_handles_null),
    cmocka_unit_test(test_circbuf_add_remove),
    cmocka_unit_test(test_circbuf_reports_full),
    cmocka_unit_test(test_circbuf_reports_empty),
    cmocka_unit_test(test_circbuf_wraps_on_add),
    cmocka_unit_test(test_circbuf_wraps_on_remove),
    cmocka_unit_test(test_circbuf_handles_overfull),
    cmocka_unit_test(test_circbuf_handles_overempty),
    cmocka_unit_test(test_circbuf_peek_returns_value),
    cmocka_unit_test(test_circbuf_peek_wraps_around),
    cmocka_unit_test(test_circbuf_peek_checks_size)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}

