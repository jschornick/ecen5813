/**
 * @file logger.c
 * @brief Logger function definitions
 *
 *
 * @author Jeff Schornick
 * @date 2017/07/27
 **/

#include <stdlib.h>
#include <string.h>

#include "io.h"
#include "conversion.h"
#include "logger.h"
#include "log_queue.h"
#include "timer.h"

#define SYSTEM_LOG_SIZE (1000)

Log_q system_log;
uint32_t log_epoch;

/* init the logging system? */
void logging_init()
{
  log_epoch = get_time();
  lq_init(&system_log, SYSTEM_LOG_SIZE);
  log_msg( LOGGER_INITIALIZED, NULL, 0 );
}

void log_data(uint8_t *data, size_t length)
{
  print_bytes(data, length);
}

void log_string(uint8_t *str)
{
  print_str(str);
}

// TODO: Use io:print_int()
void log_int(int32_t val)
{
  uint8_t str[ITOA_MAX_CHARS];
  uint8_t len;
  len = my_itoa(val, str, BASE_10);
  print_n(str, len-1);
}

void log_item(Log_t *item)
{
  item->time = get_time();
  lq_add(&system_log, item);
}

void log_msg(Log_id_t id, uint8_t *data, size_t length)
{
  Log_t log;
  log.time = get_time();
  log.id = id;
  log.length = length;
  log.data = data;
  lq_add(&system_log, &log);
}

void log_val(int32_t val, char *label)
{
  Log_t log;
  log.time = get_time();
  log.id = LABELED_VALUE;
  log.length = sizeof(val) + strlen(label) + 1;
  log.data = malloc(log.length);
  *( (uint32_t *) log.data) = val;
  strncpy((char *) log.data+4, label, log.length-sizeof(val));
  lq_add(&system_log, &log);
}

void log_info(char *str)
{
  log_msg(INFO, (uint8_t *) str, strlen(str) + 1);
}

void log_flush(void)
{
  while( !lq_empty(&system_log) )
  {
    Log_t log;
    log.data = malloc(50);
    log.length = 50;
    lq_remove(&system_log, &log);
    LOG_OUT(&log);
  }
  io_flush();
}

// display log as ascii through output device
void log_send_ascii(Log_t *log)
{
  print_int(log->time);
  print_str( (uint8_t *) " [");
  print_int(log->id);
  print_str( (uint8_t *) "] ");
  switch(log->id)
    {
    case LOGGER_INITIALIZED:
      print_str((uint8_t *) "Logger initialized.");
      break;
    case SYSTEM_INITIALIZED:
      print_str((uint8_t *) "System initialized.");
      break;
    case GPIO_INITIALIZED:
      print_str((uint8_t *) "GPIO initialized.");
      break;
    case SPI_INITIALIZED:
      print_str((uint8_t *) "SPI initialized.");
      break;
    case SYSTEM_HALTED:
      print_str((uint8_t *) "System halt.");
      break;
    case INFO:
      print_str( (uint8_t *) "INFO: " );
      print_str( log->data );
      break;
    case WARNING:
      print_str( (uint8_t *) "WARNING: " );
      print_str( log->data );
      break;
    case ERROR:
      print_str( (uint8_t *) "ERROR: " );
      print_str( log->data );
      break;
    case PROFILING_STARTED:
      print_str((uint8_t *) "Profiling start.");
      break;
    case PROFILING_COMPLETED:
      print_str((uint8_t *) "Profiling complete.");
      break;
    case PROFILING_RESULT:
      print_str((uint8_t *) "Profiling result: ");
      //TODO: add identifier and result
      break;
    case DATA_RECEIVED:
    case DATA_ANALYSIS_STARTED :
    case DATA_ANALYSIS_COMPLETED:
    case DATA_ALPHA_COUNT:
    case DATA_NUMERIC_COUNT:
    case DATA_PUNCTUATION_COUNT:
    case DATA_MISC_COUNT:
      print_str( (uint8_t *) "Data processor log entry");
      break;
    case LABELED_VALUE:
      print_str( (uint8_t *) "INFO: ");
      print_str(log->data+4);
      print_str( (uint8_t *) " = ");
      log_int(*((uint32_t *) log->data));
      break;
    case NRF_ADDRESS:
      print_str( (uint8_t *) "nRF address: ");
      print_bytes((uint8_t *) log->data, 5);
      break;
    case HEARTBEAT:
      print_str((uint8_t *) ".");
      break;
    }
  print_str( (uint8_t *) "\n");
}

void log_send_binary(Log_t *log)
{
  //TODO: replace this with a direct dump of the entire log queue buffer
  print_n((uint8_t *) log, LOG_HEADER_SIZE);
  print_n(log->data, log->length);
}
