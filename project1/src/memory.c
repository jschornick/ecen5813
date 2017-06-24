/**
 * @file memory.c
 * @brief Memory function definitions
 *
 * Memory function definitions
 *
 * @author Jeff Schornick
 * @date 2017/06/21
**/

#include <stdint.h>
#include <stddef.h>  /* size_t, NULL */
#include <stdlib.h>  /* malloc, free */
#include "memory.h"

uint8_t *my_memmove(uint8_t *src, uint8_t *dst, size_t length)
{
  /* NULL arguments are invalid, but per the specification we always return dst */
  if ((src == NULL) || (dst == NULL) || (length <= 0))
  {
    return dst;
  }

  uint8_t *dst_ptr = NULL;  /* position within destination memory region */
  uint8_t *src_end = src + (length - 1); /* last address of source region */
  if( src > dst )
  {
    /* When src>dst, start the copy at the first byte of src. This avoids a
       potential overwrite of source memory when there is overlap of the memory
       regions. */
    dst_ptr = dst;
    while( src <= src_end )
    {
      *dst_ptr++ = *src++;
    }
  } else {
    /* Otherwise, start the copy at the end of src to avoid potiential overwrite. */
    dst_ptr = dst + (length - 1);
    while( src <= src_end )
    {
      *dst_ptr-- = *src_end--;
    }
  }
  return dst;
}

uint8_t *my_memcpy(uint8_t *src, uint8_t *dst, size_t length) {
  /* Per the project specification, my_memmove() performs an equivalent
     operation to my_memcopy(), though less stringent and allowing for potential
     corruption when the memory regions overlap. */
  return my_memmove(src, dst, length);
}

uint8_t *my_memset(uint8_t *src, size_t length, uint8_t value) {
  /* NULL src is invalid, but per the specification we always return src */
  if ((src == NULL) || (length <= 0))
  {
    return src;
  }

  uint8_t *ptr = src + (length - 1);
  do
  {
    *ptr-- = value;
  } while (ptr >= src);

  return src;
}

uint8_t *my_memzero(uint8_t *src, size_t length) {
  /* Simply use my_memset unless we need to optimize */
  return my_memset(src, length, 0);
}

uint8_t *my_reverse(uint8_t *src, size_t length) {
  /* NULL src is invalid, but per the specification we always return src */
  if ((src == NULL) || (length <= 0))
  {
    return src;
  }

  uint8_t tmp;
  uint8_t *front = src;
  uint8_t *back = src + (length-1);
  while( front < back )
  {
    tmp = *front;
    *front++ = *back;
    *back-- = tmp;
  };
  return src;
}

uint32_t *reserve_words(size_t length) {
  /* malloc() will return NULL on error */
  return malloc(sizeof(uint32_t) * length);
}

void free_words(uint32_t *src) {
  /* free() is safely a no-op if src is NULL */
  free(src);
}
