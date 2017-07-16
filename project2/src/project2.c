/**
 * @file project2.c
 * @brief Project 2 deliverables
 *
 * The primary deliverable for Project 2 is a character stream data processor.
 *
 * @author Jeff Schornick
 * @date 2017/07/06
**/

#include <stdint.h>
#include "project2.h"
#include "platform.h"
#include "processor.h"
#include "io.h"

#define MAX_CHARS 100  /* Maximum to read at a time */
uint8_t chars[MAX_CHARS];

void project2(void)
{
  platform_init();

  init_stats();
  print( (uint8_t *) "\r\nData processor ready!\r\n" );

  /* Main character processing loop. Continually read any available characters
     and produce a report when requested. */
  while(1)
  {
    size_t rx_count;
    /* Read as many characters are available, and immediately process them. */
    rx_count = read_str(chars, MAX_CHARS);
    if( rx_count > 0 ) {
      process_chars( chars, rx_count );
    }

    /* If the display processor has caught a signal to report, go ahead and
       dump the statistics. */
    if( display_flag ) {
      dump_statistics();
      display_flag = 0;
    }

    /* Heartbeat so we know the system is alive and well. */
    heartbeat();
  }
}
