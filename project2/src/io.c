/**
 * @file io.c
 * @brief Basic I/O routines conforming to a platform-independent API
 *
 * @author Jeff Schornick
 * @date 2017/07/15
 **/

#include <stdint.h>

#ifdef KL25Z

#include "led.h"
#include "uart.h"
#include "io.h"

/* Send null-terminated string to output */
void print(uint8_t *str)
{
  size_t i = 0 ;
  /* Find position of null terminator */
  while(*(str+i) != '\0' )
  {
    i++;
  };
  UART_send_n(str, i);
}

void print_n(uint8_t *str, size_t len)
{
  UART_send_n(str, len);
}

size_t read_str(uint8_t *str, size_t maxlen)
{
  size_t to_read;
  size_t avail = UART_queued_rx();
  to_read = (maxlen-1 <= avail) ? maxlen-1 : avail;
  UART_receive_n(str, to_read);
  *(str+to_read) = '\0';
  return to_read;
}

void printchar(uint8_t chr)
{
  UART_send(&chr);
}

void heartbeat(void)
{
  static uint32_t i = 0;
  if( (i & 0x1FFFF) == 0 ) {
    led_toggle(GREEN_LED);
    led_toggle(RED_LED);
  }
  i++;
}


#else /* HOST/BBB */

#include <stdio.h>
#include <string.h>

void print(uint8_t *str)
{
  printf( "%s", str);
}

void print_n(uint8_t *str, size_t len)
{
  printf("%.*s", (int) len, str);
}

size_t read_str(uint8_t *str, size_t maxlen)
{
  fgets((char*) str, maxlen, stdin);
  return strlen( (char*)str);
}

void printchar(uint8_t chr)
{
  putchar(chr);
}

void heartbeat(void)
{
  static uint32_t i = 0;
  if((i & 0xFFFFF) == 0) {
    printf(".");
  }
  i++;
}

#endif
