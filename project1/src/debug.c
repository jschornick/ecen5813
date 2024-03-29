/**
 * @file debug.c
 * @brief Debugging helper function definitions
 *
 * Functions that aid in debugging.
 *
 * @author Jeff Schornick
 * @date 2017/06/21
**/

#include <stdint.h>
#include <stddef.h>  /* NULL */
#include <platform.h>  /* putchar */

/* Quick conversion from integer 0-15 to ASCII hex */
#define NIBBLE_TO_ASCII(x) ( (x)<10 ? (x)+'0' : (x)+'A'-10 )

void print_memory(uint8_t *start, uint32_t length)
{
#ifdef VERBOSE
  /* Silently return when passed invaild parameters */
  if( (start == NULL) || (length <= 0) ) {
    return;
  }

  /* Print each byte as two character ASCII hex, separated by spaces */
  uint8_t *end = start + (length - 1);
  do
  {
    putchar( NIBBLE_TO_ASCII((*start) >> 4) );
    putchar( NIBBLE_TO_ASCII((*start) & 0x0F) );
    putchar(' ');
  } while ( ++start <= end );
  putchar('\n');
#endif /* VERBOSE */
}

void (*print_array)() = print_memory;
