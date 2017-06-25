/**
 * @file conversion.h
 * @brief Data manipulation function declarations
 *
 * Functions which perform basic data manipulation.
 *
 * @author Jeff Schornick
 * @date 2017/06/21
**/

#ifndef __CONVERSION_H__
#define __CONVERSION_H__

#define BASE_2 (2)
#define BASE_8 (8)
#define BASE_10 (10)
#define BASE_16 (16)

#define CONVERT_SUCCESS (0)
#define CONVERT_FAIL (1)

#include <stdint.h>

/**
 * @brief Convert an integer to an ASCII string representation
 *
 * Converts the signed 32-bit integer provided as `data` into an ASCII string
 * representation. The magnitude of the converted value will be represented
 * using the numeric base specified by `base`, which may be any integer
 * from 2-16. The string representation will begin with a negative sign `-` if
 * the value to convert is negative. This string will be null-terminated.
 *
 * In addition, the length of the resulting string (including termination) is
 * returned. Zero will be returned on error.
 *
 * @param[in]  data The 32-bit signed integer to convert
 * @param[out] ptr  The address at which to store the string representation
 * @param[in]  base An integer (2-16) that specifies the numeric base for the conversion
 * @return Length of the resulting ASCII string, including null-termination, or
 *         zero on error.
**/
uint8_t my_itoa(int32_t data, uint8_t *ptr, uint32_t base);

/**
 * @brief Return the integer value of a number represented in an ASCII string
 *
 * Converts an ASCII string into its integer value, using `base` as the numeric
 * base for the conversion. The address of the string to convert should be passed
 * in via `ptr`, and have `digits` numeric digits not including negative sign or
 * optional string termination.
 *
 * An invalid base, or string input that contains an invalid representation in that base
 * will * result in a value of zero to be returned. A zero return value cannot be interpretted
 * as an explicit error, as it is also a vaild conversion result for the string "0".
 *
 * @param[in] ptr    The address of the ASCII string representation of the number
 * @param[in] digits The number of digits in the magnitude of the string representation
 * @param[in] base   An integer (2-16) that specifies the numeric base for the conversion
 * @return Integer value of the converted string, or zero for invalid input paramters
**/
int32_t my_atoi(uint8_t *ptr, uint8_t digits, uint32_t base);

/**
 * @brief Convert a region of memory from big-endian to little-endian representation
 *
 * Modifies a region of memory begining at `data` such that each 32-bit word
 * interpretted as big-endian is replaced with its little-endian representation.
 * This conversion is done for a total of `length` consecutive words.
 *
 * An error (nonzero) will be returned if `data` is a null pointer.
 *
 * @param[in,out] data The 32-bit signed integer to convert
 * @param[in]     length  The address at which to store the string representation
 * @return Returns CONVERT_SUCCESS on success, CONVERT_FAIL otherwise
**/
int8_t big_to_little32(uint32_t *data, uint32_t length);

/**
 * @brief Convert a region of memory from little-endian to big-endian representation
 *
 * Modifies a region of memory begining at `data` such that each 32-bit word
 * interpretted as little-endian is replaced with its big-endian representation.
 * This conversion is done for a total of `length` consecutive words.
 *
 * An error (nonzero) will be returned if `data` is a null pointer.
 *
 * @param[in,out] data The 32-bit signed integer to convert
 * @param[in]     length  The address at which to store the string representation
 * @return Returns CONVERT_SUCCESS on success, CONVERT_FAIL otherwise
**/
int8_t little_to_big32(uint32_t *data, uint32_t length);

#endif /* __CONVERSION_H__ */
