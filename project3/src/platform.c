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

#ifdef KL25Z

#include "MKL25Z4.h"
#include "system_MKL25Z4.h"
#include "core_cm0plus.h"
#include "led.h"
#include "uart.h"

void platform_init(void) {
  /* Master clock setup */
  //SystemInit();

  led_setup();
  led_on(GREEN_LED);

  UART_configure();
  UART_send_n( (uint8_t *) "\r\n** FRDM-KL25Z Reset **\r\n", 26);
}

uint32_t critical_primask = 0;

#endif
