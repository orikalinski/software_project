//
// Created by ori on 29/04/17.
//

#include "main_aux.h"
#include <fcntl.h>

#define MAX_SIZE 1024

int getBestK(int *input, int n, int k, int *bestK) {
    BPQueueElement *tmp;
    if ((tmp = (BPQueueElement *) malloc(sizeof(BPQueueElement))) == NULL) {
        return 1;
    }
    SPBPQueue *queue;
    if ((queue = spBPQueueCreate(n)) == NULL) {
        free(tmp);
        return 1;
    }
    for (int i = 0; i < n; i++)
        spBPQueueEnqueue(queue, i, -(double) input[i]);

    for (int i = 0; i < k; i++) {
        spBPQueuePeek(queue, tmp);
        bestK[i] = tmp->index;
        spBPQueueDequeue(queue);
    }
    free(tmp);
    spBPQueueDestroy(queue);
    return 0;
}

int writeToFeat(SPConfig config, int numOfFeats, SPPoint **points) {
    SP_CONFIG_MSG msg;
    int index = points[0]->index;
    int dim = points[0]->dim;
    char featFilename[MAX_SIZE];
    if ((msg = spConfigGetImageFeatPath(featFilename, config, index)) != SP_CONFIG_SUCCESS) return 1;
    FILE *file;
    if ((file = fopen(featFilename, "w+")) == NULL) {
        char errorMsg[MAX_SIZE];
        sprintf(errorMsg, "The file: %s couldn't be open for write\n", featFilename);
        spLoggerPrintError(errorMsg, __FILE__, __FUNCTION__, __LINE__);
        return 1;
    }
    fwrite(&numOfFeats, sizeof(int), 1, file);
    fwrite(&dim, sizeof(int), 1, file);
    double *data = (double *) malloc(0);
    int j, k, currentSize = 0;
    for (j = 0; j < numOfFeats; j++) {
        data = (double *) realloc(data, (currentSize + points[j]->dim) * sizeof(double));
        for (k = 0; k < points[j]->dim; k++)
            data[currentSize + k] = spPointGetData(points[j], k);
        currentSize += points[j]->dim;
    }
    fwrite(data, sizeof(double), currentSize, file);
    free(data);
    fclose(file);
    return 0;
}

SPPoint **readFromFeat(SPConfig config, int index, int *numOfFeats) {
    SP_CONFIG_MSG msg;
    char featFilename[MAX_SIZE];
    if ((msg = spConfigGetImageFeatPath(featFilename, config, index)) != SP_CONFIG_SUCCESS) return NULL;
    FILE *file;
    if ((file = fopen(featFilename, "r")) == NULL) {
        char errorMsg[MAX_SIZE];
        sprintf(errorMsg, "The file: %s couldn't be open for read\n", featFilename);
        spLoggerPrintError(errorMsg, __FILE__, __FUNCTION__, __LINE__);
        return NULL;
    }
    int dim;
    fread(numOfFeats, sizeof(int), 1, file);
    fread(&dim, sizeof(int), 1, file);
    double *data, *dataPerPoint;
    if ((data = (double *) malloc(sizeof(double) * *numOfFeats * dim)) == NULL) return NULL;
    fread(data, sizeof(double), *numOfFeats * dim, file);
    if ((dataPerPoint = (double *) malloc(sizeof(double) * dim)) == NULL) return NULL;
    int j, k;
    SPPoint **points;
    if ((points = (SPPoint **) malloc(*numOfFeats * sizeof(SPPoint *))) == NULL) return NULL;
    for (j = 0; j < *numOfFeats; j++) {
        for (k = 0; k < dim; k++)
            dataPerPoint[k] = data[j * dim + k];
        points[j] = spPointCreate(dataPerPoint, dim, index);
    }
    free(data);
    free(dataPerPoint);
    fclose(file);
    return points;
}

