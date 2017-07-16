/**
 * @file uart.c
 * @brief UART function definitions
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
#include "circular_buffer.h"

/* Circular buffers used to queue Rx/Tx data for the UART */
CircBuf_t rxbuf;
CircBuf_t txbuf;

UART_status_t UART_configure()
{
  // Enable Port A clock
  SIM->SCGC5 |= SIM_SCGC5_PORTA(1); // 1 = enabled

  PORTA->PCR[1] &= ~(PORT_PCR_MUX_MASK);
  PORTA->PCR[1] |= PORT_PCR_MUX(2); // ALT2 mode (UART0_RX)

  PORTA->PCR[2] &= ~(PORT_PCR_MUX_MASK);
  PORTA->PCR[2] |= PORT_PCR_MUX(2); // ALT2 mode (UART0_TX)

  // UART0 clock source select (RM:p196)
  SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);  // 1 = MCGFLLCLK or MCGPLLCLK/2
  SIM->SOPT2 |= SIM_SOPT2_PLLFLLSEL(1); // 1 = MCGPLLCLK/2 clock (48/2 = 24MHz)

  // UART0 clock enable
  SIM->SCGC4 |= SIM_SCGC4_UART0(1);  // 1 = enabled

  // Disable RX/TX during UART configuration
  UART0->C2 &= ~(UART0_C2_RE_MASK); // RX disable
  UART0->C2 &= ~(UART0_C2_TE_MASK); // TX disable

  // Baud rate (Ch 39.3.1)
  // 13-bit baud rate modulo divicor
  // baud rate = (baud clock / ( (OSR+1) * BR))

  // DEFAULT_SYSTEM_CLOCK == 48MHz
  // MCGOUTCLK freq == 48MHz
  // PLL input is 2MH, output is 48MHz, PLL/2 is 24MHz

  // SBR: 5 high bits
  // SBL: 8 low bits
  UART0->BDH &= ~(UART0_BDH_SBR_MASK);
  UART0->BDH |= UART0_BDH_SBR(0);
  UART0->BDL &= ~(UART0_BDL_SBR_MASK);
  UART0->BDL |= UART0_BDL_SBR(26);

  // Oversampling ratio for receiver
  // 0x00011 =  4x (min)
  // 0x11111 = 32x (max)
  UART0->C4 &= ~(UART0_C4_OSR_MASK);
  UART0->C4 |= UART0_C4_OSR(0x7); // 8x OSR

  // Double effective OSR by sampling on both edges
  UART0->C5 |= UART0_C5_BOTHEDGE(1);

  // BAUD rate = clock / ( (1+OSR)*SBR)
  // Baud = 24MHz / ( (1+7)*26 ) = 115385
  // Error ~1.0%
  // TODO: OSR and SBR should be calculated using BAUD, CLOCK

  // TX invert (0 normal, 1 = invert)
  UART0->C3 &= ~UART0_C3_TXINV_MASK;

  // Now that we're configured, re-enable RX/TX
  UART0->C2 |= UART0_C2_RE(1); // 1 = RX enable
  UART0->C2 |= UART0_C2_TE(1); // 1 = TX enable

  // Enable UART RX/TX interrupts
  //  see ch39: interrupts and status flags, p745
  UART0->C2 |= UART0_C2_RIE(1);  // Interrupt on RDRF (Rx data ready)
  UART0->C2 |= UART0_C2_TIE(1);  // Interrupt on TDRE (Tx register ready)
  UART0->C2 &= ~(UART0_C2_TCIE_MASK); // NO interrtupt on TC=1

  // Initialize the Rx/Tx circular buffers before first use
  CB_init(&rxbuf, UART_BUF_SIZE);
  CB_init(&txbuf, UART_BUF_SIZE);

  // Allow UART0 interrupt the CPU
  //  (see: core_cm0plus.h, MKL25Z4.h)
  NVIC_ClearPendingIRQ(UART0_IRQn);
  NVIC_EnableIRQ(UART0_IRQn);

  return UART_OK;
}

UART_status_t UART_send(uint8_t *data)
{

  CB_add_item(&txbuf, *data);
  NVIC_SetPendingIRQ(UART0_IRQn);  // kick interrupt handler
  while(!CB_is_empty(&txbuf)) {
    //NVIC_SetPendingIRQ(UART0_IRQn);
  };

  return UART_OK;
}

UART_status_t UART_send_n(uint8_t *data, size_t num_bytes)
{

  while( num_bytes > 0 ) {
    CB_add_item(&txbuf, *data++);
    num_bytes--;
  }
  NVIC_SetPendingIRQ(UART0_IRQn);  // kick interrupt handler
  while(!CB_is_empty(&txbuf)) {
    //NVIC_SetPendingIRQ(UART0_IRQn);
  };

  return UART_OK;
}

UART_status_t UART_receive(uint8_t *data)
{

  // block until data
  //   ...alternate, just check return status of remove_item?
  while(CB_is_empty(&rxbuf)) {};
  CB_remove_item(&rxbuf, data);

  return UART_OK;
}

UART_status_t UART_receive_n(uint8_t *data, size_t num_bytes)
{
  while( num_bytes > 0 ) {
    while(CB_is_empty(&rxbuf)) {};
    CB_remove_item(&rxbuf, data++);
    num_bytes--;
  }

  return UART_OK;
}

size_t UART_queued_rx()
{
  return rxbuf.count;
}

void UART0_IRQHandler(void)
{
  //__disable_irq();
  NVIC_ClearPendingIRQ(UART0_IRQn);

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
    //led_on(BLUE_LED);
    // reading UART0->D clears the RDRF interrupt flag
    CB_add_item(&rxbuf, UART0->D);
    //led_off(BLUE_LED);
  }

  //__enable_irq();
}
