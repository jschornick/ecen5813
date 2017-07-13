/**
 * @file uart.h
 * @brief UART function declarations
 *
 * Functions for initializing, configuring, and sending/receiving via a
 * UART device.
 *
 * @author Jeff Schornick
 * @date 2017/07/12
 **/

#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>
#include <stddef.h>
#include <circular_buffer.h>

/* The baud rate to be used when configuring the UART */
/* TODO: Also need to calculate OSR/SBR configuration values */
#define UART_BAUD_RATE 115200

#define UART_BUF_SIZE 100  /* bytes */

extern CircBuf_t rxbuf;
extern CircBuf_t txbuf;

typedef enum
{
  UART_OK = 0,
  UART_NULL
} UART_status_t;

/**
 * @brief Configure the UART
 *
 * Initializes and configures a UART in preparation for sending/receiving data.
 * Configuration will result in 8-bit LSB data, 1 start/stop bit, no parity.
 * The baud rate used is defined in the macro UART_BAUD_RATE.
 *
 * @return Returns UART_OK if configuration succeeds
 **/
UART_status_t UART_configure();

/**
 * @brief Send a single byte through the UART device
 *
 * Sends a single byte pointed to by `data` through the UART, blocking until the
 * device is ready to transmit.
 *
 * @param[in] data A pointer to the data byte to be sent
 * @return Returns UART_OK if send succeeds
 **/
UART_status_t UART_send(uint8_t *data);

/**
 * @brief Send an array of bytes through the UART device
 *
 * Takes an array of `num_bytes` bytes pointed to by `data` and sends them
 * sequentially through then UART device. This function will block and not not
 * return until all bytes have been sent.
 *
 * @param[in] data      A pointer to the data to be sent
 * @param[in] num_bytes The number of bytes to send
 * @return Returns UART_OK after all provided data has been sent
 **/
UART_status_t UART_send_n(uint8_t *data, size_t num_bytes);

/**
 * @brief Receive a single byte from the UART device
 *
 * Receives a single byte from the uart the UART and stores it at the address
 * pointed to by `data`. The function will blcok and not return until UART is
 * ready to send.
 *
 * @param[out] data A pointer to the data byte to be sent
 * @return Returns UART_OK if send succeeds
 **/
UART_status_t UART_receive(uint8_t *data);

/**
 * @brief Receive an array of bytes from the UART device
 *
 * Receives an array of `num_bytes` bytes from the UART, storing them
 * sequentially beginning at address specified in `data`. This function will
 * block and not not return until all bytes have been received.
 *
 * @param[out] data      The address where the received bytes should be stored
 * @param[in]  num_bytes The number of bytes to receive
 * @return Returns UART_OK after all expected bytes have been received
 **/
UART_status_t UART_receive_n(uint8_t *data, size_t num_bytes);

/**
 * @brief Interrupt handler called on UART transmit/receive
 *
 * Interrupt handler call on UART transmit/receive
 *
 * @return Nothing returned
 **/
void UART0_IRQHandler(void);

#endif /* __UART_H__ */
