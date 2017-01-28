#include "main_aux.h"

int main(){
    std::string imagesDirectory;
    std::string imagesPrefix;
    int numberOfImages;
    std::string imagesSuffix;
    int numberOfBins;
    int numberOfFeaturesToExtract;
    if (readInput(&imagesDirectory, &imagesPrefix, &numberOfImages, &imagesSuffix,
              &numberOfBins, &numberOfFeaturesToExtract) == 1)
        return 1;

    // calculate RGB histogram and siftDesrcriptors for each image.
    int *nFeatures;
    if ((nFeatures = (int *)malloc(sizeof(int))) == NULL)
        return 1;
    SPPoint ***RgbHist;
    if ((RgbHist = (SPPoint ***)calloc(numberOfImages, sizeof(SPPoint **))) == NULL){
        free(nFeatures);
        return 1;
    }
    SPPoint ***descriptors;
    if ((descriptors = (SPPoint ***)calloc(numberOfImages, sizeof(SPPoint **))) == NULL){
        free(nFeatures);
        free(RgbHist);
        return 1;
    }
    int *nFeaturesPerImage;
    if ((nFeaturesPerImage = (int *)calloc(numberOfImages, sizeof(int))) == NULL){
        free(nFeatures);
        free(RgbHist);
        free(descriptors);
        return 1;
    }

    if (readImages(nFeatures, RgbHist, descriptors, nFeaturesPerImage,
                   imagesDirectory, imagesPrefix, imagesSuffix, numberOfImages,
                   numberOfBins, numberOfFeaturesToExtract) == 1)
        return 1;

    printf("Enter a query image or # to terminate:\n");
    std::string queryImagePath;
    std::getline (std::cin, queryImagePath);
    double *queryRGBtoImagesDistance;
    if ((queryRGBtoImagesDistance = (double *) calloc(numberOfImages, sizeof(double))) == NULL){
        freeReadImagesObjs(nFeatures, RgbHist, descriptors, nFeaturesPerImage,
                           numberOfImages);
        return 1;
    }
    while (queryImagePath != "#") {
        SPPoint **QRGBHist;
        if ((QRGBHist = spGetRGBHist(queryImagePath.c_str(), -1, numberOfBins)) == NULL) {
            freeReadImagesObjs(nFeatures, RgbHist, descriptors, nFeaturesPerImage,
                               numberOfImages);
            return 1;
        }
        SPPoint **QDesc;
        if ((QDesc = spGetSiftDescriptors(queryImagePath.c_str(), -1, numberOfFeaturesToExtract, nFeatures)) == NULL){
            freeReadImagesObjs(nFeatures, RgbHist, descriptors, nFeaturesPerImage,
                               numberOfImages);
            destroySPPointArray(QRGBHist, 3);
            return 1;
        }
        for (int i = 0; i < numberOfImages; i++)
            queryRGBtoImagesDistance[i] = spRGBHistL2Distance(QRGBHist, RgbHist[i]);
        printf("Nearest images using global descriptors:\n");
        if (printBestFive(queryRGBtoImagesDistance, numberOfImages, 1) == 1){
            destroyQueryAndImages(nFeatures, RgbHist, descriptors, nFeaturesPerImage,
                                  numberOfImages, QRGBHist, QDesc);
            return 1;
        }
        double *imagesHits;
        if ((imagesHits = (double *) calloc(numberOfImages, sizeof(double))) == NULL){
            destroyQueryAndImages(nFeatures, RgbHist, descriptors, nFeaturesPerImage,
                                  numberOfImages, QRGBHist, QDesc);
            return 1;
        }
        int *bestSiftDistanceImageIndexes;
        for (int i = 0; i < *nFeatures; i++) {
            if ((bestSiftDistanceImageIndexes = spBestSIFTL2SquaredDistance(5, QDesc[i], descriptors,
                                                                            numberOfImages, nFeaturesPerImage)) == NULL){
                destroyQueryAndImages(nFeatures, RgbHist, descriptors, nFeaturesPerImage,
                                      numberOfImages, QRGBHist, QDesc);
                free(imagesHits);
                return 1;
            }
            for (int j = 0; j < 5; j++)
                imagesHits[bestSiftDistanceImageIndexes[j]]++;
            free(bestSiftDistanceImageIndexes);
        }
        printf("Nearest images using local descriptors:\n");
        printBestFive(imagesHits, numberOfImages, -1);
        printf("Enter a query image or # to terminate:\n");
        std::getline (std::cin, queryImagePath);
        free(imagesHits);
        destroySPPointArray(QDesc, *nFeatures);
        destroySPPointArray(QRGBHist, 3);
    }
    free(queryRGBtoImagesDistance);
    freeReadImagesObjs(nFeatures, RgbHist, descriptors, nFeaturesPerImage,
                       numberOfImages);
    printf("Exiting...\n");
    return 0;
}

