//
// Created by ori on 31/12/16.
//

#include "main_aux.h"

int main() {
    int n, d, k;
    printf("Please enter three space separated numbers: \n");
    scanf("%d %d %d", &n, &d, &k);
    assert(d >= 1 && k >= 1 && k <= n);
    SPPoint **points = getPointsByUser(n, d);
    SPPoint *q = getPointByUser(d);
    Result *results = calcDistances(n, q, points);
    results = getSortedResults(results, n);
    printKResults(results, k);
    freeObjects(results, q, points, n);
};