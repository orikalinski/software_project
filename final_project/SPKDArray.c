//
// Created by ori on 22/04/17.
//

#include <math.h>
#include "SPKDArray.h"

#define CEIL_DEVISION(x, y, res) res = (((float)(x) - 1) / (y)) + 1;

int mallocAndPopulateKDArray(SPKDArray *kdArr, int numberOfPoints,
                              int dim, SPPoint **points) {
    kdArr->numOfPoints = numberOfPoints;
    kdArr->dim = dim;
    if ((kdArr->indexesMatrix = (int **) malloc(dim * sizeof(int *))) == NULL) return 1;
    kdArr->points = points;
    return 0;
}

int initKDArray(SPPoint **arr, int size, SPKDArray *kdArr) {
    int i = 0, j;
    int dim = spPointGetDimension(arr[0]);
    if (mallocAndPopulateKDArray(kdArr, size, dim, arr) != 0) return 1;
    SortElement *elements;
    if ((elements = (SortElement *) malloc(size * elementGetSize())) == NULL) return 1;
    SortElement element;
    for (; i < dim; i++) {
        for (j = 0; j < size; j++) {
            element = elementCreate(j, spPointGetData(arr[j], i));
            elements[j] = element;
        }
        quicksort(elements, 0, size - 1);
        if ((kdArr->indexesMatrix[i] = (int *) malloc(sizeof(int) * size)) == NULL) return 1;
        for (j = 0; j < size; j++) {
            kdArr->indexesMatrix[i][j] = elementGetIndex(elements[j]);
        }
    }
    free(elements);
    return 0;
}

void destroyKDArray(SPKDArray kdArr, bool isRoot) {
    int i = 0;
    for (; i < kdArr.dim; i++) {
        free(kdArr.indexesMatrix[i]);
    }
    free(kdArr.indexesMatrix);
    if (!isRoot)
        free(kdArr.points);
}

int getMedianFromKDArray(SPKDArray kdArr, int coor, double *median) {
    int j = 0;
    int size = kdArr.numOfPoints;
    SortElement *elements;
    if ((elements = (SortElement *) malloc(size * sizeof(SortElement))) == NULL) return 1;
    SortElement element;
    for (; j < size; j++) {
        element = elementCreate(j, spPointGetData(kdArr.points[j], coor));
        elements[j] = element;
    }
    *median = findMedian(elements, size);
    free(elements);
    return 0;
}

int getMaxSpreadDim(SPKDArray kdArr) {
    int i = 0, j, maxSpreadDim = -1;
    double value, maximumSpread = -1, maximum = -1, minimum = INFINITY;
    for (; i < kdArr.dim; i++) {
        for (j = 0; j < kdArr.numOfPoints; j++) {
            value = spPointGetData(kdArr.points[j], i);
            if (value > maximum)
                maximum = value;
            if (value < minimum)
                minimum = value;
        }
        if ((maximum - minimum) > maximumSpread) {
            maximumSpread = maximum - minimum;
            maxSpreadDim = i;
        }
    }
    return maxSpreadDim;
}

void getX(SPKDArray kdArr, int coor, int *X) {
    int j = 0;
    int half;
    CEIL_DEVISION(kdArr.numOfPoints, 2, half)
    for (; j < half; j++)
        X[kdArr.indexesMatrix[coor][j]] = 0;
    for (; j < kdArr.numOfPoints; j++)
        X[kdArr.indexesMatrix[coor][j]] = 1;
}

void getPointsAndMaps(SPKDArray kdArr, int *X,
                      SPPoint **leftPoints, SPPoint **rightPoints,
                      int *map1, int *map2) {
    int j = 0, k1 = 0, k2 = 0;
    for (; j < kdArr.numOfPoints; j++) {
        if (X[j] == 0) {
            leftPoints[k1] = kdArr.points[j];
            map1[j] = k1;
            map2[j] = -1;
            k1++;
        } else {
            rightPoints[k2] = kdArr.points[j];
            map2[j] = k2;
            map1[j] = -1;
            k2++;
        }
    }
}

int buildKdArrFromXAndMap(SPKDArray originKDArr, SPKDArray *kdArr, int *X,
                           SPPoint **points, int *map, int numberOfPoints,
                           int xValue) {
    if (mallocAndPopulateKDArray(kdArr, numberOfPoints, originKDArr.dim, points) != 0) return 1;
    int i = 0, j, k, currentIndex;
    for (; i < originKDArr.dim; i++) {
        k = 0;
        if ((kdArr->indexesMatrix[i] = (int *) malloc(sizeof(int) * numberOfPoints)) == NULL) return 1;
        for (j = 0; j < originKDArr.numOfPoints; j++) {
            currentIndex = originKDArr.indexesMatrix[i][j];
            if (X[currentIndex] == xValue) {
                kdArr->indexesMatrix[i][k] = map[currentIndex];
                k++;
            }
        }
    }
    return 0;
}

int split(SPKDArray kdArr, int coor, SPKDArray *leftKDArr, SPKDArray *rightKDArr) {
    int half;
    CEIL_DEVISION(kdArr.numOfPoints, 2, half)
    int leftNumberOfPoints = half;
    int rightNumberOfPoints = kdArr.numOfPoints - half;
    SPPoint **leftPoints, **rightPoints;
    int *X, *map1, *map2;
    if ((leftPoints = (SPPoint **) malloc(sizeof(SPPoint *) * leftNumberOfPoints)) == NULL) return 1;
    if ((rightPoints = (SPPoint **) malloc(sizeof(SPPoint *) * rightNumberOfPoints)) == NULL) return 1;
    if ((X = (int *) malloc(sizeof(int) * kdArr.numOfPoints)) == NULL) return 1;
    if ((map1 = (int *) malloc(sizeof(int) * kdArr.numOfPoints)) == NULL) return 1;
    if ((map2 = (int *) malloc(sizeof(int) * kdArr.numOfPoints)) == NULL) return 1;
    getX(kdArr, coor, X);
    getPointsAndMaps(kdArr, X, leftPoints, rightPoints, map1, map2);
    buildKdArrFromXAndMap(kdArr, leftKDArr, X, leftPoints, map1, leftNumberOfPoints, 0);
    buildKdArrFromXAndMap(kdArr, rightKDArr, X, rightPoints, map2, rightNumberOfPoints, 1);
    free(X);
    free(map1);
    free(map2);
    return 0;
}