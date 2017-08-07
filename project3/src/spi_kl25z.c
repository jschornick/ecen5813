/**
 * @file spi_kl25z.c
 * @brief SPI implemenation for the KL25Z
 *
 * Functions for initializing, configuring, and sending/receiving via a
 * SPI device on the KL25Z.
 *
 * @author Jeff Schornick
 * @date 2017/07/31
**/

#include <stdint.h>
#include <stddef.h>
#include "MKL25Z4.h"
#include "logger.h"
#include "platform.h"
#include "spi.h"
#include "timer.h"

#define SPI_PRESCALE (0) /* 1x */
#define SPI_DIVIDER  (1) /* 2^2 = 4x --> 6MHz */
#define SPI_CLOCK ( BUS_CLOCK / ((SPI_PRESCALE+1) * (1 << (SPI_DIVIDER+1))) )  /* Ref: p677 */

void spi_init(void)
{

  SIM->SCGC4 |= SIM_SCGC4_SPI0(1);  // Enable SPI peripheral clock

  // SPI0_C1
  //  SPE=1 : enable
  //  MSTR=1 : master
  //  SPIE=0 : poll / no interrupt for SPRF (receive full), or MODF (mode fault)
  //  SPTIE=0 : poll / no TX empty (SPTEF) interrupt
  //  CPOL=0 : active high / idle low
  //  CPHA=0 : data center @ rising clock edge
  //  LSBFE=0 : Most significant bit first
  SPI0->C1 = SPI_C1_MSTR(1);

  // SPI0_C2
  //  MODFEN=0 : disable mode fault, SS is normal GPIO (3-wire mode)
  //  SPC0=0 : separate MOSI/MISO (not single-wire)
  //  TXDMAE=0 : No TX DMA
  //  RXDMAE=0 : No RX DMA
  SPI0->C2 = 0;

  // SPI0_BR (baud rate)
  //  SPPR : Baud rate Prescale
  //  SPR : Buad rate Divider
  //  Baud rate = (Bus Clock) / ((Prescale+1) * (2^(Divider+1)))
  SPI0->BR = SPI_BR_SPPR(SPI_PRESCALE) | SPI_BR_SPR(SPI_DIVIDER);

  // Enable the SPI device
  SPI0->C1 |= SPI_C1_SPE(1);

  LOG_VAL(SPI_INITIALIZED, SPI_CLOCK, "SPI0_CLK");
}

void spi_read_byte(uint8_t *byte)
{
  // Wait for SPI0_S->SPRF=1 (RX full)
  while( !(SPI0->S & SPI_S_SPRF_MASK) ) {};
  *byte = SPI0->D;
}

void spi_write_byte(uint8_t byte)
{
  // Wait for SPI0_S->SPTEF=1 (TX empty)
  //    Tx will be ignored if not read first!
  spi_flush();
  SPI0->D = byte;
  // FIXME: why do we need this delay to get nRF status?
  delay_us(1);
}

void spi_send_packet(uint8_t *p, size_t length)
{
  while( length-- > 0) {
    spi_flush();
    SPI0->D = *p++;
  }
  // FIXME: why do we need this delay to get nRF status?
  delay_us(1);
}

void spi_receive_packet(uint8_t *p, size_t length, uint8_t nop)
{
  while( length-- > 0) {
    spi_write_byte(nop);
    spi_read_byte(p++);
  }
}

void spi_flush(void)
{
  while( !(SPI0->S & SPI_S_SPTEF_MASK) ) {};
}
