#include <stdio.h>
#include "SPPoint.h"
#include "SPBPriorityQueue.h"
#include "main_aux.h"
#include<stdlib.h>

int main() {
    int n, d, k;
    BPQueueElement *tmpRes = (BPQueueElement *) malloc(sizeof(BPQueueElement));
    //SPPoint* tmp = (SPPoint*)malloc(sizeof(SPPoint*));
    scanf("%d %d %d", &n, &d, &k);
    SPPoint **pointToPointList = (SPPoint **) malloc(sizeof(SPPoint *) * n);
    int *kClosePoints = (int *) malloc(sizeof(int) * k);
    double* qCoordinates =  (double *)malloc(sizeof(double)*d);
    SPBPQueue *queue = spBPQueueCreate(k);
    
    for (int i = 0; i < n; i++) {
      double* coordinates = (double *)malloc(sizeof(double)*d);
      readCoordinates(d, coordinates);
      pointToPointList[i] = spPointCreate(coordinates, d, i+1);
      free(coordinates);
    }
    
    readCoordinates(d, qCoordinates);
    SPPoint *qPoint = spPointCreate(qCoordinates, d, 0);
    
    for (int i = 0; i < n; i++) {
        
        int index = spPointGetIndex(pointToPointList[i]);
        double dist = spPointL2SquaredDistance(pointToPointList[i], qPoint);

        spBPQueueEnqueue(queue, index, dist);
    }
        
    for (int i = 0; i < k; i++) {

        spBPQueuePeek(queue, tmpRes);
        kClosePoints[i] = tmpRes->index;

        spBPQueueDequeue(queue);
    }

    int i = 0;
    while (i < k) {
        if (i != k - 1)
            printf("%d, ", kClosePoints[i]);
        else
            printf("%d ", kClosePoints[i]);
        ++i;
    }

    for(int i=0; i<n; i++){
      spPointDestroy(*(pointToPointList +i));
    }

    free(pointToPointList);
    free(qCoordinates);
    spBPQueueDestroy(queue);
    free(tmpRes);
    free(kClosePoints);
    spPointDestroy(qPoint);
    return 0;
}
