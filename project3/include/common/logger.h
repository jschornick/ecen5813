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
#include "log_queue.h"

#if defined(LOG_OUT_NULL)
#define LOG_OUT(x)
#elif defined(LOG_OUT_BINARY)
#define LOG_OUT(x) log_send_binary(x)
#else
#define LOG_OUT(x) log_send_ascii(x)
#endif


void log_data(uint8_t *data, size_t length);

void log_string(uint8_t *str);

void log_int(int32_t val);


/* Non-blocking logger functions */

/* init the logging system? */
void logging_init();

void log_item(Log_t *item);

void log_msg(Log_id_t id, uint8_t *data, size_t length);

void log_info(char *str);

void log_val(int32_t val, char *label);

void log_flush(void);


/* output */

void log_send_ascii(Log_t *log);
void log_send_binary(Log_t *log);


#endif /* __LOGGER_H__ */
