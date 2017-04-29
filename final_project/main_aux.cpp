//
// Created by ori on 29/04/17.
//

#include "main_aux.h"

int getBestK(int *input, int n, int k, int *bestK){
    BPQueueElement *tmp;
    if ((tmp = (BPQueueElement *) malloc(sizeof(BPQueueElement))) == NULL){
        return 1;
    }
    SPBPQueue *queue;
    if ((queue = spBPQueueCreate(n)) == NULL){
        free(tmp);
        return 1;
    }
    for (int i = 0; i < n; i++)
        spBPQueueEnqueue(queue, i, -(double)input[i]);

    for (int i = 0; i < k; i++) {
        spBPQueuePeek(queue, tmp);
        bestK[i] = tmp->index;
        spBPQueueDequeue(queue);
    }
    free(tmp);
    spBPQueueDestroy(queue);
    return 0;
}