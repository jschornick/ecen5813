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
  DATA_TOTAL_COUNT,
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
} __attribute__((packed)) Log_t;

#define LOG_HEADER_SIZE (sizeof(Log_t) - sizeof(uint8_t *))

#if defined(LOG_OUT_NULL)
#define LOG_OUT(x)
#elif defined(LOG_OUT_BINARY)
#define LOG_OUT(x) log_send_binary(x)
#else
#define LOG_OUT(x) log_send_ascii(x)
#endif


void log_raw_data(uint8_t *data, size_t length);
void log_raw_string(const char *str);
void log_raw_int(int32_t val);

#ifdef DISABLE_LOG
#define LOG_RAW_DATA(...)
#define LOG_RAW_STRING(...)
#define LOG_RAW_INT(...)
#else
#define LOG_RAW_DATA log_raw_data
#define LOG_RAW_STRING log_raw_string
#define LOG_RAW_INT log_raw_int
#endif


/* Non-blocking logger functions */

void logging_init();
void log_item(Log_t *item);
void log_data(Log_id_t id, void *data, size_t length);
void log_id(Log_id_t id);
void log_str(Log_id_t id, const char *str);
void log_int(Log_id_t id, int32_t val);
void log_val(Log_id_t, int32_t val, char *name);
void log_info(const char *str);
void log_flush(void);

#ifdef DISABLE_LOG
#define LOGGING_INIT()
#define LOG_ITEM(...)
#define LOG_DATA(...)
#define LOG_ID(...)
#define LOG_STR(...)
#define LOG_INT(...)
#define LOG_VAL(...)
#define LOG_INFO(...)
#define LOG_FLUSH(...)
#else
#define LOGGING_INIT logging_init
#define LOG_ITEM log_item
#define LOG_DATA log_data
#define LOG_ID log_id
#define LOG_STR log_str
#define LOG_INT log_int
#define LOG_VAL log_val
#define LOG_INFO log_info
#define LOG_FLUSH log_flush
#endif


/* output */

void log_send_ascii(Log_t *log);
void log_send_binary(Log_t *log);


#endif /* __LOGGER_H__ */
