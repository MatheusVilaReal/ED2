#ifndef LIST_H
#define LIST_H

#include "types.h"

List newList();

unsigned long getListSize(List list);

Node searchListNode(List list, SearchFunction_t filter, char* cep);

Node getHead(List list);

Node getNext(Node node);

List appendList(List list, Data data);

List catList(List* list_1, List list_2);

Data getListNodeData(Node node);

void deleteNode(List list, Data data, void (*freeData)(Data));

void printList(List list, void (*printNode)(Data));

void freeList(List list, void (*freeData)(Data));

#endif