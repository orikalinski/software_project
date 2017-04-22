//
// Created by ori on 22/04/17.
//

#include <stdlib.h>
#include "SPKDTree.h"

struct kd_tree_node {
    int dim;
    double val;
    KDTreeNode *left;
    KDTreeNode *right;
    SPPoint *point;
};

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

//void printTree(KDTreeNode *node){
//    printf("dim: %d, val: %f\n", node->dim, node->val);
//    if (node->left)
//        printTree(node->left);
//    if (node->right)
//        printTree(node->right);
//}

void main() {
    SPPoint *points[5];
    double data1[2] = {1, 2};
    double data2[2] = {123, 70};
    double data3[2] = {2, 7};
    double data4[2] = {9, 11};
    double data5[2] = {3, 4};
    points[0] = spPointCreate(data1, 2, 0);
    points[1] = spPointCreate(data2, 2, 1);
    points[2] = spPointCreate(data3, 2, 2);
    points[3] = spPointCreate(data4, 2, 3);
    points[4] = spPointCreate(data5, 2, 4);
    KDTreeNode *root = initKDTree(points, 5);
//    printTree(root);
}