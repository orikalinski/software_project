//
// Created by ori on 13/01/17.
//

#include "sp_image_proc_util.h"
#include <iostream>
#include <opencv/cv.h>
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
    for (int i = 0; i < 3; i++){
        colorsHistograms[i] = spPointCreate(emptyData, nBins, imageIndex);
    }
    free(emptyData);
    Mat image = imread(str, CV_LOAD_IMAGE_COLOR);
    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << str << std::endl ;
        return NULL;
    }
    vector<Mat> bgr_planes;
    split(image, bgr_planes);
    Mat b_hist, g_hist, r_hist;
    float range[] = {0, 256};
    const float* histRange = { range };
    bool uniform = true; bool accumulate = false;
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &nBins, &histRange, uniform, accumulate);
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &nBins, &histRange, uniform, accumulate);
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &nBins, &histRange, uniform, accumulate);
    for (int i = 0; i < nBins; i++){
        colorsHistograms[0]->data[i] = (double)(cvRound(r_hist.at<float>(i)));
        colorsHistograms[1]->data[i] = (double)(cvRound(g_hist.at<float>(i)));
        colorsHistograms[2]->data[i] = (double)(cvRound(b_hist.at<float>(i)));
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

SPPoint** spGetSiftDescriptors(const char* str, int imageIndex,
                               int nFeaturesToExtract, int* nFeatures) {
    cv::Mat image = cv::imread(str, CV_LOAD_IMAGE_GRAYSCALE);
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
    cv::Ptr<cv::xfeatures2d::SiftDescriptorExtractor> detect =
            cv::xfeatures2d::SIFT::create(nFeaturesToExtract);

    detect->detect(image, keypoints);
    detect->compute(image, keypoints, descriptors);

    *nFeatures = descriptors.rows;

    SPPoint** result = (SPPoint**)malloc(sizeof(SPPoint*) * descriptors.rows);

    double emptyData[128];
    for (int i = 0;i < descriptors.rows; i++){
        result[i] = spPointCreate(emptyData, 128, imageIndex);
        for (int j = 0; j < 128; j++){
            result[i]->data[j] = (double) descriptors.at<float>(i, j);
        }
    }

    return result;
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
    for (int i = 0; i < numberOfImages; i++)
        numberOfFeatures += nFeaturesPerImage[i];
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