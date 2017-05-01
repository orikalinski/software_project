//
// Created by ori on 22/04/17.
//

#ifndef FINAL_PROJECT_SPKDARRAY_H
#define FINAL_PROJECT_SPKDARRAY_H

#endif //FINAL_PROJECT_SPKDARRAY_H

#include "sp_utils.h"

typedef struct sp_kd_array {
    int numOfPoints;
    int dim;
    SPPoint **points;
    int **indexesMatrix;
} SPKDArray;

void mallocAndPopulateKDArray(SPKDArray *kdArr, int numberOfPoints,
                              int dim, SPPoint **points);

void initKDArray(SPPoint **arr, int size, SPKDArray *kdArr);

void destroyKDArray(SPKDArray kdArr, bool isRoot);

double getMedianFromKDArray(SPKDArray kdArr, int coor);

int getMaxSpreadDim(SPKDArray kdArr);

void getX(SPKDArray kdArr, int coor, int *X);

void getPointsAndMaps(SPKDArray kdArr, int *X,
                      SPPoint **leftPoints, SPPoint **rightPoints,
                      int *map1, int *map2);

void buildKdArrFromXAndMap(SPKDArray originKDArr, SPKDArray *kdArr, int *X,
                           SPPoint **points, int *map, int numberOfPoints,
                           int xValue);

void split(SPKDArray kdArray, int coor, SPKDArray *leftKDArr, SPKDArray *rightKDArr);