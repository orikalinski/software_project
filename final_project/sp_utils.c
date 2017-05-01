//
// Created by ori on 22/04/17.
//

#include <assert.h>
#include <stdlib.h>
#include "sp_utils.h"

SortElement elementCreate(int index, double value) {
    SortElement element;
    element.index = index;
    element.value = value;
    return element;
}

size_t elementGetSize() {
    return sizeof(SortElement);
}

int elementGetIndex(SortElement element) {
    return element.index;
}

void quicksort(SortElement *valuesArray, int low, int high) {
    int pivot, i, j;
    SortElement temp;
    if (low < high) {
        pivot = low;
        i = low;
        j = high;
        while (i < j) {
            while (i <= high && (valuesArray[i].value < valuesArray[pivot].value ||
                                 (valuesArray[i].value == valuesArray[pivot].value &&
                                  valuesArray[i].index <= valuesArray[pivot].index))) {
                i++;
            }
            while (j >= low && (valuesArray[j].value > valuesArray[pivot].value ||
                                (valuesArray[j].value == valuesArray[pivot].value &&
                                 valuesArray[j].index > valuesArray[pivot].index))) {
                j--;
            }
            if (i < j) {
                temp = valuesArray[i];
                valuesArray[i] = valuesArray[j];
                valuesArray[j] = temp;
            }
        }
        temp = valuesArray[j];
        valuesArray[j] = valuesArray[pivot];
        valuesArray[pivot] = temp;
        quicksort(valuesArray, low, j - 1);
        quicksort(valuesArray, j + 1, high);
    }
}

double findMedian(SortElement *valuesArray, int size) {
    quicksort(valuesArray, 0, size - 1);
    if (size % 2 == 0)
        return ((valuesArray[size / 2].value +
                 valuesArray[size / 2 - 1].value) / 2.0);
    else
        return valuesArray[size / 2].value;
}