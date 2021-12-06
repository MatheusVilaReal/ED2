#include <stdio.h>
#include <stdlib.h>

#include "binarytree.h"

#define SPACING 5

/**************
*Parte privada*
**************/

typedef struct Node{

    LinkedList objects;

    treekey_t key;

    double maxX, 
           minX;

    struct Node* parent,
               * left,
               * right;

} Node_t;

typedef struct Tree{

    Node_t* root;

    unsigned long size;

} Tree_t;

static Node_t* newNode(Object o, treekey_t key, double maxX, double minX);

static Node_t* nodeAppend(Node_t* node, Object o);

static Node_t* rotateLeft(Node_t* first);

static Node_t* rotateRight(Node_t* first);

static Node_t* getSmallestNode(Node_t* node);

static Node_t* util_add(Tree_t* tree, Node_t* node, Object o, treekey_t key, double maxX, double minX);

static Node_t* util_remove(Node_t* node, treekey_t key, Destroyer_t o_free);

static LinkedList util_get(Node_t*, treekey_t key);

static void util_free(Node_t* node, Destroyer_t o_free);

static void util_printTree(Node_t* node, int space);

static void freeNode(Node_t* node, Destroyer_t o_free);

static double getMaxX(double a, double b, double c);

static unsigned long max(unsigned long a, unsigned long b);

/**************
*Parte pública*
**************/

BinaryTree newBinaryTree(){

    Tree_t* tree = (Tree_t*) malloc(sizeof(Tree_t));

    tree->size = 0;
    tree->root = NULL;

    return tree;
}

Iterator tree_it_start(BinaryTree tree){

    if(tree == NULL)
        return NULL;

    Tree_t* t = (Tree_t*) tree;

    return t->root;
}

Iterator tree_it_parent(Iterator it){

    if(it == NULL)
        return NULL;

    Node_t* nodeParent = (Node_t*) it;

    return nodeParent->parent;
}

Iterator tree_it_nextLeft(Iterator it){

    if(it == NULL)
        return NULL;

    Node_t* node = (Node_t*) it;

    return node->left;    
}

Iterator tree_it_nextRight(Iterator it){

    if(it == NULL)
        return NULL;

    Node_t* node = (Node_t*) it;

    return node->right;    
}

LinkedList tree_it_getObjects(Iterator it){

    if(it == NULL)
        return NULL;

    Node_t* node = (Node_t*) it;

    return node->objects;
}

LinkedList tree_get(BinaryTree tree, treekey_t key){

    if(tree == NULL)
        return NULL;

    Tree_t* t = (Tree_t*) tree;

    return util_get(t->root, key);
}

Object* tree_getArray(BinaryTree tree, treekey_t key){

    if(tree == NULL)
        return NULL;

    Tree_t* t = (Tree_t*) tree;

    LinkedList objects = util_get(t->root, key);

    return list_toArray(objects);
}

treekey_t tree_it_getKey(Iterator it){

    if(it == NULL)
        return 0;

    Node_t* node = (Node_t*) it;

    return node->key;
}

double tree_it_getMaxX(Iterator it){

    if(it == NULL)
        return 0;

    Node_t* node = (Node_t*) it;

    return node->maxX;
}

double tree_it_getMinX(Iterator it){

    if(it == NULL)
        return 0;

    Node_t* node = (Node_t*) it;

    return node->minX;
}

unsigned long tree_it_getHeight(Iterator it){

    if(it == NULL)
        return 0;

    Node_t* node = (Node_t*) it;

    unsigned long leftHeight = tree_it_getHeight(node->left);
    unsigned long rightHeight = tree_it_getHeight(node->right);

    return max(leftHeight, rightHeight) + 1;
}

int tree_it_getBalance(Iterator it){

    if(it == NULL)
        return 0;

    Node_t* node = (Node_t*) it;

    return tree_it_getHeight(node->left) - tree_it_getHeight(node->right);
}

bool tree_it_isLeaf(Iterator it){

    if(it == NULL)
        return false;

    Node_t* toLeaf = (Node_t*) it;

    return !(toLeaf->left && toLeaf->right);
}

void tree_it_setMaxX(Iterator it, double maxX){

    if(it == NULL)
        return;

    Node_t* toSet = (Node_t*) it;

    toSet->maxX = maxX;
}

void tree_it_setMinX(Iterator it, double minX){

    if(it == NULL)
        return;

    Node_t* toSet = (Node_t*) it;

    toSet->minX = minX;
}

void tree_add(BinaryTree tree, Object o, treekey_t key, double maxX, double minX){

    Tree_t* toInsert = (Tree_t*) tree;

    toInsert->root = util_add(tree, toInsert->root, o, key, maxX, minX);
}

