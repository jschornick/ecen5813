/**
 * @file timer_posix.c
 * @brief Function definitions for timer peripherals and SysTick.
 *
 * Includes initialization/configuration of the TPM0 timer and SysTick (system
 * timer) peripherals, profiling functions, and tick-to-time conversions.
 *
 * @author Jeff Schornick
 * @date 2017/08/04
 **/

#include <time.h>
#include <stdint.h>

uint32_t get_time()
{
  return time(NULL);
}
