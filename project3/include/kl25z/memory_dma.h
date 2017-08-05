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

/**
 * @brief Sets every byte in a region of memory to a specified value.
 *
 * Sets each each byte of memory to `value`, beginning at the address `src`, and
 * continuing for `length` bytes in total. Assumes the memory region has already
 * been allocated.
 *
 * @param[out] src    The start address of the memory region to be set
 * @param[in]  length The number of bytes in the memory region to set
 * @param[in]  value  The value to which each byte in the memory region will be set
 * @return Returns the address of the memory region
**/
uint8_t *memset_dma(uint8_t *src, size_t length, uint8_t value);

#endif /* __MEMORY_DMA_H__ */