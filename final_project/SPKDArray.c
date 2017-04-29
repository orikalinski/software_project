//
// Created by ori on 22/04/17.
//

#include <math.h>
#include "SPKDArray.h"

#define CEIL_DEVISION(x, y, res) res = (((float)(x) - 1) / (y)) + 1;

void mallocAndPopulateKDArray(SPKDArray *kdArr, int numberOfPoints,
                              int dim, SPPoint **points) {
    kdArr->numOfPoints = numberOfPoints;
    kdArr->dim = dim;
    kdArr->indexesMatrix = (int **) malloc(dim * sizeof(int *));
    kdArr->points = points;
}

void initKDArray(SPPoint **arr, int size, SPKDArray *kdArr) {
    int i = 0, j;
    int dim = spPointGetDimension(arr[0]);
    mallocAndPopulateKDArray(kdArr, size, dim, arr);
    SortElement **elements = (SortElement **) malloc(size * sizeof(SortElement *));
    SortElement *element;
    for (; i < dim; i++) {
        for (j = 0; j < size; j++) {
            element = elementCreate(j, spPointGetData(arr[j], i));
            elements[j] = element;
        }
        quicksort(elements, 0, size - 1);
        kdArr->indexesMatrix[i] = (int *) malloc(sizeof(int) * size);
        for (j = 0; j < size; j++) {
            kdArr->indexesMatrix[i][j] = elementGetIndex(elements[j]);
        }
        for (j = 0; j < size; j++)
            free(elements[j]);
    }
    free(elements);
}

double getMedianFromKDArray(SPKDArray kdArr, int coor){
    int j = 0;
    int size = kdArr.numOfPoints;
    SortElement **elements = (SortElement **) malloc(size * sizeof(SortElement *));
    SortElement *element;
    for (; j < size; j++) {
        element = elementCreate(j, spPointGetData(kdArr.points[j], coor));
        elements[j] = element;
    }
    double median = findMedian(elements, size);
    for (j = 0; j < size; j++)
        free(elements[j]);
    free(elements);
    return median;
}

int getMaxSpreadDim(SPKDArray kdArr){
    int i = 0, j, maxSpreadDim = -1;
    double value, maximumSpread = -1, maximum = -1, minimum = INFINITY;
    for (; i < kdArr.dim; i++){
        for (j = 0; j < kdArr.numOfPoints; j++){
            value = spPointGetData(kdArr.points[j], i);
            if (value > maximum)
                maximum = value;
            if (value < minimum)
                minimum = value;
        }
        if ((maximum - minimum) > maximumSpread){
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

void buildKdArrFromXAndMap(SPKDArray originKDArr, SPKDArray *kdArr, int *X,
                           SPPoint **points, int *map, int numberOfPoints,
                           int xValue) {
    mallocAndPopulateKDArray(kdArr, numberOfPoints, originKDArr.dim, points);
    int i = 0, j, k, currentIndex;
    for (; i < originKDArr.dim; i++) {
        k = 0;
        kdArr->indexesMatrix[i] = (int *) malloc(sizeof(int) * numberOfPoints);
        for (j = 0; j < originKDArr.numOfPoints; j++) {
            currentIndex = originKDArr.indexesMatrix[i][j];
            if (X[currentIndex] == xValue) {
                kdArr->indexesMatrix[i][k] = map[currentIndex];
                k++;
            }
        }
    }
}

void split(SPKDArray kdArr, int coor, SPKDArray *leftKDArr, SPKDArray *rightKDArr) {
    int half;
    CEIL_DEVISION(kdArr.numOfPoints, 2, half)
    int leftNumberOfPoints = half;
    int rightNumberOfPoints = kdArr.numOfPoints - half;
    SPPoint **leftPoints = (SPPoint **) malloc(sizeof(SPPoint *) * leftNumberOfPoints);
    SPPoint **rightPoints = (SPPoint **) malloc(sizeof(SPPoint *) * rightNumberOfPoints);
    int *X = (int *) malloc(sizeof(int) * kdArr.numOfPoints);
    int *map1 = (int *) malloc(sizeof(int) * kdArr.numOfPoints);
    int *map2 = (int *) malloc(sizeof(int) * kdArr.numOfPoints);
    getX(kdArr, coor, X);
    getPointsAndMaps(kdArr, X, leftPoints, rightPoints, map1, map2);
    buildKdArrFromXAndMap(kdArr, leftKDArr, X, leftPoints, map1, leftNumberOfPoints, 0);
    buildKdArrFromXAndMap(kdArr, rightKDArr, X, rightPoints, map2, rightNumberOfPoints, 1);
    free(X);
    free(map1);
    free(map2);
}
