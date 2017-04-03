#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_

#include <stdbool.h>

/**
 * SP Bounded Priority Queue summary
 *
 *   represents a bounded queue which contains array of elements,
 *   max size of the array and current size of the array
 *   each element in the array contains value(double) and index(integer)
 *
 * The following functions are supported:
 *
 * 1- spBPQueueCreate- creates an empty queue with a given maximum capacity (complexity O(n))
 * 2- spBPQueueCopy- creates a copy of a given queue (complexity O(n))
 * 3- spBPQueueDestroy- frees all memory allocation associated with the queue (complexity O(n))
 * 4- spBPQueueClear- removes all the elements in the queue (complexity O(n) / Could be in O(1))
 * 5- spBPQueueSize- returns the number of elements in the queue (complexity O(1))
 * 6- spBPQueueGetMaxSize- returns the maximum capacity of the queue (complexity O(1))
 * 7- spBPQueueEnqueue- Inserts an element to the queue (complexity O(n))
 * 8- spBPQueueDequeue- removes the element with the lowest value (complexity O(1)
 * 9- spBPQueuePeek- Returns a copy of the element with the lowest value (Complexity O(1))
 * 10- spBPQueuePeekLast - Returns a copy of the element with the highest value (Complexity O(1))
 * 11- spBPQueueMinValue- returns the minimum value in the queue (complexity O(1))
 * 12- spBPQueueMaxValue- returns the maximum value in the queue(complexity O(1))
 * 13- spBPQueueIsEmpty – returns true if the queue is empty(complexity O(1))
 * 14- spBPQueueIsFull- returns true if the queue is full(complexity O(1))
 *
 */


/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t SPBPQueue;

typedef struct sp_bpq_element_t {
    int index;
    double value;
} BPQueueElement;

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
    SP_BPQUEUE_OUT_OF_MEMORY,
    SP_BPQUEUE_FULL,
    SP_BPQUEUE_EMPTY,
    SP_BPQUEUE_INVALID_ARGUMENT,
    SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 * Allocates a new queue in the memory.
 * Given the maximal number of elements that
 * the queue can contain.
 *
 * @param maxSize - an integer
 * @return
 * NULL in case allocation failure occurred
 * Otherwise, a new queue which can contain at most
 * maxSize of elements is returned
 */
SPBPQueue *spBPQueueCreate(int maxSize);

/**
 * Allocates a copy of the given queue.
 *
 * Given the queue source, the function returns a
 * new queue (q') such that:
 * the queue and q' have the same maxSize
 * the queue and q' have the same number of elements
 * the i'th element of q' and the i'th element of queue are the same
 *
 * @param source the source of the queue
 * @assert (source != NUlL)
 * @return
 * NULL in case memory allocation occurs
 * Others a copy of source is returned.
 */
SPBPQueue *spBPQueueCopy(SPBPQueue *source);

/**
 * Free all memory allocation associated with queue
 * in case that queue isn't a NULL pointer
 * @param source - the source of the queue
 */
void spBPQueueDestroy(SPBPQueue *source);

/**
 * Free all memory allocation associated with queue
 * in case that queue isn't a NULL pointer
 * @param source - the source of the queue
 */
void spBPQueueClear(SPBPQueue *source);

/**
 *  A getter of the number of elements in the queue.
 *
 * @param source - the source of the queue
 * @assert (source != NULL)
 * @return
 * The number of elements in the queue.
 */
int spBPQueueSize(SPBPQueue *source);

/**
 * A getter of the maximal number of elements which the queue can contains.
 *
 * @param source - the source of the queue
 * @assert (source != NULL)
 * @return
 * The maximal number of elements which the queue can contains.
 */
int spBPQueueGetMaxSize(SPBPQueue *source);

/**
 * if the queue is not a NULL pointer and the data is not a NULL pointer
 * Set new element in the queue for the following cases:
 * 1. the queue is not full
 * 2. the queue biggest value is bigger than "value"
 * 3. the queue biggest value equals "value"
 * and "index" is smaller than the (biggest value)'s index
 * @param source - the queue
 * @param index - the index of the element
 * @param value - the value of the element
 * @return
 * The status of the enqueue,
 * SUCCESS if element was inserted
 * INVALID if source is a NULL pointer or data is a NULL pointer
 * FULL if the element doesn't fit any of the above cases
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue *source, int index, double value);

/**
 * if the input_data is valid and the queue is not empty
 * (valid means that the queue is not NULL
 * and the array data is not NULL)
 * Pops the element with the largest value
 *
 * @param source - the queue
 * @return
 * The status of the dequeue,
 * SUCCESS if element was popped
 * INVALID if source is a NULL pointer or data is a NULL pointer
 * EMPTY if the queue is empty (size 0)
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue *source);

/**
 * Returns a copy of the element with the lowest value
 * @param source - the queue
 * @param res - the result (after the execution of the method,
 * res is a copy of the element with the lowest value)
 * @return
 * the status of the peek,
 * SUCCESS if the element was copied to res
 * INVALID if source is a NULL pointer or data is a NULL pointer
 * EMPTY if the queue is empty (size 0)
 */
SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue *source, BPQueueElement *res);

/**
 * Returns a copy of the element with the largest value
 * @param source - the queue
 * @param res - the result (after the execution of the method,
 * res is a copy of the element with the largest value)
 * @return
 * the status of the peek_last,
 * SUCCESS if the element was copied to res
 * INVALID if source is a NULL pointer or data is a NULL pointer
 * EMPTY if the queue is empty (size 0)
 */
SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue *source, BPQueueElement *res);

/**
 * Returns the minimum value in the queue
 * @param source - the queue
 * @assert (source != NULL)
 * @return
 * returns the lowest value of all the elements' values
 */
double spBPQueueMinValue(SPBPQueue *source);

/**
 * Returns the largest value in the queue
 * @param source - the queue
 * @assert (source != NULL)
 * @return
 * returns the largest value of all the elements' values
 */
double spBPQueueMaxValue(SPBPQueue *source);

/**
 * Returns True if the queue is empty, otherwise False.
 *
 * @param source - the queue
 * @assert (source != NULL)
 * @return
 * True - if the number of elements in the queue is zero.
 * Otherwise - False.
 */
bool spBPQueueIsEmpty(SPBPQueue *source);

/**
 * Returns True if the queue is full,otherwise False.
 *
 * @param source - the queue
 * @assert (source != NULL)
 * @return
 * True - if the number of elements in the queue is the
 * maximal number of elements which the queue can contain.
 * Otherwise - False.
 */
bool spBPQueueIsFull(SPBPQueue *source);

/**
 * Creates a new element
 * @param index - integer index
 * @param value - double value
 * @return
 * returns a new element
 */
BPQueueElement *createElement(int index, double value);

#endif
