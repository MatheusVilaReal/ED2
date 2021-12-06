#include <stdlib.h>

#include "list.h"

/**************
*Parte privada*
**************/

typedef struct Node{

    Data data;

    struct Node* next;

} Node_t;

typedef struct List{

    Node_t* head;

    unsigned long size;

} List_t;

static void util_freeList(Node_t* node, FreeData_t freeData);

/**************
*Parte pública*
**************/

List newList(){

    List_t* list = (List_t*) malloc(sizeof(List_t));

    list->head = NULL;
    list->size = 0;

    return list;
}

List appendList(List list, Data data){

    if(list == NULL)
        return NULL;

    List_t* toAppend = (List_t*) list;
    Node_t* newNode = (Node_t*) malloc(sizeof(Node_t));
    Node_t* buf = toAppend->head;

    newNode->data = data;
    newNode->next = NULL;

    if(buf){
        while(buf->next)
            buf = buf->next;

        buf->next = newNode;
    
    } else
        toAppend->head = newNode;

    toAppend->size++;

    return toAppend;
}

List catList(List* list_1, List list_2){

    List_t** toAppend = (List_t**) list_1;
    List_t* toCat = (List_t*) list_2;
    Node_t* buffer = NULL;

    if(toAppend == NULL)
        *toAppend = newList();

    if(toCat)
        buffer = toCat->head;

    while(buffer){

        appendList(*toAppend, buffer->data);

        buffer = buffer->next;
    }

    return *toAppend;
}

Node searchListNode(List list, SearchFunction_t filter, char* cep){

    List_t* toSearch = (List_t*) list;

    Node_t* buf = toSearch->head;

    while(buf && !filter(buf->data, cep))
        buf = buf->next;

    return buf;
}

Node getHead(List list){

    if(list == NULL)
        return NULL;

    List_t* toGet = (List_t*) list;

    return toGet->head;
}

Node getNext(Node node){

    if(node == NULL)
        return NULL;

    Node_t* toGet = (Node_t*) node;

    return toGet->next;
}

Data getListNodeData(Node node){

    if(node == NULL)
        return NULL;

    Node_t* toGet = (Node_t*) node;

    return toGet->data;
}

unsigned long getListSize(List list){

    if(list == NULL)
        return 0;

    List_t* toGet = (List_t*) list;

    return toGet->size;
}

void deleteNode(List list, Data data, void (*freeData)(Data)){

    List_t* toSearch = (List_t*) list;

    Node_t* toDel = toSearch->head, *buf = NULL;

    if(toDel == NULL)
        return;

    while(toDel && toDel->data != data){

        buf = toDel;
        toDel = toDel->next;
    }

    if(toDel == NULL)
        return;

    if(buf)
        buf->next = toDel->next;
    else
        toSearch->head = toDel->next;

    freeData(toDel->data);
    free(toDel);

    toSearch->size--;

    if(toSearch->size == 0)
        toSearch->head = NULL;
}

void printList(List list, void (*printNode)(Data)){

    if(list == NULL)
        return;

    List_t* toPrint = (List_t*) list;

    Node_t* buf = toPrint->head;

    while(buf){

        printNode(buf->data);

        buf = buf->next;
    }
}

void freeList(List list, FreeData_t freeData){

    List_t* toFree = (List_t*) list;

    util_freeList(toFree->head, freeData);
    
    free(toFree);
}

/*************************************************************************************************/

static void util_freeList(Node_t* node, FreeData_t freeData){

    if(node == NULL)
        return;

    util_freeList(node->next, freeData);

    freeData(node->data);
    free(node);
}