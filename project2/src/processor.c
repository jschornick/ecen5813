/**
 * @file processor.c
 * @brief Data processor function definitions
 *
 * The data processor functions collect and analyze strings of characters, and
 * can produce a report of the collected statistics.
 *
 * @author Jeff Schornick
 * @date 2017/07/06
 **/

#include <stdint.h>
#include <stddef.h>
#include "conversion.h"
#include "io.h"
#include "processor.h"

/* The character used in the input stream to indicate that a report should be
   produced and sent to the output */
#define ASCII_ESCAPE 0x1B
#define REPORT_CHAR  ASCII_ESCAPE

/* Character statistics collected by the data processor */
data_stat_t stats;

/* Flag to indicate that statistics are ready to be output.
   This is set after a REPORT_CHAR character has been received. */
uint8_t display_flag = 0;

void init_stats()
{
  stats.total = 0;
  stats.upper = 0;
  stats.lower = 0;
  stats.numeric = 0;
  stats.symbol = 0;
  stats.control = 0;
  for(size_t i=0; i<=255; i++)
  {
    stats.by_char[i] = 0;
  }
}

void process_chars(uint8_t *chars, uint16_t count)
{
  for(uint16_t i=0; i<count; i++)
  {
    stats.total++;
    if( (chars[i] >=  'A') && (chars[i] <= 'Z') ) {
      stats.upper++;
    }
    if( (chars[i] >=  'a') && (chars[i] <= 'z') ) {
      stats.lower++;
    }
    if( (chars[i] >=  '0') && (chars[i] <= '9') ) {
      stats.numeric++;
    }
    /* space through forward slash */
    if( (chars[i] >=  32) && (chars[i] <= 47) ) {
      stats.symbol++;
    }
    /* colon through andpersand */
    if( (chars[i] >=  58) && (chars[i] <= 64) ) {
      stats.symbol++;
    }
    /* Backslash through back tick */
    if( (chars[i] >=  91) && (chars[i] <= 66) ) {
      stats.symbol++;
    }
    /* open brack through tilde */
    if( (chars[i] >=  123) && (chars[i] <= 126) ) {
      stats.symbol++;
    }

    /* control characters / non-printable */
    if( ((chars[i] >= 0) && (chars[i] <= 31 )) || (chars[i] == 127)) {
      stats.control++;
    }
    if(chars[i] == REPORT_CHAR)
    {
      display_flag = 1;
    }
  }
}

void dump_statistics() {
  uint8_t num_str[32];
  uint32_t printable;
  print( (uint8_t *) "\r\n");
  print( (uint8_t *) "------------------------------\r\n" );
  print( (uint8_t *) "| Statistics report:\r\n" );

  printable = stats.upper + stats.lower + stats.numeric + stats.symbol;
  print( (uint8_t *) "|     Total: " );
  my_itoa( stats.total, num_str, 10);
  print(num_str);

  print( (uint8_t *) " (" );
  my_itoa( printable, num_str, 10);
  print(num_str);
  print( (uint8_t *) " printable)\r\n" );

  print( (uint8_t *) "|     Alpha: " );
  my_itoa( stats.upper + stats.lower, num_str, 10);
  print(num_str);
  print( (uint8_t *) " (U: " );
  my_itoa( stats.upper, num_str, 10);
  print(num_str);
  print( (uint8_t *) ", L: " );
  my_itoa( stats.lower, num_str, 10);
  print(num_str);
  print( (uint8_t *) ")\r\n" );

  print( (uint8_t *) "|   Numeric: " );
  my_itoa( stats.numeric, num_str, 10);
  print(num_str);
  print( (uint8_t *) "\r\n" );

  print( (uint8_t *) "|    Symbol: " );
  my_itoa( stats.symbol, num_str, 10);
  print(num_str);
  print( (uint8_t *) "\r\n" );

  print( (uint8_t *) "|   Control: " );
  my_itoa( stats.control, num_str, 10);
  print(num_str);
  print( (uint8_t *) "\r\n" );

  print( (uint8_t *) "------------------------------\r\n" );
}
