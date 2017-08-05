/**
 * @file log_queue.h
 * @brief Log queue function declarations
 *
 * Fixed size log buffer?
 *
 * @author Jeff Schornick
 * @date 2017/07/23
 **/

#ifndef __LOG_QUEUE_H__
#define __LOG_QUEUE_H__

#include <stdint.h>
#include <stddef.h>

#include "logger.h"

typedef struct
{
  volatile uint8_t *buffer; /* The address of the data buffer allocation */
  volatile uint8_t *head;   /* Points to the latest log item */
  volatile uint8_t *tail;   /* Points to the the first log item */
  size_t size;     /* Total amount of log data (bytes) */
  volatile size_t free;     /* Number of free bytes in the queue */
} Log_q;

typedef enum
{
  LQ_FALSE = 0, /* Buffer test was not true */
  LQ_OK,        /* Operation on buffer successful */
  LQ_ALLOC_ERR, /* Unable to allocate memory for new buffer */
  LQ_NULL,      /* Attempt to operate on null-pointer */
  LQ_SIZE_ERR,  /* Bad index or size parameter */
  LQ_FULL,      /* Buffer is full */
  LQ_EMPTY      /* Buffer is empty */
} Log_status_t;


/**
 * @brief Allocate and initialize a new log queue
 *
 * Allocate and initialize a new log queue that can hold up to `size` bytes of
 * log data. Log data includes both the log item meta data and all associated
 * data.
 *
 * A pointer to an existing Log_q structure must be provided as `queue`,
 * but none of its contents will be considered during initialization. Upon
 * successful allocation, LQ_OK will be returned and `queue` is ready to use.
 *
 * If allocation fails, LQ_ALLOC_FAIL will be returned.
 *
 * @param[in,out] queue A pointer to a circular buffer record
 * @param[in]     size  The number of bytes reserved for the log queue
 * @return Returns LQ_OK after successful initialization, otherwise an error status
 **/
Log_status_t lq_init(Log_q *queue, size_t size);

/**
 * @brief Destroy (deallocate) an existing log queue
 *
 * Deallocates the existing log queue buffer pointed to by `queue`.
 *
 * @param[in,out] queue A pointer to an existing log queue
 * @return Returns LQ_OK if successful, otherwise an error status
 **/
Log_status_t lq_destroy(Log_q *queue);

/**
 * @brief Add an log item to the queue
 *
 * Attemps to add a single uint8_t `item` to the head of the circular buffer
 * pointed to by `queue`.
 *
 * If the buffer is already full, the item will not be added and LQ_FULL will be
 * returned.
 *
 * @param[in,out] queue A pointer to an initialized log queue
 * @param[in]     item  Pointer to the log item to add
 * @return Returns LQ_OK if item is successfully added, otherwise and error status
 **/
Log_status_t lq_add(Log_q *queue, Log_t *item);

/**
 * @brief Remove an item from the log queue
 *
 * Attemps to remove a single uint8_t from the tail of the circular buffer. If successful,
 * the item will be stored at the address specified in `item` and LQ_OK will be returned.
 *
 * If the buffer is already empty, LQ_EMPTY will be returned.
 * An error of LQ_NULL will be returned if either parameter is null.
 *
 * @param[in,out] queue A pointer to an initialized circular buffer
 * @param[in]     data    The address where the removed item should be stored
 * @return Returns LQ_OK if item is successfully added, otherwise and error status
 **/
Log_status_t lq_remove(Log_q *queue, Log_t *item);

/**
 * @brief Check if the circular buffer is empty
 *
 * Returns LQ_EMPTY (nonzero) if the `queue` points to an empty circular
 * buffer.
 *
 * If the buffer is not empty, LQ_FALSE (0) will be returned instead.
 *
 * @param[in] queue A pointer to the initialized circular buffer to be checked
 * @return Returns LQ_EMPTY if buffer is empty, LQ_FALSE or error otherwise
 **/
Log_status_t lq_empty(Log_q *queue);



#endif /* __LOG_QUEUE_H__ */
