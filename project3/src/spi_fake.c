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
  LOG_ID(SPI_INITIALIZED);
  LOG_INFO("Using fake SPI driver");
}

void spi_read_byte(uint8_t *byte)
{
  LOG_INFO("Fake SPI read");
  *byte = 42;
}

void spi_write_byte(uint8_t byte)
{
  LOG_VAL(INFO, byte, "Fake SPI write");
}

void spi_send_packet(uint8_t *p, size_t length)
{
  while( length-- > 0) {
    LOG_VAL(INFO, *p++, "Fake SPI send");
  }
}

void spi_receive_packet(uint8_t *p, size_t length, uint8_t nop)
{
  LOG_INFO("Fake SPI RX packet");
  while( length-- > 0) {
    *p++ = 0xab;
  }
}

void spi_flush(void)
{
}
