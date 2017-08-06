/**
 * @file memory_kl25z.h
 * @brief DMA function declarations for the KL25Z
 *
 * Functions which configure the DMA controller, perform transfers, and report
 * transfer status.
 *
 * @author Jeff Schornick
 * @date 2017/07/27
**/

#include <stdint.h>
#include <stddef.h>

#define DMA_MEM_CHAN 0
#define DMA_ERROR_MASKS (DMA_DSR_BCR_CE_MASK|DMA_DSR_BCR_BED_MASK|DMA_DSR_BCR_BES_MASK)

#define DMA_NO_INC 0
#define DMA_INC 1

#define DMA_8_BIT  1
#define DMA_16_BIT 2
#define DMA_32_BIT 0

/**
 * @brief Setup the DMA controller
 *
 * Configure DMA controller...
 *   Channels?
 *
 * @return Nothing returned
 **/
void dma_setup(void);

/**
 * @brief Wait for a DMA transfer to complete
 *
 * Waits for the DMA transfer initiated on channel `channel` to complete. If no
 * transfer is initiated on the channel, this function will never return.
 *
 * @param[in] channel The DMA channel to wait on
 * @return Nothing returned
**/
void dma_wait(uint8_t channel);

/**
 * @brief Perform a DMA memory transfer between two addresses, one byte at a time
 *
 * Configures the DMA controller for a transfer between source address `src` and
 * destination `dst`. A total of `len` bytes will be transferred, one byte at a
 * time.

 * This function returns immediately after scheduling the transfer. To wait for
 * completion, call the dma_wait() function after return.
 *
 * @param[in] channel The DMA channel to use for the transfer
 * @param[in] src     The source address of the data to transfer
 * @param[in] dst     The destination address to receive the data
 * @param[in] len     The number of bytes to transfer
 * @param[in] size    The size (1,2,4 byte) of DMA transfer to use
 * @param[in] src_inc Set to DMA_INC if the source should increment during transfers
 * @return Nothing returned
 **/
void dma_transfer(uint8_t channel, void *src, void *dst, size_t len, uint8_t size, uint8_t src_inc);
