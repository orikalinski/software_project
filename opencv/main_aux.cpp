//
// Created by ori on 21/01/17.
//


#include "main_aux.h"

extern "C"{
#include "SPBPriorityQueue.h"
}

void printBestFive(double *input, int n, int sign){
    BPQueueElement *tmp = (BPQueueElement *) malloc(sizeof(BPQueueElement));
    int numberOfBests = n < 5 ? n : 5;
    SPBPQueue *queue = spBPQueueCreate(n);
    for (int i = 0; i < n; i++)
        spBPQueueEnqueue(queue, i, sign * input[i]);

    std::string pattern;
    for (int i = 0; i < numberOfBests; i++) {
        pattern = i < numberOfBests - 1 ? "%d, " : "%d\n";
        spBPQueuePeek(queue, tmp);
        printf(pattern.c_str(), tmp->index);
        spBPQueueDequeue(queue);
    }
    free(tmp);
    spBPQueueDestroy(queue);
}