/**
 * @file timer.h
 * @brief Function declarations for timer peripherals and SysTick.
 *
 * Includes initialization/configuration of the TPM0 timer and SysTick (system
 * timer) peripherals, profiling functions, and tick-to-time conversions.
 *
 * @author Jeff Schornick
 * @date 2017/07/22
**/

#ifndef __TIMER_H__
#define __TIMER_H__

#include "MKL25Z4.h"  /* include core_cm0plus.h */


/*** SysTick, System Timer ***/

/* At 48MHz, reloads every ~0.35 seconds */
#define SYSTICK_MAX (0x00FFFFFF)

/* Tick to time conversion macros */
/* To maintain numerical accuracy, first multiply ticks (24-bit) by 250 */
#define TICKS_IN_MS(x) ( ((x) * 250) / (DEFAULT_SYSTEM_CLOCK/4))
#define TICKS_IN_US(x) ( ((x) * 250) / (DEFAULT_SYSTEM_CLOCK/4000))

/**
 * @brief Configure and enable the System Timer (SysTick)
 *
 * SysTick is initialized to decrement at each pulse of the core clock. No
 * interrupts will be generated.
 *
 * @return Nothing returned
**/
void systick_setup(void);

/**
 * @brief Get the current SysTick tick count
 *
 * SysTick counts down once per core clock pulse, rolling over from zero to
 * SYSTICK_MAX. This funciton can be used to store the start time for profiling.
 *
 * @return The current SysTick tick count
**/
__attribute__((always_inline)) static inline uint32_t get_ticks()
{
  return SysTick->VAL;
}

/**
 * @brief Calculate the elapsed ticks between two SysTick values
 *
 * Since the SysTick rolls over when it reaches zero, this function can only
 * return an accurate difference if the total timing period is less than
 * SYSTICK_MAX.
 *
 * @param[in] start The tick count at the beginning of the period
 * @param[in] end   The tick count at the end of the period
 * @return The number of ticks that occurred between start and end
**/
__attribute__((always_inline)) static inline uint32_t diff_ticks(uint32_t start, uint32_t end)
{
  return (start > end) ? (start-end) : (SYSTICK_MAX - end + 1 + start);
}

/**
 * @brief Calculate the elapsed ticks from a SysTick value and now
 *
 * Since the SysTick rolls over when it reaches zero, this function can only
 * return an accurate difference if the start time occured less than SYSTICK_MAX
 * ticks prior.
 *
 * @param[in] start The tick count at the beginning of the period
 * @return The number of ticks that occurred between start and now
 **/
__attribute__((always_inline)) static inline uint32_t elapsed_ticks(uint32_t start)
{
  /* Minimize error by getting the ticks as soon as possible */
  uint32_t end = SysTick->VAL;
  return diff_ticks(start, end);
}


/*** TPM0 Timer Peripheral ***/

/* Using CLOCK_SETUP==1, this is 48MHz */
#define TIMER_CLK_FREQ DEFAULT_SYSTEM_CLOCK

/* Prescaler setting, resulting scale factor, and max period w/48MHz clock */
//    0 (  1x) :  1.36 ms
//    1 (  2x):   2.73 ms
//    2 (  4x):   5.54 ms
//    3 (  8x):  10.92 ms
//    4 ( 16x):  21.84 ms
//    5 ( 32x):  43.69 ms
//    6 ( 64x):  87.38 ms
//    7 (128x): 174.76 ms
#define TIMER_PS_VAL (7)
#define TIMER_SCALE (128)

/* Target timer period, 125ms */
#define TIMER_TARGET_MS (125)

/* Up-counting modulo value calculation (see KL25 TRM p563) */
#define TIMER_MOD_VAL (((TIMER_CLK_FREQ / TIMER_SCALE) * TIMER_TARGET_MS) / 1000 - 1)

/* A counter incremented each time the timer overflows */
extern volatile uint32_t timer_counter;

/**
 * @brief Configure the TPM0 timer and enable it
 *
 * The TPM0 timer peripheral is configured to overflow every TIMER_TARGET_MS
 * milliseconds. Interrupts are configured such that the TPM0 ISR is called
 * every overflow.
 *
 * @return Nothing returned
**/
void timer_setup(void);

#endif /* __TIMER_H__ */
