#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include "types.h"

typedef void* PriorityQueue;

typedef double priority_t;

/* Aloca em memória e retorna um ponteiro para uma fila de prioridade de tamanho n. */
PriorityQueue newPriorityQueue();

/* Insere o objeto o na fila queue, com prioridade priority, e retorna queue.
Se queue igual a NULO, nada é feito e retorna NULO. */
PriorityQueue queue_push(PriorityQueue queue, Object o, priority_t priority);

/* Retorna um ponteiro para iterar sobre os elementos armazenados em queue. Se queue igual a NULO,
retorna NULO. */
Iterator queue_it_start(PriorityQueue queue);

/* Retorna o elemento seguinte ao apontado por it. Se igual a NULO, retorna NULO. */
Iterator queue_it_next(Iterator it);

/* Remove o elemento com menor prioridade de queue e o retorna. Se queue igual a NULO,
retorna NULO. */
Object queue_pop(PriorityQueue queue);

/* Retorna o tamanho da fila queue. */
long queue_size(PriorityQueue queue);

/* Altera a prioridade do elemento de índice index contido em queue para priority. */
void queue_setPriority(PriorityQueue queue, long index, priority_t priority);

/* Imprime a fila de prioridade queue, usando a função especial o_print para imprimir os objetos 
armazenados. Se o_print igual a NULO, uma impressão genérica é feita. */
void queue_print(PriorityQueue queue, Printer_t o_print);

/* Libera a memória utilizada por queue, e o_free libera os objetos armazenados em queue. Se
o_print igual a NULO, nada é feito. */
void queue_free(PriorityQueue queue, Destroyer_t o_free);

#endif