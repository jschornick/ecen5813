/**
 * @file memory.h
 * @brief Memory function declarations
 *
 * Functions which perform basic memory manipulation.
 *
 * @author Jeff Schornick
 * @date 2017/06/21
**/

#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Move a range of bytes from source to destination
 *
 * Moves a range of bytes from a source region to a destination region. Assumes
 * at least `length` bytes of memory pointed to by `dst` has been allocated.
 *
 * @param[in]  src    The source address of the memory region to move
 * @param[out] dst    The destination address for the moved region of memory
 * @param[in]  length The number of bytes in the memory region to move
 * @return Returns a pointer to the destination
**/
uint8_t *my_memmove(uint8_t *src, uint8_t *dst, size_t length);

/**
 * @brief Copy a range of bytes from source to destination
 *
 * Copies a range of bytes from a source region to a destination region. Assumes
 * at least `length` bytes of memory pointed to by `dst` has been allocated. If
 * the memory regions of `src` and `dst` overlap, a portion of `src` may be
 * corrupted by the copy.
 *
 * @param[in]  src    The source address of the memory region to move
 * @param[out] dst    The destination address for the moved region of memory
 * @param[in]  length The number of bytes in the memory region to move
 * @return Returns a pointer to the destination
**/
uint8_t *my_memcpy(uint8_t *src, uint8_t *dst, size_t length);

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
uint8_t *my_memset(uint8_t *src, size_t length, uint8_t value);

/**
 * @brief Zero every byte in a region of memory
 *
 * Sets each each byte of memory to 0, beginning at the address `src`, and
 * continuing for `length` bytes in total. Assumes the memory region has
 * already been allocated.
 *
 * @param[out] src    The start address of the memory region to zero
 * @param[in]  length The number of bytes in the memory region to zero
 * @return Returns the address of the memory region
**/
uint8_t *my_memzero(uint8_t *src, size_t length);

/**
 * @brief Reverse the bytes in a region of memory
 *
 * Modifies a region of memory begining at `src` such that the values of
 * `length` bytes are replaced with the same values but in reverse order.
 * In other words, after reversing, the first byte of the region will have the
 * the last byte's original value, the 2nd byte the next-to-last original value,
 * and so on until the last byte has the original first value.
 *
 * @param[in,out] src    The start address of the memory region to reverse
 * @param[in]     length The number of bytes in the memory region to reverse
 * @return Returns the address of the memory region
**/
uint8_t *my_reverse(uint8_t *src, size_t length);

/**
 * @brief Allocates a specified number of words of dynamic memory
 *
 * Attemps to allocate `length` 32-bit words of dynamic memory, and returns the
 * address of the allocated region. It is possible for this attempt to fail if
 * a largest contiguous region of free memory is not sufficient to hold the
 * requested number of words. The memory region is not explicitly initialized.
 *
 * @param[in] length The number of words to allocate
 * @return Returns the address of the allocated memory region, or NULL if
 *         allocation was unsuccessful
**/
uint32_t *reserve_words(size_t length);

/**
 * @brief Deallcoates the specified region of dynamic memory
 *
 * Deallocates the dynamic memory region previously allocated at address `src`.
 * This function may produce undefined behavior if the provided address was
 * not previously allocated via `reserve_words()`.
 *
 * @param[in] src The address of the memory region to deallocate
 * @return None
**/
void free_words(uint32_t *src);

#endif /* __MEMORY_H__ */
