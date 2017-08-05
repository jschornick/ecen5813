/**
 * @file io.h
 * @brief Basic I/O routines conforming to a platform-independent API
 *
 * @author Jeff Schornick
 * @date 2017/07/15
 **/

#ifndef __IO_H__
#define __IO_H__

#include <stdint.h>
#include <stddef.h>

/**
 * @brief Print a null-terminated string to the output device
 *
 * Prints a null-terminated string in a platform-independent manner.
 * On Linux-based systesm, this will send charactesrs to the standard output. On
 * a bare-metal MCU, characters are sent through the serial port.
 *
 * @param[in] str The string to output
 * @return Nothing returned
 **/
void print_str(const char *str);

/**
* @brief Print a character array of specified length to the output device
*
* Outputs `len` characters in a platform-independent manner. On Linux-based
* systesm, this will send charactesrs to the standard output. On
* a bare-metal MCU, characters are sent through the serial port.
*
* @param[in]  str A pointer to the characters to output
* @param[out] str The number of characters
* @return Nothing returned
**/
void print_n(uint8_t *str, size_t len);

void print_bytes(uint8_t *data, size_t len);

void print_int(int32_t val);

void print_int_pad(int32_t val, uint8_t padsize);

size_t read_str(char *str, size_t maxlen);

/**
 * @brief Send a character to the output device
 *
 * Prints a chacracter in a platform-independent manner. On Linux-based systesm,
 * this will send charactesrs to the standard output. On a bare-metal MCU,
 * characters are sent through the serial port.
 *
 * @param[in] chr The character to output
 * @return Nothing returned
 **/
void printchar(uint8_t chr);


/**
 * @brief Flush the I/O device
 *
 * @return Nothing returned
 **/
void io_flush(void);

#endif /* __IO_H__ */
