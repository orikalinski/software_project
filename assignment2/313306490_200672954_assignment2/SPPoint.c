#include "SPPoint.h"
#include <assert.h>
#include <stdlib.h>


struct sp_point_t {
  double *data;
  int dim;
  int index;
};

int copyDoubleArraySPPointField(SPPoint *p, double *arr) {
  double *arr_cpy = (double *)malloc(sizeof(double) * p->dim);
  if (arr_cpy == NULL) {
    return 1;
  }
  for (int i = 0; i < p->dim; ++i) {
    *(arr_cpy + i) = *(arr + i);
  }
  p->data = arr_cpy;
  return 0;
}

SPPoint *spPointCreate(double *data, int dim, int index) {
  SPPoint *p = (SPPoint *)malloc(sizeof(struct sp_point_t));
  if (p == NULL) {
    return p;
  }
  p->index = index;
  p->dim = dim;
  if (!copyDoubleArraySPPointField(p, data)) return p;
  return NULL;
}

SPPoint *spPointCopy(SPPoint *source) {
  assert(source != NULL);
  return spPointCreate(source->data, source->dim, source->index);
}

void spPointDestroy(SPPoint *point) {
  if (point == NULL) {
  } else {
    free(point->data);
    free(point);
  }
}

int spPointGetDimension(SPPoint *point) {
  assert(point != NULL);
  return point->dim;
}

int spPointGetIndex(SPPoint *point) {
  assert(point != NULL);
  return point->index;
}

double spPointGetAxisCoor(SPPoint *point, int axis) {
  assert(point != NULL);
  assert(axis < (point->dim));
  assert(axis >= 0);
  return point->data[axis ];
}

double spPointL2SquaredDistance(SPPoint *p, SPPoint *q) {
  assert(p != NULL);
  assert(q != NULL);
  assert(p->dim == q->dim);
  double ip = 0;
  for (int axis = 0; axis < p->dim; ++axis) {
    double ret = ((p->data[axis]) - (q->data[axis]));
    ip += ret*ret;
  }
  return ip;
}

