//
// Created by ori on 31/12/16.
//

#include "main_aux.h"

Result *getSortedResults(Result *results, int n){
    Result r;
    for (int i = 0; i < n; ++i)
    {
        for (int j = i + 1; j < n; ++j)
        {
            if (results[i].distance > (results[j]).distance)
            {
                r = results[i];
                results[i] = results[j];
                results[j] = r;
            }
        }
    }
    return results;
}

SPPoint *getPointByUser(int d){
    double *dValues = (double *)calloc(d, sizeof(double));
    for (int j = 0; j < d - 1; j++)
        scanf("%lf ", &dValues[j]);
    scanf("%lf", &dValues[d - 1]);
    SPPoint *point = spPointCreate(dValues, d, 0);
    free(dValues);
    return point;
}

SPPoint **getPointsByUser(int n, int d){
    SPPoint **points = (SPPoint **)calloc(n, spPointSizeOf());
    printf("Please enter %d space separated doubles\n", d);
    for (int i = 0; i < n; i++){
        points[i] = getPointByUser(d);
    }
    return points;
}

Result *calcDistances(int n, SPPoint *q, SPPoint **points){
    Result *results = (Result *) calloc(n, sizeof(Result));
    Result r;
    for (int i = 0; i < n; i++){
        r.arrayIndex = i;
        r.distance = spPointL2SquaredDistance(points[i], q);
        results[i] = r;
    }
    return results;
}

void printKResults(Result *results, int k){
    int i;
    for (i = 0; i < k - 1; i++)
        printf("%d ", results[i].arrayIndex + 1);
    printf("%d\n", results[i].arrayIndex + 1);
}

void freeObjects(Result *results, SPPoint *q,
                 SPPoint **points, int n){
    free(results);
    spPointDestroy(q);
    for (int i = 0; i < n; i++){
        spPointDestroy(points[i]);
    }
    free(points);
}