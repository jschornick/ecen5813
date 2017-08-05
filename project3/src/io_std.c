/**
 * @file io_std.c
 * @brief C standard I/O routines conforming to a platform-independent API
 *
 * @author Jeff Schornick
 * @date 2017/07/15
 **/

#include <stdint.h>
#include <stdio.h>
#include <string.h>

void print_str(const char *str)
{
  printf( "%s", str);
}

void print_n(uint8_t *str, size_t len)
{
  fwrite(str, 1, len, stdout);
}

void print_int(int32_t val)
{
  printf( "%d", val);
}

void print_int_pad(int32_t val, uint8_t padsize)
{
  printf( "%*d", padsize, val);
}

void print_bytes(uint8_t *data, size_t len)
{
  for(;len>0; len--) {
    printf("%#04x ", *data++);
  }
}

size_t read_str(char *str, size_t maxlen)
{
  return strlen( fgets(str, maxlen, stdin));
}

void printchar(uint8_t chr)
{
  putchar(chr);
}

void io_flush(void)
{
  fflush(stdout);
}
