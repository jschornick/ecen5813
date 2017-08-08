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
#include <stdlib.h>
#include "project3.h"
#include "platform.h"
#include "logger.h"
#include "nrf.h"
#include "conversion.h"
#include "string.h"  // memmove, memset
#include "profile.h"
#include "memory.h"
#include "io.h"
#include "processor.h"
#ifdef KL25Z
#include "memory_dma.h"
#include "dma.h"
#endif


void logging_demo(void)
{

  // Raw, non-queued logging
  uint8_t data[] = { 0xAB, 0xCD, 0xEF, 0x01 };
  data[0] = data[0];
  LOG_RAW_STRING("Hello\n");
  LOG_RAW_DATA( (uint8_t *) &data, 4);
  LOG_RAW_STRING("\n");
  LOG_RAW_INT(1234567890);
  LOG_RAW_STRING("\n");

  // Log queue logging
  Log_t foo;
  foo = foo;
  foo.id = INFO;
  foo.type = LD_STR;
  foo.length = 10;
  foo.data = (uint8_t *) "123456789";
  LOG_ITEM( &foo );

  LOG_STR(ERROR, "Life is better when you don't have to count");
  LOG_VAL(WARNING, 42, "My cool variable");
  LOG_INFO("Super quick debug!");

  LOG_FLUSH();
}


void nrf_demo() {

  uint8_t addr[5];
  LOG_INFO("Read NRF Registers");
  LOG_VAL(INFO, nrf_read_config(), "CONFIG");
  LOG_VAL(INFO, nrf_read_rf_setup(), "RF_SETUP");
  LOG_VAL(INFO, nrf_read_rf_ch(), "RF_CH");
  LOG_VAL(INFO, nrf_read_fifo_status(), "FIFO_STATUS");
  LOG_VAL(INFO, nrf_read_status(), "STATUS");
  nrf_read_tx_addr(addr);
  LOG_DATA(NRF_ADDRESS, addr, 5);
  LOG_FLUSH();

  LOG_INFO("Write NRF Registers");
  uint8_t config_reg;
  config_reg = nrf_read_config();
  LOG_VAL(INFO, config_reg, "CONFIG");

  LOG_INFO("Disabling CRC");
  nrf_write_config( config_reg & ~NRF_CNF_EN_CRC_MASK);
  config_reg = nrf_read_config();
  LOG_VAL(INFO, config_reg, "CONFIG");

  LOG_INFO("Enabling CRC");
  nrf_write_config( config_reg | NRF_CNF_EN_CRC(1) );
  LOG_VAL(INFO, nrf_read_config(), "CONFIG");

  uint8_t rf_setup;
  rf_setup = nrf_read_rf_setup();
  LOG_VAL(INFO, rf_setup, "RF_SETUP");

  LOG_INFO("Setting power to -6dBm");
  rf_setup &= ~NRF_RFS_RF_PWR_MASK;
  nrf_write_rf_setup( rf_setup | NRF_RFS_RF_PWR(2));
  rf_setup = nrf_read_rf_setup();
  LOG_VAL(INFO, rf_setup, "RF_SETUP");

  LOG_INFO("Setting power to -12dBm");
  rf_setup &= ~NRF_RFS_RF_PWR_MASK;
  nrf_write_rf_setup( rf_setup | NRF_RFS_RF_PWR(1));
  rf_setup = nrf_read_rf_setup();
  LOG_VAL(INFO, rf_setup, "RF_SETUP");

  uint8_t test_addr1[] = { 0x01, 0x02, 0x03, 0x04, 0x05 };
  uint8_t test_addr2[] = { 0xab, 0xcd, 0xef, 0x01, 0x02 };
  LOG_INFO("Change TX Address");
  nrf_write_tx_addr(test_addr1);
  nrf_read_tx_addr(addr);
  LOG_DATA(NRF_ADDRESS, addr, 5);
  nrf_write_tx_addr(test_addr2);
  nrf_read_tx_addr(addr);
  LOG_DATA(NRF_ADDRESS, addr, 5);
  LOG_FLUSH();
}

#ifdef KL25Z
extern uint32_t __BUFFER_START;
uint8_t *buffer1 = (uint8_t *) &__BUFFER_START;
uint8_t *buffer2 = ((uint8_t *) &__BUFFER_START) + PROF_BUFFER_SIZE;
#else
uint8_t buffer1[PROF_BUFFER_SIZE];
uint8_t buffer2[PROF_BUFFER_SIZE];
#endif

void print_result(char *func, uint32_t *results, uint8_t sizes) {
  print_str("| ");
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
  size_t test_sizes[] = {10,100,1000,5000};
  uint32_t results[num_funcs][num_sizes];
  uint8_t func;

  LOG_ID(PROFILING_STARTED);

  for(uint8_t i=0; i<num_sizes; i++)
  {
    uint32_t test_size = test_sizes[i];
    func = 0;

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

  LOG_ID(PROFILING_COMPLETED);

  print_str("+------------------------------------------+\n");
  print_str("| -+-        Profiling Results         -+- |\n");
  print_str("|------------------------------------------|\n");
  print_str("|              |   10 |  100 | 1000 | 5000 |\n");
  print_str("|------------------------------------------|\n");

  func = 0;
  print_result("memset      ", results[func++], num_sizes);
  print_result("my_memset   ", results[func++], num_sizes);
  #ifdef DMA_AVAILABLE
  print_result("memset_dma  ", results[func++], num_sizes);
  print_result("memset_dma8 ", results[func++], num_sizes);
#endif
  print_str("|------------------------------------------|\n");
  print_result("memmove     ", results[func++], num_sizes);
  print_result("my_memmove  ", results[func++], num_sizes);
  #ifdef DMA_AVAILABLE
  print_result("memmove_dma ", results[func++], num_sizes);
  print_result("memmove_dma8", results[func++], num_sizes);
  #endif

  print_str("+------------------------------------------+\n");
  LOG_FLUSH();

}

#define MAX_CHARS 100  /* Maximum to read at a time */
uint8_t chars[MAX_CHARS];
void project3(void)
{
  platform_init();
  //logging_demo();

  #ifdef PROFILER
  profile_calibrate();
  profile_memory();
  profile_memory();
  profile_memory();
  profile_memory();
  #endif

  #ifdef NRF
  nrf_demo();
  #endif

  #ifdef DATAPROCESSOR
  processor_init();
  while (1) {
    size_t rx_count;
    rx_count = read_str( (char *) chars, MAX_CHARS);
    if( rx_count > 0 ) {
      process_chars( chars, rx_count );
    }
    LOG_FLUSH();
  }
  #endif

  // Show a few heartbeats...
  while(1)
  {
    LOG_FLUSH();
  }
  LOG_ID(SYSTEM_HALTED);
  LOG_FLUSH();
}

