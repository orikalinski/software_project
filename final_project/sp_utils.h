//
// Created by ori on 22/04/17.
//

#ifndef FINAL_PROJECT_SP_UTILS_H
#define FINAL_PROJECT_SP_UTILS_H

#endif //FINAL_PROJECT_SP_UTILS_H

#include <malloc.h>
#include "SPPoint.h"
#include "SPBPriorityQueue.h"

typedef struct sort_element SortElement;

struct sort_element {
    int index;
    double value;
};

SortElement elementCreate(int index, double value);

int elementGetIndex(SortElement element);

size_t elementGetSize();

void quicksort(SortElement *valuesArray, int low, int high);

double findMedian(SortElement *valuesArray, int size);