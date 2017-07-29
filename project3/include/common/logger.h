/**
 * @file logger.h
 * @brief Logger function declarations
 *
 * @author Jeff Schornick
 * @date 2017/07/27
**/

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <stdint.h>
#include <stddef.h>

typedef enum
{
  LOGGER_INITIALZED,
  GPIO_INITIALZED,
  SYSTEM_INITIALIZED,
  SYSTEM_HALTED,
  INFO,
  WARNING,
  ERROR,
  PROFILING_STARTED,
  PROFILING_COMPLETED,
  PROFILING_RESULT,
  DATA_RECEIVED,
  DATA_ANALYSIS_STARTED ,
  DATA_ANALYSIS_COMPLETED,
  DATA_ALPHA_COUNT,
  DATA_NUMERIC_COUNT,
  DATA_PUNCTUATION_COUNT,
  DATA_MISC_COUNT,
  HEARTBEAT
} Log_id_t;

typedef struct
{
  Log_id_t id;
  uint32_t time;
  size_t length;
  void *data;
} Log_t;


void log_data(uint8_t *data, size_t length);

void log_string(uint8_t *str);

void log_int(int32_t val);

void log_flush(void);


#endif /* __LOGGER_H__ */
