/**
 * @file project3.c
 * @brief Project 3 deliverables
 *
 * The primary deliverable for Project 3 is...
 *
 * @author Jeff Schornick
 * @date 2017/07/22
**/

#include <stdint.h>
#include "project3.h"
#include "platform.h"
#include "logger.h"
#include "nrf.h"
#include "conversion.h"
#include "string.h"  // memmove, memset
#include "profile.h"
#include "memory.h"
#include "io.h"
#ifdef KL25Z
#include "memory_dma.h"
#include "dma.h"
#endif

#include <stdlib.h>

Log_t foo;
uint8_t data[] = { 0xAB, 0xCD, 0xEF, 0x01 };
void log_test(void)
{

  // Raw, non-queued logging
  LOG_RAW_STRING("Hello\n");
  LOG_RAW_DATA( (uint8_t *) &data, 4);
  LOG_RAW_STRING("\n");
  LOG_RAW_INT(1234567890);
  LOG_RAW_STRING("\n");

  // Log queue logging
  foo.id = INFO;
  foo.length = 10;
  foo.data = (uint8_t *) "A message";
  LOG_ITEM( &foo );

  LOG_STR(WARNING, "A much too verbose message for the logger");
  LOG_INFO("This is much simpler!");

  LOG_FLUSH();
}


void nrf_test() {
  uint8_t addr[5];
  uint8_t test_addr1[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
  uint8_t test_addr2[] = { 0xab, 0xcd, 0xef, 0x01, 0x02 };

  LOG_VAL(INFO, nrf_read_config(), "CONFIG");
  LOG_VAL(INFO, nrf_read_rf_setup(), "RF_SETUP");
  LOG_VAL(INFO, nrf_read_rf_ch(), "RF_CH");
  LOG_VAL(INFO, nrf_read_fifo_status(), "FIFO_STATUS");
  LOG_VAL(INFO, nrf_read_status(), "STATUS");
  nrf_read_tx_addr(addr);
  LOG_DATA(NRF_ADDRESS, addr, 5);
  LOG_FLUSH();

  LOG_INFO("---");
  LOG_INFO("Writing 0");
  nrf_write_config(0);  // TX
  LOG_VAL(INFO, nrf_read_config(), "CONFIG");
  LOG_INFO("---");
  LOG_INFO("Writing 1");
  nrf_write_config(1);  // TX
  LOG_VAL(INFO, nrf_read_config(), "CONFIG");
  LOG_INFO("---");
  LOG_INFO("Writing 2");
  nrf_write_config(2);  // TX
  LOG_VAL(INFO, nrf_read_config(), "CONFIG");
  LOG_INFO("---");
  LOG_FLUSH();

  nrf_write_tx_addr(test_addr1);
  nrf_read_tx_addr(addr);
  LOG_DATA(NRF_ADDRESS, addr, 5);
  nrf_write_tx_addr(test_addr2);
  nrf_read_tx_addr(addr);
  LOG_DATA(NRF_ADDRESS, addr, 5);
}

#ifdef KL25Z
extern uint32_t __BUFFER_START;
uint8_t *buffer1 = (uint8_t *) &__BUFFER_START;
uint8_t *buffer2 = ((uint8_t *) &__BUFFER_START) + PROF_BUFFER_SIZE;
#else
//TODO: malloc these to get off the stack on linux?
uint8_t buffer1[PROF_BUFFER_SIZE];
uint8_t buffer2[PROF_BUFFER_SIZE];
#endif

void print_result(char *func, uint32_t *results, uint8_t sizes) {
  print_str(func);
  print_str(" |");
  for(uint8_t i=0; i<sizes; i++) {
    print_int_pad(results[i], 5);
    print_str(" |");
  }
  printchar('\n');
}

void profile_memory() {

  uint8_t num_sizes = 4;
  uint8_t num_funcs = 8;
  size_t test_sizes[] = {10,50,1000,5000};
  uint32_t results[num_funcs][num_sizes];
  uint8_t func;

  profile_calibrate();

  for(uint8_t i=0; i<num_sizes; i++)
  {
    uint32_t test_size = test_sizes[i];
    func = 0;

    LOG_INFO("---");
    if( PROF_BUFFER_SIZE < test_size )
    {
      LOG_VAL(WARNING, test_size, "Test too big, buffer");
      for(uint8_t j=0; j<num_funcs; j++) { results[i][j] = -1; }
      continue;
    }
    LOG_VAL(INFO, test_size, "Test size");
    LOG_FLUSH();

    PROFILE( "memset", memset(buffer1, i, test_size) , &results[func++][i] );
    PROFILE( "my_memset", my_memset(buffer1, test_size, i) , &results[func++][i] );
    #ifdef DMA_AVAILABLE
    PROFILE( "memset_dma", memset_dma(buffer1, test_size, i); dma_wait(DMA_MEM_CHAN); , &results[func++][i] );
    PROFILE( "memset_dma8", memset_dma8(buffer1, test_size, i); dma_wait(DMA_MEM_CHAN); , &results[func++][i] );
    #endif
    LOG_FLUSH();

    PROFILE( "memmove", memmove(buffer2, buffer1, test_size) , &results[func++][i]);
    PROFILE( "my_memmove", my_memmove(buffer1, buffer2, test_size) , &results[func++][i]);
    #ifdef DMA_AVAILABLE
    PROFILE( "memmove_dma", memmove_dma(buffer1, buffer2, test_size); dma_wait(DMA_MEM_CHAN); , &results[func++][i]);
    PROFILE( "memmove_dma8", memmove_dma8(buffer1, buffer2, test_size); dma_wait(DMA_MEM_CHAN); , &results[func++][i]);
    #endif
    LOG_FLUSH();
  }

  print_str("Profile Result Summary\n");
  print_str("------------------------------------------\n");
  print_str("             |   10 |   50 | 1000 | 5000 |\n");
  print_str("------------------------------------------\n");

  func = 0;
  print_result("memset      ", results[func++], num_sizes); 
  print_result("my_memset   ", results[func++], num_sizes);
  #ifdef DMA_AVAILABLE
  print_result("memset_dma  ", results[func++], num_sizes);
  print_result("memset_dma8 ", results[func++], num_sizes);
  #endif
  print_str("------------------------------------------\n");
  print_result("memmove     ", results[func++], num_sizes);
  print_result("my_memmove  ", results[func++], num_sizes);
  #ifdef DMA_AVAILABLE
  print_result("memmove_dma ", results[func++], num_sizes);
  print_result("memmove_dma8", results[func++], num_sizes);
  #endif

  print_str("------------------------------------------\n");

}

void project3(void)
{
  platform_init();

  //log_test();

  profile_memory();

  nrf_test();

  LOG_ID(SYSTEM_HALTED);
  LOG_FLUSH();

  //while(1) { log_flush(); };

  uint32_t i = 0;
  while (1) {
    i++;

    // TODO: figure out why this breaks logger on KL25Z (uart is fine in blocking mode)
    /* log_val(i, "Foo"); */

    LOG_FLUSH();

    //print_int(i); print_str((uint8_t*)" FTW!\n");
    //io_flush();
  }
}

