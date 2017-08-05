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
  LOGGER_INITIALIZED = 0,
  SYSTEM_INITIALIZED,
  SYSTEM_HALTED,
  GPIO_INITIALIZED,
  SPI_INITIALIZED,
  INFO,
  WARNING,
  ERROR,
  PROFILING_STARTED,
  PROFILING_COMPLETED,
  PROFILING_RESULT,
  NRF_ADDRESS,
  DATA_RECEIVED,
  DATA_ANALYSIS_STARTED ,
  DATA_ANALYSIS_COMPLETED,
  DATA_ALPHA_COUNT,
  DATA_NUMERIC_COUNT,
  DATA_PUNCTUATION_COUNT,
  DATA_MISC_COUNT,
  HEARTBEAT,
  LOG_ID_MAX
} Log_id_t;

typedef enum
{
  LD_NULL = 0, /* no data */
  LD_DATA,     /* length, data */
  LD_INT,      /* 32-bit int */
  LD_STR,      /* null-terminated */
  LD_NVAL      /* 32-bit int, null-terminated label */
} Log_data_t;

typedef struct
{
  Log_id_t id;    /* log type */
  Log_data_t type;  /* log data type */
  uint32_t time;    /* time in seconds */
  uint32_t ms;    /* time in ms */
  size_t length;  /* length of data */
  uint8_t *data;  /* log data of `length` bytes */
} Log_t;

/* TODO: Does this handle all alignment padding isuuse? Equals 16, not 12!  */
#define LOG_HEADER_SIZE (sizeof(Log_t) - sizeof(uint8_t *))

#if defined(LOG_OUT_NULL)
#define LOG_OUT(x)
#elif defined(LOG_OUT_BINARY)
#define LOG_OUT(x) log_send_binary(x)
#else
#define LOG_OUT(x) log_send_ascii(x)
#endif


void log_raw_data(uint8_t *data, size_t length);

void log_raw_string(uint8_t *str);

void log_raw_int(int32_t val);


/* Non-blocking logger functions */

/* init the logging system? */
void logging_init();

void log_item(Log_t *item);

void log_data(Log_id_t id, void *data, size_t length);

void log_id(Log_id_t id);
void log_str(Log_id_t id, char *str);
void log_int(Log_id_t id, int32_t val);
void log_val(Log_id_t, int32_t val, char *name);

void log_info(char *str);

void log_flush(void);


/* output */

void log_send_ascii(Log_t *log);
void log_send_binary(Log_t *log);


#endif /* __LOGGER_H__ */
