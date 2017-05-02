#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "unit_test_util.h" //SUPPORTING MACROS ASSERT_TRUE/ASSERT_FALSE etc..
#include "../SPKDTree.h"

#define MALLOC_NODE(node, returnValue) \
    if ((node = (KDTreeNode *) malloc(sizeof(KDTreeNode))) == NULL) \
        return returnValue;

void getInitTestPoints(SPPoint **points) {
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
}

void populateNode(KDTreeNode *node, int dim, double val, KDTreeNode *left,
                  KDTreeNode *right, SPPoint *point) {
    node->dim = dim;
    node->val = val;
    node->left = left;
    node->right = right;
    node->point = point;
}

int getInitTestGoodKDTree(KDTreeNode *root){
    SPPoint **points;
    int numOfPoints = 5;
    if ((points = (SPPoint **)malloc(sizeof(SPPoint *) * numOfPoints)) == NULL) return 1;
    getInitTestPoints(points);
    KDTreeNode *node1, *node2, *node3, *node4, *node5, *node6, *node7, *node8;
    MALLOC_NODE(node1, 1)
    MALLOC_NODE(node2, 1)
    MALLOC_NODE(node3, 1)
    MALLOC_NODE(node4, 1)
    MALLOC_NODE(node5, 1)
    MALLOC_NODE(node6, 1)
    MALLOC_NODE(node7, 1)
    MALLOC_NODE(node8, 1)
    populateNode(root, 0, 3, node1, node6, NULL);
    populateNode(node1, 1, 4, node2, node5, NULL);
    populateNode(node2, 1, 3, node3, node4, NULL);
    populateNode(node3, -1, -1, NULL, NULL, points[0]);
    populateNode(node4, -1, -1, NULL, NULL, points[4]);
    populateNode(node5, -1, -1, NULL, NULL, points[2]);
    populateNode(node6, 0, 66, node7, node8, NULL);
    populateNode(node7, -1, -1, NULL, NULL, points[3]);
    populateNode(node8, -1, -1, NULL, NULL, points[1]);
    return 0;
}

bool comparePoint(SPPoint *p1, SPPoint *p2) {
    if (p1 == NULL && p2 == NULL) return true;
    if (p1->dim != p2->dim || p1->index != p2->index) return false;
    int i = 0;
    for (; i < p1->dim; i++)
        if (p1->data[i] != p2->data[i]) return false;
    return true;
}

bool compareKDNodes(KDTreeNode *curr1, KDTreeNode *curr2){
    if (curr1->val != curr2->val || curr1->dim != curr2->dim) return false;
    if (!comparePoint(curr1->point, curr2->point)) return false;
    bool res = true;
    if (curr1->left && curr2->left)
        res = res && compareKDNodes(curr1->left, curr2->left);
    if (curr1->right && curr2->right)
        res = res && compareKDNodes(curr1->right, curr2->right);
    return res;
}

void printTree(KDTreeNode *node) {
    if (isLeaf(node)) {
        printf("dim: %d, val: %f, point_index: %d\n", node->dim, node->val, node->point->index);
    }
    else {
        printf("dim: %d, val: %f\n", node->dim, node->val);
    }
    if (node->left)
        printTree(node->left);
    if (node->right)
        printTree(node->right);
}

static bool initKDTreeTest() {
    SPPoint **points;
    int numOfPoints = 5;
    if ((points = (SPPoint **)malloc(sizeof(SPPoint *) * numOfPoints)) == NULL) return false;
    getInitTestPoints(points);
    KDTreeNode *expectedRoot;
    MALLOC_NODE(expectedRoot, false)
    getInitTestGoodKDTree(expectedRoot);
    KDTreeNode *root = initKDTree(points, numOfPoints);
    ASSERT_TRUE(compareKDNodes(root, expectedRoot));
    destroyKDTree(root);
    destroyKDTree(expectedRoot);
    return true;
}

int main() {
    RUN_TEST(initKDTreeTest);
    return 0;
}
