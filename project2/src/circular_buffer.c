/**
 * @file circular_buffer.c
 * @brief Circular buffer function definitions
 *
 * Implementaiton of the circular buffer.
 *
 * @author Jeff Schornick
 * @date 2017/07/12
 **/

#include <stdint.h>
#include <stdlib.h>
#include "circular_buffer.h"
#include "platform.h"

CB_status_t CB_init(CircBuf_t *circbuf, size_t size)
{
  if( circbuf == NULL )
  {
    return CB_NULL;
  }
  if( size <= 0 )
  {
    return CB_SIZE_ERR; /* Need a postive size */
  }

  circbuf->buffer = malloc(sizeof(cb_item_t) * size);
  if( circbuf->buffer == NULL )
  {
    return CB_ALLOC_ERR;
  }
  circbuf->head = circbuf->buffer;
  circbuf->tail = circbuf->buffer;
  circbuf->size = size;
  circbuf->count = 0;

  return CB_OK;
}

CB_status_t CB_destroy(CircBuf_t *circbuf)
{
  if( circbuf == NULL )
  {
    return CB_NULL;
  }
  free((void *) circbuf->buffer);
  return CB_OK;
}

CB_status_t CB_add_item(CircBuf_t *circbuf, uint8_t item)
{
  if( circbuf == NULL )
  {
    return CB_NULL;
  }
  if( circbuf->count == circbuf->size )
  {
    return CB_FULL;
  }

  /* NOTE: The modification of the head pointer, data buffer, and item count
     below must be treated as a critical section. The code needs to be protected
     to guarantee consistency when the circular buffer is modified and accessed
     simultaneously both in and out of interrupt. */

  critical_start();
  if( ++(circbuf->head) == (circbuf->buffer + circbuf->size) )
  {
    circbuf->head = circbuf->buffer;
  }
  *(circbuf->head) = item;
  circbuf->count++;
  critical_end();

  return CB_OK;
}

CB_status_t CB_remove_item(CircBuf_t *circbuf, uint8_t *item)
{
  if( circbuf == NULL || item == NULL)
  {
    return CB_NULL;
  }
  if( circbuf->count == 0 )
  {
    return CB_EMPTY;
  }

  /* NOTE: The modification of the tail pointer, data buffer, and item count
     must be treated as a critical section. The code needs to be protected to
     guarantee consistency when the circular buffer is modified and accessed
     simultaneously both in and out of interrupt. */

  critical_start();
  if( ++(circbuf->tail) == (circbuf->buffer + circbuf->size) )
  {
    circbuf->tail = circbuf->buffer;
  }
  *item = *(circbuf->tail);
  circbuf->count--;
  critical_end();

  return CB_OK;
}

CB_status_t CB_is_full(CircBuf_t *circbuf)
{
  if( circbuf == NULL )
  {
    return CB_NULL;
  }
  if( circbuf->count != circbuf->size )
  {
    return CB_FALSE;
  }
  return CB_FULL;
}

CB_status_t CB_is_empty(CircBuf_t *circbuf)
{
  if( circbuf == NULL )
  {
    return CB_NULL;
  }
  if( circbuf->count != 0 )
  {
    return CB_FALSE;
  }
  return CB_EMPTY;
}

CB_status_t CB_peek(CircBuf_t *circbuf, size_t position, uint8_t *item)
{
  if( circbuf == NULL || item == NULL)
  {
    return CB_NULL;
  }
  if( position+1 > circbuf->count )
  {
    return CB_SIZE_ERR;
  }

  /* NOTE: The calculation of the head offset and the associated data retrieval
     must be treated as a critical section. The code needs to be protected to
     guarantee consistency in the case of simultaneous read/modify in and out of
     interrupt. */

  /* Find the peeked item, but wrap around if necessary */
  critical_start();
  if( circbuf->head - position >= circbuf->buffer )
  {
    *item = *(circbuf->head - position);
  }
  else
  {
    *item = *((circbuf->head - position) + circbuf->size);
  }
  critical_end();

  return CB_OK;
}

