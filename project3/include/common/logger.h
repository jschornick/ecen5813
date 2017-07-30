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

void log_data(uint8_t *data, size_t length);

void log_string(uint8_t *str);

void log_int(int32_t val);

/* init the logging system? */
void logging_init();

void log_item(Log_t *item);

void log_msg(Log_id_t id, uint8_t *data, size_t length);

void log_flush(void);


#endif /* __LOGGER_H__ */
