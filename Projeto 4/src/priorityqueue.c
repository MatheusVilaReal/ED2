#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include "priorityqueue.h"

/**************
*Parte privada*
**************/

typedef struct Node{

    struct Node* next;
    
    Object o;

    double priority;

    long index;

} Node_t;

typedef struct PriorityQueue{

    Node_t* top;

    long size;

} Queue_t;

static Node_t* newNode(Object o, double priority);

static void freeNode(Node_t* node, Destroyer_t o_free);

static void util_freeQueue(Node_t* node, Destroyer_t o_free);

/**************
*Parte pública*
**************/

PriorityQueue newPriorityQueue(){

    Queue_t* queue = (Queue_t*) malloc(sizeof(Queue_t));

    queue->size = 0;

    queue->top = NULL;

    return queue;
}

PriorityQueue queue_push(PriorityQueue queue, Object o, double priority){

    if(queue == NULL)
        return NULL;

    Queue_t* q = (Queue_t*) queue;
    Node_t* node = q->top,
          * new = newNode(o, priority);
    
    new->index = q->size++;
    
    if(node){

        while(node->next)
            node = node->next;

        node->next = new;
    
    } else
        q->top = new;

    return q;
}

Iterator queue_it_start(PriorityQueue queue){

    if(queue == NULL)
        return NULL;

    Queue_t* q = (Queue_t*) queue;

    return q->top;
}

Iterator queue_it_next(Iterator it){

    if(it == NULL)
        return NULL;

    Node_t* node = (Node_t*) it;

    return node->next;
}

Object queue_pop(PriorityQueue queue){

    if(queue == NULL)
        return NULL;

    Queue_t* q = (Queue_t*) queue;

    Node_t* node = q->top,
          * toPop = q->top,
          * prev = NULL,
          * toPop_prev = NULL;

    double priority = DBL_MAX;

    while(node){

        if(node->priority < priority){

            toPop = node;
            toPop_prev = prev;
            priority = node->priority;
        }

        prev = node;
        node = node->next;
    }

    Object o = toPop->o;

    if(toPop == q->top)
        q->top = toPop->next;
    else
        toPop_prev->next = toPop->next;

    freeNode(toPop, NULL);

    --q->size;

    return o;
}

long queue_size(PriorityQueue queue){

    if(queue == NULL)
        return -1;

    Queue_t* q = (Queue_t*) queue;

    return q->size;
}

void queue_setPriority(PriorityQueue queue, long index, double priority){

    if(queue == NULL)
        return;

    Queue_t* q = (Queue_t*) queue;

    if(index >= q->size)
        return;

    Node_t* node = q->top;

    while(node->next && node->index < index)
        node = node->next;

    node->priority = priority;
}

void queue_print(PriorityQueue queue, Printer_t o_print){

    if(queue == NULL)
        return;

    Queue_t* q = (Queue_t*) queue;

    for(Node_t* node = q->top; node != NULL; node = node->next)
        if(o_print)
            o_print(node->o);
        else
            printf("[%ld:%lf] Node at %p\n", node->index, node->priority, (void*) node->o);
}

void queue_free(PriorityQueue queue, Destroyer_t o_free){

    if(queue == NULL)
        return;

    Queue_t* q = (Queue_t*) queue;

    util_freeQueue(q->top, o_free);

    free(q);
}

/**************************************************************************************************/

static Node_t* newNode(Object o, double priority){

    Node_t* node = (Node_t*) malloc(sizeof(Node_t));

    node->o = o;
    node->priority = priority;

    node->next = NULL;

    node->index = 0;

    return node;
}

static void freeNode(Node_t* node, Destroyer_t o_free){

    if(node == NULL)
        return;

    if(o_free)
        o_free(node->o);

    free(node);
}

static void util_freeQueue(Node_t* node, Destroyer_t o_free){

    if(node == NULL)
        return;

    util_freeQueue(node->next, o_free);

    freeNode(node, o_free);
}