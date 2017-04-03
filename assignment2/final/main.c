//
// Created by ori on 31/12/16.
//

#include "SPPoint.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef struct result {
    double distance;
    int arrayIndex;
} Result;

typedef struct sp_point_t {
    double *data;
    int index;
    int dim;
} SPPoint;


int main() {
    int n, d, k;
    printf("Please enter three space separated numbers: \n");
    scanf("%d %d %d", &n, &d, &k);
    assert(d >= 1 && k >= 1 && k <= n);
    SPPoint **points = (SPPoint **)calloc(n, sizeof(SPPoint*));
    double *dValues = (double *)calloc(d, sizeof(double));
    printf("Please enter %d space separated doubles\n", d);
    for (int i = 0; i < n; i++){
        for (int j = 0; j < d - 1; j++)
            scanf("%lf ", &dValues[j]);
        scanf("%lf", &dValues[d - 1]);
        points[i] = spPointCreate(dValues, d, 0);
    }
    for (int j = 0; j < d - 1; j++)
        scanf("%lf ", &dValues[j]);
    scanf("%lf", &dValues[d - 1]);
    SPPoint *q = spPointCreate(dValues, d, 0);
    Result *results = (Result *) calloc(n, sizeof(Result));
    Result r;
    for (int i = 0; i < n; i++){
        r.arrayIndex = i;
        r.distance = spPointL2SquaredDistance(points[i], q);
        results[i] = r;
    }
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
    int i;
    for (i = 0; i < k - 1; i++)
        printf("%d ", results[i].arrayIndex + 1);
    printf("%d\n", results[i].arrayIndex + 1);
    free(results);
    free((*q).data);
    free(q);
    free(dValues);
    for (i = 0; i < n;i++){
        spPointDestroy(points[i]);
    }
    free(points);
};