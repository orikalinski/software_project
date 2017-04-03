#include "SPBPriorityQueue.h"
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>


typedef struct sp_bp_queue_t {
    int maxSize;
    int currentSize;
    BPQueueElement *data;
} SPBPQueue;

SPBPQueue *spBPQueueCreate(int maxSize) {
    SPBPQueue *queue = (SPBPQueue *) malloc(sizeof(SPBPQueue));
    if (queue == 0)
        return NULL;
    (*queue).data = calloc(maxSize, sizeof(BPQueueElement));
    if ((*queue).data == 0) {
        free(queue);
        return NULL;
    }
    (*queue).maxSize = maxSize;
    (*queue).currentSize = 0;
    return queue;
}

SPBPQueue *spBPQueueCopy(SPBPQueue *source) {
    assert(source != NULL);
    SPBPQueue *queue = spBPQueueCreate((*source).maxSize);
    (*queue).currentSize = (*source).currentSize;
    memcpy((*queue).data, (*source).data, (*queue).maxSize *
                                       sizeof(BPQueueElement));
    return queue;
}

void spBPQueueDestroy(SPBPQueue *source) {
    if (source == NULL)
        return;
    free((*source).data);
    free(source);
}

void spBPQueueClear(SPBPQueue *source) {
    if (source == NULL)
        return;
    free((*source).data);
    (*source).data  = calloc((*source).maxSize,
                                        sizeof(BPQueueElement));
    (*source).currentSize = 0;
}

int spBPQueueSize(SPBPQueue *source) {
    assert(source != NULL);
    return (*source).currentSize;
}

int spBPQueueGetMaxSize(SPBPQueue *source) {
    assert(source != NULL);
    return (*source).maxSize;
}

int findIndexToInsert(SPBPQueue *source, int index, double value) {
    int i;
    for (i = 0; i < spBPQueueSize(source); i++) {
        if ((*((*source).data + i)).value > value)
            return i;
        else if ((*((*source).data + i)).value == value)
            if ((*((*source).data + i)).index > index)
                return i;
    }
    return i;
}

BPQueueElement *createElement(int index, double value) {
    BPQueueElement *element = (BPQueueElement *) malloc(sizeof(BPQueueElement));
    (*element).index = index;
    (*element).value = value;
    return element;
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue *source, int index, double value) {
    if (source == NULL || (*source).data == NULL)
        return SP_BPQUEUE_INVALID_ARGUMENT;
    int indexToInsert = findIndexToInsert(source, index, value);
    int i;
    for (i = spBPQueueSize(source); i > indexToInsert; i--) {
        if (i == spBPQueueGetMaxSize(source)) {
            continue;
        }
        (*((*source).data + i)) = (*((*source).data + i - 1));
    }
    if (i == spBPQueueGetMaxSize(source))
        return SP_BPQUEUE_FULL;
    BPQueueElement *elementToInsert = createElement(index, value);
    *((*source).data + indexToInsert) = *elementToInsert;
    free(elementToInsert);
    (*source).currentSize = spBPQueueSize(source) + 1 < spBPQueueGetMaxSize(source) ?
                            spBPQueueSize(source) + 1 :
                            spBPQueueGetMaxSize(source);
    return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue *source) {
    if (source == NULL || (*source).data == NULL)
        return SP_BPQUEUE_INVALID_ARGUMENT;
    if (spBPQueueSize(source) == 0)
        return SP_BPQUEUE_EMPTY;
    (*source).currentSize -= 1;
    return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue *source, BPQueueElement *res) {
    if (source == NULL || (*source).data == NULL)
        return SP_BPQUEUE_INVALID_ARGUMENT;
    if (spBPQueueSize(source) == 0)
        return SP_BPQUEUE_EMPTY;
    memcpy(res, (*source).data, sizeof(BPQueueElement));
    return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue *source, BPQueueElement *res) {
    if (source == NULL || (*source).data == NULL)
        return SP_BPQUEUE_INVALID_ARGUMENT;
    if (spBPQueueSize(source) == 0)
        return SP_BPQUEUE_EMPTY;
    memcpy(res, (*source).data + spBPQueueSize(source) - 1, sizeof(BPQueueElement));
    return SP_BPQUEUE_SUCCESS;
}

double spBPQueueMinValue(SPBPQueue *source) {
    assert(source != NULL);
    BPQueueElement *element = createElement(0, -1.0);
    spBPQueuePeek(source, element);
    double value = (*element).value;
    free(element);
    return value;
}

double spBPQueueMaxValue(SPBPQueue *source) {
    assert(source != NULL);
    BPQueueElement *element = createElement(0, -1.0);
    spBPQueuePeekLast(source, element);
    double value = (*element).value;
    free(element);
    return value;
}

bool spBPQueueIsEmpty(SPBPQueue *source) {
    assert(source != NULL);
    return spBPQueueSize(source) == 0;
}

bool spBPQueueIsFull(SPBPQueue *source) {
    assert(source != NULL);
    return spBPQueueSize(source) == spBPQueueGetMaxSize(source);
}