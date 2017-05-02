//
// Created by ori on 22/04/17.
//

#include <stdlib.h>
#include <math.h>
#include "SPKDTree.h"

int buildKDTree(SPKDArray kdArr, KDTreeNode *kdNode, int spKDTreeSplitMethod, bool isRoot) {
    if (kdArr.numOfPoints == 1) {
        kdNode->dim = -1;
        kdNode->val = -1;
        kdNode->left = NULL;
        kdNode->right = NULL;
        kdNode->point = kdArr.points[0];
        destroyKDArray(kdArr, isRoot);
        return 0;
    }
    int dim;
    if (spKDTreeSplitMethod == 0)
        dim = rand() % kdArr.dim;
    else if (spKDTreeSplitMethod == 1)
        dim = getMaxSpreadDim(kdArr);
    else
        dim = kdNode->dim + 1;
    SPKDArray leftKDArr, rightKDArr;
    split(kdArr, dim, &leftKDArr, &rightKDArr);
    KDTreeNode *leftNode, *rightNode;
    if ((leftNode = (KDTreeNode *) malloc(sizeof(KDTreeNode))) == NULL) return 1;
    if ((rightNode = (KDTreeNode *) malloc(sizeof(KDTreeNode))) == NULL) return 1;
    if (buildKDTree(leftKDArr, leftNode, spKDTreeSplitMethod, false) != 0) return 1;
    if (buildKDTree(rightKDArr, rightNode, spKDTreeSplitMethod, false) != 0) return 1;
    kdNode->left = leftNode;
    kdNode->right = rightNode;
    kdNode->dim = dim;
    double median;
    if (getMedianFromKDArray(kdArr, dim, &median) != 0) return 1;
    kdNode->val = median;
    kdNode->point = NULL;
    destroyKDArray(kdArr, isRoot);
    return 0;
}

KDTreeNode *initKDTree(SPPoint **arr, int size) {
    SPKDArray kdArr;
    if (initKDArray(arr, size, &kdArr) != 0) return NULL;
    KDTreeNode *root;
    if ((root = (KDTreeNode *) malloc(sizeof(KDTreeNode))) == NULL) return NULL;
    if (buildKDTree(kdArr, root, 1, true) != 0) return NULL;
    return root;
}

bool isLeaf(KDTreeNode *node) {
    if (!node->left && !node->right)
        return true;
    return false;
}

void destroyKDTree(KDTreeNode *curr) {
    if (isLeaf(curr))
        spPointDestroy(curr->point);
    if (curr->left)
        destroyKDTree(curr->left);
    if (curr->right)
        destroyKDTree(curr->right);
    free(curr);
}

void kNearestNeighbors(KDTreeNode *curr, SPBPQueue *bpq, SPPoint *P) {
    if (curr == NULL)
        return;
    if (isLeaf(curr)) {
        spBPQueueEnqueue(bpq, spPointGetIndex(curr->point), spPointL2SquaredDistance(curr->point, P));
        return;
    }
    bool isRight = true;
    if (spPointGetData(P, curr->dim) <= curr->val) {
        isRight = false;
        kNearestNeighbors(curr->left, bpq, P);
    } else {
        kNearestNeighbors(curr->right, bpq, P);
    }
    if (!spBPQueueIsFull(bpq) || pow(curr->val - spPointGetData(P, curr->dim), 2.0) < spBPQueueMaxValue(bpq)) {
        if (isRight)
            kNearestNeighbors(curr->left, bpq, P);
        else
            kNearestNeighbors(curr->right, bpq, P);
    }

}

int *kNearestNeighborSearch(SPConfig conf, KDTreeNode *root, SPPoint *P) {
    SPBPQueue *queue;
    SP_CONFIG_MSG msg;
    int k = spConfigGetKNN(conf, &msg);
    if (msg != SP_CONFIG_SUCCESS) return NULL;
    if ((queue = spBPQueueCreate(k)) == NULL) return NULL;
    kNearestNeighbors(root, queue, P);
    BPQueueElement *tmp;
    if ((tmp = (BPQueueElement *) malloc(sizeof(BPQueueElement))) == NULL)
        return NULL;

    int *kClosest;
    if ((kClosest = (int *) calloc(k, sizeof(int))) == NULL) {
        free(tmp);
        return NULL;
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