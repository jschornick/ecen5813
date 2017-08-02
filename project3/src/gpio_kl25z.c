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

/* KL25Z SPI port/pin options (CLK, MOSI/MOSI, MISO/MOSI) */
/*   Port A: 15, 16, 17  (acceleromemter) */
/*   Port C:  5,  6,  7  (J1: 9, 11, 1) */
/*   Port D:  1,  2,  3  (D13, D11, D12, J2: 12, 8, 10) */
#define SPI_PORT (PORTC)
#define SPI_MUX_MODE (2)
#define SPI_SCK_PIN  (5)  /* PTC5, J1:9  */
#define SPI_MOSI_PIN (6)  /* PTC6, J1:11 */
#define SPI_MISO_PIN (7)  /* PTC7, J1:1  */

#define NRF_CS_PORT (PORTC)
#define NRF_CS_GPIO (GPIOC)
#define NRF_CS_PIN (11)   /* PTC11, J1:15 */
#define GPIO_MUX_MODE (0)

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

void gpio_nrf_init(void)
{
  // Enable Port C clock for SPI0 CLK,MOSI,MISO, and nRF CS
  SIM->SCGC5 |= SIM_SCGC5_PORTC(1); // 1 = enabled

  // Select SPI0 mux mode for all SPI0 pins
  set_mux_mode(SPI_PORT, SPI_SCK_PIN, SPI_MUX_MODE);
  set_mux_mode(SPI_PORT, SPI_MOSI_PIN, SPI_MUX_MODE);
  set_mux_mode(SPI_PORT, SPI_MISO_PIN, SPI_MUX_MODE);

  // Set GPIO direction for nRF SPI CS as output
  set_mux_mode(NRF_CS_PORT, NRF_CS_PIN, GPIO_MUX_MODE);
  NRF_CS_GPIO->PDDR |= (1<<NRF_CS_PIN);  /* 1 -> output mode */
}
