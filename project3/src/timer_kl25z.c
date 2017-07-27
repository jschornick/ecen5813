/**
 * @file timer.c
 * @brief Function definitions for timer peripherals and SysTick.
 *
 * Includes initialization/configuration of the TPM0 timer and SysTick (system
 * timer) peripherals, profiling functions, and tick-to-time conversions.
 *
 * @author Jeff Schornick
 * @date 2017/07/22
**/

#include "MKL25Z4.h"  /* includes core_cm0plus.h */
#include "led.h"
#include "timer.h"

volatile uint32_t timer_counter = 0;

void systick_setup(void)
{
  /* Initiazliation order, per Cortex-M0+ UG:
      1. Program reload value
      2. Clear current value
      3. Program Control register   */

  /* Use maximum reload value of 2^24-1 clock pulses */
  SysTick->LOAD = SYSTICK_MAX;
  /* clear current value */
  SysTick->VAL = 0;

  /* CLKSOURCE (KL25 ref: p 51) */
  /*   0 = core clock / 16 */
  /*   1 = core clock */
  SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk; // core clock
  // Disable interrupt trigger
  SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
  // Start timer running
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void timer_setup(void)
{
  /* Make sure peripheral clock is enabled */
  SIM->SCGC6 |= SIM_SCGC6_TPM0(1); // 1 = enabled

  /* Clock source configuration (SIM->SOPT2) is already set by SystemInit */
  /* For CLOCK_SETUP==1 (PEE):
       UART0SRC=0,TPMSRC=1,USBSRC=0,PLLFLLSEL=1,CLKOUTSEL=0,RTCCLKOUTSEL=0 */

  /* TPM0 Status and Control register */
  TPM0->SC = 0;
  TPM0->SC |= TPM_SC_TOF(1);    // Overflow when counter increments past MOD
  TPM0->SC |= TPM_SC_TOIE(1);   // Interrupt on overflow
  TPM0->SC |= TPM_SC_CPWMS(0);  // Up counting mode

  /* Counter prescaler (2^n), see timer.h */
  /*   (MUST be set before CMOD enables timer!) */
  TPM0->SC |= TPM_SC_PS(TIMER_PS_VAL);

  /* Modulo value, see timer.h */
  TPM0->MOD = TIMER_MOD_VAL;

  TPM0->SC |= TPM_SC_CMOD(1);  // Increment on internal clock source (enables timer)

  /* Allow TPM0 interrupt the CPU */
  /*   (see: core_cm0plus.h, MKL25Z4.h) */
  NVIC_ClearPendingIRQ(TPM0_IRQn);
  NVIC_EnableIRQ(TPM0_IRQn);
}

/* Increment counter and process heartbeat every overflow */
void TPM0_IRQHandler(void)
{
  timer_counter++;
  if (TPM0->SC & TPM_SC_TOF_MASK)
  {
    if( (timer_counter & 0x7) == 0) {
      led_toggle(GREEN_LED);
    }
    // Write 1 to TOF to clear flag
    TPM0->SC |= TPM_SC_TOF_MASK;
  }
}

