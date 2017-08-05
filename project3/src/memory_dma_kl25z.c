/**
 * @file memory_dma_kl25z.c
 * @brief Memory DMA function definitions for the KL25Z
 *
 * Functions which perform memory transfers via DMA on the KL25Z.
 *
 * @author Jeff Schornick
 * @date 2017/07/27
**/

#include <stdint.h>
#include <stddef.h>  /* size_t, NULL */
#include "memory_dma.h"
#include "dma.h"

/* Move DMA config to separate file? */
#include "MKL25Z4.h"

uint8_t *memmove_dma(uint8_t *src, uint8_t *dst, size_t length)
{
  /* NULL arguments are invalid, return 0 address to indicate failure */
  if ((src == NULL) || (dst == NULL) || (length <= 0))
  {
    return MEM_FAIL;
  }

  if( src > dst )
  {
    /* When src > dst, it is safe to start the copy at the first byte of src.
       The transfer can be performed in one shot without corruption.
       NOTE: If 16/32-bit transfers or AA is used, we need to check that
             src >= dst + transfer_size */

    dma_transfer8(DMA_MEM_CHAN, src, dst, length, DMA_INC);
    dma_wait(DMA_MEM_CHAN);
  }
  else if (src < dst)
  {
    /* When src < dst, we need to avoid potentially overwriting the end of src
       before it is moved. Unfortunately, the DMA controller can't run the copy
       backwards, so alternately calculate the largest chunk that can be transfered.
       Worst case when dst-src=1, best case when dst-src>=len (on shot).
       NOTE: At some chunk size it's probably best to use a temporary buffer
     */
    size_t chunk = dst - src;
    while( length != 0 )
    {
      if ( length > chunk)  /* partial transfer */
      {
        length -= chunk;
        dma_transfer8(DMA_MEM_CHAN, src+length, dst+length, chunk, DMA_INC);
      }
      else /* we can finish the transfer */
      {
        dma_transfer8(DMA_MEM_CHAN, src, dst, length, DMA_INC);
        length = 0;
      }
    }

  }

  return dst;
}

uint8_t *memset_dma(uint8_t *src, size_t length, uint8_t value) {
  /* NULL src is invalid */
  if ((src == NULL) || (length <= 0))
  {
    return NULL;
  }

  dma_transfer8(DMA_MEM_CHAN, &value, src, length, DMA_NO_INC);
  dma_wait(DMA_MEM_CHAN);

  return src;
}

