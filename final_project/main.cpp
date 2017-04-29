#include <iostream>
#include <string.h>

#include "SPImageProc.h"
#include "main_aux.h"

extern "C" {
#include "SPKDTree.h"
}

#define MAX_SIZE 1024

int main(int argc, char *argv[]) {
    SP_CONFIG_MSG msg;
    if ((argc == 3 && strcmp(argv[1], "-c")) || (argc != 1 && argc != 3)) {
        printf("Invalid command line : use -c <config_filename>");
        return 1;
    }
    const char *filename = argc == 1 ? "spcbir.config" : argv[2];
    const SPConfig config = spConfigCreate(filename, &msg);
    if (msg != SP_CONFIG_SUCCESS) return 1;
    char loggerFileName[MAX_SIZE];
    msg = spConfigGetLoggerFileName(loggerFileName, config);
    if (msg != SP_CONFIG_SUCCESS) return 1;
    SP_LOGGER_LEVEL loggerLevel = spConfigGetLoggerLevel(config, &msg);
    if (msg != SP_CONFIG_SUCCESS) return 1;
    spLoggerCreate(loggerFileName, loggerLevel);
    sp::ImageProc imageProc(config);
    int i = 0, j = 0, m = 0;
    int numOfFeats;
    SPPoint **currentFeatures;
    SPPoint **allFeatures;
    int numberOfImages;
    char imagePath[MAX_SIZE];
    char queryPath[MAX_SIZE];
    numberOfImages = spConfigGetNumOfImages(config, &msg);
    if (msg != SP_CONFIG_SUCCESS)
        return 1;
    if ((msg = spConfigGetImagePath(imagePath, config, i)) != SP_CONFIG_SUCCESS) return 1;
    allFeatures = imageProc.getImageFeatures(imagePath, i, &numOfFeats);
    m += numOfFeats;
    for (i = 1; i < numberOfImages; i++) {
        msg = spConfigGetImagePath(imagePath, config, i);
        if (msg != SP_CONFIG_SUCCESS) return 1;
        currentFeatures = imageProc.getImageFeatures(imagePath, i, &numOfFeats);
        allFeatures = (SPPoint **)realloc(allFeatures, (m + numOfFeats) * sizeof(SPPoint *));
        for (j = 0; j < numOfFeats; j++)
            allFeatures[m + j] = currentFeatures[j];
        m += numOfFeats;
    }
    KDTreeNode *root = initKDTree(allFeatures, m);
    int *nearestIndexes;
    int *counterPerImage = (int *)malloc(sizeof(int) * numberOfImages);
    int k = spConfigGetKNN(config, &msg);
    if (msg != SP_CONFIG_SUCCESS)
        return 1;
    int numberOfSimilarImages = spConfigGetNumberOfSimilarImages(config, &msg);
    if (msg != SP_CONFIG_SUCCESS)
        return 1;
    int *bestK = (int *)malloc(sizeof(int) * numberOfSimilarImages);
    bool minimalGui = spConfigMinimalGui(config, &msg);
    if (msg != SP_CONFIG_SUCCESS) return 1;
    while (true) {
        for (j = 0; j < numberOfImages; j++)
            counterPerImage[j] = 0;
        printf("Please enter an image path:\n");
        scanf("%s", queryPath);
        if (strcmp(queryPath, "<>") == 0)
            break;
        currentFeatures = imageProc.getImageFeatures(queryPath, -1, &numOfFeats);
        for (j = 0; j < numOfFeats; j++) {
            nearestIndexes = kNearestNeighborSearch(config, root, currentFeatures[j]);
            for (m = 0; m < k; m++){
                counterPerImage[nearestIndexes[m]] += 1;
            }
        }

        getBestK(counterPerImage, numberOfImages, numberOfSimilarImages, bestK);
        for (j = 0; j < numberOfSimilarImages; j++){
            msg = spConfigGetImagePath(imagePath, config, bestK[j]);
            if (msg != SP_CONFIG_SUCCESS) return 1;
            if (minimalGui)
                imageProc.showImage(imagePath);
            else {
                if (j == 0)
                    printf("Best candidates for - %s - are:\n", queryPath);
                printf("%s\n", imagePath);
            }
        }
    }

    spConfigDestroy(config);
    return 0;
}