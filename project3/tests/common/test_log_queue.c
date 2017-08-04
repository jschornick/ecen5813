/**
 * @file test_log_queue.c
 * @brief CMocka unittests for log queue
 *
 * @author Jeff Schornick
 * @date 2017/07/29
 **/

#include <stdarg.h>  /* for cmocka */
#include <stddef.h>  /* for cmocka */
#include <setjmp.h>  /* for cmocka */
#include <cmocka.h>
#include <stdlib.h>
#include "log_queue.h"

#include <stdio.h>

/* Verify that lq_init and lq_destory handle dynamic allocation/deallocation of
 * the data buffer. */
void log_queue_allocate_free(void **state)
{
  Log_q lq1, lq2;

  /* Force the buffer pointer to null to ensure allocation occurs */
  lq1.buffer = NULL;
  lq2.buffer = NULL;

  /* No data buffers should be NULL after allocation */
  lq_init(&lq1, 10);
  lq_init(&lq2, 10);

  /* Different heap memory should be allocated for each */
  assert_non_null(lq1.buffer);
  assert_non_null(lq2.buffer);
  assert_ptr_not_equal(lq1.buffer, lq2.buffer);

  /* Ensure destroy completes succesfully */
  assert_int_equal(lq_destroy(&lq1), LQ_OK);
  assert_int_equal(lq_destroy(&lq2), LQ_OK);

  assert_null(lq1.buffer);
}

void log_queue_initialize(void **state)
{
  Log_q lq;
  size_t size = 10;

  lq_init(&lq, size);

  /* After lq_init, all members should be set to the default values. */
  assert_non_null(lq.buffer);
  assert_int_equal(lq.free, size);
  assert_int_equal(lq.size, size);
  assert_ptr_equal(lq.head, lq.buffer);
  assert_ptr_equal(lq.tail, lq.buffer);
}

/* Do all the log queue functions handle null pointers gracefully? */
void log_queue_handles_null(void **state)
{
  Log_q lq;
  Log_t x = {LOGGER_INITIALIZED, 1234, 0, 0, NULL};

  /* Check that we can handle NULL Log_q pointers. Any null parameter should
     result in a status of LQ_NULL. */
  assert_int_equal( lq_init(NULL, 1), LQ_NULL );
  assert_int_equal( lq_destroy(NULL), LQ_NULL );
  assert_int_equal( lq_add(NULL, &x), LQ_NULL );
  assert_int_equal( lq_remove(NULL, &x), LQ_NULL );
  assert_int_equal( lq_empty(NULL), LQ_NULL );

  /* Check that we can handle NULL item pointers as well. */
  lq_init(&lq, 10);
  assert_int_equal( lq_remove(&lq, NULL), LQ_NULL );
}

/* Ensure we can add and remove the same items for the entire length of the
   buffer, receiving OK status each time. */
void log_queue_add_remove_no_data(void **state)
{
  Log_q lq;
  size_t count = 5;
  size_t size = count * LOG_HEADER_SIZE;  /* no data in this test */
  Log_t item;

  lq_init(&lq, size);

  /* Push the five unique log items onto the queue, verify status is LQ_OK. */
  for( uint8_t i=0; i<count; i++) {
    item.id = i;
    item.time = 100+i;
    item.length = 0;
    assert_int_equal( lq_add(&lq, &item), LQ_OK );
  }

  /* Verify that we get the 5 items back in the same order, each with CB_OK
     status. */
  for( uint8_t i=0; i<count; i++) {
    assert_int_equal( lq_remove(&lq, &item), LQ_OK );
    assert_int_equal(item.id, i);
    assert_int_equal(item.time, 100+i);
    assert_int_equal(item.length, 0);
  }
}

