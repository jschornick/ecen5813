/**
 * @file nrf.c
 * @brief Nordic nRF module function definitions
 *
 * Functions for configuring and sending/receiving via a Nordic nRF 24L01+
 * module over SPI.
 *
 * @author Jeff Schornick
 * @date 2017/07/31
 **/

#include <stdint.h>

uint8_t nrf_read_register(uint8_t reg)
{
  return 0;
}

void nrf_write_register(uint8_t reg, uint8_t value)
{
}

uint8_t nrf_read_status(void)
{
  return 0;
}


uint8_t nrf_read_config(void)
{
  return 0;
}

void nrf_write_config(uint8_t config)
{
}

uint8_t nrf_read_rf_setup(void)
{
  return 0;
}

void nrf_write_rf_setup(uint8_t config)
{
}

uint8_t nrf_read_rf_ch(void)
{
  return 0;
}

void nrf_write_rf_ch(uint8_t channel)
{
}

void nrf_read_tx_addr(uint8_t * address)
{
}

void nrf_write_tx_addr(uint8_t * tx_addr)
{
}

uint8_t nrf_read_fifo_status(void)
{
  return 0;
}

void nrf_flush_tx_fifo(void)
{
}

void nrf_flush_rx_fifo(void)
{
}
