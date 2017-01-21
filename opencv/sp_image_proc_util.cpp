//
// Created by ori on 13/01/17.
//

#include "sp_image_proc_util.h"
#include <opencv/cv.h>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/xfeatures2d.hpp"

extern "C"{
#include "SPBPriorityQueue.h"
}

using namespace std;
using namespace cv;

struct sp_point_t {
    double *data;
    int index;
    int dim;
};

SPPoint **spGetRGBHist(const char* str, int imageIndex, int nBins) {
    SPPoint **colorsHistograms = (SPPoint **)calloc(3, sizeof(SPPoint *));
    double *emptyData = (double *)calloc(nBins, sizeof(double));
    double numOfValsPerBin = 256.0 / nBins;
    for (int i = 0; i < 3; i++){
        colorsHistograms[i] = spPointCreate(emptyData, nBins, imageIndex);
    }
    free(emptyData);
    Mat image = imread(str, CV_LOAD_IMAGE_COLOR);
    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return NULL;
    }
    Vec3b v;
    float color;
    for (int i = 0; i < image.rows; i++){
        for (int j = 0; j < image.cols; j++){
            v = image.at<Vec3b>(i, j);
            for (int k = 0; k < 3; k++){
                color = v.val[k];
                colorsHistograms[k]->data[(int)(color / numOfValsPerBin)]++;
            }
        }
    }
    return colorsHistograms;

}

double spRGBHistL2Distance(SPPoint** rgbHistA, SPPoint** rgbHistB){
    double avgL2 = 0;
    for (int i = 0; i < 3; i++){
        avgL2 += 0.33 * spPointL2SquaredDistance(rgbHistA[i], rgbHistB[i]);
    }
    return avgL2;
}


SPPoint** spGetSiftDescriptors(const char* str, int imageIndex, int nFeaturesToExtract, int *nFeatures){
    cv::Ptr<Feature2D> f2d = xfeatures2d::SIFT::create(nFeaturesToExtract);
    std::vector<KeyPoint> keypoints1;
    Mat image = imread(str, CV_LOAD_IMAGE_COLOR);
    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return NULL;
    }
    f2d->detect(image, keypoints1);
    Mat descriptors;
    f2d->compute(image, keypoints1, descriptors);
    int dim = descriptors.rows;
    *nFeatures = dim;
    SPPoint **siftDescriptors = (SPPoint **)malloc(dim * sizeof(SPPoint *));
    double emptyData[128];
    for (int i = 0;i < dim; i++){
        siftDescriptors[i] = spPointCreate(emptyData, 128, imageIndex);
        for (int j = 0; j < 128; j++){
            siftDescriptors[i]->data[j] = descriptors.at<double>(i, j);
        }
    }
    return siftDescriptors;
}

int* kClosestPointToQ(int n, int k, SPPoint** points, SPPoint* q){

    BPQueueElement *tmp = (BPQueueElement *) malloc(sizeof(BPQueueElement));

    int *kClosest = (int *) calloc(k, sizeof(int));
    SPBPQueue *queue = spBPQueueCreate(n);
    for (int i = 0; i < n; i++) {
        int index = spPointGetIndex(points[i]);
        double distance = spPointL2SquaredDistance(points[i], q);
        spBPQueueEnqueue(queue, index, distance);
    }

    for (int i = 0; i < k; i++) {
        spBPQueuePeek(queue, tmp);
        kClosest[i] = tmp->index;
        spBPQueueDequeue(queue);
    }
    free(tmp);
    spBPQueueDestroy(queue);
    return kClosest;
}

int* spBestSIFTL2SquaredDistance(int kClosest, SPPoint* queryFeature,
                                 SPPoint*** databaseFeatures, int numberOfImages,
                                 int* nFeaturesPerImage){
    int numberOfFeatures = 0;
    for (int i = 0; i < numberOfImages; i++){
        for (int j = 0; j < nFeaturesPerImage[i]; j++){
            numberOfFeatures++;
        }
    }
    SPPoint **flattenPoints = (SPPoint **)calloc(numberOfFeatures, sizeof(SPPoint *));
    int k = 0;
    for (int i = 0; i < numberOfImages; i++){
        for (int j = 0; j < nFeaturesPerImage[i]; j++){
            flattenPoints[k] = databaseFeatures[i][j];
            k++;
        }
    }
    return kClosestPointToQ(numberOfFeatures, kClosest, flattenPoints, queryFeature);
}