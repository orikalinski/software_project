//
// Created by ori on 13/01/17.
//

#include "sp_image_proc_util.h"
#include "SPPoint.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdlib.h>

using namespace std;
using namespace cv;

typedef struct sp_point_t {
    double *data;
    int index;
    int dim;
} SPPoint;

SPPoint **spGetRGBHist(const char* str, int imageIndex, int nBins) {
    SPPoint **colorsHistograms = (SPPoint **)calloc(3, sizeof(SPPoint *));
    double *emptyData = (double *)calloc(nBins, sizeof(double));
    double numOfValsPerBin = 256.0 / nBins;
    for (int i = 0; i < 3; i++){
        colorsHistograms[i] = spPointCreate(emptyData, nBins, imageIndex);
    }
    free(emptyData);
    Mat image;
    image = imread(str, CV_LOAD_IMAGE_COLOR);
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
    cout << "Mat = "<< endl << " "  << colorsHistograms[0]->data[0] << endl << endl;
    return colorsHistograms;

}

int main(){
    const  char* str = "./images/img1.png";
    int imageIndex = 1;
    int nBins = 4;
    spGetRGBHist(str, imageIndex, nBins);
    return -1;
}
