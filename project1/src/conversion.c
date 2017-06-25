/**
 * @file conversion.c
 * @brief Data manipulation function definitions
 *
 * Functions which perform basic data manipulation.
 *
 * @author Jeff Schornick
 * @date 2017/06/21
**/

#include <stdint.h>
#include <stddef.h> /* NULL */
#include <stdlib.h> /* malloc, free */
#include "conversion.h"

#define MAX_INT32_DIGITS (31)
#define TO_ASCII(x) ( (x)<10 ? (x)+'0' : (x)+'A'-10 )
#define FROM_ASCII(x) ( (x)<='9' ? (x)-'0' : (x)-'A'+10 )

#define BYTE1 ( (uint32_t) 0x000000FF)
#define BYTE2 ( (uint32_t) 0x0000FF00)
#define BYTE3 ( (uint32_t) 0x00FF0000)
#define BYTE4 ( (uint32_t) 0xFF000000)

uint8_t my_itoa(int32_t data, uint8_t *ptr, uint32_t base)
{
  if( (ptr == NULL) || (base<2) || (base>16) )
  {
    return 0;
  }

  uint8_t length = 1; /* Start counting null-termator only */
  uint32_t mag = data; /* cast from signed, only correct when positive */
  if( data < 0 )
  {
    *ptr++ = (uint8_t) '-';
    length++;
    mag = -data;
  }

  uint8_t *digits = malloc(MAX_INT32_DIGITS);
  uint8_t *digit_ptr = digits;
  do
  {
    *digit_ptr++ = mag % base;
    mag /= base;
  } while( mag > 0 );
  length += digit_ptr - digits;

  /* Digit_ptr is now one spot past what will be the leftmost digit.
     Play the digits back in reverse to create the expected order. */
  while(digit_ptr-- != digits)
  {
    *ptr++ = TO_ASCII(*digit_ptr);
  }
  free(digits);

  *ptr = (uint8_t) '\0';

  return length;

}

int32_t my_atoi(uint8_t *ptr, uint8_t digits, uint32_t base)
{
  if( (ptr == NULL) || (base<2) || (base>16) )
  {
    return 0;
  }

  int8_t sign = 1;
  if( *ptr == '-' ) {
    sign = -1;
    ptr++;
    digits--;
  }
  int32_t value = 0;

  /* Our digit count is now #digits + 1 (null) */
  do
  {
    value *= base;
    value += FROM_ASCII(*ptr);
    ptr++;
  } while( --digits > 1 );

  return sign * value;
}


int8_t big_to_little32(uint32_t *data, uint32_t length)
{
  /* Fail if passed bad parameters */
  if (data == NULL)
  {
    return CONVERT_FAIL;
  }

  uint32_t tmp;
  while(length > 0)
  {
    tmp = 0;
    tmp = (*data & BYTE1) << 24;
    tmp |= (*data & BYTE2) << 8;
    tmp |= (*data & BYTE3) >> 8;
    tmp |= (*data & BYTE4) >> 24;
    *data++ = tmp;
    length--;
  }

  return CONVERT_SUCCESS;
}

int8_t little_to_big32(uint32_t *data, uint32_t length)
{
  /* little-to-big and big-to-little conversion swap the same bytes */
  return big_to_little32(data, length);
}