SPPoint **processPoints(SPConfig config, int numberOfImages, sp::ImageProc imageProc, int *numOfFeatures) {
    int i = 0, j = 0, m = 0;
    SP_CONFIG_MSG msg;
    int numOfConfigFeats = spConfigGetNumOfFeatures(config, &msg);
    int numOfFeats = numOfConfigFeats;
    if (msg != SP_CONFIG_SUCCESS) return NULL;
    SPPoint **allFeatures = (SPPoint **) malloc(0);
    char imagePath[MAX_SIZE], featFilename[MAX_SIZE];
    SPPoint **currentFeatures;
    bool isExtractionMode = spConfigIsExtractionMode(config, &msg);
    if (isExtractionMode) {
        for (i = 0; i < numberOfImages; i++) {
            if ((msg = spConfigGetImagePath(imagePath, config, i)) != SP_CONFIG_SUCCESS)
                return NULL;
            currentFeatures = imageProc.getImageFeatures(imagePath, i, &numOfFeats);
            allFeatures = (SPPoint **) realloc(allFeatures, (m + numOfFeats) * sizeof(SPPoint *));
            for (j = 0; j < numOfFeats; j++)
                allFeatures[m + j] = currentFeatures[j];
            m += numOfFeats;
            writeToFeat(config, numOfFeats, currentFeatures);
            numOfFeats = numOfConfigFeats;
        }
    } else {
        if (msg != SP_CONFIG_SUCCESS) return NULL;
        for (i = 0; i < numberOfImages; i++) {
            if ((msg = spConfigGetImageFeatPath(featFilename, config, i)) != SP_CONFIG_SUCCESS)
                return NULL;
            currentFeatures = readFromFeat(config, i, &numOfFeats);
            allFeatures = (SPPoint **) realloc(allFeatures, (m + numOfFeats) * sizeof(SPPoint *));
            for (j = 0; j < numOfFeats; j++)
                allFeatures[m + j] = currentFeatures[j];
            m += numOfFeats;
            free(currentFeatures);
        }
    }
    *numOfFeatures = m;
    return allFeatures;
}

void freeAfterExecution(int numOfFeats, SPConfig config,
                        SPPoint **queryFeatures, int *counterPerImage,
                        int *bestK, KDTreeNode *root){
    int j = 0;
    for (j = 0; j < numOfFeats; j++) {
        spPointDestroy(queryFeatures[j]);
    }
    free(queryFeatures);
    free(counterPerImage);
    free(bestK);
    destroyKDTree(root);
    spConfigDestroy(config);
    spLoggerDestroy();
}

int handleQueryImages(SPConfig config, KDTreeNode *root, sp::ImageProc imageProc, int numberOfImages){
    char queryPath[MAX_SIZE], imagePath[MAX_SIZE];
    int j, m, k, numOfFeats;
    int *nearestIndexes;
    SP_CONFIG_MSG msg;
    SPPoint **queryFeatures;
    numOfFeats = spConfigGetNumOfFeatures(config, &msg);
    if (msg != SP_CONFIG_SUCCESS) return 1;
    k = spConfigGetKNN(config, &msg);
    if (msg != SP_CONFIG_SUCCESS)
        return 1;
    int numberOfSimilarImages = spConfigGetNumberOfSimilarImages(config, &msg);
    if (msg != SP_CONFIG_SUCCESS)
        return 1;
    int *bestK = (int *)malloc(sizeof(int) * numberOfSimilarImages);
    bool minimalGui = spConfigMinimalGui(config, &msg);
    if (msg != SP_CONFIG_SUCCESS) return 1;
    int *counterPerImage;
    if ((counterPerImage = (int *)malloc(sizeof(int) * numberOfImages)) == NULL) return 1;
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
    freeAfterExecution(numOfFeats, config, queryFeatures, counterPerImage, bestK, root);
    return 0;
}

int createLogger(SPConfig config) {
    SP_CONFIG_MSG msg;
    char loggerFileName[MAX_SIZE];
    msg = spConfigGetLoggerFileName(loggerFileName, config);
    if (msg != SP_CONFIG_SUCCESS) return 1;
    SP_LOGGER_LEVEL loggerLevel = spConfigGetLoggerLevel(config, &msg);
    if (msg != SP_CONFIG_SUCCESS) return 1;
    spLoggerCreate(loggerFileName, loggerLevel);
    return 0;
}

KDTreeNode *getKDTree(SPConfig config, sp::ImageProc imageProc, int numberOfImages){
    int m;
    KDTreeNode *root;
    SPPoint **allFeatures;
    if ((allFeatures = processPoints(config, numberOfImages, imageProc, &m)) == NULL) return NULL;
    if ((root = initKDTree(allFeatures, m)) == NULL) return NULL;
    free(allFeatures);
    return root;
}