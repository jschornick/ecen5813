/**
 * @file nrf.h
 * @brief Nordic nRF module function declarations
 *
 * Functions for configuring and sending/receiving via a Nordic nRF 24L01+
 * module over SPI.
 *
 * @author Jeff Schornick
 * @date 2017/07/31
 **/

#include <stdint.h>

#define NRF_CMD_READ (0x00)
#define NRF_CMD_WRITE (0x20)
#define NRF_CMD_FLUSH_TX (0xE1)
#define NRF_CMD_FLUSH_RX (0xE0)
#define NRF_CMD_NOP (0xFF)  /* can be used to read status */

#define NRF_REG_CONFIG (0x00)
#define NRF_REG_RF_CH (0x05)
#define NRF_REG_RF_SETUP (0x06)
#define NRF_REG_STATUS (0x07)
#define NRF_REG_TX_ADDR (0x10)
#define NRF_REG_FIFO_STATUS (0x17)

#define NRF_PWR_MASK (0x02)
#define NRF_PWR_UP(x) (x<<1)

/**
 * @brief Read an nRF register and return the value
 *
 * Reads a generic 8-bit nRF register over SPI.
 *
 * @return Returns the 8-bit value of the register
 **/
uint8_t nrf_read_register(uint8_t reg);

/**
 * @brief Write to nRF register
 *
 * Writes an 8-bit value to a generic nRF over SPI
 *
 * @return Nothing returned
 **/
void nrf_write_register(uint8_t reg, uint8_t value);

/**
 * @brief Reads the nRF Status register
 *
 * Reads and returns the 8-bit nRF Status register (STATUS)
 *
 * @return Returns the 8-bit value of the Status register
 **/
uint8_t nrf_read_status(void);

/**
 * @brief Reads the nRF Configuration register
 *
 * Reads and returns the 8-bit nRF Configuration register (CONFIG)
 *
 * @return Returns the 8-bit value of the Configuration register
 **/
uint8_t nrf_read_config(void);

/**
 * @brief Writes the 8-bit nRF Configuration register
 *
 * Writes an 8-bit value to the nRF Configuration register (CONFIG)
 *
 * @return Nothing returned
 **/
void nrf_write_config(uint8_t config);

/**
 * @brief Reads the nRF RF Setup register
 *
 * Reads and returns the 8-bit nRF RF Setup register (RF_SETUP
 *
 * @return Returns the 8-bit value of the RF Setup register
 **/
uint8_t nrf_read_rf_setup(void);

/**
 * @brief Writes the 8-bit nRF RF Setup register
 *
 * Writes an 8-bit value to the nRF RF Setup register (RF_SETUP)
 *
 * @return Nothing returned
 **/
void nrf_write_rf_setup(uint8_t config);

/**
 * @brief Reads the nRF RF Channel register
 *
 * Reads and returns the 8-bit nRF RF Channel register (RF_CH)
 *
 * @return Returns the 8-bit value of the RF Channel register
 **/
uint8_t nrf_read_rf_ch(void);

/**
 * @brief Writes the 8-bit nRF RF Channel register
 *
 * Writes an 8-bit value to the nRF RF Channel register (RF_CH)
 *
 * @return Nothing returned
 **/
void nrf_write_rf_ch(uint8_t channel);

/**
 * @brief Reads the 5-byte nRF Transmit Address register
 *
 * Reads and returns the 40-bit nRF Transmit Address register (TX_ADDR)
 * Least-significant byte is first.
 *
 * @param[out] address The address where the 40-bit register value will be stored
 * @return Nothing returned
 **/
void nrf_read_tx_addr(uint8_t * address);

/**
 * @brief Writes the 5-byte nRF Transmit Address register
 *
 * Writes the 40-bit value pointed to by `address` to the nRF Transmit Address
 * register (TX_ADDR). Least-significant byte is first.
 *
 * @param[in] address The address that points to the 40-bit register value to write
 * @return Nothing returned
 **/
void nrf_write_tx_addr(uint8_t * tx_addr);

/**
 * @brief Reads the nRF FIFO Status register
 *
 * Reads and returns the 8-bit nRF FIFO Status register (FIFO_STATUS)
 *
 * @return Returns the 8-bit value of the FIFO Status register
 **/
uint8_t nrf_read_fifo_status(void);

/**
 * @brief Commands the nRF module to flush the transmit FIFO
 *
 * Sends the command TX_FLUSH to the nRF module, which should flush the FIFO
 * buffer for the trasnmitter.
 *
 * @return Nothing returned.
 **/
void nrf_flush_tx_fifo(void);

/**
 * @brief Commands the nRF module to flush the receive FIFO
 *
 * Sends the command RX_FLUSH to the nRF module, which should flush the FIFO
 * buffer for the receiver.
 *
 * @return Nothing returned.
 **/
void nrf_flush_rx_fifo(void);
