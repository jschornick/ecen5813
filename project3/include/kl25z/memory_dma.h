/**
 * @file memory_dma.h
 * @brief Memory DMA function declarations
 *
 * Functions which perform basic memory manipulation via DMA.
 *
 * @author Jeff Schornick
 * @date 2017/06/21
**/

#ifndef __MEMORY_DMA_H__
#define __MEMORY_DMA_H__

#include <stdint.h>
#include <stddef.h>

#define MEM_FAIL 0

#define MEM_ALIGN32(addr) (addr & 0xFFFFFFFC)  /* clear last 2 bits */
#define MEM_ALIGN16(addr) (addr & 0xFFFFFFFE)  /* clear last bit */

#define ALIGN_MASK_32 (0xFFFFFFFC)  /* clears last 2 bits */
#define ALIGN_MASK_16 (0xFFFFFFFE)  /* clears last bit */
#define ALIGN_MASK_8 (0xFFFFFFFF)  /* clears nothing */

#define DMA_AVAILABLE

/**
 * @brief Move a range of bytes from source to destination
 *
 * Moves a range of bytes from a source region to a destination region. Assumes
 * at least `length` bytes of memory pointed to by `dst` has been allocated.
 *
 * @param[in]  src    The source address of the memory region to move
 * @param[out] dst    The destination address for the moved region of memory
 * @param[in]  length The number of bytes in the memory region to move
 * @return Returns a pointer to the destination, or 0 on failure
**/
uint8_t *memmove_dma(uint8_t *src, uint8_t *dst, size_t length);

// same but with simple 8-bit DMA transfers only
uint8_t *memmove_dma8(uint8_t *src, uint8_t *dst, size_t length);

/**
 * @brief Sets every byte in a region of memory to a specified value.
 *
 * Sets each each byte of memory to `value`, beginning at the address `ptr`, and
 * continuing for `length` bytes in total. Assumes the memory region has already
 * been allocated.
 *
 * For efficiency, 32-bit transfers are used where possible.
 *
 * This function returns immediately after scheduling majority of the data
 * transfer. To wait for completion, call the dma_wait() function after return.
 *
 * @param[out] ptr    The start address of the memory region to be set
 * @param[in]  length The number of bytes in the memory region to set
 * @param[in]  value  The value to which each byte in the memory region will be set
 * @return Returns the address of the memory region
**/
uint8_t *memset_dma(uint8_t *ptr, size_t length, uint8_t value);

// same but with simple 8-bit DMA transfers only
uint8_t *memset_dma8(uint8_t *ptr, size_t length, uint8_t value);

#endif /* __MEMORY_DMA_H__ */
