#ifdef KL25Z

#include "uart.h"
#include "unity_helpers.h"

void unity_output_start(void) {
  UART_configure();
  unity_output_char('\n');
}

void unity_output_char(int a) {
  uint8_t c = a;
  UART_send(&c);
}

#endif
