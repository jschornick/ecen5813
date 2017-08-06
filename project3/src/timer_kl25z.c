/**
 * @file timer_kl52z.c
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
#include "logger.h"
#include "timer.h"

volatile uint32_t timer_counter = 0;
volatile uint32_t timer_ticks;
uint32_t startup_ticks;

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

  startup_ticks = get_ticks();
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

  timer_ticks = get_ticks();

  /* Allow TPM0 interrupt the CPU */
  /*   (see: core_cm0plus.h, MKL25Z4.h) */
  NVIC_ClearPendingIRQ(TPM0_IRQn);
  NVIC_EnableIRQ(TPM0_IRQn);
}


/* Increment counter and process heartbeat every overflow */
void TPM0_IRQHandler(void)
{
  timer_counter++;
  timer_ticks = get_ticks();
  if (TPM0->SC & TPM_SC_TOF_MASK)
  {
    if( (timer_counter % 10 ) == 0) {
      led_toggle(GREEN_LED);
    }
    // Write 1 to TOF to clear flag
    TPM0->SC |= TPM_SC_TOF_MASK;
  }
}


void rtc_setup(void)
{
  // RTC Access Control
  SIM->SCGC6 |= SIM_SCGC6_RTC(1); // Access and interrupts enabled
  SIM->SCGC5 |= SIM_SCGC5_PORTC(1);  // enabled

  // from CLOCK_SETUP=1
  // MCG_C1: IRCLKEN=1, MCGIRCLK active
  // MCG_C2: IRCS=0, slow internal reference clock

  // RTC_CLKIN: PTC1 (mode 1)
  PORTC_PCR1 |= PORT_PCR_MUX(1);

  // RTC_CKLOUT: PTC3 [J1:5] (mode 5), PTE0 [J2:18] (mode 4)
  PORTC_PCR3 |= PORT_PCR_MUX(5);

  // Clock out selection
  // 010 Bus clock
  // 011 LPO clock (1 kHz)
  // 100 MCGIRCLK
  // 110 OSCERCLK
  SIM->SOPT2 |= SIM_SOPT2_CLKOUTSEL(4); // MCGIRCLK to CLK_OUT

  SIM->SOPT1 &= ~(SIM_SOPT1_OSC32KSEL_MASK);
  SIM->SOPT1 |= SIM_SOPT1_OSC32KSEL(2);  // 2=RTC_CLKIN

  // RTC Control Register
  //RTC->CR = RTC_CR_OSCE(1);  // Oscillator Enable

  // RTC Status Register
  //   Write to TSR to set time and clear Timer Invalid Flag
  if (RTC->SR & RTC_SR_TIF_MASK) {
    RTC_TSR = 1502055699;
  }
  RTC->SR |= RTC_SR_TCE(1); // Time Counter Enable (read-only, incrementing);

  // Timer seconds interrupt
  RTC->IER = RTC_IER_TSIE(1);

  NVIC_ClearPendingIRQ(RTC_Seconds_IRQn);
  NVIC_EnableIRQ(RTC_Seconds_IRQn);
}

void RTC_Seconds_IRQHandler(void)
{
  // No flag to clear Timer Seconds interrupt
  LOG_ID(HEARTBEAT);
}
