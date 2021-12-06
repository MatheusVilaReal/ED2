#ifndef LIST_H
#define LIST_H

#include "types.h"

typedef void* LinkedList;

/* Aloca em memória e retorna um ponteiro para uma lista ligada. */
LinkedList newLinkedList();

/* Insere o objeto o no final da lista list e retorna list. Se list igual a NULO,
nada é feito e retorna NULO.*/
LinkedList list_append(LinkedList list, Object o);

/* Insere o objeto o no início da lista list e retorna list. Se list igual a NULO,
nada é feito e retorna NULO.*/
LinkedList list_prepend(LinkedList list, Object o);

/* Insere o objeto na lista list de modo que seja o elemento de número index da lista,
e retorna list. Se list igual a NULO, nada é feito e retorna NULO.*/
LinkedList list_add(LinkedList list, long index, Object o);

/* Retorna um ponteiro para iterar sobre os elementos armazenados em list. Se list igual a NULO,
retorna NULO. */
Iterator list_it_start(LinkedList list);

/* Retorna o elemento seguinte ao apontado por it. Se igual a NULO, retorna NULO. */
Iterator list_it_next(Iterator it);

/* Retorna um vetor de tamanho variável terminado em NULO contendo todos os elementos da lista
list. Se list igual a NULO, retorna NULO. */
Object* list_toArray(LinkedList list);

/* Retorna o objeto armazenado no elemento apontado por it. Se it igual a NULO, retorna NULO. */
Object list_it_getObject(Iterator it);

/* Retorna o objeto de índice index armazenado em list. Se list igual a NULO e index inválido,
nada é feito e retorna NULO. */
Object list_get(LinkedList list, long index);

/* Retorna o tamanho da lista list. */
long list_size(LinkedList list);

/* Percorre a lista comparando o objeto p com os objetos armazenados na lista, utilizando a função
equals. Quando equals retornar VERDADEIRO, a função retorna o índice do objeto encontrado. Se list,
object ou p iguais a NULO, nada é feito e retorna NULO. */
long list_indexOf(LinkedList list, Object p, Compare_t equals);

/* Percorre a lista comparando o ponteiro d com os objetos armazenados em list. Se d contido em
list, retorna VERDADEIRO. Caso contrário, retorna FALSO. */
bool list_contains(LinkedList list, Object d);

/* Remove o elemento de índice index da lista list. A função o_free realiza a liberação de memória.
Se o_free igual a NULO, não é liberado memória. */
void list_remove(LinkedList list, long index, Destroyer_t o_free);

/* Imprime a lista lista, usando a função especial o_print para imprimir os objetos armazenados.
Se o_print igual a NULO, uma impressão genérica é feita. */
void list_print(LinkedList list, Printer_t o_print);

/* Libera a memória utilizada por list, e o_free libera os objetos armazenados em table. Se
o_print igual a NULO, nada é feito. */
void list_free(LinkedList list, Destroyer_t o_free);

#endif