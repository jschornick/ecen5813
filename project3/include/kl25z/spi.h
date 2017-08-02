/**
 * @file spi.h
 * @brief SPI function declarations
 *
 * Functions for initializing, configuring, and sending/receiving via a
 * SPI device.
 *
 * @author Jeff Schornick
 * @date 2017/07/31
**/

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Enable and configure a SPI peripheral
 *
 * Initializes and configures SPI0 in preparation for sending/receiving data.
 * Configuration will result in 3-wire master mode, with a max 8MHz SPI clock,
 * CPOL=0 (idle low) and CPHA=0 (rising edge), MSB first.
 *
 * @return Nothing returned.
 **/
void spi_init(void);

/**
 * @brief Receive a single byte from the SPI bus
 *
 * Receives a single byte of data from the SPI device and stores it at the
 * address specified by `byte`.
 *
 * @param[out] byte The address where the received byte should be stored
 * @return Nothing returned.
 **/
void spi_read_byte(uint8_t *byte);

/**
 * @brief Send a single byte out the SPI bus
 *
 * Sends the byte of data passed in `byte` to the SPI device.
 *
 * @param[in] byte The byte which should be sent as to the SPI device
 * @return Nothing returned.
 **/
void spi_write_byte(uint8_t byte);

/**
 * @brief Sends a number of bytes out the SPI bus
 *
 * Sends `length` bytes of data pointed to by `p` out the SPI device.
 *
 * @param[in] p Points the the array of bytes to be sent
 * @param[in] length The number of bytes to send
 * @return Nothing returned.
 **/
void spi_send_packet(uint8_t *p, size_t length);

/**
 * @brief Blocks until the SPI transmission is complete
 *
 * Blocks until the SPI transmit queue is empty and the transmitter has send the
 * last byte.
 *
 * @return Nothing returned.
 **/
void spi_flush(void);
