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

#ifdef KL25Z

#include "MKL25Z4.h"
#include "system_MKL25Z4.h"
#include "core_cm0plus.h"

/* Red LED connected to PB18. Active low. */
#define RED_LED_PORT PORTB
#define RED_LED_GPIO GPIOB
#define RED_LED_PIN 18
#define RED_LED RED_LED_GPIO, RED_LED_PIN

/* Green LED connected to PB19. Active low. */
#define GREEN_LED_PORT PORTB
#define GREEN_LED_GPIO GPIOB
#define GREEN_LED_PIN 19
#define GREEN_LED GREEN_LED_GPIO, GREEN_LED_PIN

/* Blue LED connected to PD1. Active low. */
#define BLUE_LED_PORT PORTD
#define BLUE_LED_GPIO GPIOD
#define BLUE_LED_PIN 1
#define BLUE_LED BLUE_LED_GPIO, BLUE_LED_PIN

// NOTE: Can't use PTC7 on ECEN KL25Z (bad board?)
/* KL25Z SPI port/pin options (CLK, MOSI/MISO, MISO/MOSI, CS) */
/*   Port A: 15, 16, 17, 14  (acceleromemter) */
/*   Port C:  5,  6,  7,  4  (J1: 9, 11, 1, 7) */
/*   Port D:  1,  2,  3,  0  (D13, D11, D12, D10) (J2: 12, 8, 10, 6) */
#define SPI_PORT (PORTC)
#define SPI_MUX_MODE (2)
#define SPI_ALT_MUX_MODE (5)  /* swaps MISO/MOSI */
#define SPI_SCK_PIN  (5)  /* PTC5, J1:9  */
#define SPI_MOSI_PIN (6)  /* PTC6, J1:11 */
#define SPI_MISO_PIN (7)  /* PTC7, J1:1  */

#define GPIO_MUX_MODE (1)

#define NRF_CS_PORT (PORTC)
#define NRF_CS_GPIO (GPIOC)
#define NRF_CS_PIN (4)   /* PTC4, J1:7 */
#define NRF_CS NRF_CS_GPIO, NRF_CS_PIN

#define NRF_CE_PORT (PORTC)
#define NRF_CE_GPIO (GPIOC)
#define NRF_CE_PIN (11)   /* PTC11, J1:15 */
#define NRF_CE NRF_CE_GPIO, NRF_CE_PIN

#if (CLOCK_SETUP == 1)
#define BUS_CLOCK (24000000) /* 24 MHz */
#endif


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
#define START_CRITICAL() critical_primask = __get_PRIMASK(); __disable_irq();

/**
 * @brief Finish critical section
 *
 * Leaves the critical section by restoring interrupts if they had been active
 * before entering the section.
 *
 * @return Nothing returned
 **/
#define END_CRITICAL() if(!critical_primask) { __enable_irq(); }


#else /* HOST/BBB */

/* Interrupts not used on platform */
#define START_CRITICAL()
#define END_CRITICAL()

#define PORT_Type char
#define GPIO_Type char

#define NRF_CS "nrf_cs",1
#define NRF_CE "nrf_ce",2

#endif

#endif /* __PLATFORM_H__ */
