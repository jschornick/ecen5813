/**
 * @file processor.h
 * @brief Data processor function declarations and types
 *
 * The data processor functions collect and analyze strings of characters, and
 * can produce a report of the collected statistics.
 *
 * @author Jeff Schornick
 * @date 2017/07/15
 **/

#ifndef __PROCESSOR_H__
#define __PROCESSOR_H__

typedef struct {
  uint32_t total;
  uint32_t upper;
  uint32_t lower;
  uint32_t numeric;
  uint32_t symbol;
  uint32_t control;
} data_stat_t;

/* A flag that indicates processing has completed, and it statistics are ready
   to be displayed. */
extern uint8_t display_flag;

/**
 * @brief Initialize the data processor statistics
 *
 * Zeros all counters that are collected by the data processor. Can be called at
 * any time to reset the statistics.
 *
 * @return Nothing returned
 **/
void processor_init();

/**
 * @brief Process a string of characters and update the statistic counters
 *
 * Reads exactly `count` characters from the character array pointed to by
 * `chars`. Each character is analayzed and the appropriate statistic counters
 * are updated. The `display_flag` global will be updated to reflect if the
 * report-request trigger character was received in the input stream.
 *
 * @param[in] chars A pointer to the character array to analyze
 * @param[in] count The number of characters to analyze
 * @return Nothing returned, `display_flag` global may be updated
 **/
void process_chars(uint8_t *chars, uint16_t count);

/**
 * @brief Display a report of the collected statistics
 *
 * Displays a formatted report of the character statistics previously collected
 * by the data processor. The report will be displayed via whatever mechanism is
 * appropriate for the platform (e.g., UART, standard out).
 *
 * @return Nothing returned
 **/
void dump_statistics();

/**
 * @brief Log a report of the collected statistics
 *
 * Logs the counts of the character statistics previously collected
 * by the data processor.
 *
 * @return Nothing returned
 **/
void log_statistics(void);

#endif /* __PROCESSOR_H__ */
