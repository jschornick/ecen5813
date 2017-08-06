/**
 * @file log_queue.c
 * @brief Log queue function declarations
 *
 * A fixed sized logging queue.
 *
 * @author Jeff Schornick
 * @date 2017/07/27
**/

#include <stdlib.h>
#include "memory.h"
#include "platform.h"
#include "logger.h"
#include "log_queue.h"

Log_status_t lq_init(Log_q *queue, size_t size)
{
  if( queue == NULL ) {
    return LQ_NULL;
  }
  if( size <= 0 ) {
    return LQ_SIZE_ERR;
  }

  queue->buffer = malloc( size );
  if( queue->buffer == NULL )
  {
    return LQ_ALLOC_ERR;
  }
  queue->head = queue->buffer;
  queue->tail = queue->buffer;
  queue->size = size;
  queue->free = size;

  return LQ_OK;
}

Log_status_t lq_destroy(Log_q *queue)
{
  if( queue == NULL ) {
    return LQ_NULL;
  }

  free((void *) queue->buffer);
  queue->buffer = NULL;
  queue->size = 0;
  queue->free = 0;

  return LQ_OK;
}

/* internal use only, does not perform any checking */
void lq_add_bytes(Log_q *queue, uint8_t *bytes, size_t length)
{
  size_t contiguous = (queue->buffer + queue->size) - queue->head;
  if( contiguous < length )  /* wrap required */
  {
    // First copy the data which fits up to the end of the buffer
    my_memcpy( bytes, (uint8_t *) queue->head, contiguous );
    // Then copy the remainer at the beginning
    my_memcpy( &bytes[contiguous], (uint8_t *) queue->buffer, (length - contiguous) );
    // Offset head to reflect wrap
    queue->head = queue->buffer + (length - contiguous);
  }
  else /* all bytes fit without wrapping*/
  {
    my_memcpy( bytes, (uint8_t *) queue->head, length);
    queue->head += length;
  }

  queue->free -= length;
}

Log_status_t lq_add(Log_q *queue, Log_t *item)
{
  if( queue == NULL ) {
    return LQ_NULL;
  }

  /* The item header and data will be stored directly on the queue, but not the data pointer */
  if( queue->free < (LOG_HEADER_SIZE + item->length) ) {
    return LQ_FULL;
  }

  START_CRITICAL();
  lq_add_bytes(queue, (uint8_t *) item, LOG_HEADER_SIZE);
  lq_add_bytes(queue, item->data, item->length);
  END_CRITICAL();

  return LQ_OK;
}


/* internal use only, does not perform any checking */
void lq_remove_bytes(Log_q *queue, uint8_t *bytes, size_t length)
{
  size_t contiguous = (queue->buffer + queue->size) - queue->tail;
  if( contiguous < length )  /* wrap required */
  {
    // First copy the data which fits up to the end of the buffer
    my_memcpy( (uint8_t *) queue->tail, bytes, contiguous );
    // Then copy the remainer at the beginning
    my_memcpy( (uint8_t *) queue->buffer, &bytes[contiguous], (length - contiguous) );
    // Offset tail to reflect wrap
    queue->tail = queue->buffer + (length - contiguous);
  }
  else /* all bytes fit without wrapping*/
  {
    my_memcpy( (uint8_t *) queue->tail, bytes, length );
    queue->tail += length;
  }
  queue->free += length;
}

/* internal only, no checking! */
void lq_drop_bytes(Log_q *queue, size_t length)
{
  queue->tail += length;
  if( queue->tail >= (queue->buffer + queue->size) ) {
    queue->tail -= queue->size;
  }
  queue->free += length;
}

Log_status_t lq_remove(Log_q *queue, Log_t *item)
{
  if( (queue == NULL) || (item == NULL) ) {
    return LQ_NULL;
  }
  if( lq_empty(queue) ) {
    return LQ_EMPTY;
  }

  size_t max_data = item->length;  /* save so we don't copy in too much data */
  size_t copy_size;

  START_CRITICAL();
  /* read header */
  lq_remove_bytes(queue, (uint8_t *) item, LOG_HEADER_SIZE);
  copy_size = (max_data < item->length) ? max_data : item->length;
  /* read data */
  lq_remove_bytes(queue, item->data, copy_size);
  if( copy_size < item->length ) {
    lq_drop_bytes(queue, item->length - copy_size);
  }
  END_CRITICAL();

  item->length = copy_size;

  return LQ_OK;
}

Log_status_t lq_empty(Log_q *queue)
{
  if( queue == NULL ) {
    return LQ_NULL;
  }
  if( queue->free != queue->size )
  {
    return LQ_FALSE;
  }
  return LQ_EMPTY;
}

