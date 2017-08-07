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
#include "memory.h"
#include "memory_dma.h"
#include "dma.h"

#include "io.h"

/* Move DMA config to separate file? */
#include "MKL25Z4.h"

uint8_t *memmove_dma8(uint8_t *src, uint8_t *dst, size_t length)
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

    dma_transfer(DMA_MEM_CHAN, src, dst, length, DMA_8_BIT, DMA_INC);
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
        dma_transfer(DMA_MEM_CHAN, src+length, dst+length, chunk, DMA_8_BIT, DMA_INC);
      }
      else /* we can finish the transfer */
      {
        dma_transfer(DMA_MEM_CHAN, src, dst, length, DMA_8_BIT, DMA_INC);
        length = 0;
      }
    }

  }

  return dst;
}

uint8_t *memmove_dma(uint8_t *src, uint8_t *dst, size_t length)
{
  /* NULL arguments are invalid, return 0 address to indicate failure */
  if ((src == NULL) || (dst == NULL) || (length <= 0))
  {
    return MEM_FAIL;
  }

  uint8_t *orig_dst = dst;

  // Src and dest need transfer using a common alignment
  //   Even if begin/end aren't on a boundary, we can still take advantage in the
  //   middle chunks if aligned
  //     last 2 bits (src,dst)
  //         xy    xy       -> 32-bit  (all bits match)
  //         0x    1x       -> 16-bit  (only low bit matches)
  //         1y    0y       -> 16-bit  (only low bit matches)
  //         x0    y1       ->  8-bit  (low bit mismatch)
  uint8_t src_align = (uint32_t ) src & 0x3;
  uint8_t dst_align = (uint32_t ) dst & 0x3;
  uint8_t dma_align = DMA_8_BIT;
  uint32_t align_mask = ALIGN_MASK_8;
  if ( src_align == dst_align) {
    dma_align = DMA_32_BIT;
    align_mask = ALIGN_MASK_32;
  }
  else if ((src_align & 0x1) == (dst_align & 0x1)) {
    dma_align = DMA_16_BIT;
    align_mask = ALIGN_MASK_16;
  }

  /* **  Source may be inside dest ** */
  if( src > dst )
  {
    /* When src > dst, it is safe to start the copy at the first byte of src.
       The transfer can be performed in one shot without corruption. */

    // align start bytes of src/dst as best possible
    // Set any initial bytes aligned to 32-bit (length allowing)
    if ( ((dma_align == DMA_16_BIT) || (dma_align == DMA_32_BIT)) && ( (uint32_t) src & 0x1) && (length >= 1) ) {
      // last bit of addreses are both 1, align to 0
      *dst++ = *src++;
      length--;
    }
    if ( (dma_align == DMA_32_BIT) && ( (uint32_t) src & 0x2) && (length >= 2) ) {
      // last 2 bits of address are 1x... align to 0x
      my_memcpy(src, dst, 2);
      src += 2;
      dst += 2;
      length -= 2;
    }
    // Start bytes are now optimally aligned

    // The order transfering the end and middle bytes depend on overlap
    //   If we don't overlap, tranfer and odd ends first, then do an efficient DMA of the middle,
    //   If we do overlap, we have to do the middle first, and wait for the DMA to complete if there are odd end bytes

    if( (src >= dst+length) && (length & 0x3) && (length > 0) ) {
      // copy any unaligned end final bytes first
      my_memcpy(src + MEM_ALIGN32(length), dst + MEM_ALIGN32(length), (length & 0x3));
      // only the middle is left
      length = MEM_ALIGN32(length);
      dma_transfer(DMA_MEM_CHAN, src, dst, length, dma_align, DMA_INC);
    } else {
      // middle first, in dma_align sized chunks
      dma_transfer(DMA_MEM_CHAN, src, dst, (length & align_mask), dma_align, DMA_INC);
      src += (length & align_mask);
      dst += (length & align_mask);
      length -= (length & align_mask);
      if ( length > 0 ) {
        // have to copy odd ends last
        dma_wait(DMA_MEM_CHAN);
        my_memcpy(src, dst, length);
      }
    }

    // TODO: if we overlap and length ends oddly, we have to DMA < 32-bit... are we?
    // TODO: verify if overlap offset < 4, we're doing 8/16 bit transfers (should be okay due to dma_align)

  }


  /* **  Dest may be inside source! ** */
  else if (src < dst)
  {
    /* When src < dst, we need to avoid potentially overwriting the end of src
       before it is moved. Unfortunately, the DMA controller can't run the copy
       backwards, so alternately calculate the largest chunk that can be transfered.
       Worst case when dst-src=1, best case when dst-src>=len (on shot).
       NOTE: At some chunk size it's probably best to use a temporary buffer
     */
    // We're need to WORK BACKWARDS, so align END bytes to 32-bit
    if ( ((dma_align == DMA_16_BIT) || (dma_align == DMA_32_BIT)) && !( (uint32_t) (src+length-1) & 0x1) && (length >= 1) ) {
      // last bit of final addreses are both 0, align to 1 (so aligned when transfering 16-bit)
      *(dst + length -1 ) = *(src + length - 1);
      length--;
    }
    if ( (dma_align == DMA_32_BIT) && !( (uint32_t) (src+length-1) & 0x2) && (length >= 2) ) {
      // last 2 bits of last addresses are 01... align to 11 (last 00..11 will be transfered as 32-bit)
      my_memcpy(src+length-2, dst+length-2, 2);
      length -= 2;
    }

    // ends now aligned, copy middle and first bytes
    size_t front_len = length & (~align_mask);
    size_t offset = dst - src;
    // when no overlap, copy any odd first bytes first so we can chunk DMA the middle
    if( (length <= offset) && (front_len > 0) ) {
      my_memcpy(src, dst, front_len);
      src += front_len;
      dst += front_len;
      length -= front_len;
    }

    // The offset determines how much dma at a time
    // there is certainly a minimum offset for which dma is slower than CPU copies
    while( length != 0 )
    {
      if ( length > offset)  /* partial transfer */
      {
        length -= offset;
        // if offset is unaligned, it would be reflected in dma_align
        // so we can alwasy transfer the full offset using dma_align-sized blocks
        dma_transfer(DMA_MEM_CHAN, src+length, dst+length, offset, dma_align, DMA_INC);
      }
      else /* we can finish the transfer */
      {
        front_len = length & (~align_mask);
        dma_transfer(DMA_MEM_CHAN, src + front_len, dst + front_len, (length&align_mask), dma_align, DMA_INC);
        if(front_len) {
          dma_wait(DMA_MEM_CHAN);
          my_memcpy(src, dst, front_len);
        }
        length = 0;
      }
    }

  }

  return orig_dst;
}


