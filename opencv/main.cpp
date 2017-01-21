#include <iostream>
#include "SPPoint.h"
#include "sp_image_proc_util.h"
#include <unistd.h>

struct sp_point_t {
    double *data;
    int index;
    int dim;
};


int main() {
    const  char* str1 = "./images/img1.png";
    const  char* str2 = "./images/img2.png";
    SPPoint **a = spGetRGBHist(str1, 1, 4);
    SPPoint **b = spGetRGBHist(str2, 2, 4);
//    const  char* str7 = "./images/img   7.png";
    int *nFeatures = (int *)malloc(sizeof(int));
    SPPoint **siftDescriptors = spGetSiftDescriptors(str1, 7, 700, nFeatures);
    printf("answer1: %f\n", spRGBHistL2Distance(a, b));
    printf("answer2: %d, %f\n", *nFeatures, siftDescriptors[3]->data[4]);
    int size;
    const char* fileNames[5];
    fileNames[0] = "./images/img0.png";
    fileNames[1] = "./images/img1.png";
    fileNames[2] = "./images/img2.png";
    fileNames[3] = "./images/img3.png";
    fileNames[4] = "./images/img4.png";
    for (int i = 0;i<5; i++) {
        printf("str: %s\n", fileNames[i]);
        siftDescriptors = spGetSiftDescriptors(fileNames[i], i, 10000, nFeatures);
        size += *nFeatures * sizeof(SPPoint *);
        printf("size: %d\n", size);
    }
    sleep(5);
    SPPoint ***databaseFeatures = (SPPoint ***)malloc(size);
    int *nFeaturesPerImage = (int *)calloc(5, sizeof(int));
    for (int i = 0;i<5; i++) {
        siftDescriptors = spGetSiftDescriptors(fileNames[i], i, 10000, nFeatures);
        nFeaturesPerImage[i] = *nFeatures;
        for (int j = 0; j < *nFeatures; j++){
            databaseFeatures[i][j] = siftDescriptors[j];
        }
    }
    printf("size: %d\n", size);
    int* kClosest = spBestSIFTL2SquaredDistance(10, databaseFeatures[3][100], databaseFeatures, 5, nFeaturesPerImage);
    for (int i = 0;i < 10;i++){
        printf("row: %d\n", kClosest[i]);
    }
    return -1;
}

