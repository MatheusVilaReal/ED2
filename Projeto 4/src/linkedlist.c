#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"

/**************
*Parte privada*
**************/

typedef struct Node{

    Object o;

    struct Node* next;

} Node_t;

typedef struct List{

    Node_t* head;

    long size;

} List_t;

static Node_t* newNode(Object o);

static void freeNode(Node_t* node, Destroyer_t o_free);

static void util_freeList(Node_t* node, Destroyer_t o_free);

/**************
*Parte pública*
**************/

LinkedList newLinkedList(){

    List_t* list = (List_t*) malloc(sizeof(List_t));

    list->head = NULL;
    list->size = 0;

    return list;
}

LinkedList list_append(LinkedList list, Object o){

    if(list == NULL)
        return NULL;

    List_t* toAppend = (List_t*) list;
    Node_t* buffer = toAppend->head;

    if(buffer){

        while(buffer->next)
            buffer = buffer->next;

        buffer->next = newNode(o);
    
    } else
        toAppend->head = newNode(o);

    ++toAppend->size;

    return toAppend;
}

LinkedList list_prepend(LinkedList list, Object o){

    if(list == NULL)
        return NULL;

    List_t* toPrepend = (List_t*) list;
    Node_t* buffer = toPrepend->head;

    toPrepend->head = newNode(o);

    toPrepend->head->next = buffer;

    ++toPrepend->size;

    return toPrepend;
}

LinkedList list_add(LinkedList list, long index, Object o){

    if(list == NULL)
        return NULL;

    List_t* toAdd = (List_t*) list;
    Node_t* buffer = toAdd->head, 
          * buffer_2 = NULL;

    if(buffer && index < toAdd->size){
      
        --index;
      
        for(long i = 0; i < index; i++)
            buffer = buffer->next;

    } else
        return list;

    buffer_2 = buffer->next;
    buffer->next = newNode(o);

    buffer->next->next = buffer_2;

    --toAdd->size;

    return toAdd;
}

Iterator list_it_start(LinkedList list){

    if(list == NULL)
        return NULL;

    List_t* this = (List_t*) list;

    return this->head;
}

Iterator list_it_next(Node node){

    if(node == NULL)
        return NULL;

    Node_t* this = (Node_t*) node;

    return this->next;
}

Object* list_toArray(LinkedList list){

    if(list == NULL)
        return NULL;

    List_t* l = (List_t*) list;

    Object* array = (Object*) malloc(sizeof(Object) * (l->size + 1));
    unsigned long index = 0;

    for(Node_t* node = l->head; node != NULL; node = node->next)
        array[index++] = node->o;

    array[index] = NULL;

    return array;
}

Object list_it_getObject(Iterator it){

    if(it == NULL)
        return NULL;

    Node_t* this = (Node_t*) it;

    return this->o;
}

Object list_get(LinkedList list, long index){

    if(list == NULL)
        return NULL;

    List_t* toSearch = (List_t*) list;
    Node_t* node = NULL;

    if(index < toSearch->size){

        node = toSearch->head;

        for(long i = 0; i < index; i++)
            node = node->next;
    }

    return node->o;
}

long list_size(LinkedList list){

    if(list == NULL)
        return 0;

    List_t* this = (List_t*) list;

    return this->size;
}

long list_indexOf(LinkedList list, Object p, Compare_t equals){

    if(list == NULL)
        return -1;

    List_t* l = (List_t*) list;

    long index = 1;

    if(equals)    
        for(Node_t* node = l->head; node != NULL; node = node->next){

            if(equals(node->o, p))
                return -1 + index;

            ++index;
        }
    else
         for(Node_t* node = l->head; node != NULL; node = node->next){

            if(node->o == p)
                return -1 + index;

            ++index;
        }

    return -1;
}

bool list_contains(LinkedList list, Object d){

    if(list == NULL)
        return false;

    List_t* this = (List_t*) list;
    Node_t* node = this->head;

    while(node){
        if(node->o == d)
            return true;

        node = node->next;
    }

    return false;
}

void list_remove(LinkedList list, long index, Destroyer_t o_free){

    if(list == NULL)
        return;

    List_t* l = (List_t*) list;
    Node_t* toDel = l->head, *buffer = NULL;

    if(toDel && index < l->size)
        for(long i = 0; i < index; i++){

            buffer = toDel;
            toDel = toDel->next;
        }
    else
        return;

    if(toDel == NULL)
        return;

    if(buffer)
        buffer->next = toDel->next;
    else
        l->head = toDel->next;

    freeNode(toDel, o_free);

    l->size--;
}

void list_print(LinkedList list, Printer_t o_print){

    if(list == NULL)
        return;

    List_t* toPrint = (List_t*) list;
    Node_t* buffer = toPrint->head;

    while(buffer){

        if(o_print)
            o_print(buffer->o);
        else
            printf("Node at %p\n", (void*) buffer);

        buffer = buffer->next;
    }
}

void list_free(LinkedList list, Destroyer_t o_free){

    List_t* l = (List_t*) list;

    util_freeList(l->head, o_free);
    
    free(l);
}

/**************************************************************************************************/

static Node_t* newNode(Object o){

    Node_t* node = (Node_t*) malloc(sizeof(Node_t));

    node->next = NULL;
    node->o = o;

    return node;
}

static void util_freeList(Node_t* node, Destroyer_t o_free){

    if(node == NULL)
        return; 

    util_freeList(node->next, o_free);

    if(o_free)
        o_free(node->o);
        
    free(node);
}

static void freeNode(Node_t* node, Destroyer_t o_free){

    if(node == NULL)
        return;

    if(o_free)
        o_free(node->o);

    free(node);
}