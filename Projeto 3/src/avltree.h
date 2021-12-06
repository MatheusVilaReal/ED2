#ifndef AVLTREE_H
#define AVLTREE_H

#include "types.h"

Tree newTree();

Node getRoot(Tree tree);

Node getNodeParent(Node node);

Node getLeftChild(Node node);

Node getRightChild(Node node);

Node rotateRight(Node node);

Node rotateLeft(Node node);

Node searchTree(Tree tree, double key);

Node getSmallestNode(Node node);

Data getNodeData(Node node);

double getNodeKey(Node node);

double getNodeMaxX(Node node);

double getNodeMinX(Node node);

unsigned long getBranchHeight(Node node);

unsigned long getNodeDepth(Node node);

unsigned long getNodeIndex(Node node);

int getBranchBalance(Node node);

bool isLeaf(Node node);

void delTreeNode(Tree tree, double key, void (*freeData)(Data));

void setNodeMaxX(Node node, double maxX);

void setNodeMinX(Node node, double minX);

void insertNode(Tree tree, Data data, double key, double maxX, double minX);

void printBranch(Node node, int space);

void printTree(Tree tree);

void chopBranch(Node node, void (*freeList)(List, FreeData_t), void (*freeData)(Data));

void chopTree(Tree tree, void (*freeList)(List, FreeData_t), void (*freeData)(Data));

#endif