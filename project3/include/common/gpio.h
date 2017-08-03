/**
 * @file gpio.h
 * @brief GPIO function declarations
 *
 * Functions for initializing and configuring a GPIO port.
 *
 * @author Jeff Schornick
 * @date 2017/07/31
 **/

#include <stdint.h>
#include "platform.h"

/**
 * @brief Configure the GPIO pins to communicate with an nRF module over SPI
 *
 * Enables and sets the correct GPIO/port modes for communication with the nRF module
 * in a 3-wire SPI configuration. This includes using dedicated GPIO as SPI chip
 * select.
 *
 * @return Nothing returned.
 **/
void gpio_nrf_init(void);

void gpio_spi_init(void);

void gpio_high(GPIO_Type *gpio, uint8_t pin);
void gpio_low(GPIO_Type *gpio, uint8_t pin);
