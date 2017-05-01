//
// Created by ori on 29/04/17.
//

#ifndef FINAL_PROJECT_MAIN_AUX_H
#define FINAL_PROJECT_MAIN_AUX_H

#endif //FINAL_PROJECT_MAIN_AUX_H

#include "SPImageProc.h"

extern "C" {
#include "SPConfig.h"
#include "SPBPriorityQueue.h"
}

int getBestK(int *input, int n, int k, int *bestK);

SPPoint **processPoints(SPConfig config, int numberOfImages, sp::ImageProc imageProc, int *numOfFeatures);