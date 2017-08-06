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

volatile uint8_t dma_transfer_complete;

void dma_setup(void)
{
  // Enable the clock for the DMA Mux
  SIM->SCGC6 |= SIM_SCGC6_DMAMUX(1); // 1 = clock enabled
  dma_transfer_complete = 0;
  NVIC_ClearPendingIRQ(DMA0_IRQn);
  NVIC_EnableIRQ(DMA0_IRQn);
}

void dma_wait(uint8_t channel)
{
  /* Wait for interrupt handler to signal completion */
  while(!dma_transfer_complete) {};
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
    | DMA_DCR_DINC(DMA_INC)    // increment destination
    | DMA_DCR_EINT(1);         // enable interrupt on transfer complete
                               // CS=0, continuously transfer until complete

  dma_transfer_complete = 0;
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
    | DMA_DCR_SINC(src_inc)  // increment source based on param
    | DMA_DCR_DSIZE(size)    // destiantion size, 8/16/32 bit
    | DMA_DCR_DINC(DMA_INC)  // increment destination
    | DMA_DCR_EINT(1);       // enable interrupt on transfer complete
                             // CS=0, continuously transfer until complete

  dma_transfer_complete = 0;
  /* Start transfer */
  DMA0->DMA[channel].DCR |= DMA_DCR_START(1);
}

void DMA0_IRQHandler(void)
{
  /* Check if interrupt was due to transfer complete */
  if( (DMA0->DMA[0].DSR_BCR & (DMA_DSR_BCR_DONE_MASK|DMA_ERROR_MASKS)) )
  {
    /* Clear all periperhal status flags */
    DMA0->DMA[0].DSR_BCR |= DMA_DSR_BCR_DONE(1);
    /* Signal the main application */
    dma_transfer_complete = 1;
  }

  /* Check if there was also an error */
  if( DMA0->DMA[0].DCR & DMA_ERROR_MASKS )
  {
    led_on(RED_LED);
    log_str(WARNING, "DMA transfer error!");
  }

}
