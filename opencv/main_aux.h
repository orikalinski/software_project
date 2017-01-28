//
// Created by ori on 21/01/17.
//

#ifndef OPENCV_MAIN_AUX_H
#define OPENCV_MAIN_AUX_H

#endif //OPENCV_MAIN_AUX_H

#include <iostream>
#include "sp_image_proc_util.h"
#include <unistd.h>
#include <stdio.h>
#include <limits>

using namespace std;

int printBestFive(double *input, int n, int sign);

int readInput(string *imagesDirectory, string *imagesPrefix, int *numberOfImages,
              string *imagesSuffix, int *numberOfBins, int *numberOfFeaturesToExtract);

int readImages(int *nFeatures, SPPoint ***RgbHist, SPPoint ***descriptors,
               int *nFeaturesPerImage, std::string imagesDirectory,
               std::string imagesPrefix, std::string imagesSuffix,
               int numberOfImages, int numberOfBins,
               int numberOfFeaturesToExtract);

void freeReadImagesObjs(int *nFeatures, SPPoint ***RgbHist, SPPoint ***descriptors,
                        int *nFeaturesPerImage, int i);

void destroySPPointArray(SPPoint **spPointArray, int len);

void destroyQueryAndImages(int *nFeatures, SPPoint ***RgbHist, SPPoint ***descriptors,
                           int *nFeaturesPerImage, int numberOfImages,
                           SPPoint **QRGBHist, SPPoint **QDesc);