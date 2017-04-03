#include "SPBPriorityQueue.h"
#include <stdbool.h>
#include "SPBPriorityQueue.h"
//#inclode <SPBPriorityQueue.h>
#include <stdlib.h>

#include <assert.h>


struct sp_bp_queue_t {
  int Maxsize;               // the max size of the queue
  BPQueueElement* elements;  // the elements of the queue
  int size;                  // how many items are in the queue
};

void swapElements(BPQueueElement* arr, int index){
  BPQueueElement tmp = *(arr+index);
  *(arr+index)=*(arr+index+1);
  *(arr+index+1) =tmp; 
}


SPBPQueue* spBPQueueCreate(int maxsize) {
  SPBPQueue* queue = (SPBPQueue*)malloc(sizeof(SPBPQueue));  // pointer to new queue
  if (queue == NULL) return NULL;
  queue->Maxsize = maxsize;
  BPQueueElement* elem = (BPQueueElement*)malloc(sizeof(BPQueueElement)*maxsize);
  queue->elements = elem;
  if (queue->elements == NULL)  // Allocation failure - need to free queue
  {
    free(queue);
    return NULL;
  }
  queue->size = 0;  // empty queue
  return (queue);
}

BPQueueElement* ElementsCopy(
    BPQueueElement* elements,
    int size)  // copy the elements and return pointer to the new elements
{
  if (elements == NULL) return NULL;
  BPQueueElement* new_elements = (BPQueueElement*)malloc(sizeof(BPQueueElement) * size);  // the number of the elements is the index of the queue
  for (int i = 0; i < size; i++)  // for each element update the index and the
                                  // value to be the same as elements
  {
    new_elements[i].index = elements[i].index;
    new_elements[i].value = elements[i].value;
  }
  return new_elements;
}



SPBPQueue* spBPQueueCopy(SPBPQueue* source) {
  assert (source != NULL);
  int maxsize = source->Maxsize;
  int indx = source->size;
  SPBPQueue* res =
      (SPBPQueue*)malloc(maxsize * sizeof(BPQueueElement));  // new queue
  BPQueueElement* elem = source->elements;  // we copy pointer by value, but
                                            // both are pointers to the same
                                            // struct
  BPQueueElement* new_elements = ElementsCopy(elem, indx);
  res->Maxsize = maxsize;  // the same maxsize, couse we copy int by value
  res->size = indx;        // the same index
  res->elements = new_elements;

  return res;
}

void spBPQueueDestroy(SPBPQueue* source) {
  if (source == NULL) return;
  if(source->elements == NULL)
    {
      free(source);
      return;
    }
  free(source->elements);
  free(source);
  return;
}

void spBPQueueClear(SPBPQueue* source) {
  if (source == NULL) return;
  source->size = 0;
  
  return;
}

int spBPQueueSize(SPBPQueue* source) {
  if (source == NULL) return 0;
  return source->size;
}

int spBPQueueGetMaxSize(SPBPQueue* source) {
  if (source == NULL) return 0;
  return source->Maxsize;
}

int retrieveIndex(SPBPQueue *source, int index, double value) {
  int i;
  for (i = 0; i < spBPQueueSize(source); i++) {
    if (source->elements[i].value < value)
      return i;
    else if (source->elements[i].value == value)
      if (source->elements[i].index < index)
        return i;
  }
  return i;
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue *source, int index, double value) {
  if (source == NULL || source->elements == NULL)
    return SP_BPQUEUE_INVALID_ARGUMENT;
  int indexToInsert = retrieveIndex(source, index, value);
  if (spBPQueueIsFull(source)){
    indexToInsert--;
    if (indexToInsert == -1)
      return SP_BPQUEUE_FULL;
    for (int i = 0; i < indexToInsert; i++)
      source->elements[i] = source->elements[i + 1];
  }
  else
    for (int i = spBPQueueSize(source); i > indexToInsert; i--)
      source->elements[i] = source->elements[i - 1];

  source->elements[indexToInsert].value = value;
  source->elements[indexToInsert].index = index;
  source->size = spBPQueueSize(source) + 1 < spBPQueueGetMaxSize(source) ?
                        spBPQueueSize(source) + 1 :
                        spBPQueueGetMaxSize(source);
  return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue *source) {
  if (source == NULL || source->elements == NULL)
    return SP_BPQUEUE_INVALID_ARGUMENT;
  if (spBPQueueSize(source) == 0)
    return SP_BPQUEUE_EMPTY;
  source->size -= 1;
  return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueuePeek(SPBPQueue* source, BPQueueElement* res) {
  if (source == NULL) return SP_BPQUEUE_INVALID_ARGUMENT;

  if (source->size == 0) return SP_BPQUEUE_EMPTY;

  //  int max_size = source->Maxsize;
  int source_size = source->size;
  BPQueueElement* elements = source->elements;

  // res = (BPQueueElement*)malloc(sizeof(BPQueueElement));

  res->index = elements[source_size-1].index;
  res->value = elements[ source_size -1].value;

  return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueuePeekLast(SPBPQueue* source, BPQueueElement* res) {
  if (source == NULL) return SP_BPQUEUE_INVALID_ARGUMENT;

  if (source->size == 0) return SP_BPQUEUE_EMPTY;

  BPQueueElement* elements = source->elements;

  res->index = elements[0].index;
  res->value = elements[0].value;

  return SP_BPQUEUE_SUCCESS;
}

double spBPQueueMinValue(SPBPQueue* source) {
  if (source == NULL) return -1;  // TOASK!!!!

  if (source->size == 0) return -1;

  return (source->elements)[source->size - 1].value;
}

double spBPQueueMaxValue(SPBPQueue* source) {
  if (source == NULL) return -1;  // TOASK!!!!

  if (source->size == 0) return -1;

  return (source->elements)[0].value;
}

bool spBPQueueIsEmpty(SPBPQueue* source) {
  assert(source != NULL);
  return (source->size == 0);
}

bool spBPQueueIsFull(SPBPQueue* source) {
  assert(source != NULL);
  return (source->size == source->Maxsize);
}
