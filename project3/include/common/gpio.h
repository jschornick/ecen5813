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

/**
 * @brief Configure GPIO pins for 3-wire SPI communication
 *
 * Enables and sets the correct GPIO/port modes for communication in a 3-wire
 * SPI configuration. The SPI pins for MISO, MOSI, and SCLK will be controlled
 * by the SPI peripheral. select.
 *
 * @return Nothing returned.
 **/
void gpio_spi_init(void);

/**
 * @brief Raise a GPIO pin high, logic "1"
 *
 * @param [in] gpio The device overlay for the GPIO port to configure
 * @param [in] pin  The pin on the port to raise high
 * @return Nothing returned.
 **/
void gpio_high(GPIO_Type *gpio, uint8_t pin);

/**
 * @brief Bring a GPIO pin low, logic "0"
 *
 * @param [in] gpio The device overlay for the GPIO port to configure
 * @param [in] pin  The pin on the port to bring low
 * @return Nothing returned.
 **/
void gpio_low(GPIO_Type *gpio, uint8_t pin);
