//
// Created by ori on 21/01/17.
//


#include "main_aux.h"

extern "C"{
#include "SPBPriorityQueue.h"
}

int printBestFive(double *input, int n, int sign){
    BPQueueElement *tmp;
    if ((tmp = (BPQueueElement *) malloc(sizeof(BPQueueElement))) == NULL){
        return 1;
    }
    int numberOfBests = n < 5 ? n : 5;
    SPBPQueue *queue;
    if ((queue = spBPQueueCreate(n)) == NULL){
        free(tmp);
        return 1;
    }
    for (int i = 0; i < n; i++)
        spBPQueueEnqueue(queue, i, sign * input[i]);

    std::string pattern;
    for (int i = 0; i < numberOfBests; i++) {
        pattern = i < numberOfBests - 1 ? "%d, " : "%d\n";
        spBPQueuePeek(queue, tmp);
        printf(pattern.c_str(), tmp->index);
        spBPQueueDequeue(queue);
    }
    free(tmp);
    spBPQueueDestroy(queue);
    return 0;
}

int readInput(std::string *imagesDirectory, std::string *imagesPrefix, int *numberOfImages,
              std::string *imagesSuffix, int *numberOfBins, int *numberOfFeaturesToExtract){
    printf("Enter images directory path:\n");
    std::getline (std::cin, *imagesDirectory);

    printf("Enter images prefix:\n");
    std::getline (std::cin, *imagesPrefix);

    printf("Enter number of images:\n");
    std::cin >> *numberOfImages;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (*numberOfImages < 1) {
        printf("An error occurred - invalid number of images\n");
        return 1;
    }

    printf("Enter images suffix:\n");
    std::getline (std::cin, *imagesSuffix);

    printf("Enter number of bins:\n");
    std::cin >> *numberOfBins;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (*numberOfBins < 1 || *numberOfBins > 255) {
        printf("An error occurred - invalid number of bins\n");
        return 1;
    }

    printf("Enter number of features:\n");
    std::cin >> *numberOfFeaturesToExtract;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (*numberOfFeaturesToExtract < 1){
        printf("An error occurred - invalid number of features\n");
        return 1;
    }
    return 0;
}

void destroyRGBHistSPPoint(SPPoint ***obj, int i){
    for (int j = 0; j < i; j++){
        for (int k = 0; k < 3; k++)
            spPointDestroy(obj[j][k]);
        free(obj[j]);
    }
    free(obj);
}

void destroyDescriptorsSPPoint(SPPoint ***obj, int *nFeaturesPerImage, int i){
    for (int j = 0; j < i; j++){
        for (int k = 0; k < nFeaturesPerImage[j]; k++)
            spPointDestroy(obj[j][k]);
        free(obj[j]);
    }
    free(obj);
}

void freeReadImagesObjs(int *nFeatures, SPPoint ***RgbHist, SPPoint ***descriptors,
                        int *nFeaturesPerImage, int i){
    destroyRGBHistSPPoint(RgbHist, i);
    destroyDescriptorsSPPoint(descriptors, nFeaturesPerImage, i);
    free(nFeaturesPerImage);
    free(nFeatures);
}

int readImages(int *nFeatures, SPPoint ***RgbHist, SPPoint ***descriptors,
               int *nFeaturesPerImage, std::string imagesDirectory,
               std::string imagesPrefix, std::string imagesSuffix,
               int numberOfImages, int numberOfBins,
               int numberOfFeaturesToExtract){
    std::string imagePath;
    for (int i=0; i < numberOfImages; i++){
        imagePath = imagesDirectory + imagesPrefix + std::to_string(i) + imagesSuffix;
        if ((RgbHist[i] = spGetRGBHist(imagePath.c_str(), i, numberOfBins)) == NULL){
            freeReadImagesObjs(nFeatures, RgbHist, descriptors, nFeaturesPerImage, i);
            return 1;
        }
        if ((descriptors[i] = spGetSiftDescriptors(imagePath.c_str(), i, numberOfFeaturesToExtract, nFeatures)) == NULL){
            freeReadImagesObjs(nFeatures, RgbHist, descriptors, nFeaturesPerImage, i);
            return 1;
        }
        nFeaturesPerImage[i] = *nFeatures;
    }
    return 0;
}

void destroySPPointArray(SPPoint **spPointArray, int len){
    for (int k = 0; k < len ; k++)
        spPointDestroy(spPointArray[k]);
    free(spPointArray);
}

void destroyQueryAndImages(int *nFeatures, SPPoint ***RgbHist, SPPoint ***descriptors,
                           int *nFeaturesPerImage, int numberOfImages,
                           SPPoint **QRGBHist, SPPoint **QDesc) {
    freeReadImagesObjs(nFeatures, RgbHist, descriptors, nFeaturesPerImage,
                       numberOfImages);
    destroySPPointArray(QRGBHist, 3);
    destroySPPointArray(QDesc, *nFeatures);
}