void log_queue_add_remove_with_data(void **state)
{
  Log_q lq;
  size_t count = 5;
  size_t size = count * (LOG_HEADER_SIZE + 2); /* header + data of each*/
  Log_t item;
  Log_t tmp_item;

  tmp_item.data = malloc(10);
  tmp_item.length = 10;

  lq_init(&lq, size);

  /* Each item will get two of these values */
  uint8_t data[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 };

  /* Push the five unique log items onto the queue, verify status is LQ_OK. */
  for( uint8_t i=0; i<count; i++) {
    item.id = i;
    item.time = 100+i;
    item.length = 2;
    item.data = &data[i];
    assert_int_equal( lq_add(&lq, &item), LQ_OK );
  }

  /* Verify that we get the 5 items back in the same order, each with CB_OK
     status. */
  for( uint8_t i=0; i<count; i++) {
    assert_int_equal( lq_remove(&lq, &tmp_item), LQ_OK );
    assert_int_equal(tmp_item.id, i);
    assert_int_equal(tmp_item.time, 100+i);
    assert_int_equal(tmp_item.length, 2);
    assert_memory_equal(tmp_item.data, &data[i], 2);
  }
}

void log_queue_wrap_header(void **state)
{
  Log_q lq;
  size_t size = sizeof(Log_t) + 20;
  Log_t item_msg;
  Log_t item_empty;
  Log_t item_tmp;
  Log_t item_test;

  // 16 byte header + 15 byte data
  item_msg.id = 42;
  item_msg.time = 1001;
  item_msg.data = (uint8_t *) "A long message";
  item_msg.length = 15;

  item_tmp.id = 0;
  item_tmp.time = 0;
  item_tmp.length = 20;  // at least as big as message
  item_tmp.data = malloc(20);

  item_empty.id = 23;
  item_empty.time = 1002;
  item_empty.length = 0;

  item_test.id = 0;
  item_test.time = 0;
  item_test.length = 0;

  lq_init(&lq, size);

  // Add and remove item 1 to advance buffer
  assert_int_equal( lq_add(&lq, &item_msg), LQ_OK );
  assert_int_equal( lq_remove(&lq, &item_tmp), LQ_OK );
  assert_true( lq_empty(&lq) );
  assert_int_equal( lq.tail, lq.head );

  // Item 1 used 16 bytes header + 15 data,
  //   so we're 31 bytes into the buffer for item_empty
  // Item 2 header won't fit without wrapping
  assert_int_equal( lq_add(&lq, &item_empty), LQ_OK );
  // Check that head wrapped around
  assert_true( lq.head < lq.tail );
  assert_int_equal( lq_remove(&lq, &item_test), LQ_OK );
  // Check that tail wrapped around
  assert_int_equal( lq.head, lq.tail );

  // Check that the full header is retreived intact
  assert_int_equal( item_test.id, item_empty.id);
  assert_int_equal( item_test.length, item_empty.length);
  assert_int_equal( item_test.time, item_empty.time);

  free(item_tmp.data);
}

void log_queue_wrap_data(void **state)
{
  Log_q lq;
  size_t size = 50; // will wrap at data of second add
  Log_t item_msg1;
  Log_t item_msg2;
  Log_t item_tmp;
  Log_t item_test;

  // 16 byte header + 15 byte data
  item_msg1.id = 42;
  item_msg1.time = 1001;
  item_msg1.data = (uint8_t *) "A long message";
  item_msg1.length = 15;

  // 16 byte header + 15 byte data
  item_msg2.id = 23;
  item_msg2.time = 1002;
  item_msg2.data = (uint8_t *) "An even longer message";
  item_msg2.length = 23;

  item_tmp.id = 0;
  item_tmp.time = 0;
  item_tmp.length = 20;  // at least as big as message 1
  item_tmp.data = malloc(20);

  item_test.id = 0;
  item_test.time = 0;
  item_test.length = 25;
  item_test.data = malloc(25); // at least as big as message 2

  lq_init(&lq, size);

  // Add and remove first item to advance buffer
  assert_int_equal( lq_add(&lq, &item_msg1), LQ_OK );
  assert_int_equal( lq_remove(&lq, &item_tmp), LQ_OK );

  // Item 1 used 16 bytes header + 15 data,
  //   so we're 31 bytes into the buffer for item_empty
  // Item 2 header will fit before wrap, but not the data
  assert_int_equal( lq_add(&lq, &item_msg2), LQ_OK );
  // Check that head wrapped around
  assert_true( lq.head < lq.tail );
  assert_int_equal( lq_remove(&lq, &item_test), LQ_OK );
  // Check that tail wrapped around
  assert_int_equal( lq.head, lq.tail );

  // Check everything is retreived intact
  assert_int_equal( item_test.id, item_msg2.id);
  assert_int_equal( item_test.length, item_msg2.length);
  assert_int_equal( item_test.time, item_msg2.time);
  assert_memory_equal( item_test.data, item_msg2.data, item_msg2.length);

  free(item_tmp.data);

}

