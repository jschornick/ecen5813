/**
 * @file memory_kl25z.c
 * @brief DMA function definitions for the KL25Z
 *
 * Functions which configure the DMA controller, perform transfers, and report
 * transfer status.
 *
 * @author Jeff Schornick
 * @date 2017/07/27
**/

#include "MKL25Z4.h"
#include "led.h"
#include "logger.h"
#include "dma.h"
#include "io.h"

void dma_setup(void)
{
  // Enable the clock for the DMA Mux
  SIM->SCGC6 |= SIM_SCGC6_DMAMUX(1); // 1 = clock enabled
}

void dma_wait(uint8_t channel)
{
  /* print_str("DMA wait...\n"); */
  /* io_flush(); */
  /* Poll status during active transfer, wait for either done or error */
  while( !(DMA0->DMA[channel].DSR_BCR & (DMA_DSR_BCR_DONE_MASK|DMA_ERROR_MASKS)) ) {};

  /* TODO: log any error */
  if( DMA0->DMA[0].DCR & DMA_ERROR_MASKS )
  {
    led_on(RED_LED);
    log_str(WARNING, "DMA error");
    /* print_str("error\n"); */
    /* io_flush(); */
  }
  /* print_str("ok\n"); */
  /* io_flush(); */
}

void dma_transfer8(uint8_t channel, void *src, void *dst, size_t len, uint8_t src_inc)
{
  /* Set addresses for transfer */
  DMA0->DMA[channel].SAR = (uint32_t) src;
  DMA0->DMA[channel].DAR = (uint32_t) dst;

  DMA0->DMA[channel].DSR_BCR = DMA_DSR_BCR_DONE(1);  // clear status flags
  DMA0->DMA[channel].DSR_BCR = DMA_DSR_BCR_BCR(len); // set transfer size in bytes

  DMA0->DMA[channel].DCR =
    DMA_DCR_SSIZE(DMA_8_BIT)   // 8-bit source
    | DMA_DCR_SINC(src_inc)    // increment source based on param
    | DMA_DCR_DSIZE(DMA_8_BIT) // 8-bit dest
    | DMA_DCR_DINC(DMA_INC);   // increment destination
                               // CS=0, continuously transfer until complete

  /* Start transfer */
  DMA0->DMA[channel].DCR |= DMA_DCR_START(1);
}

// generic dma transfer for 8/16/32 bit sizes... should ba aligned before calling!
void dma_transfer(uint8_t channel, void *src, void *dst, size_t len, uint8_t size, uint8_t src_inc)
{
  /* Set addresses for transfer */
  DMA0->DMA[channel].SAR = (uint32_t) src;
  DMA0->DMA[channel].DAR = (uint32_t) dst;

  DMA0->DMA[channel].DSR_BCR = DMA_DSR_BCR_DONE(1);  // clear status flags
  DMA0->DMA[channel].DSR_BCR = DMA_DSR_BCR_BCR(len); // set transfer size in bytes

  DMA0->DMA[channel].DCR =
    DMA_DCR_SSIZE(size)      // source size, 8/16/32 bit
    | DMA_DCR_SINC(src_inc)    // increment source based on param
    | DMA_DCR_DSIZE(size)    // destiantion size, 8/16/32 bit
    | DMA_DCR_DINC(DMA_INC);   // increment destination
                               // CS=0, continuously transfer until complete

  /* Start transfer */
  DMA0->DMA[channel].DCR |= DMA_DCR_START(1);
}
