/**
 * @file cbfifo.c
 * @brief Fixed-size FIFO implemented via circular buffer
 *
 * This c file provides functionality for
 * initializing and reading/writing to a circular buffer
 *
 * @author Maurice Takeda
 * @date November 3, 2022
 * @version 1.0
 *
 */
#include <string.h>
#include <assert.h>
#include "MKL25Z4.h"
#include "cbfifo.h"


/**
 * @brief Initialize the data in the circular buffer FIFO
 *
 * @param cb - Pointer to circular buffer data structure
 *
 * @return none
 */
void cbfifo_init(cbfifo_t *cb) {
	// Check for validity of circular buffer
	if(!cb) return;

	// Initialize circular buffer
	memset(cb, 0, sizeof(cb->cbfifo));
	cb->wptr = 0;
	cb->rptr = 0;
	cb->length = 0;
	cb->full = false;
} // cbfifo_init()

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
size_t cbfifo_enqueue(cbfifo_t *cb, void *buf, size_t nbyte) {
	uint32_t masking_state;

    // Check for valid input
	if(!cb) return 0;
    if(!buf) return 0;

    size_t bytesEnqueued = 0;

    // Attempt to enqueue nbytes
    for(size_t i = 0; i < nbyte; i++) {
        // If full return amount of bytes enqueued
        if(cb->full) return bytesEnqueued;
        // Place bytes from buf into cbfifo one at a time
        cb->cbfifo[cb->wptr] = *(char*)(buf + i);
        // Increment number of bytes written from buf
        bytesEnqueued++;
        // Increment number the number of elements currently in the cbfifo
        masking_state = __get_PRIMASK();
        __disable_irq();
        cb->length++;
        __set_PRIMASK(masking_state);
        // wptr = (wptr + 1) % capacity
        cb->wptr = (cb->wptr + 1) & (CAPACITY - 1);
        // Update full flag
        cb->full = (cb->wptr == cb->rptr) ? true : false;
    }

    return bytesEnqueued;
} // cbfifo_enqueue()

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
size_t cbfifo_dequeue(cbfifo_t *cb, void *buf, size_t nbyte) {
	uint32_t masking_state;

    // Check for valid input
	if(!cb) return 0;
    if(!buf) return 0;

    size_t bytesCopied = 0;

    // Attempt to copy bytes
    for(size_t i = 0; i < nbyte; i++) {
        // If empty return amount of bytes copied
        if((cb->wptr == cb->rptr) && !(cb->full)) return bytesCopied;
        // Place bytes from cbfifo into buf one at a time
        *(char*)(buf + i) = cb->cbfifo[cb->rptr];
        // Increment number of bytes copied to buf
        bytesCopied++;
        // Decrement current amount of elements in the cbfifo
        masking_state = __get_PRIMASK();
        __disable_irq();
        cb->length--;
        __set_PRIMASK(masking_state);
        // rptr = (rptr + 1) % capacity
        cb->rptr = (cb->rptr + 1) & (CAPACITY - 1);
        // Update full flag
        cb->full = false;
    }

    return bytesCopied;
} // cbfifo_dequeue()


/**
 * @brief Check if the circular buffer is empty.
 *
 * @param cb - Pointer to circular buffer data structure
 *
 * @return True for empty, false otherwise.
 */
bool cbfifo_empty(cbfifo_t *cb) {
	return (cb->length == 0);
} // cbfifo_empty()

/**
 * @brief Check if the circular buffer is full.
 *
 * @param cb - Pointer to circular buffer data structure
 *
 * @return True for full, false otherwise
 */
bool cbfifo_full(cbfifo_t *cb) {
	return (cb->full);
} // cbfifo_full()


/**
 * @brief Returns the number of bytes currently on the FIFO.
 *
 * @param cb - Pointer to circular buffer data structure
 *
 * @return Number of bytes currently available to be dequeued from the FIFO
 */
size_t cbfifo_length(cbfifo_t *cb) {
	return (cb->length);
} // cbfifo_length()

/**
 * @brief Tests functionality of circular buffer API
 *
 * @return 0 for success.
 */
int cbfifo_test() {
	cbfifo_t cb_test;
	// Test cbfifo_init()
	cbfifo_init(&cb_test);
	for(int i = 0; i < CAPACITY; i++) {
		assert(cb_test.cbfifo[i] == 0);
	}
	assert(cb_test.full == false);
	assert(cb_test.length == 0);
	assert(cb_test.rptr == 0);
	assert(cb_test.wptr == 0);
	// Test cbfifo_empty()
	assert(cbfifo_empty(&cb_test));
	// Test cbfifo_enqueue()
	char buf_test[CAPACITY];
	memset(buf_test, 'x', CAPACITY);
	assert(cbfifo_enqueue(&cb_test, buf_test, CAPACITY) == CAPACITY);
	// Test cbfifo_length()
	assert(cbfifo_length(&cb_test) == CAPACITY);
	// Test cbfifo_full()
	assert(cbfifo_full(&cb_test));
	// Test cbfifo_dequeue()
	char char_test;
	assert(cbfifo_dequeue(&cb_test, &char_test, sizeof(char_test)) == sizeof(char_test));

	return 0;
} // test_cbfifo()
