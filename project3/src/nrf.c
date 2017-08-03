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


static inline void nrf_chip_enable()
{
  gpio_low(NRF_CS);
}

static inline void nrf_chip_disable()
{
  gpio_high(NRF_CS);
  // TODO: need delay between disable/enable? (Tcwh=50ns?)
}

static inline void nrf_transmit_enable()
{
  // Power up or CE pin?
  nrf_write_register(NRF_REG_CONFIG, NRF_PWR_UP(1));
}

static inline void nrf_transmit_disable()
{
  // Power down???
  nrf_write_register(NRF_REG_CONFIG, NRF_PWR_UP(0));
}

uint8_t nrf_read_register(uint8_t reg)
{
  nrf_chip_enable();

  spi_write_byte(NRF_CMD_READ|reg);

  // The nRF automatically sends a status byte, read it before tyring to read
  // the result
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
  // NOTE: Write registers only in "Power down" mode (track state?)
  nrf_chip_enable();

  /* uint8_t status; */
  /* spi_write_byte(NRF_CMD_WRITE|reg); */
  /* spi_write_byte(value); */
  /* spi_read_byte(&status); */
  /* log_val(status, "  W stat"); */

  uint8_t p[] = {NRF_CMD_WRITE|reg, value};
  spi_send_packet( p, 2);
  /* uint8_t status; */
  /* spi_read_byte(&status); */
  /* log_val(status, "   W stat"); */


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
}

void nrf_write_tx_addr(uint8_t * tx_addr)
{
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
