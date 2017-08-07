/**
 * @file led.c
 * @brief Functions for configuring and using onboard LEDs
 *
 * @author Jeff Schornick
 * @date 2017/07/12
 **/

#include <stdint.h>
#include <MKL25Z4.h>
#include "led.h"

__attribute__((always_inline)) inline void led_on(GPIO_Type *gpio, uint8_t pin)
{
  gpio->PCOR = (1<<pin);  // set low (on)
}

__attribute__((always_inline)) inline void led_off(GPIO_Type *gpio, uint8_t pin)
{
  gpio->PSOR = (1<<pin);  // set high (off)
}

__attribute__((always_inline)) inline void led_toggle(GPIO_Type *gpio, uint8_t pin)
{
  gpio->PTOR = (1<<pin); // toggle output
}

/* Only used internally. Call led_setup() instead. */
void led_enable(PORT_Type *port, GPIO_Type *gpio, uint8_t pin)
{
  port->PCR[pin] &= ~(PORT_PCR_MUX_MASK);
  port->PCR[pin] |= PORT_PCR_MUX(1);  // ALT1 mode (GPIO)
  gpio->PDDR |= (1<<pin);  // set to output
}

void led_setup()
{
  // Enable Port B and D clocks for LEDs
  // Clock Gating and Configuration (KL25_RM Ch12)
  SIM->SCGC5 |= SIM_SCGC5_PORTB(1); // 1 = enabled
  SIM->SCGC5 |= SIM_SCGC5_PORTD(1);

  // Enable LEDs by configuring the Port and GPIO for each
  led_enable(RED_LED_PORT, RED_LED);
  led_enable(GREEN_LED_PORT, GREEN_LED);
  led_enable(BLUE_LED_PORT, BLUE_LED);

  led_off(RED_LED);
  led_off(GREEN_LED);
  led_off(BLUE_LED);
}
