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
#include "spi.h"
#include "gpio.h"
#include "logger.h"
#include "nrf.h"


__attribute__((always_inline)) static inline void nrf_chip_enable()
{
  gpio_low(NRF_CS);
}

__attribute__((always_inline)) static inline void nrf_chip_disable()
{
  gpio_high(NRF_CS);
}

__attribute__((always_inline)) static inline void nrf_transmit_enable()
{
  // Power up then riase CE pin
  //nrf_write_register(NRF_REG_CONFIG, NRF_PWR_UP(1));
  gpio_high(NRF_CE);
}

__attribute__((always_inline)) static inline void nrf_transmit_disable()
{
  // Drop CE and power down
  gpio_low(NRF_CE);
  //nrf_write_register(NRF_REG_CONFIG, NRF_PWR_UP(0));
}

uint8_t nrf_read_register(uint8_t reg)
{
  nrf_chip_enable();

  spi_write_byte(NRF_CMD_READ|reg);

  // The nRF automatically sends a status byte, read it before tyring to
  // read the result
  uint8_t status;
  spi_read_byte(&status);
  //log_val(status, "  R stat");

  uint8_t result;
  spi_write_byte(NRF_CMD_NOP);
  spi_read_byte(&result);

  nrf_chip_disable();
  return result;
}

void nrf_write_register(uint8_t reg, uint8_t value)
{
  nrf_chip_enable();

  spi_write_byte(NRF_CMD_WRITE|reg);
  spi_write_byte(value);

  uint8_t status;
  spi_read_byte(&status);

  nrf_chip_disable();
}

uint8_t nrf_read_status(void)
{
  uint8_t status;
  nrf_chip_enable();
  spi_write_byte(NRF_CMD_NOP);
  spi_read_byte(&status);
  nrf_chip_disable();
  return status;
}


uint8_t nrf_read_config(void)
{
  return nrf_read_register(NRF_REG_CONFIG);
}

void nrf_write_config(uint8_t config)
{
  nrf_write_register(NRF_REG_CONFIG, config);
}

uint8_t nrf_read_rf_setup(void)
{
  return nrf_read_register(NRF_REG_RF_SETUP);
}

void nrf_write_rf_setup(uint8_t config)
{
  nrf_write_register(NRF_REG_RF_SETUP, config);
}

uint8_t nrf_read_rf_ch(void)
{
  return nrf_read_register(NRF_REG_RF_CH);
}

void nrf_write_rf_ch(uint8_t channel)
{
  nrf_write_register(NRF_REG_RF_CH, channel);
}

uint8_t nrf_read_fifo_status(void)
{
  return nrf_read_register(NRF_REG_FIFO_STATUS);
}

void nrf_read_tx_addr(uint8_t * address)
{
  uint8_t status;
  nrf_chip_enable();

  spi_write_byte(NRF_CMD_READ|NRF_REG_TX_ADDR);
  spi_read_byte(&status);
  spi_receive_packet( address, 5, NRF_CMD_NOP);

  nrf_chip_disable();
}

void nrf_write_tx_addr(uint8_t * address)
{
  uint8_t status;
  nrf_chip_enable();

  spi_write_byte(NRF_CMD_WRITE|NRF_REG_TX_ADDR);
  spi_read_byte(&status);
  spi_send_packet( address, 5);

  nrf_chip_disable();
}

void nrf_flush_tx_fifo(void)
{
  nrf_chip_enable();
  spi_write_byte(NRF_CMD_FLUSH_TX);
  nrf_chip_disable();
}

void nrf_flush_rx_fifo(void)
{
  nrf_chip_enable();
  spi_write_byte(NRF_CMD_FLUSH_TX);
  nrf_chip_disable();
}
