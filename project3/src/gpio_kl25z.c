/**
 * @file gpio_kl25z.c
 * @brief Function declarations for GPIO on the KL25Z
 *
 * Functions for initializing and configuring a GPIO port on the KL25Z.
 *
 * @author Jeff Schornick
 * @date 2017/07/31
 **/

#include <stdint.h>
#include "MKL25Z4.h"
#include "logger.h"
#include "gpio.h"


inline void gpio_high(GPIO_Type *gpio, uint8_t pin)
{
  gpio->PSOR = (1<<pin);  // set (high)
}

inline void gpio_low(GPIO_Type *gpio, uint8_t pin)
{
  gpio->PCOR = (1<<pin);  // clear (low)
}

/**
 * @brief Sets the pin mux mode for a port/pin pair
 *
 * Used internally to configure port multiplex modes on the KL25Z.
 *
 * @param[in] port A Port_Type macro (PORTx) for the port to configure
 * @param[in] pin  The pin number to configure on the port
 * @param[in] mode The pin mux mode to set (0-7)
 **/
void set_mux_mode(PORT_Type *port, uint8_t pin, uint8_t mode)
{
  port->PCR[pin] &= ~(PORT_PCR_MUX_MASK);
  port->PCR[pin] |= PORT_PCR_MUX(mode);
}

void gpio_spi_init(void)
{
  // Enable Port C clock for SPI0 CLK, MOSI, and MISO
  SIM->SCGC5 |= SIM_SCGC5_PORTC(1); // 1 = enabled

  // Select SPI0 mux mode for all SPI0 pins
  set_mux_mode(SPI_PORT, SPI_SCK_PIN, SPI_MUX_MODE);

  /* set_mux_mode(SPI_PORT, SPI_MOSI_PIN, SPI_MUX_MODE); */
  /* set_mux_mode(SPI_PORT, SPI_MISO_PIN, SPI_MUX_MODE); */

  set_mux_mode(SPI_PORT, SPI_MOSI_PIN, SPI_ALT_MUX_MODE);
  set_mux_mode(SPI_PORT, SPI_MISO_PIN, SPI_ALT_MUX_MODE);

  LOG_STR(GPIO_INITIALIZED, "SPI pins");
}

void gpio_nrf_init(void)
{
  // Enable Port C clock for nRF CS
  SIM->SCGC5 |= SIM_SCGC5_PORTC(1); // 1 = enabled

  // Set GPIO direction for nRF SPI CS as output
  set_mux_mode(NRF_CS_PORT, NRF_CS_PIN, GPIO_MUX_MODE);
  NRF_CS_GPIO->PDDR |= (1<<NRF_CS_PIN);  /* 1 -> output mode */
  gpio_high(NRF_CS);  // High = disabled (active low)

  // Set GPIO direction for nRF SPI CS as output
  set_mux_mode(NRF_CE_PORT, NRF_CE_PIN, GPIO_MUX_MODE);
  NRF_CE_GPIO->PDDR |= (1<<NRF_CE_PIN);  /* 1 -> output mode */
  gpio_high(NRF_CE);  // High = disabled (active low)

  LOG_STR(GPIO_INITIALIZED, "NRF pins");
}

