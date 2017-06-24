/**
 * @file debug.h
 * @brief Debugging helper function declarations
 *
 * Declarations for functions that aid in debugging.
 *
 * @author Jeff Schornick
 * @date 2017/06/21
**/

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdint.h>

/**
 * @brief Display a region of memory as hexadecimal bytes.
 *
 * Reads the memory region starting at the `start` address and displays
 * each consecutive byte for a total of `length` bytes. The value of each byte
 * is displayed as two-digit hexadecimal, separated by spaces. A newline is
 * included after the last value.
 *
 * The output from this function only occurs if the `VERBOSE` is defined at
 * compile time. When undefined, this function simply returns.
 *
 * NOTE: length is `uint32_t` per the project document, but should be `size_t`
 *
 * @param[in] start  The start address of memory to display
 * @param[in] length The number of bytes of memory to display
 * @return None
**/
void print_memory(uint8_t *start, uint32_t length);

/**
  * @brief An alias for `print_memory()`
**/
void (*print_array)();

#endif /* __DEBUG_H__ */
