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
#include "log_queue.h"
#include "timer.h"
#include "logger.h"

#define SYSTEM_LOG_SIZE (2000)

Log_q system_log;
uint32_t log_epoch;

/* init the logging system? */
void logging_init()
{
  log_epoch = get_time();
  lq_init(&system_log, SYSTEM_LOG_SIZE);
  log_id( LOGGER_INITIALIZED );
}

/* ** RAW LOGGING **/

void log_raw_data(uint8_t *data, size_t length)
{
  print_bytes(data, length);
}

void log_raw_string(uint8_t *str)
{
  print_str(str);
}

void log_raw_int(int32_t val)
{
  print_int(val);
}

/* ** QUEUED LOGGING **/

void log_item(Log_t *item)
{
  item->time = get_time();
  lq_add(&system_log, item);
}

void logx(Log_id_t id, Log_data_t type, void *data, size_t length)
{
  Log_t log;
  log.time = get_time();
  log.id = id;
  log.type = type;
  log.length = length;
  log.data = data;
  lq_add(&system_log, &log);
}

void log_data(Log_id_t id, void *data, size_t length)
{
  logx(id, LD_DATA, data, length);
}

void log_str(Log_id_t id, char *str)
{
  logx(id, LD_STR, str, strlen(str) + 1);
}

void log_int(Log_id_t id, int32_t val)
{
  logx(id, LD_INT, &val, 4);
}

void log_val(Log_id_t id, int32_t val, char *name)
{
  Log_t log;
  log.time = get_time();
  log.id = id;
  log.type = LD_NVAL;
  log.length = sizeof(val) + strlen(name) + 1;
  log.data = malloc(log.length);
  *( (uint32_t *) log.data) = val;
  strncpy((char *) log.data+ sizeof(val), name, log.length-sizeof(val));
  lq_add(&system_log, &log);
  free(log.data); /* Leak fixed?! */
}

void log_id(Log_id_t id)
{
  logx(id, LD_NULL, NULL, 0);
}

void log_info(char *str)
{
  logx(INFO, LD_STR, str, strlen(str) + 1);
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

const char *log_id_str[] =
  {
    "LOGGER_INITIALIZED",
    "SYSTEM_INITIALIZED",
    "SYSTEM_HALTED",
    "GPIO_INITIALIZED",
    "SPI_INITIALIZED",
    "INFO",
    "WARNING",
    "ERROR",
    "PROFILING_STARTED",
    "PROFILING_COMPLETED",
    "PROFILING_RESULT",
    "NRF_ADDRESS",
    "DATA_RECEIVED",
    "DATA_ANALYSIS_STARTED",
    "DATA_ANALYSIS_COMPLETED",
    "DATA_ALPHA_COUNT",
    "DATA_NUMERIC_COUNT",
    "DATA_PUNCTUATION_COUNT",
    "DATA_MISC_COUNT",
    "HEARTBEAT"
  };

// display log as ascii through output device
void log_send_ascii(Log_t *log)
{
  print_int(log->time);
  print_str( (uint8_t *) " [");
  print_str( (uint8_t *) log_id_str[log->id] );
  print_str( (uint8_t *) "] ");
  switch(log->type) {
    case LD_NULL:
      break;
    case LD_INT:
      print_int( *(log->data) );
      break;
    case LD_STR:
      print_str( log->data );
      break;
    case LD_NVAL:
      print_str(log->data+4);
      print_str( (uint8_t *) " = ");
      print_int(*((uint32_t *) log->data));
      break;
    case LD_DATA:
      switch(log->id) {
      case NRF_ADDRESS:
        print_str( (uint8_t *) "nRF address: ");
        print_bytes((uint8_t *) log->data, 5);
        break;
      default:
        break;
      }
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