void log_queue_drops_if_item_too_small(void **state)
{
  Log_q lq;
  size_t size = 50;
  Log_t item_msg;
  Log_t item_small;

  // 16 byte header + 15 byte data
  item_msg.id = 42;
  item_msg.time = 1001;
  item_msg.data = (uint8_t *) "A long message";
  item_msg.length = 15;

  item_small.id = 0;
  item_small.time = 0;
  item_small.length = 10;  // not enough space for message
  item_small.data = malloc(10);

  lq_init(&lq, size);

  assert_int_equal( lq_add(&lq, &item_msg), LQ_OK );

  // ok to remove into smaller item, but queue should stay consistent
  assert_int_equal( lq_remove(&lq, &item_small), LQ_OK );
  assert_true( lq_empty(&lq) );
  assert_int_equal( lq.head, lq.tail );
  assert_int_equal( item_small.length, 10 );
  assert_memory_equal( item_small.data, item_msg.data, item_small.length );

  free(item_small.data);
}


/* Check that adding without enough space reports full, but not before! */
void log_queue_reports_full(void **state)
{
  Log_q lq;
  size_t count = 5;
  size_t size = count * LOG_HEADER_SIZE;
  Log_t item_no_data;
  Log_t item_with_data;
  Log_t tmp_item;

  lq_init(&lq, size);

  item_no_data.id = 1;
  item_no_data.time = 23;
  item_no_data.length = 0;

  item_with_data.id = 2;
  item_with_data.time = 42;
  item_with_data.length = 1;
  item_with_data.data = (uint8_t *) "x";

  /* Five items should fit */
  for( uint8_t i=0; i<count; i++) {
    assert_int_equal( lq_add(&lq, &item_no_data), LQ_OK) ;
  }
  /* Add another last item, which should not fit. */
  assert_int_equal( lq_add(&lq, &item_no_data), LQ_FULL );

  /* Remove an item to make space. */
  assert_int_equal( lq_remove(&lq, &tmp_item), LQ_OK );
  assert_int_equal( lq_add(&lq, &item_no_data), LQ_OK );

  /* Remove again, try to add larger item (due to data) */
  assert_int_equal( lq_remove(&lq, &tmp_item), LQ_OK );
  assert_int_equal( lq_add(&lq, &item_with_data), LQ_FULL );

  /* Remove another, large item should fit now */
  assert_int_equal( lq_remove(&lq, &tmp_item), LQ_OK );
  assert_int_equal( lq_add(&lq, &item_with_data), LQ_OK );

  /* But not even one more w/o data */
  assert_int_equal( lq_add(&lq, &item_no_data), LQ_FULL );
}

int main(void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(log_queue_allocate_free),
    cmocka_unit_test(log_queue_handles_null),
    cmocka_unit_test(log_queue_add_remove_no_data),
    cmocka_unit_test(log_queue_add_remove_with_data),
    cmocka_unit_test(log_queue_wrap_header),
    cmocka_unit_test(log_queue_wrap_data),
    cmocka_unit_test(log_queue_drops_if_item_too_small),
    cmocka_unit_test(log_queue_reports_full),
    cmocka_unit_test(log_queue_initialize)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
