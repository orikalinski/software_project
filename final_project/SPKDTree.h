//
// Created by ori on 22/04/17.
//

#ifndef FINAL_PROJECT_SPKDTREE_H
#define FINAL_PROJECT_SPKDTREE_H

#endif //FINAL_PROJECT_SPKDTREE_H

#include "SPKDArray.h"
#include "SPConfig.h"
#include "SPBPriorityQueue.h"

typedef struct kd_tree_node KDTreeNode;

struct kd_tree_node {
    int dim;
    double val;
    KDTreeNode *left;
    KDTreeNode *right;
    SPPoint *point;
};

void buildKDTree(SPKDArray kdArr, KDTreeNode *kdNode, int spKDTreeSplitMethod, bool isRoot);

KDTreeNode *initKDTree(SPPoint **arr, int size);

bool isLeaf(KDTreeNode *node);

void destroyKDTree(KDTreeNode *curr);

void kNearestNeighbors(KDTreeNode *curr, SPBPQueue *bpq, SPPoint *P);

int *kNearestNeighborSearch(SPConfig conf, KDTreeNode *root, SPPoint *P);