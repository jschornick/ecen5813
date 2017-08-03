/**
 * @file led.h
 * @brief Functions declarations and macros for the KL25Z LEDs
 *
 * @author Jeff Schornick
 * @date 2017/07/12
 **/

#ifndef __LED_H__
#define __LED_H__

#include <stdint.h>
#include "platform.h"

/**
 * @brief Configure the KL25Z so that the LEDs can be used
 *
 * Configures the clocks, ports, and GPIOs so that the led_on/off/toggle
 * functions can be used.
 *
 * LEDs will be off after configured.
 *
 * @return Nothing returned
 **/
void led_setup();

/**
 * @brief Turn an LED on
 *
 * Sets the GPIO output for the LED such that the LED is turned on. It is
 * simplest to use the {RED, GREEN, BLUE}_LED macros to pass both `gpio` and
 * `pin` parameters.
 *
 * @param[in] gpio GPIO overlay that includs the output register for the LED
 * @param[in] pin  The pin number of the LED
 * @return Nothing returned
 **/
void led_on(GPIO_Type *gpio, uint8_t pin);

/**
 * @brief Turn an LED off
 *
 * Sets the GPIO output for the LED such that the LED is turned off. It is
 * simplest to use the {RED, GREEN, BLUE}_LED macros to pass both `gpio` and
 * `pin` parameters.
 *
 * @param[in] gpio GPIO overlay that includs the output register for the LED
 * @param[in] pin  The pin number of the LED
 * @return Nothing returned
 **/
void led_off(GPIO_Type *gpio, uint8_t pin);

/**
 * @brief Toggle an LED
 *
 * Toggles the GPIO output for the LED. It is simplest to use the
 * {RED, GREEN, BLUE}_LED macros to pass both `gpio` and `pin` parameters.
 *
 * @param[in] gpio GPIO overlay that includs the output register for the LED
 * @param[in] pin  The pin number of the LED
 * @return Nothing returned
 **/
void led_toggle(GPIO_Type *gpio, uint8_t pin);

#endif /* __LED_H__ */
