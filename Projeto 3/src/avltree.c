#include <stdio.h>
#include <stdlib.h>

#include "avltree.h"

#define SPACING 5

/**************
*Parte privada*
**************/

typedef struct Node{

    Data data;

    double key;
    double maxX, minX;

    unsigned long index;

    struct Node* parent;
    struct Node* left, *right;

} Node_t;

typedef struct Tree{

    Node_t* root;

    unsigned long size;

} Tree_t;

static Node_t* newAvlNode(Tree_t* tree, Data data, double key, double maxX, double minX);

static Node_t* util_insertNode(Tree_t* tree, Node_t* node, Data data, double key, double maxX, double minX);

static Node_t* util_delNode(Node_t* node, double key, void (*freeData)(Data));

static Node_t* util_searchTree(Node_t* node, double key);

static double getMaxX(double a, double b, double c);

static unsigned long max(unsigned long a, unsigned long b);

/**************
*Parte pública*
**************/

Tree newTree(){

    Tree_t* tree = (Tree_t*) malloc(sizeof(Tree_t));

    tree->size = 0;
    tree->root = NULL;

    return tree;
}

Node getRoot(Tree tree){

    if(tree == NULL)
        return NULL;

    Tree_t* toGet = (Tree_t*) tree;

    return (Node) toGet->root;
}

Node getNodeParent(Node node){

    if(node == NULL)
        return NULL;

    Node_t* toGetParent = (Node_t*) node;

    return (Node) toGetParent->parent;
}

Node getLeftChild(Node node){

    if(node == NULL)
        return NULL;

    Node_t* toGetLeft = (Node_t*) node;

    return (Node) toGetLeft->left;    
}

Node getRightChild(Node node){

    if(node == NULL)
        return NULL;

    Node_t* toGetRight = (Node_t*) node;

    return (Node) toGetRight->right;    
}

Node rotateRight(Node node){

    Node_t* first = (Node_t*) node;
    Node_t* second = first->left;
    Node_t* third = second->right;

    double toUpdateMaxL = 0.0,
           toUpdateMaxR = 0.0;

    second->parent = first->parent;

    second->right = first;
    first->parent = second;

    first->left = third;

    if(third){

        third->parent = first;

        toUpdateMaxL = third->maxX;

        first->minX = third->minX;
    }

    if(first->right)
        toUpdateMaxR = first->right->maxX;

    first->maxX = getMaxX(first->maxX, toUpdateMaxL, toUpdateMaxR);

    toUpdateMaxL = 0.0;
    toUpdateMaxR = 0.0;

    second->minX = first->minX;

    if(second->left)
        toUpdateMaxL = second->left->maxX;

    second->maxX = getMaxX(second->maxX, toUpdateMaxL, first->maxX);

    return (Node) second;
}

Node rotateLeft(Node node){

    Node_t* first = (Node_t*) node;
    Node_t* second = first->right;
    Node_t* third = second->left;

    double toUpdateMaxL = 0,
           toUpdateMaxR = 0;

    second->parent = first->parent;

    second->left = first;
    first->parent = second;

    first->right = third;

    if(third){

        third->parent = first;

        toUpdateMaxR = third->maxX;
    }

    if(first->left){

        toUpdateMaxL = first->left->maxX;

        first->minX = first->left->minX;
    }

    first->maxX = getMaxX(first->maxX, toUpdateMaxL, toUpdateMaxR);

    toUpdateMaxL = 0.0;
    toUpdateMaxR = 0.0;

    if(second->right)
        toUpdateMaxR = second->right->maxX;

    second->minX = first->minX;

    second->maxX = getMaxX(second->maxX, first->maxX, toUpdateMaxR);

    return (Node) second;
}

Node searchTree(Tree tree, double key){

    Tree_t* toSearch = (Tree_t*) tree;

    return util_searchTree(toSearch->root, key);
}

Node getSmallestNode(Node node){

    if(node == NULL)
        return NULL;

    Node_t* smallest = (Node_t*) node;

    while(smallest->left)
        smallest = smallest->left;

    return smallest;
}

Data getNodeData(Node node){

    if(node == NULL)
        return NULL;

    Node_t* toGet = (Node_t*) node;

    return toGet->data;
}

double getNodeKey(Node node){

    if(node == NULL)
        return 0;

    Node_t* toGet = (Node_t*) node;

    return toGet->key;
}

double getNodeMaxX(Node node){

    if(node == NULL)
        return 0;

    Node_t* toGet = (Node_t*) node;

    return toGet->maxX;
}

double getNodeMinX(Node node){

    if(node == NULL)
        return 0;

    Node_t* toGet = (Node_t*) node;

    return toGet->minX;
}

unsigned long getBranchHeight(Node node){

    if(node == NULL)
        return 0;

    Node_t* toGet = (Node_t*) node;

    unsigned long leftHeight = getBranchHeight(toGet->left);
    unsigned long rightHeight = getBranchHeight(toGet->right);

    return max(leftHeight, rightHeight) + 1;
}

unsigned long getNodeIndex(Node node){

    if(node == NULL)
        return __LONG_MAX__;

    Node_t* toGet = (Node_t*) node;
    
    return toGet->index;
}

int getBranchBalance(Node node){

    if(node == NULL)
        return 0;

    Node_t* toGet = (Node_t*) node;

    return getBranchHeight(toGet->left) - getBranchHeight(toGet->right);
}

