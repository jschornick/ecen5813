/**
 * @file uart_kl25z_blocking.c
 * @brief UART functions implemented in blocking fashion
 *
 * Functions for initializing, configuring, and sending/receiving via a
 * UART device.
 *
 * @author Jeff Schornick
 * @date 2017/07/12
 **/

#include "MKL25Z4.h"
#include "core_cm0plus.h"
#include "led.h"
#include "uart.h"

/* Circular buffers used to queue Rx/Tx data for the UART */
CircBuf_t rxbuf;
CircBuf_t txbuf;

UART_status_t UART_configure()
{
  // Enable Port A clock for UART0 RX/TX pins
  SIM->SCGC5 |= SIM_SCGC5_PORTA(1); // 1 = enabled

  // Select UART0 mux mode for PA1 and PA2
  PORTA->PCR[1] &= ~(PORT_PCR_MUX_MASK);
  PORTA->PCR[1] |= PORT_PCR_MUX(2); // ALT2 mode (UART0_RX)

  PORTA->PCR[2] &= ~(PORT_PCR_MUX_MASK);
  PORTA->PCR[2] |= PORT_PCR_MUX(2); // ALT2 mode (UART0_TX)

  // UART0 clock source select (Ref Manual:p196)
  SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);  // 1 = MCGFLLCLK or MCGPLLCLK/2
  // SIM->OPT2_PLLFLLSEL is set to the correct value in SystemInit()
  //   It determines if the UART0 clock is a PLL/2 or FLL clock source

  // UART0 clock enable
  SIM->SCGC4 |= SIM_SCGC4_UART0(1); // 1 = enabled

  // Disable RX/TX during UART configuration
  UART0->C2 &= ~(UART0_C2_RE_MASK); // RX disable
  UART0->C2 &= ~(UART0_C2_TE_MASK); // TX disable

  // Baud rate clock (Ch 39.3.1)
  //   baud clock rate = ( (UART0 clock) / ((OSR+1)*BR) )
  // SBR is a 13-bit baud rate modulo divisor, split across 2 registers.
  //    SBH: 5 high bits
  //    SBL: 8 low bits
  UART0->BDH &= ~(UART0_BDH_SBR_MASK);
  UART0->BDH |= UART0_BDH_SBR(UART0_SBR>>8); /* 5 high bits */

  UART0->BDL &= ~(UART0_BDL_SBR_MASK);
  UART0->BDL |= UART0_BDL_SBR(UART0_SBR & 0xFF); /* 8 low bits */

  // Oversampling ratio for receiver
  //   0x00011 =  4x (min)
  //   0x11111 = 32x (max)
  UART0->C4 &= ~(UART0_C4_OSR_MASK);
  UART0->C4 |= UART0_C4_OSR(UART0_OVERSAMPLING - 1);

  // Double effective OSR by sampling on both edges
  UART0->C5 |= UART0_C5_BOTHEDGE(1);

  // TX invert (0 normal, 1 = invert)
  UART0->C3 &= ~UART0_C3_TXINV_MASK;

  // Now that we're configured, re-enable RX/TX
  UART0->C2 |= UART0_C2_RE(1); // 1 = RX enable
  UART0->C2 |= UART0_C2_TE(1); // 1 = TX enable

  // Blocking confiugration, no UART RX/TX interrupts

  return UART_OK;
}

UART_status_t UART_send(uint8_t data)
{
  while( !(UART0->S1 & UART0_S1_TDRE_MASK) ) {};
  UART0->D = data;

  return UART_OK;
}

UART_status_t UART_send_n(const uint8_t *data, size_t num_bytes)
{
  while( num_bytes > 0 ) {
    while( !(UART0->S1 & UART0_S1_TDRE_MASK)) {};
    UART0->D = *data++;
    num_bytes--;
  }

  return UART_OK;
}

UART_status_t UART_receive(uint8_t *data)
{
  while(UART0->S1 & UART0_S1_RDRF_MASK) {
    *data = UART0->D;
  }

  return UART_OK;
}

UART_status_t UART_receive_n(uint8_t *data, size_t num_bytes)
{
  while( num_bytes > 0 ) {
    while(UART0->S1 & UART0_S1_RDRF_MASK) {
      *data++ = UART0->D;
      num_bytes--;
    }
  }
  return UART_OK;
}

size_t UART_queued_rx()
{
  return -1;
}

void UART_flush()
{
  while(!CB_is_empty(&txbuf)) {};
  return;
}

void UART0_IRQHandler(void)
{
  __disable_irq();

  uint8_t item = 0x0;
  if(UART0->S1 & UART0_S1_TDRE_MASK) {
    if(!CB_is_empty(&txbuf)) {
      CB_remove_item(&txbuf, &item);
      UART0->D = item;
    } else {
      // We have to set UART0->D even if we didn't send so that interrupt clears
      // Would it be better to just send null?
      UART0->C2 &= ~(UART0_C2_TE_MASK); // TX disable
      UART0->D = item;
      UART0->C2 |= UART0_C2_TE(1); // 1 = TX enable
    }
  }

  if(UART0->S1 & UART0_S1_RDRF_MASK) {
    // reading UART0->D clears the RDRF interrupt flag
    CB_add_item(&rxbuf, UART0->D);
  }

  __enable_irq();
}
