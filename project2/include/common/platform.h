/**
 * @file platform.h
 * @brief Platform indpendence layer
 *
 * Combined with compile-time flags, creates a platform independent interface layer
 * to handle initialization and various lower-level functions.
 *
 * @author Jeff Schornick
 * @date 2017/07/12
 **/

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

/**
 * @brief Perform platform-specific initialization
 *
 * Performs all necessary platform-specific initialization, placing the system
 * in a state where it can execute common application code.
 *
 * @return Nothing returned
**/
void platform_init(void);

#endif /* __PLATFORM_H__ */
