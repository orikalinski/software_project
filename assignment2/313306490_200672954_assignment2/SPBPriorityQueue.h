#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include <stdbool.h>



 /* A bounded priority queue (BPQ) is similar to a regular minimum priority queue, except that there is a fixed */
 /*      upper bound on the number of elements that can be stored in the BPQ. Whenever a new element is */
 /*      added to the queue, if the queue is at full capacity, the element with the highest priority value is ejected */
 /*   from the queue. */


/* bounded queue contains: */
/* array of elements, max size(Max_size) of the queue and the current size(size) */

/* The following functions are supported: */
/* 1- spBPQueueCreate- creates an empty queue with a given maximum capacity (complexity O(n)) */
/* 2- spBPQueueCopy- creates a copy of a given queue (complexity O(n)) */
/* 3- spBPQueueDestroy- frees all memory allocation associated with the queue (complexity O(n)) */
/* 4- spBPQueueClear- removes all the elements in the queue (complexity O(n) / Could be in O(1)) */
/* 5- spBPQueueSize- returns the number of elements in the queue (complexity O(1)) */
/* 6- spBPQueueGetMaxSize- returns the maximum capacity of the queue (complexity O(1)) */
/* 7- spBPQueueEnqueue- Inserts an element to the queue (complexity O(n)) */
/* 8- spBPQueueDequeue- removes the element with the lowest value (complexity O(1) */
/* 9- spBPQueuePeek- Returns a copy of the element with the lowest value (Complexity O(1)) */
/* 10- spBPQueuePeekLast - Returns a copy of the element with the highest value (Complexity O(1)) */
/* 11- spBPQueueMinValue- returns the minimum value in the queue (complexity O(1)) */
/* 12- spBPQueueMaxValue- returns the maximum value in the queue(complexity O(1)) */
/* 13- spBPQueueIsEmpty – returns true if the queue is empty(complexity O(1)) */
/* 14- spBPQueueIsFull- returns true if the queue is full(complexity O(1)) */


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
 * given the max capacity, allocate new queue in the memory
  @pram maxSize - a positive integer
  @return
  in case of allocation failure NULL pointer is returned,
  otherwise, pointer to new queue which can contain maxSize elements is returned
**/
SPBPQueue* spBPQueueCreate(int maxSize);

/**
 * given a pointer to queue (source),
 create a new queue (res),
 copy the source queue and return a pointer to new queue(res)
 @pram source queue
 @assert source != NULL
 @return
 in case of allocation failure NULL pointer is returned,
 otherwise, a copy of the source queue is returned
 */
SPBPQueue* spBPQueueCopy(SPBPQueue* source);

/**
 *Free all memory allocation
 @pram source queue

 */
void spBPQueueDestroy(SPBPQueue* source);

/**
 *Free all memory allocation associated with queue
  @param the source of the queue
 */
void spBPQueueClear(SPBPQueue* source);

/**
 * get the number of the elements
  @param the source of the queue
 * @assert (source != NULL)
 * @return
 */
int spBPQueueSize(SPBPQueue* source);

/**
 * get the maximal number of the elements that the queue can contain
 @param source - the source of the queue
 @return
 */
int spBPQueueGetMaxSize(SPBPQueue* source);

/**
 *add an element to the queue
 @param source - the queue
 @param index - the index of the element
 @param value - the value of the element
 @return
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue* source, int index, double value);

/**
 * removes the element with the lowest value
  @param source - the queue
 * @return
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue* source);

/**
 * Returns a copy of the element with the lowest value
 * @param the queue
 * @param the result (after the execution of the method,
 * res is a copy of the element with the lowest value)
 * @return
 */
SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue* source, BPQueueElement* res);

/**
 * Returns a copy of the element with the largest value
 * @param the queue
 * @param the result (after the execution of the method,
 * res is a copy of the element with the largest value)
 * @return
 */
SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue* source, BPQueueElement* res);

/**
 * Returns the minimum value in the queue
 * @param source - the queue
 * @assert (source != NULL)
 * @return
 */
double spBPQueueMinValue(SPBPQueue* source);

/**
 * Returns the largest value in the queue
 * @param source - the queue
 * @assert (source != NULL)
 * @return
 */
double spBPQueueMaxValue(SPBPQueue* source);

/**
 * Returns True if the queue is empty, otherwise False.
 *
 * @param source - the queue
 * @assert (source != NULL)
 * @return
 */
bool spBPQueueIsEmpty(SPBPQueue* source);

/**
 * Returns True if the queue is full,otherwise False.
 *
 * @param source - the queue
 * @assert (source != NULL)
 * @return
 */
bool spBPQueueIsFull(SPBPQueue* source);

#endif
