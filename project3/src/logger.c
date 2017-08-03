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

#define SYSTEM_LOG_SIZE (1000)

Log_q system_log;
uint32_t faketime = 1000;

void log_data(uint8_t *data, size_t length)
{
  print_bytes(data, length);
}

void log_string(uint8_t *str)
{
  print_str(str);
}

void log_int(int32_t val)
{
  uint8_t str[ITOA_MAX_CHARS];
  uint8_t len;
  len = my_itoa(val, str, BASE_10);
  print_n(str, len-1);
}

/* init the logging system? */
void logging_init()
{
  lq_init(&system_log, SYSTEM_LOG_SIZE);
  log_msg( LOGGER_INITIALIZED, NULL, 0 );
}

void log_item(Log_t *item)
{
  item->time = faketime++;
  lq_add(&system_log, item);
}

void log_msg(Log_id_t id, uint8_t *data, size_t length)
{
  Log_t log;
  log.time = faketime++;
  log.id = id;
  log.length = length;
  log.data = data;
  lq_add(&system_log, &log);
}

void log_val(int32_t val, char *label)
{
  Log_t log;
  log.time = faketime++;
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
    // TODO: output readable vs binary?
    Log_t log;
    log.data = malloc(50);
    log.length = 50;
    lq_remove(&system_log, &log);
    log_int(log.time);
    log_string( (uint8_t *) " [");
    log_int(log.id);
    log_string( (uint8_t *) "] ");
    switch(log.id)
    {
      case LOGGER_INITIALIZED:
        log_string((uint8_t *) "Logger initialized.");
        break;
      case SYSTEM_INITIALIZED:
        log_string((uint8_t *) "System initialized.");
        break;
      case GPIO_INITIALIZED:
        log_string((uint8_t *) "GPIO initialized.");
        break;
      case SPI_INITIALIZED:
        log_string((uint8_t *) "SPI initialized.");
        break;
      case SYSTEM_HALTED:
        log_string((uint8_t *) "System halt.");
        break;
      case INFO:
        log_string( (uint8_t *) "INFO: " );
        log_string( log.data );
        break;
      case WARNING:
        log_string( (uint8_t *) "WARNING: " );
        log_string( log.data );
        break;
      case ERROR:
        log_string( (uint8_t *) "ERROR: " );
        log_string( log.data );
        break;
      case PROFILING_STARTED:
        log_string((uint8_t *) "Profiling start.");
        break;
      case PROFILING_COMPLETED:
        log_string((uint8_t *) "Profiling complete.");
        break;
      case PROFILING_RESULT:
        log_string((uint8_t *) "Profiling result: ");
        //TODO: add identifier and result
        break;
      case DATA_RECEIVED:
      case DATA_ANALYSIS_STARTED :
      case DATA_ANALYSIS_COMPLETED:
      case DATA_ALPHA_COUNT:
      case DATA_NUMERIC_COUNT:
      case DATA_PUNCTUATION_COUNT:
      case DATA_MISC_COUNT:
        log_string((uint8_t *) "Data processor log entry");
        break;
      case LABELED_VALUE:
        log_string( (uint8_t *) "INFO: ");
        log_string(log.data+4);
        log_string( (uint8_t *) " = ");
        log_int(*((uint32_t *) log.data));
        break;
      case HEARTBEAT:
        log_string((uint8_t *) ".");
        break;
    }
    log_string( (uint8_t *) "\n");
  }

}
