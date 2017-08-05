/**
 * @file platform.c
 * @brief Platform indpendence layer
 *
 * Combined with compile-time flags, creates a platform independent interface layer
 * to handle initialization and various lower-level functions.
 *
 * @author Jeff Schornick
 * @date 2017/07/12
**/

#include "logger.h"
#include "platform.h"

#ifdef KL25Z

#include "MKL25Z4.h"
#include "system_MKL25Z4.h"
#include "core_cm0plus.h"
#include "led.h"
#include "uart.h"
#include "timer.h"
#include "dma.h"
#include "spi.h"
#include "gpio.h"

void platform_init(void) {

  led_setup();
  led_on(GREEN_LED);

  systick_setup();
  timer_setup();  // every xyz ms
  dma_setup();

  UART_configure();
  UART_send_n( (uint8_t *) "\n** FRDM-KL25Z Reset **\n\n", 26);

  LOGGING_INIT();

  gpio_spi_init();
  gpio_nrf_init();

  spi_init();

  LOG_ID(SYSTEM_INITIALIZED);
  LOG_FLUSH();
}

uint32_t critical_primask = 0;

#else

void platform_init(void) {
 LOGGING_INIT();
 LOG_ID(SYSTEM_INITIALIZED);
 LOG_FLUSH();
}

#endif
