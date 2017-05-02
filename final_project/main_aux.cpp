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
    int fd = open(featFilename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (fd < 0) {
        printf("The file: %s couldn't be open for write\n", featFilename);
        return 1;
    }
    write(fd, &numOfFeats, sizeof(int));
    write(fd, &dim, sizeof(int));
    double *data = (double *) malloc(0);
    int j, k, currentSize = 0;
    for (j = 0; j < numOfFeats; j++) {
        data = (double *) realloc(data, (currentSize + points[j]->dim) * sizeof(double));
        for (k = 0; k < points[j]->dim; k++)
            data[currentSize + k] = spPointGetData(points[j], k);
        currentSize += points[j]->dim;
    }
    write(fd, data, currentSize * sizeof(double));
    free(data);
    close(fd);
    return 0;
}

SPPoint **readFromFeat(SPConfig config, int index, int *numOfFeats) {
    SP_CONFIG_MSG msg;
    char featFilename[MAX_SIZE];
    if ((msg = spConfigGetImageFeatPath(featFilename, config, index)) != SP_CONFIG_SUCCESS) return NULL;
    int fd = open(featFilename, O_RDONLY, 0777);
    if (fd < 0) {
        printf("The file: %s couldn't be open for write\n", featFilename);
        return NULL;
    }
    int dim;
    read(fd, numOfFeats, sizeof(int));
    read(fd, &dim, sizeof(int));
    double *data = (double *) malloc(sizeof(double) * *numOfFeats * dim);
    read(fd, data, sizeof(double) * *numOfFeats * dim);
    double *dataPerPoint = (double *) malloc(sizeof(double) * dim);
    int j, k;
    SPPoint **points = (SPPoint **) malloc(*numOfFeats * sizeof(SPPoint *));
    for (j = 0; j < *numOfFeats; j++) {
        for (k = 0; k < dim; k++)
            dataPerPoint[k] = data[j * dim + k];
        points[j] = spPointCreate(dataPerPoint, dim, index);
    }
    free(data);
    free(dataPerPoint);
    close(fd);
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