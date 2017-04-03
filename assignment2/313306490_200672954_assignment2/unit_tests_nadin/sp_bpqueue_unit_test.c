//
// Created by ori on 31/12/16.
//

#include "../SPBPriorityQueue.h"
#include "unit_test_util.h"
#include <stdbool.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <stdbool.h>

BPQueueElement elArr [7]={
			 {1,4.0},
			 {3,2.0},
			 {7,10.0},
			 {6,5.0},
			 {8,2.0},
			 {1,33.0},
			 {2,2.0}
			 };

BPQueueElement dQElArr [5]={
			   {3,2.0},  
			   {8,2.0},
			   {1,4.0},
			   {6,5.0},
			   {7,10.0}
			   };



int getUsage(){
    struct rusage* memory = malloc(sizeof(struct rusage));
    getrusage(RUSAGE_SELF, memory);
    int usage = (*memory).ru_maxrss;
    free(memory);
    return usage;
}



// Checks create, copy, enqueue, size, min, max, destroy
bool queueBasicCopyTest() {
    SPBPQueue* queue = spBPQueueCreate(4);
    for(int i=0;i<4;i++)
      spBPQueueEnqueue(queue, elArr [i].index, elArr [i].value);
    
    SPBPQueue* queueCopy = spBPQueueCopy(queue);
    ASSERT_TRUE(spBPQueueSize(queue) == spBPQueueSize(queueCopy));
    ASSERT_TRUE(spBPQueueMinValue(queue) == spBPQueueMinValue(queueCopy));
    ASSERT_TRUE(spBPQueueMaxValue(queue) == spBPQueueMaxValue(queueCopy));
    spBPQueueDestroy(queue);
    spBPQueueDestroy(queueCopy);
    return true;
}

// Checks create, dequeue, createElement, size,
// peek, peekLast, min, max, isFull, destroy
bool queueEnqueueDropLastTest() {
    SPBPQueue* queue = spBPQueueCreate(2);
    BPQueueElement* element = (BPQueueElement*)malloc(sizeof(BPQueueElement));
    int i=0;
    while(spBPQueueEnqueue(queue,elArr [i].index, elArr [i].value)!=SP_BPQUEUE_FULL){
      ASSERT_TRUE(spBPQueueSize(queue) == (i+1));
      i++;
    }
    ASSERT_TRUE(i==2);
    ASSERT_TRUE(spBPQueuePeekLast(queue,element)== SP_BPQUEUE_SUCCESS);
    printf("index %d \n", element->index);
    printf("value %lf \n", element->value);

    ASSERT_TRUE(element->index == 1);
    ASSERT_TRUE(element->value == 4.0);
    ASSERT_TRUE(spBPQueuePeek(queue,element)==SP_BPQUEUE_SUCCESS);
    printf("index %d \n", element->index);
    printf("value %lf \n", element->value);

    ASSERT_TRUE(element->index == 3);
    ASSERT_TRUE(element->value == 2.0);
    free(element);
    spBPQueueDestroy(queue);
    return true;
}

// Checks create, enqueue, isEmpty, peekLast,
// size, createElement, isEmpty, destroy
bool queueDequeueTest() {
    SPBPQueue* queue = spBPQueueCreate(5);
    BPQueueElement* element = (BPQueueElement*)malloc(sizeof(BPQueueElement));
    int i=0;
    while(spBPQueueEnqueue(queue,elArr [i].index, elArr [i].value)!=SP_BPQUEUE_FULL){
      ASSERT_TRUE(spBPQueueSize(queue) == (i+1));
      i++;
    }
    ASSERT_TRUE(i==5);
    do{
      // spBPQueuePeek(queue, element);
      
      ASSERT_TRUE(spBPQueuePeek(queue, element)==SP_BPQUEUE_SUCCESS);
      printf("index %d \n", element->index);
      printf("value %lf \n", element->value);
      ASSERT_TRUE(element->index == dQElArr [5-i].index);
      ASSERT_TRUE(element->value == dQElArr [5-i].value);
      i--;
    }while(spBPQueueDequeue(queue)!= SP_BPQUEUE_EMPTY && spBPQueueSize(queue)>0);
    ASSERT_TRUE(i==0);
    
    free(element);
    spBPQueueDestroy(queue);
    return true;
}

// checks if clear works
bool queueClearTest() {
    SPBPQueue* queue = spBPQueueCreate(5);
    
    int i=0;
    while(spBPQueueEnqueue(queue,elArr [i].index, elArr [i].value)!=SP_BPQUEUE_FULL){
      ASSERT_TRUE(spBPQueueSize(queue) == (i+1));
      i++;
    }
    spBPQueueClear(queue);
    ASSERT_TRUE(spBPQueueSize(queue) == 0);
    spBPQueueDestroy(queue);
    return true;
}

int main() {

  RUN_TEST(queueBasicCopyTest);
  RUN_TEST(queueEnqueueDropLastTest);
  RUN_TEST(queueDequeueTest);
  RUN_TEST(queueClearTest);
  return 0;
}