bool isLeaf(Node node){

    if(node == NULL)
        return false;

    Node_t* toLeaf = (Node_t*) node;

    return !(toLeaf->left && toLeaf->right);
}

void setNodeMaxX(Node node, double maxX){

    if(node == NULL)
        return;

    Node_t* toSet = (Node_t*) node;

    toSet->maxX = maxX;
}

void setNodeMinX(Node node, double minX){

    if(node == NULL)
        return;

    Node_t* toSet = (Node_t*) node;

    toSet->minX = minX;
}

void insertNode(Tree tree, Data data, double key, double maxX, double minX){

    Tree_t* toInsert = (Tree_t*) tree;

    toInsert->root = util_insertNode(tree, toInsert->root, data, key, maxX, minX);
}

void delTreeNode(Tree tree, double key, void (*freeData)(Data)){

    Tree_t* toInsert = (Tree_t*) tree;

    toInsert->root = util_delNode(toInsert->root, key, freeData);
}

void printBranch(Node node, int space){

    if(node == NULL)
        return;

    space += SPACING;

    Node_t* toPrint = (Node_t*) node;

    printBranch(toPrint->left, space);
    printBranch(toPrint->right, space);

    printf("[minX = %.2lf; maxX = %.2lf]\n", toPrint->minX, toPrint->maxX);
}

void printTree(Tree tree){

    printBranch(getRoot(tree), 0);

    putchar('\n');
}

void chopBranch(Node node, void (*freeList)(List, FreeData_t), void (*freeData)(Data)){

    if(node == NULL)
        return;

    chopBranch(getLeftChild(node), freeList, freeData);
    chopBranch(getRightChild(node), freeList, freeData);

    freeList(getNodeData(node), freeData);
    free(node);
}

void chopTree(Tree tree, void (*freeList)(List, FreeData_t), void (*freeData)(Data)){

    chopBranch(getRoot(tree), freeList, freeData);

    free(tree);
}

/*************************************************************************************************/

static Node_t* newAvlNode(Tree_t* tree, Data data, double key, double maxX, double minX){

    Node_t* node = (Node_t*) malloc(sizeof(Node_t));

    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;

    node->data = data;

    node->index = tree->size++;

    node->key = key;
    node->maxX = maxX;
    node->minX = minX;

    return (Node) node;
}

static Node_t* util_insertNode(Tree_t* tree, Node_t* node, Data data, double key, double maxX, double minX){

    double toUpdateMaxL = 0, toUpdateMaxR = 0;

    if(node == NULL)
        return newAvlNode(tree, data, key, maxX, minX);

    if(key < node->key){

        node->left = util_insertNode(tree, node->left, data, key, maxX, minX);
        node->left->parent = node;
        node->minX = node->left->minX;

    } else if(key > node->key){
        
            node->right = util_insertNode(tree, node->right, data, key, maxX, minX);
            node->right->parent = node;

        } else return node;

    if(node->left)
        toUpdateMaxL = node->left->maxX;

    if(node->right)
        toUpdateMaxR = node->right->maxX;

    node->maxX = getMaxX(node->maxX, toUpdateMaxL, toUpdateMaxR);

    int balance = getBranchBalance(node);

    if(balance > 1 && key < node->left->key)
        return rotateRight(node);

    if(balance < -1 && key > node->right->key)
        return rotateLeft(node);

    if(balance > 1 && key > node->left->key){  

        node->left = rotateLeft(node->left);

        return rotateRight(node);
    }

    if(balance < -1 && key < node->right->key){ 

        node->right = rotateRight(node->right);

        return rotateLeft(node);
    }

    return node;
}

static Node_t* util_delNode(Node_t* node, double key, void (*freeData)(Data)){

    if(node == NULL)
        return node;

    if(key < node->key)
        node->left = util_delNode(node->left, key, freeData);
    else
        if(key > node->key)
            node->right = util_delNode(node->right, key, freeData);
        else{

            if(node->left == NULL || node->right == NULL){

                Node_t* buffer = (node->right)? node->right : node->left;

                if(buffer == NULL){

                    buffer = node;
                    node = NULL;
                
                } else
                    *node = *buffer;

                freeData(buffer->data);
                free(buffer);
            
            } else{

                Node_t* buffer = getSmallestNode(node->right);

                *node = *buffer;

                node->right = util_delNode(node->right, buffer->key, freeData);
            }
        }

    if(node == NULL)
        return node;

    int balance = getBranchBalance(node);

    if(balance > 1 && getBranchBalance(node->left) >= 0)
        return rotateRight(node);
 
    if(balance > 1 && getBranchBalance(node->left) < 0){

        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
 
    if(balance < -1 && getBranchBalance(node->right) <= 0)
        return rotateLeft(node);
 
    if(balance < -1 && getBranchBalance(node->right) > 0){

        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

static Node_t* util_searchTree(Node_t* node, double key){

    if(node == NULL)
        return NULL;

    if(node->key == key)
        return node;
    else
        if(key < node->key)
            return util_searchTree(node->left, key);
        else
            return util_searchTree(node->right, key);
}

static double getMaxX(double a, double b, double c){

    double largest = (a > c)? a : c;

    return (largest > b)? largest : b ;
}

static unsigned long max(unsigned long a, unsigned long b){

    return (a > b)? a : b;
}