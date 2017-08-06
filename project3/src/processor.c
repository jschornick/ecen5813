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
#include "logger.h"
#include "processor.h"

/* The character used in the input stream to indicate that a report should be
   produced and sent to the output */
#define ASCII_ESCAPE 0x1B
//#define REPORT_CHAR  '\n'
#define REPORT_CHAR  ASCII_ESCAPE

/* Character statistics collected by the data processor */
data_stat_t stats;

/* Flag to indicate that statistics are ready to be output.
   This is set after a REPORT_CHAR character has been received. */
uint8_t display_flag = 0;

void processor_init()
{
  stats.total = 0;
  stats.upper = 0;
  stats.lower = 0;
  stats.numeric = 0;
  stats.symbol = 0;
  stats.control = 0;
  LOG_ID(DATA_ANALYSIS_STARTED);
}

void process_chars(uint8_t *chars, uint16_t count)
{
  for(uint16_t i=0; i<count; i++)
  {
    if(chars[i] == REPORT_CHAR)
    {
      LOG_ID(DATA_ANALYSIS_COMPLETED);
      log_statistics();
      processor_init();
      continue;
    }
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
  }
}

void log_statistics()
{
  LOG_INT(DATA_ALPHA_COUNT, stats.upper + stats.lower);
  LOG_INT(DATA_NUMERIC_COUNT, stats.numeric);
  LOG_INT(DATA_PUNCTUATION_COUNT, stats.symbol);
  LOG_INT(DATA_MISC_COUNT, stats.control);
}

void dump_statistics()
{
  uint32_t printable;
  print_str("\r\n");
  print_str("------------------------------\r\n" );
  print_str("| Statistics report:\r\n" );

  printable = stats.upper + stats.lower + stats.numeric + stats.symbol;
  print_str("|     Total: " );
  print_int(stats.total);

  print_str(" (" );
  print_int(printable);
  print_str(" printable)\r\n" );

  print_str("|     Alpha: " );
  print_int(stats.upper + stats.lower);
  print_str(" (U: " );
  print_int(stats.upper);
  print_str(", L: " );
  print_int(stats.lower);
  print_str(")\r\n" );

  print_str("|   Numeric: " );
  print_int(stats.numeric);
  print_str("\r\n" );

  print_str("|    Symbol: " );
  print_int(stats.symbol);
  print_str("\r\n" );

  print_str("|   Control: " );
  print_int(stats.control);
  print_str("\r\n" );

  print_str("------------------------------\r\n" );
}
