/**
 * @file platform.h
 * @brief Platform indpendence layer
 *
 * Combined with compile-time flags, creates a platform independent interface layer
 * to various platform-specific lower-level functions.
 *
 * @author Jeff Schornick
 * @date 2017/06/21
**/

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

/* If the compile-time flags for our platform specify not to use the standard I/O library,
   disable the I/O functions which are normally used throughout the code. */
#ifndef NO_STDIO
  #include <stdio.h>
#else
  #define printf(...)
  /* putchar is already defined as a macro if using newlib, allow redefine if necessary */
  #undef putchar
  #define putchar(x)
#endif

#endif /* __PLATFORM_H__ */