uint8_t *memset_dma(uint8_t *ptr, size_t length, uint8_t value)
{
  /* NULL ptr is invalid */
  if ((ptr == NULL) || (length <= 0))
  {
    return NULL;
  }

  // Make an 32-bit aligned repeated copy of the value
  uint32_t aligned_val = value | (value<<8) | (value<<16) | (value<<24);

  // Set any initial bytes to aligned ptr to 32-bit (length allowing)
  //   Use CPU copying here, no need for DMA on small transfers
  if ( (((uint32_t) ptr & 0x3) == 0x1) && (length >= 3) )
  {
    // last 2 bits of address are "01", 3 positions off from 4-byte alignment
    my_memcpy( (uint8_t *) &aligned_val, ptr, 3 );
    ptr += 3;
    length -= 3;
  }
  else if ( (((uint32_t) ptr & 0x3) == 0x2) && (length >= 2) )
  {
    // last 2 bits of address are "10", 2 positions off from 4-byte alignment
    my_memcpy( (uint8_t *) &aligned_val, ptr, 2 );
    ptr += 2;
    length -= 2;
  }
  else if ( (((uint32_t) ptr & 0x3) == 0x3) && (length >= 1) )
  {
    // last 2 bits of address are "11", 1 positions off from 4-byte alignment
    *ptr++ = value;
    length--;
  }

  // Set 1-3 unaligned bytes at the end if length not divisible by 4
  if( (length & 0x3) && (length > 0) ) {
    dma_transfer(DMA_MEM_CHAN, &aligned_val, ptr + MEM_ALIGN32(length), (length & 0x3), DMA_8_BIT, DMA_NO_INC);
    dma_wait(DMA_MEM_CHAN);
  }

  //transfer the rest of the 32-bit chunks (the aligned middle)
  dma_transfer(DMA_MEM_CHAN, &aligned_val, ptr, MEM_ALIGN32(length), DMA_32_BIT, DMA_NO_INC);

  return ptr;
}

uint8_t *memset_dma8(uint8_t *ptr, size_t length, uint8_t value) {
  /* NULL ptr is invalid */
  if ((ptr == NULL) || (length <= 0))
    {
      return NULL;
    }

  dma_transfer(DMA_MEM_CHAN, &value, ptr, length, DMA_8_BIT, DMA_NO_INC);

  return ptr;
}