void tree_remove(BinaryTree tree, treekey_t key, Destroyer_t o_free){

    Tree_t* toInsert = (Tree_t*) tree;

    toInsert->root = util_remove(toInsert->root, key, o_free);
}

void tree_print(BinaryTree tree){

    if(tree == NULL)
        return;

    Tree_t* t = (Tree_t*) tree;

    util_printTree(t->root, 0);

    putchar('\n');
}

void tree_free(BinaryTree tree, Destroyer_t o_free){

    if(tree == NULL)
        return;

    Tree_t* t = (Tree_t*) tree;

    util_free(t->root, o_free);

    free(t);
}

/*************************************************************************************************/

static Node_t* newNode(Object o, treekey_t key, double maxX, double minX){

    Node_t* node = (Node_t*) malloc(sizeof(Node_t));

    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;

    node->objects = newLinkedList();
    list_append(node->objects, o);

    node->key = key;
    node->maxX = maxX;
    node->minX = minX;

    return node;
}

static Node_t* nodeAppend(Node_t* node, Object o){

    if(node)
        list_append(node->objects, o);

    return node;
}

static Node_t* rotateRight(Node_t* first){

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

    return second;
}

static Node_t* rotateLeft(Node_t* first){

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

    return second;
}

static Node_t* getSmallestNode(Node_t* node){

    if(node == NULL)
        return NULL;

    while(node->left)
        node = node->left;

    return node;
}

static Node_t* util_add(Tree_t* tree, Node_t* node, Object o, treekey_t key, double maxX, double minX){

    double toUpdateMaxL = 0, toUpdateMaxR = 0;

    if(node == NULL)
        return newNode(o, key, maxX, minX);

    if(key < node->key){

        node->left = util_add(tree, node->left, o, key, maxX, minX);
        node->left->parent = node;
        node->minX = node->left->minX;

    } else if(key > node->key){
        
            node->right = util_add(tree, node->right, o, key, maxX, minX);
            node->right->parent = node;

        } else return nodeAppend(node, o);

    if(node->left)
        toUpdateMaxL = node->left->maxX;

    if(node->right)
        toUpdateMaxR = node->right->maxX;

    node->maxX = getMaxX(node->maxX, toUpdateMaxL, toUpdateMaxR);

    int balance = tree_it_getBalance(node);

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

static Node_t* util_remove(Node_t* node, treekey_t key, Destroyer_t o_free){

    if(node == NULL)
        return node;

    if(key < node->key)
        node->left = util_remove(node->left, key, o_free);
    else
        if(key > node->key)
            node->right = util_remove(node->right, key, o_free);
        else{

            if(node->left == NULL || node->right == NULL){

                Node_t* buffer = (node->right)? node->right : node->left;

                if(buffer == NULL){

                    buffer = node;
                    node = NULL;
                
                } else
                    *node = *buffer;

                freeNode(buffer, o_free);
            
            } else{

                Node_t* buffer = getSmallestNode(node->right);

                *node = *buffer;

                node->right = util_remove(node->right, buffer->key, o_free);
            }
        }

    if(node == NULL)
        return node;

    int balance = tree_it_getBalance(node);

    if(balance > 1 && tree_it_getBalance(node->left) >= 0)
        return rotateRight(node);
 
    if(balance > 1 && tree_it_getBalance(node->left) < 0){

        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
 
    if(balance < -1 && tree_it_getBalance(node->right) <= 0)
        return rotateLeft(node);
 
    if(balance < -1 && tree_it_getBalance(node->right) > 0){

        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

static LinkedList util_get(Node_t* node, treekey_t key){

    if(node == NULL)
        return NULL;

    if(node->key == key)
        return node->objects;
    else
        if(key < node->key)
            return util_get(node->left, key);
        else
            return util_get(node->right, key);
}

static void util_free(Node_t* node, Destroyer_t o_free){

    if(node == NULL)
        return;

    util_free(node->left, o_free);
    util_free(node->right, o_free);

    freeNode(node, o_free);
}

static void util_printTree(Node_t* node, int space){

    if(node == NULL)
        return;

    space += SPACING;

    util_printTree(node->left, space);
    util_printTree(node->right, space);

    printf("[minX = %.2lf; maxX = %.2lf]\n", node->minX, node->maxX);
}

static void freeNode(Node_t* node, Destroyer_t o_free){

    if(node == NULL)
        return;

    list_free(node->objects, o_free);
    free(node);
}

static double getMaxX(double a, double b, double c){

    double largest = (a > c)? a : c;

    return (largest > b)? largest : b ;
}

static unsigned long max(unsigned long a, unsigned long b){

    return (a > b)? a : b;
}