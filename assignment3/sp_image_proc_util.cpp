//
// Created by ori on 13/01/17.
//

#include "sp_image_proc_util.h"
#include <iostream>
#include <opencv/cv.h>
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

SPPoint **spGetRGBHist(const char* str, int imageIndex, int nBins) {
    SPPoint **colorsHistograms = (SPPoint **)calloc(3, sizeof(SPPoint *));
    double *emptyData;
    if ((emptyData = (double *)calloc(nBins, sizeof(double))) == NULL)
        return NULL;
    for (int i = 0; i < 3; i++){
        if ((colorsHistograms[i] = spPointCreate(emptyData, nBins, imageIndex)) == NULL){
            for (int j = 0; j < i; j++)
                spPointDestroy(colorsHistograms[j]);
            return NULL;
        }
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
        spPointSetData(colorsHistograms[0], i, (double)(cvRound(r_hist.at<float>(i))));
        spPointSetData(colorsHistograms[1], i, (double)(cvRound(g_hist.at<float>(i))));
        spPointSetData(colorsHistograms[2], i, (double)(cvRound(b_hist.at<float>(i))));
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
    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << str << std::endl ;
        return NULL;
    }
    std::vector<cv::KeyPoint> keypoints;
    cv::Mat descriptors;
    cv::Ptr<cv::xfeatures2d::SiftDescriptorExtractor> detect =
            cv::xfeatures2d::SIFT::create(nFeaturesToExtract);

    detect->detect(image, keypoints);
    detect->compute(image, keypoints, descriptors);

    *nFeatures = descriptors.rows;
    SPPoint** result;
    if((result = (SPPoint**)malloc(sizeof(SPPoint*) * descriptors.rows)) == NULL)
        return NULL;

    double emptyData[128];
    for (int i = 0;i < descriptors.rows; i++){
        if ((result[i] = spPointCreate(emptyData, 128, imageIndex)) == NULL){
            for (int j = 0; j < i; j++)
                spPointDestroy(result[j]);
            return NULL;
        }
        for (int j = 0; j < 128; j++){
            spPointSetData(result[i], j, (double) descriptors.at<float>(i, j));
        }
    }

    return result;
}

int* kClosestPointToQ(int n, int k, SPPoint** points, SPPoint* q){

    BPQueueElement *tmp;
    if ((tmp = (BPQueueElement *) malloc(sizeof(BPQueueElement))) == NULL)
        return NULL;

    int *kClosest;
    if ((kClosest = (int *) calloc(k, sizeof(int))) == NULL) {
        free(tmp);
        return NULL;
    }
    SPBPQueue *queue;
    if ((queue = spBPQueueCreate(n)) == NULL){
        free(tmp);
        free(kClosest);
        return NULL;
    }
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
    SPPoint **flattenPoints;
    if ((flattenPoints = (SPPoint **)calloc(numberOfFeatures, sizeof(SPPoint *))) == NULL)
        return NULL;
    int k = 0;
    for (int i = 0; i < numberOfImages; i++){
        for (int j = 0; j < nFeaturesPerImage[i]; j++){
            flattenPoints[k] = databaseFeatures[i][j];
            k++;
        }
    }
    int *results;
    if ((results = kClosestPointToQ(numberOfFeatures, kClosest, flattenPoints, queryFeature)) == NULL){
        free(flattenPoints);
        return NULL;
    }
    free(flattenPoints);
    return results;
}