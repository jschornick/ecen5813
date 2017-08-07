/**
 * @file nrf.h
 * @brief Nordic nRF module (nRF24L01) function declaration sand register defintions.
 *
 * Functions for configuring and sending/receiving via a Nordic nRF 24L01+
 * module over SPI.
 *
 * @author Jeff Schornick
 * @date 2017/07/31
 **/

#include <stdint.h>

#define NRF_CMD_READ           (0x00)  /* Read register    */
#define NRF_CMD_WRITE          (0x20)  /* Write to regiser */
#define NRF_CMD_RX_PAYLOAD     (0x61)  /* Read RX payload  */
#define NRF_CMD_TX_PAYLOAD     (0x90)  /* Write TX payload */
#define NRF_CMD_FLUSH_TX       (0xE1)  /* Flush TX FIFO    */
#define NRF_CMD_FLUSH_RX       (0xE2)  /* Flush RX FIFO    */
#define NRF_CMD_REUSE_TX_PL    (0xE3)  /* Reuse last TX payload */

#define NRF_CMD_ACTIVATE       (0x50)  /* Activate a feature          */
#define NRF_CMD_R_RX_WID       (0x60)  /* Read RX payload width       */
#define NRF_CMD_W_ACK_PL       (0xA8)  /* Write payload with ACK      */
#define NRF_CMD_W_ACK_PL_NOACK (0xB0)  /* Disalble auto-ACK on packet */
#define NRF_CMD_NOP            (0xFF)  /* No operation. Can be used to read status. */

#define NRF_REG_CONFIG         (0x00)  /* Configuration register     */
#define NRF_REG_RF_CH          (0x05)  /* RF Channel register        */
#define NRF_REG_RF_SETUP       (0x06)  /* RF Setup register5         */
#define NRF_REG_STATUS         (0x07)  /* Status register            */
#define NRF_REG_TX_ADDR        (0x10)  /* Transmit address (5-bytes) */
#define NRF_REG_FIFO_STATUS    (0x17)  /* FIFO Status register       */


/*** CONFIG Register 0x00 ***/

#define NRF_CNF_MASK_RX_DR_MASK  (1<<6)  /* Mask interrupt from RX_DR  */
#define NRF_CNF_MASK_RX_DR(x)    (x<<6)
#define NRF_CNF_MASK_TX_DS_MASK  (1<<5)  /* Mask interrupt from TX_DS  */
#define NRF_CNF_MASK_TX_DS(x)    (x<<5)
#define NRF_CNF_MASK_MAX_RT_MASK (1<<5)  /* Mask interrupt from MAX_RT */
#define NRF_CNF_MASK_MAX_RT(x)   (x<<5)
#define NRF_CNF_EN_CRC_MASK      (1<<3)  /* Enable CRC                 */
#define NRF_CNF_EN_CRC(x)        (x<<3)
#define NRF_CNF_CRCO_MASK        (1<<2)  /* CRC encoding, 0:1-byte, 1:2-bytes */
#define NRF_CNF_CRCO(x)          (x<<2)
#define NRF_CNF_PWR_MASK         (1<<1)  /* 1:Power Up, 0: Power Down         */
#define NRF_CNF_PWR_UP(x)        (x<<1)
#define NRF_CNF_PRIM_RX_MASK     (1<<0)  /* RX/TX control, 1:PRX, 2:PTX       */
#define NRF_CNF_PRIM_RX(x)       (x<<0)


/*** RF_CH (RF Channel) Register 0x05 ***/

#define NRF_RFCH_RF_CH_MASK      (0x3d)  /* RF channel frequency */
#define NRF_RFCH_RF_CH(x)          (x)


/*** RF_SETUP Register 0x06 ***/

#define NRF_RFS_PLL_LOCK_MASK    (1<<4)  /* Force PLL Lock      */
#define NRF_RFS_PLL_LOCK(x)      (x<<1)
#define NRF_RFS_RF_DR_MASK       (1<<3)  /* Air data rate       */
#define NRF_RFS_RF_DR(x)         (x<<1)  /*    0:1Mb, 1:2M      */
#define NRF_RFS_RF_PWR_MASK      (3<<1)  /* Set RF output power */
#define NRF_RFS_RF_PWR(x)        (x<<1)
#define NRF_RFS_LNA_HCURR_MASK   (   1)  /* Setup LNA gain      */
#define NRF_RFS_LNA_HCURR(x)     (   x)


/*** STATUS Register 0x07 ***/

#define NRF_STAT_RX_DR_MASK      (1<<6)  /* Data Read FIFO interrupt */
#define NRF_STAT_RX_DR_(x)       (x<<6)
#define NRF_STAT_TX_DS_MASK      (1<<5)  /* Data Sent FIFO interrupt */
#define NRF_STAT_TX_DS_(x)       (x<<5)
#define NRF_STAT_MAX_RT_MASK     (1<<4)  /* Max TX retransmits interrupt */
#define NRF_STAT_MAX_RT(x)       (x<<4)
#define NRF_STAT_RX_P_NO_MASK    (7<<1)  /* Data pip number for payload  */
#define NRF_STAT_RX_P_NO(x)      (x<<1)
#define NRF_STAT_TX_FULL_MASK    (   1)  /* TX FIFO full flag */
#define NRF_STAT_TX_FULL(x)      (   x)


/*** FIFO_STATUS Register 0x17 ***/

#define NRF_FS_TX_REUSE_MASK      (1<<6)  /* Reuse last TX'd packet  */
#define NRF_FS_TX_REUSE(x)        (x<<6)
#define NRF_FS_TX_FULL_MASK       (1<<5)  /* TX FIFO full flag       */
#define NRF_FS_TX_FULL(x)         (x<<5)
#define NRF_FS_TX_EMPTY_MASK      (1<<4)  /* TX FIFO full flag       */
#define NRF_FS_TX_EMPTY(x)        (x<<4)
#define NRF_FS_RX_FULL_MASK       (1<<1)  /* RX FIFO full flag       */
#define NRF_FS_RX_FULL(x)         (x<<1)
#define NRF_FS_RX_EMPTY_MASK      (   1)  /* RX FIFO full flag       */
#define NRF_FS_RX_EMPTY(x)        (   x)


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
 * Reads and returns the 8-bit nRF RF Channel register (RF_CH)_HCURR
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
void nrf_write_tx_addr(uint8_t * address);

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
