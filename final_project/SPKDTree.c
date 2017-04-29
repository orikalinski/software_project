//
// Created by ori on 22/04/17.
//

#include <stdlib.h>
#include <math.h>
#include "SPKDTree.h"

void buildKDTree(SPKDArray kdArr, KDTreeNode *kdNode, int spKDTreeSplitMethod) {
    if (kdArr.numOfPoints == 1) {
        kdNode->dim = -1;
        kdNode->val = -1;
        kdNode->left = NULL;
        kdNode->right = NULL;
        kdNode->point = kdArr.points[0];
        return;
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
    KDTreeNode *leftNode = (KDTreeNode *) malloc(sizeof(KDTreeNode));
    KDTreeNode *rightNode = (KDTreeNode *) malloc(sizeof(KDTreeNode));
    buildKDTree(leftKDArr, leftNode, spKDTreeSplitMethod);
    buildKDTree(rightKDArr, rightNode, spKDTreeSplitMethod);
    kdNode->left = leftNode;
    kdNode->right = rightNode;
    kdNode->dim = dim;
    kdNode->val = getMedianFromKDArray(kdArr, dim);
    kdNode->point = NULL;
}

KDTreeNode *initKDTree(SPPoint **arr, int size) {
    SPKDArray kdArr;
    initKDArray(arr, size, &kdArr);
    KDTreeNode *root = (KDTreeNode *) malloc(sizeof(KDTreeNode));
    buildKDTree(kdArr, root, 1);
    return root;
}

bool isLeaf(KDTreeNode node){
    if (!node.left && !node.right)
        return true;
    return false;
}

void kNearestNeighbors(KDTreeNode *curr, SPBPQueue *bpq, SPPoint *P) {
    if (curr == NULL)
        return;
    if (isLeaf(*curr)) {
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

int *kNearestNeighborSearch(SPConfig conf, KDTreeNode *root, SPPoint *P){
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



void printTree(KDTreeNode *node){
    printf("dim: %d, val: %f\n", node->dim, node->val);
    if (node->left)
        printTree(node->left);
    if (node->right)
        printTree(node->right);
}

//int main(int argc, char *argv[]) {
//    SPPoint *points[5];
//    double data1[2] = {1, 2};
//    double data2[2] = {123, 70};
//    double data3[2] = {2, 7};
//    double data4[2] = {9, 11};
//    double data5[2] = {3, 4};
//    points[0] = spPointCreate(data1, 2, 0);
//    points[1] = spPointCreate(data2, 2, 1);
//    points[2] = spPointCreate(data3, 2, 2);
//    points[3] = spPointCreate(data4, 2, 3);
//    points[4] = spPointCreate(data5, 2, 4);
//    KDTreeNode *root = initKDTree(points, 5);
//    printTree(root);
//    SP_CONFIG_MSG a;
//    if ((argc == 3 && strcmp(argv[1], "-c")) || (argc != 1 && argc != 3)){
//        printf("Invalid command line : use -c <config_filename>");
////        return 1;
//    }
//    char *filename = argc == 1 ? "spcbir.config" : argv[2];
//    SPConfig config = spConfigCreate(filename, &a);
//    double data6[2] = {122, 60};
//    SPPoint *P = spPointCreate(data6, 2, 5);
//    int *results = kNearestNeighborSearch(config, root, P);
//    int i = 0;
//    SP_CONFIG_MSG msg;
//    int k = spConfigGetKNN(config, &msg);
//    for (; i < k; i++){
//        printf("Result: %d\n", results[i]);
//    }
//    spConfigDestroy(config);
//    return 0;
//}