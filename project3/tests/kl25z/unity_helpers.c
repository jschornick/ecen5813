#ifdef KL25Z

#include "uart.h"
#include "unity_helpers.h"

void unity_output_start(void) {
  UART_configure();
  unity_output_char('\n');
}

void unity_output_char(int a) {
  UART_send(a);
}

#endif
