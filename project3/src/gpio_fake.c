/**
 * @file gpio_fake.c
 * @brief Function declarations for fake GPIO
 *
 * Functions for initializing and configuring a fake GPIO 
 *
 * @author Jeff Schornick
 * @date 2017/07/31
 **/

#include <stdint.h>
#include "logger.h"
#include "gpio.h"


inline void gpio_high(GPIO_Type *gpio, uint8_t pin)
{
  LOG_VAL(INFO, 1,gpio);
}

inline void gpio_low(GPIO_Type *gpio, uint8_t pin)
{
  LOG_VAL(INFO, 0, gpio);
}

void gpio_spi_init(void)
{
}

void gpio_nrf_init(void)
{
}

