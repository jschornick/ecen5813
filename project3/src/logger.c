/**
 * @file logger.c
 * @brief Logger function definitions
 *
 *
 * @author Jeff Schornick
 * @date 2017/07/27
 **/

#include "io.h"
#include "conversion.h"
#include "logger.h"

void log_data(uint8_t *data, size_t length)
{
  print_bytes(data, length);
}

void log_string(uint8_t *str)
{
  print_str(str);
}

void log_int(int32_t val)
{
  uint8_t str[ITOA_MAX_CHARS];
  uint8_t len;
  len = my_itoa(val, str, 10);
  print_n(str, len-1);
}

void log_flush(void)
{
}
