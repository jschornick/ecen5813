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

#include "MKL25Z4.h"

void platform_init(void) {

  led_setup();
  led_on(GREEN_LED);

  timer_setup();  // heartbeat, systick
  dma_setup();

  UART_configure();
  UART_send_n( (uint8_t *) "\r\n** FRDM-KL25Z Reset **\r\n", 26);

  logging_init();

  gpio_spi_init();
  gpio_nrf_init();
  spi_init() ;

  log_msg(SYSTEM_INITIALIZED, NULL, 0);
  log_flush();
}

uint32_t critical_primask = 0;

#else

void platform_init(void) {
 logging_init();
 log_msg(SYSTEM_INITIALIZED, NULL, 0);
 log_flush();
}

#endif
