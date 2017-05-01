#include <string.h>
#include <stdio.h>

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
    int m, j;
    int numberOfImages;
    numberOfImages = spConfigGetNumOfImages(config, &msg);
    if (msg != SP_CONFIG_SUCCESS)
        return 1;
    SPPoint **allFeatures = processPoints(config, numberOfImages, imageProc, &m);
    j = 0;
    KDTreeNode *root = initKDTree(allFeatures, m);
    int *nearestIndexes;
    int numOfFeats;
    SPPoint **queryFeatures;
    char queryPath[MAX_SIZE], imagePath[MAX_SIZE];;
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
        queryFeatures = imageProc.getImageFeatures(queryPath, -1, &numOfFeats);
        for (j = 0; j < numOfFeats; j++) {
            nearestIndexes = kNearestNeighborSearch(config, root, queryFeatures[j]);
            for (m = 0; m < k; m++){
                counterPerImage[nearestIndexes[m]] += 1;
            }
            free(nearestIndexes);
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
    spLoggerDestroy();
    free(allFeatures);
    for (j = 0; j < numOfFeats; j++)
        spPointDestroy(queryFeatures[j]);
    free(queryFeatures);
    free(counterPerImage);
    free(bestK);
    destroyKDTree(root);
    return 0;
}