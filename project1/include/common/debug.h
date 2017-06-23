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
 * Reads the memory region starting at the `start` address and diplays
 * each consecutive byte for a total of `length` bytes. Each byte is displayed
 * as an two-character hexadecimal. Bytes are displayed four per line, with a
 * space between each byte.
 *
 * This output of this function is only enabled if the `VERBOSE` is defined at
 * compile time. When undefined, this function simply returns.
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
