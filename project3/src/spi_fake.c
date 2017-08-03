/**
 * @file spi_fake.c
 * @brief Fake SPI implemenation when hardware isn't present
 *
 * Functions for initializing, configuring, and sending/receiving via a
 * SPI device when no SPI hardware is available.
 *
 * @author Jeff Schornick
 * @date 2017/07/31
**/

#include <stdint.h>
#include <stddef.h>
#include "logger.h"

void spi_init(void)
{
  log_msg(SPI_INITIALIZED, NULL, 0);
  log_info("Using fake SPI driver");
}

void spi_read_byte(uint8_t *byte)
{
  log_info("Fake SPI read");
  *byte = 42;
}

void spi_write_byte(uint8_t byte)
{
  log_val(byte, "Fake SPI write");
}

void spi_send_packet(uint8_t *p, size_t length)
{
  while( length-- > 0) {
    log_val(*p++, "Fake SPI send");
  }
}

void spi_flush(void)
{
}
