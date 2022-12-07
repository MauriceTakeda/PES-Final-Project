/**
 * @file cbfifo.h
 * @brief Fixed-size FIFO implemented via circular buffer
 *
 * This h file provides functionality for
 * initializing and reading/writing to a circular buffer
 *
 * @author Maurice Takeda
 * @date November 3, 2022
 * @version 1.0
 *
 */
#ifndef CBFIFO_H_
#define CBFIFO_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define CAPACITY    256  // Capacity of circular buffer

// Circular Buffer Data Structure
typedef struct cbfifo_s {
	char     cbfifo[CAPACITY];  // Circular buffer
	uint32_t length;            // Number of elements currently in the cbfifo
	uint32_t wptr;              // Write pointer
	uint32_t rptr;              // Read pointer
	bool     full;              // Is full flag
} cbfifo_t;

/**
 * @brief Initialize the data in the circular buffer FIFO
 *
 * @param cb - Pointer to circular buffer data structure
 *
 * @return none
 */
void cbfifo_init(cbfifo_t *cb);

/**
 * @brief Enqueues data onto the FIFO, up to the limit of the available FIFO
 *        capacity.
 *
 * @param cb    - Pointer to circular buffer data structure
 * @param buf   - Pointer to the data to enqueue
 * @param nbyte - Max number of bytes to enqueue
 *
 * @return The number of bytes actually enqueued, which could be 0. In case
 *         of an error, returns (size_t) -1.
 */
size_t cbfifo_enqueue(cbfifo_t *cb, void *buf, size_t nbyte);

/**
 * @brief Attempts to remove ("dequeue") up to nbyte bytes of data from the
 *        FIFO. Removed data will be copied into the buffer pointed to by buf.
 *
 * @param cb    - Pointer to circular buffer data structure
 * @param buf   - Destination for the dequeued data
 * @param nbyte - Bytes of data requested
 *
 * @return The number of bytes actually copied, which will be between 0 and
 *         nbyte.
 */
size_t cbfifo_dequeue(cbfifo_t *cb, void *buf, size_t nbyte);

/**
 * @brief Check if the circular buffer is empty.
 *
 * @param cb - Pointer to circular buffer data structure
 *
 * @return True for empty, false otherwise.
 */
bool cbfifo_empty(cbfifo_t *cb);

/**
 * @brief Check if the circular buffer is full.
 *
 * @param cb - Pointer to circular buffer data structure
 *
 * @return True for full, false otherwise
 */
bool cbfifo_full(cbfifo_t *cb);

/**
 * @brief Returns the number of bytes currently on the FIFO.
 *
 * @param cb - Pointer to circular buffer data structure
 *
 * @return Number of bytes currently available to be dequeued from the FIFO
 */
size_t cbfifo_length(cbfifo_t *cb);

/**
 * @brief Tests functionality of circular buffer API
 *
 * @return 0 for success.
 */
int cbfifo_test();

#endif /* CBFIFO_H_ */
