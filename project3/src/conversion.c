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

/* Byte masks for endianness conversion */
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
  /* Handle negative values, then continue on as if positive */
  if( data < 0 )
  {
    *ptr++ = (uint8_t) '-';
    length++;
    mag = -data;
  }

  uint8_t digits[ITOA_MAX_DIGITS]; /* allocate on stack to avoid malloc/free */
  uint8_t *digit_ptr = digits;
  /* Record the least significant digit, drop an order of magnitude, and repeat
     until nothing is left */
  do
  {
    *digit_ptr++ = mag % base;
    mag /= base;
  } while( mag > 0 );
  length += digit_ptr - digits;

  /* digit_ptr is now one spot past what will be the leftmost digit.
     Play the digits back in reverse to create the expected order. */
  while(digit_ptr-- != digits)
  {
    *ptr++ = TO_ASCII(*digit_ptr);
  }
  *ptr = (uint8_t) '\0';

  return length;

}

int32_t my_atoi(uint8_t *ptr, uint8_t digits, uint32_t base)
{
  if( (ptr == NULL) || (base<2) || (base>16) )
  {
    return 0;
  }

  /* Record the sign if present and advance to the actual digits */
  int8_t sign = 1;
  if( *ptr == '-' ) {
    sign = -1;
    ptr++;
    digits--;
  }

  /* Our digit count is now #digits + 1 (null) */
  /* For each digit (from most-significant down), increase the order of
     magnitude and add the digit's value. */
  int32_t value = 0;
  do
  {
    value *= base;
    value += FROM_ASCII(*ptr);
    ptr++;
  } while( --digits > 1 );

  /* Return mangnitude with the previously recorded sign */
  return sign * value;
}


int8_t big_to_little32(uint32_t *data, uint32_t length)
{
  /* Fail if passed bad parameters */
  if (data == NULL)
  {
    return CONVERT_FAIL;
  }

  /* Build a new word by masking each byte and shifting it to a mirrored position */
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
