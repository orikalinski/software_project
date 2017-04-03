//
// Created by ori on 31/12/16.
//

#include "../SPBPriorityQueue.h"
#include "unit_test_util.h"
#include <stdbool.h>
#include <stdlib.h>

// Checks create, copy, enqueue, size, min, max, destroy
bool queueBasicCopyTest() {
    SPBPQueue* q = spBPQueueCreate(5);
    spBPQueueEnqueue(q, 5, 1.0);
    spBPQueueEnqueue(q, 2, 5.0);
    spBPQueueEnqueue(q, 7, 17.0);
    spBPQueueEnqueue(q, 6, 5.0);
    SPBPQueue* q1 = spBPQueueCopy(q);
    ASSERT_TRUE(spBPQueueSize(q) == spBPQueueSize(q1));
    ASSERT_TRUE(spBPQueueMinValue(q) == spBPQueueMinValue(q1));
    ASSERT_TRUE(spBPQueueMaxValue(q) == spBPQueueMaxValue(q1));
    spBPQueueDestroy(q);
    spBPQueueDestroy(q1);
    return true;
}

// Checks create, dequeue, size,
// peek, peekLast, min, max, isFull, destroy
bool queueEnqueueTest() {
    SPBPQueue* q = spBPQueueCreate(5);
    spBPQueueEnqueue(q, 6, 1.0);
    spBPQueueEnqueue(q, 2, 5.0);
    spBPQueueEnqueue(q, 7, 17.0);
    spBPQueueEnqueue(q, 6, 5.0);
    spBPQueueEnqueue(q, 7, 16.0);
    spBPQueueEnqueue(q, 8, 17.0);
    spBPQueueEnqueue(q, 5, 17.0);
    BPQueueElement* element = (BPQueueElement*)malloc(sizeof(BPQueueElement));
    spBPQueuePeekLast(q, element);
    ASSERT_TRUE((*element).index == 5);
    spBPQueuePeek(q, element);
    ASSERT_TRUE((*element).index == 6);
    ASSERT_TRUE(spBPQueueSize(q) == spBPQueueGetMaxSize(q));
    ASSERT_TRUE(spBPQueueMaxValue(q) == 17.0);
    ASSERT_TRUE(spBPQueueMinValue(q) == 1.0);
    ASSERT_TRUE(spBPQueueIsFull(q));
    free(element);
    spBPQueueDestroy(q);
    return true;
}

// Checks create, enqueue, isEmpty, peekLast,
// size, isEmpty, destroy
bool queueDequeueTest() {
    SPBPQueue* q = spBPQueueCreate(5);
    spBPQueueEnqueue(q, 5, 1.0);
    spBPQueueEnqueue(q, 2, 5.0);
    spBPQueueEnqueue(q, 7, 17.0);
    spBPQueueEnqueue(q, 6, 5.0);
    ASSERT_TRUE(spBPQueueSize(q) == 4);
    ASSERT_TRUE(spBPQueueDequeue(q) == SP_BPQUEUE_SUCCESS);
    ASSERT_TRUE(spBPQueueDequeue(q) == SP_BPQUEUE_SUCCESS);
    ASSERT_TRUE(spBPQueueDequeue(q) == SP_BPQUEUE_SUCCESS);
    ASSERT_TRUE(spBPQueueDequeue(q) == SP_BPQUEUE_SUCCESS);
    ASSERT_TRUE(spBPQueueDequeue(q) == SP_BPQUEUE_EMPTY);
    ASSERT_TRUE(spBPQueueIsEmpty(q));
    BPQueueElement* element = (BPQueueElement*)malloc(sizeof(BPQueueElement));
    ASSERT_TRUE(spBPQueuePeekLast(q, element) == SP_BPQUEUE_EMPTY);
    free(element);
    spBPQueueDestroy(q);
    return true;
}

// Checks create, enqueue, dequeue, peekLast,
// size, isEmpty, destroy
bool queueEnqueueDequeueTest() {
    SPBPQueue* q = spBPQueueCreate(4);
    spBPQueueEnqueue(q, 5, 1.0);
    spBPQueueEnqueue(q, 2, 1.0);
    spBPQueueEnqueue(q, 7, 1.0);
    spBPQueueDequeue(q);
    spBPQueueEnqueue(q, 5, 2.0);
    spBPQueueEnqueue(q, 6, 5.0);
    spBPQueueEnqueue(q, 6, 5.0);
    spBPQueueEnqueue(q, 6, 17.0);
    BPQueueElement* element = (BPQueueElement*)malloc(sizeof(BPQueueElement));
    ASSERT_TRUE(spBPQueuePeekLast(q, element) == SP_BPQUEUE_SUCCESS);
    ASSERT_TRUE(element->value == 5);
    ASSERT_TRUE(spBPQueuePeek(q, element) == SP_BPQUEUE_SUCCESS);
    ASSERT_TRUE(element->value == 1.0 && element->index==5);
    spBPQueueDequeue(q);
    spBPQueueEnqueue(q, 6, 2.0);
    spBPQueueDequeue(q);
    spBPQueueDequeue(q);
    spBPQueueEnqueue(q, 6, 2.0);
    ASSERT_TRUE(spBPQueueSize(q) == 3);
    ASSERT_TRUE(spBPQueueDequeue(q) == SP_BPQUEUE_SUCCESS);
    ASSERT_TRUE(spBPQueueDequeue(q) == SP_BPQUEUE_SUCCESS);
    ASSERT_TRUE(spBPQueueDequeue(q) == SP_BPQUEUE_SUCCESS);
    ASSERT_TRUE(spBPQueueDequeue(q) == SP_BPQUEUE_EMPTY);
    ASSERT_TRUE(spBPQueueDequeue(q) == SP_BPQUEUE_EMPTY);
    ASSERT_TRUE(spBPQueuePeekLast(q, element) == SP_BPQUEUE_EMPTY);
    free(element);
    spBPQueueDestroy(q);
    return true;
}

// checks if clear works
bool queueClearTest() {
    SPBPQueue* q = spBPQueueCreate(5);
    spBPQueueEnqueue(q, 5, 1.0);
    spBPQueueEnqueue(q, 2, 5.0);
    spBPQueueEnqueue(q, 7, 17.0);
    spBPQueueEnqueue(q, 6, 5.0);
    spBPQueueClear(q);
    ASSERT_TRUE(spBPQueueSize(q) == 0);
    spBPQueueDestroy(q);
    return true;
}

int main() {
    RUN_TEST(queueBasicCopyTest);
    RUN_TEST(queueEnqueueTest);
    RUN_TEST(queueDequeueTest);
    RUN_TEST(queueEnqueueDequeueTest);
    RUN_TEST(queueClearTest);
    return 0;
}