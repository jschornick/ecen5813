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

#ifdef KL25Z

#include "MKL25Z4.h"
#include "system_MKL25Z4.h"
#include "core_cm0plus.h"

/**
 * @brief Perform platform-specific initialization
 *
 * Performs all necessary platform-specific initialization, placing the system
 * in a state where it can execute common application code.
 *
 * @return Nothing returned
**/
void platform_init(void);

/* Backup of PRIMASK before modifying it in critical section */
extern uint32_t critical_primask;

/**
 * @brief Enter critical section
 *
 * Disables interrupts so that critial section can be completed as one logically
 * atomic unit. The old PRIMASK state is recorded so that we don't enable
 * interrupts if they had previously been disabled.
 *
 * At the end of the critical section, `critical_end()` should be called to
 * restore earlier interrupt state.
 *
 * @return Nothing returned
 **/
static inline void critical_start(void)
{
  critical_primask = __get_PRIMASK();
  __disable_irq();
}

/**
 * @brief Finish critical section
 *
 * Leaves the critical section by restoring interrupts if they had been active
 * before entering the section.
 *
 * @return Nothing returned
 **/
static inline void critical_end(void)
{
  if(!critical_primask) {
    __enable_irq();
  }
}

#else /* HOST/BBB */

/* No initialization required on platform */
#define platform_init()

/* Interrupts not used on platform */
#define critical_start()
#define critical_end()

#endif

#endif /* __PLATFORM_H__ */
