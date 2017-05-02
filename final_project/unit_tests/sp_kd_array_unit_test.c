#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "unit_test_util.h" //SUPPORTING MACROS ASSERT_TRUE/ASSERT_FALSE etc..
#include "../SPKDArray.h"

void getInitTestPoints(SPPoint **points) {
    double data1[2] = {1, 2};
    double data2[2] = {123, 70};
    double data3[2] = {2, 7};
    double data4[2] = {9, 11};
    double data5[2] = {3, 4};
    points[0] = spPointCreate(data1, 2, 0);
    points[1] = spPointCreate(data2, 2, 1);
    points[2] = spPointCreate(data3, 2, 2);
    points[3] = spPointCreate(data4, 2, 3);
    points[4] = spPointCreate(data5, 2, 4);
}

void getSplitTestPoints(SPPoint **leftPoints, SPPoint **rightPoints) {
    double data1[2] = {1, 2};
    double data2[2] = {123, 70};
    double data3[2] = {2, 7};
    double data4[2] = {9, 11};
    double data5[2] = {3, 4};
    leftPoints[0] = spPointCreate(data1, 2, 0);
    leftPoints[1] = spPointCreate(data3, 2, 2);
    leftPoints[2] = spPointCreate(data5, 2, 4);
    rightPoints[0] = spPointCreate(data2, 2, 1);
    rightPoints[1] = spPointCreate(data4, 2, 3);
}

void populateKDArrayMatrix(SPKDArray *kdArr, int n, int indexesMatrix[][n]){
    if ((kdArr->indexesMatrix = (int **)malloc(kdArr->dim * sizeof(int *))) == NULL) return;
    int i = 0, j;
    for (; i < kdArr->dim; i++){
        if ((kdArr->indexesMatrix[i] = (int *)malloc(kdArr->numOfPoints * sizeof(int))) == NULL) return;
        for (j = 0; j < kdArr->numOfPoints; j++) {
            kdArr->indexesMatrix[i][j] = indexesMatrix[i][j];
        }
    }
}

void getInitTestGoodKDArray(SPKDArray *kdArr) {
    kdArr->dim = 2;
    kdArr->numOfPoints = 5;
    SPPoint **points;
    if ((points = (SPPoint **)malloc(sizeof(SPPoint *) * kdArr->numOfPoints)) == NULL) return;
    getInitTestPoints(points);
    kdArr->points = points;
    int indexesMatrix[2][5] = {{0, 2, 4, 3, 1}, {0, 4, 2, 3, 1}};
    populateKDArrayMatrix(kdArr, kdArr->numOfPoints, indexesMatrix);
}

void getSplitTestGoodKDArray(SPKDArray *leftKDArray, SPKDArray *rightKDArray) {
    leftKDArray->dim = 2;
    rightKDArray->dim = 2;
    leftKDArray->numOfPoints = 3;
    rightKDArray->numOfPoints = 2;
    SPPoint **leftPoints, **rightPoints;
    if ((leftPoints = (SPPoint **)malloc(sizeof(SPPoint *) * leftKDArray->numOfPoints)) == NULL) return;
    if ((rightPoints = (SPPoint **)malloc(sizeof(SPPoint *) * rightKDArray->numOfPoints)) == NULL) return;
    getSplitTestPoints(leftPoints, rightPoints);
    leftKDArray->points = leftPoints;
    rightKDArray->points = rightPoints;
    int leftIndexesMatrix[2][3] = {{0, 1, 2}, {0, 2, 1}};
    int rightIndexesMatrix[2][2] = {{1, 0}, {1, 0}};
    populateKDArrayMatrix(leftKDArray, leftKDArray->numOfPoints, leftIndexesMatrix);
    populateKDArrayMatrix(rightKDArray, rightKDArray->numOfPoints, rightIndexesMatrix);
}

bool comparePoint(SPPoint *p1, SPPoint *p2) {
    if (p1->dim != p2->dim || p1->index != p2->index) return false;
    int i = 0;
    for (; i < p1->dim; i++)
        if (p1->data[i] != p2->data[i]) return false;
    return true;
}

bool comparePoints(int numOfPoints, SPPoint **points1, SPPoint **points2) {
    int i = 0;
    for (; i < numOfPoints; i++) {
        if (!comparePoint(points1[i], points2[i])) return false;
    }
    return true;
}

bool compareIndexesMatrix(int rows, int cols, int **matrix1, int **matrix2) {
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (matrix1[i][j] != matrix2[i][j])
                return false;
        }
    }
    return true;
}

bool compareKDArrays(SPKDArray kdArr1, SPKDArray kdArr2){
    ASSERT_TRUE(kdArr1.dim == kdArr2.dim);
    ASSERT_TRUE(kdArr1.numOfPoints == kdArr2.numOfPoints);
    ASSERT_TRUE(compareIndexesMatrix(kdArr1.dim, kdArr1.numOfPoints,
                                     kdArr1.indexesMatrix, kdArr2.indexesMatrix));
    ASSERT_TRUE(comparePoints(kdArr1.numOfPoints, kdArr1.points, kdArr2.points));
    return true;
}

static bool initKDArrayTest() {
    SPKDArray kdArr;
    SPKDArray expectedKDArr;
    getInitTestGoodKDArray(&expectedKDArr);
    int numOfPoints = 5;
    SPPoint **points;
    if ((points = (SPPoint **)malloc(sizeof(SPPoint *) * numOfPoints)) == NULL) return false;
    getInitTestPoints(points);
    ASSERT_TRUE(initKDArray(points, 5, &kdArr) == 0);
    bool res = compareKDArrays(kdArr, expectedKDArr);
    destroyKDArray(kdArr, false);
    destroyKDArray(expectedKDArr, false);
    return res;
}

static bool splitKDArrayTest() {
    SPKDArray kdArr;
    getInitTestGoodKDArray(&kdArr);
    SPKDArray expectedLeftKDArr, expectedRightKDArr, leftKDArr, rightKDArr;
    getSplitTestGoodKDArray(&expectedLeftKDArr, &expectedRightKDArr);
    split(kdArr, 0, &leftKDArr, &rightKDArr);
    bool res;
    res = compareKDArrays(leftKDArr, expectedLeftKDArr);
    res = res && compareKDArrays(rightKDArr, expectedRightKDArr);
    destroyKDArray(leftKDArr, false);
    destroyKDArray(rightKDArr, false);
    destroyKDArray(expectedLeftKDArr, false);
    destroyKDArray(expectedRightKDArr, false);
    return res;
}

int main() {
    RUN_TEST(initKDArrayTest);
    RUN_TEST(splitKDArrayTest);
    return 0;
}
