/**
 * @file circular_buffer.h
 * @brief Circular buffer function declarations
 *
 * A circular buffer is a FIFO data structure which consumes a fixed amount of
 * contiguous memory for its data. It supports adding a new item onto the head,
 * or removing the oldest item from the tail.
 *
 * This circular buffer only supports data elements of type uint8_t.
 *
 * @author Jeff Schornick
 * @date 2017/07/12
 **/

#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

#include <stdint.h>
#include <stddef.h>

typedef struct
{
  volatile uint8_t *buffer;
  volatile uint8_t *head;
  volatile uint8_t *tail;
  size_t size;
  volatile size_t count;
} CircBuf_t;

typedef enum
  {
    CB_FALSE = 0, /* Buffer test was not true */
    CB_OK,        /* Operation on buffer successful */
    CB_ALLOC_ERR, /* Unable to allocate memory for new buffer */
    CB_NULL,      /* Attempt to operate on null-pointer */
    CB_FULL,      /* Buffer is full */
    CB_EMPTY,     /* Buffer is empty */
    CB_INDEX_ERR  /* Tried to operate on an invalid index */
  } CB_status_t;


/**
 * @brief Allocate and initialize a new circular buffer
 *
 * Allocate and initialize a new circular buffer that can hold `size` byte-sized
 * elements.
 *
 * A pointer to an existing CircBuf_t structure must be provided as `circbuf`,
 * but none of its contents will be considered during initialization. Upon
 * successful allocation, CB_OK will be returned and `circbuf` is ready to use.
 *
 * If allocation fails, CB_ALLOC_FAIL will be returned.
 *
 * @param[in,out] circbuf A pointer to a circular buffer record
 * @param[in]     size    The number of elements the new buffer should hold
 * @return Returns CB_OK after successful initialization, otherwise an error status
 **/
CB_status_t CB_init(CircBuf_t *circbuf, size_t size);

/**
 * @brief Destroy (deallocate) an existing circular buffer
 *
 * Deallocates the existing circular buffer pointed to by `circbuf`.
 *
 * @param[in,out] circbuf A pointer to an existing circular buffer
 * @return Returns CB_OK if successful, otherwise an error status
 **/
CB_status_t CB_destory(CircBuf_t *circbuf);

/**
 * @brief Add an item to a circular buffer
 *
 * Attemps to add a single uint8_t `item` to the head of the circular buffer
 * pointed to by `circbuf`.
 *
 * If the buffer is already full, the item will not be added and CB_FULL will be
 * returned.
 *
 * @param[in,out] circbuf A pointer to an initialized circular buffer
 * @param[in]     data    The item to add to the buffer
 * @return Returns CB_OK if item is successfully added, otherwise and error status
 **/
CB_status_t CB_add_item(CircBuf_t *circbuf, uint8_t item);

/**
 * @brief Remove an item to a circular buffer
 *
 * Attemps to remove a single uint8_t from the tail of the circular buffer. If successful,
 * the item will be stored at the address specified in `item` and CB_OK will be returned.
 *
 * If the buffer is already empty, CB_EMPTY will be returned.
 * An error of CB_NULL will be returned if either parameter is null.
 *
 * @param[in,out] circbuf A pointer to an initialized circular buffer
 * @param[in]     data    The address where the removed item should be stored
 * @return Returns CB_OK if item is successfully added, otherwise and error status
 **/
CB_status_t CB_remove_item(CircBuf_t *circbuf, uint8_t *item);

/**
 * @brief Check if the circular buffer is full
 *
 * Returns CB_FULL (nonzero) if the `circbuf` points to a full circular buffer.
 *
 * If the buffer is not full, CB_FALSE (0) will be returned instead.
 *
 * @param[in] circbuf A pointer to the initialized circular buffer to be checked
 * @return Returns CB_FULL if buffer is full, CB_FALSE or error otherwise
 **/
CB_status_t CB_is_full(CircBuf_t *circbuf);

/**
 * @brief Check if the circular buffer is empty
 *
 * Returns CB_EMPTY (nonzero) if the `circbuf` points to an empty circular
 * buffer.
 *
 * If the buffer is not empty, CB_FALSE (0) will be returned instead.
 *
 * @param[in] circbuf A pointer to the initialized circular buffer to be checked
 * @return Returns CB_EMPTY if buffer is empty, CB_FALSE or error otherwise
 **/
CB_status_t CB_is_empty(CircBuf_t *circbuf);

/**
 * @brief Read on the Nth newest item in the circular buffer
 *
 * Reads the Nth newest item (N items back from the head) in the circular buffer
 * pointed to by `circbuf` and returns CB_OK on success.
 *
 * If N-1 is greater than the number of items stored in the buffer, CB_INDEX_ERR
 * will be returned.
 *
 * TODO: Should this return the value read?
 *
 * @param[in] circbuf  A pointer to the initialized circular buffer to be read
 * @param[in] position The position back from the head to peek into
 * @return Returns CB_EMPTY if buffer is empty, CB_FALSE or error otherwise
 **/
CB_status_t CB_peek(CircBuf_t *circbuf, size_t position);


#endif /* __CIRCULAR_BUFFER_H__ */
