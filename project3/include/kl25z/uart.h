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

/* The target baud rate. A clock divisor will be calculated to best approximate
   this baud rate. With a 48MHz core clock (24MHz UART0 clock), 115.2k is a
   stable baud rate. */
#define UART0_BAUD_RATE (115200)

/* The oversampling rate (OSR) that will be used on the receiver (4x-16x).
   Lowering this value will reduce receiver accuracy, but may allow better
   approximations of the target baud rate. The value is effectively doubled
   due to sampling on both edges of the clock. */
#define UART0_OVERSAMPLING (8)

/* The UART0 clock is PLL source divided by 2, or the unmodified FLL source */
#define PLLFLL_DIV (((SYSTEM_SIM_SOPT2_VALUE & SIM_SOPT2_PLLFLLSEL_MASK) >> SIM_SOPT2_PLLFLLSEL_SHIFT) + 1)
#define UART0_CLK_SPEED (DEFAULT_SYSTEM_CLOCK / PLLFLL_DIV)

/* Determine 13-bit clock divider setting (SBR) to create the UART baud rate clock.
   The formula for the baud rate clock is: UART0_CLK / ((OSR + 1) * SBR).
   The system PLL clock is divided by 2 for UART0. */
#define UART0_SBR (UART0_CLK_SPEED / (UART0_BAUD_RATE * UART0_OVERSAMPLING))

/* The buffer size to use for the RX/TX circular buffers. */
#define UART_BUF_SIZE 200  /* bytes */

extern CircBuf_t rxbuf;
extern CircBuf_t txbuf;

typedef enum
{
  UART_OK = 0,  /* UART function was successful */
  UART_NULL     /* UART tried to operate on null pointer */
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
UART_status_t UART_send(uint8_t data);

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
 * @brief Return the number of items queued in the RX buffer
 *
 * Bytes asynchronously received by the UART are stored in a private buffer
 * until processed by the `UART_receieve*` functions. This reports the number
 * of received but unprocessed bytes. Calling `receive_n` with this count
 * will return without blocking.
 *
 * @return Returns the number of bytes queued in the UART RX buffer
 **/
size_t UART_queued_rx();

/**
 * @brief Flush the UART RX/TX queues
 *
 * @return Nothing returned
 **/
void UART_flush(void);

/**
 * @brief Interrupt handler called on UART transmit/receive
 *
 * Interrupt handler call on UART transmit/receive
 *
 * @return Nothing returned
 **/
void UART0_IRQHandler(void);

#endif /* __UART_H__ */
