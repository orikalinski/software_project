//
// Created by ori on 31/12/16.
//
#ifndef SP2_MAIN_AUX_H
#define SP2_MAIN_AUX_H

#include "SPPoint.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef struct result {
    double distance;
    int arrayIndex;
} Result;

Result *getSortedResults(Result *results, int n);

SPPoint *getPointByUser(int d);

SPPoint **getPointsByUser(int n, int d);

Result *calcDistances(int n, SPPoint *q, SPPoint **points);

void printKResults(Result *results, int k);

void freeObjects(Result *results, SPPoint *q,
                 SPPoint **points, int n);

#endif //SP2_MAIN_AUX_H
