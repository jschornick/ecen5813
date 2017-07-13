/**
 * @file uart.c
 * @brief UART function definitions
 *
 * Functions for initializing, configuring, and sending/receiving via a
 * UART device.
 *
 * @author Jeff Schornick
 * @date 2017/07/12
 **/

#include "uart.h"

UART_status_t UART_configure()
{
  return UART_OK;
}

UART_status_t UART_send(uint8_t *data)
{
  return UART_OK;
}

UART_status_t UART_send_n(uint8_t *data, size_t num_bytes)
{
  return UART_OK;
}

UART_status_t UART_receive(uint8_t *data)
{
  return UART_OK;
}

UART_status_t UART_receive_n(uint8_t *data, size_t num_bytes)
{
  return UART_OK;
}

void UART0_IRQHandler(void)
{
}
