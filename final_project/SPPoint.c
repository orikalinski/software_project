//
// Created by ori on 24/12/16.
//

#include <stdio.h>
#include <stdlib.h>
#include "SPPoint.h"
#include <assert.h>
#include <string.h>
#include <math.h>


struct sp_point_t {
    double *data;
    int index;
    int dim;
};

SPPoint *spPointCreate(double *data, int dim, int index) {
    if (data == NULL)
        return NULL;
    SPPoint *point;
    point = (SPPoint *) malloc(sizeof(SPPoint));
    double *point_data = (double *)malloc(dim * sizeof(double));
    if (point == 0 || point_data == 0)
        return NULL;
    memcpy(point_data, data, dim * sizeof(double));
    point->index = index;
    point->dim = dim;
    point->data = point_data;
    return point;
}

SPPoint *spPointCopy(SPPoint *source) {
    assert(source != NULL);
    return spPointCreate(source->data,
                         source->dim,
                         source->index);
}

void spPointDestroy(SPPoint *point) {
    free(point->data);
    free(point);
}

int spPointGetDimension(SPPoint *point) {
    assert(point != NULL);
    return point->dim;
}

int spPointGetIndex(SPPoint *point) {
    assert(point != NULL);
    return point->index;
}

double spPointGetData(SPPoint* point, int i){
    assert(point != NULL);
    return point->data[i];
}

double spPointGetAxisCoor(SPPoint *point, int axis) {
    assert(point != NULL && axis < point->dim);
    return point->data[axis];
}

double spPointL2SquaredDistance(SPPoint *p, SPPoint *q) {
    assert(p != NULL && q != NULL && p->dim == q->dim);
    double result = 0;
    for (int i = 0; i < q->dim; i++) {
        result += pow(q->data[i] - p->data[i], 2);
    }
    return result;
}

size_t spPointSizeOf(){
    return sizeof(SPPoint);
}

void spPointSetIndex(SPPoint *point, int index) {
    point->index = index;
}

void spPointSetData(SPPoint *point, int i, double value) {
    point->data[i] = value;
}