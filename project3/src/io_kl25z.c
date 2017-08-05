/**
 * @file io.c
 * @brief KL25Z I/O routines conforming to a platform-independent API
 *
 * @author Jeff Schornick
 * @date 2017/07/15
 **/

#include <stdint.h>

#include "uart.h"
#include "conversion.h"
#include "io.h"

/* Send null-terminated string to output */
void print_str(const char *str)
{
  size_t i = 0 ;
  /* Find position of null terminator */
  while(*(str+i) != '\0' )
  {
    i++;
  };
  UART_send_n( (uint8_t *) str, i);
}

void print_int(int32_t val)
{
  uint8_t str[ITOA_MAX_CHARS];
  uint8_t len;
  len = my_itoa(val, str, BASE_10);
  print_n(str, len-1);
}

void print_int_pad(int32_t val, uint8_t padsize)
{
  uint8_t str[ITOA_MAX_CHARS];
  uint8_t len;
  len = my_itoa(val, str, BASE_10) - 1;
  for(int8_t i=0; i<(padsize - len); i++) {
    printchar(' ');
  }
  print_n(str, len);
}

void print_n(uint8_t *str, size_t len)
{
  UART_send_n(str, len);
}

void print_bytes(uint8_t *data, size_t len)
{
  while( len > 0 ) {
    UART_send( '0' );
    UART_send( 'x' );
    UART_send( TO_ASCII(*data >> 4) );
    UART_send( TO_ASCII(*data & 0xf) );
    UART_send( ' ' );
    data++;
    len--;
  }
}

size_t read_str(char *str, size_t maxlen)
{
  size_t to_read;
  size_t avail = UART_queued_rx();
  to_read = (maxlen-1 <= avail) ? maxlen-1 : avail;
  UART_receive_n( (uint8_t *) str, to_read);
  *(str+to_read) = '\0';
  return to_read;
}

void printchar(uint8_t chr)
{
  UART_send(chr);
}

void io_flush(void)
{
  UART_flush();
}
