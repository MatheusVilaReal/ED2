#ifndef DSET_H
#define DSET_H

#include "types.h"

typedef void* DisjointSet;

/* Aloca em memória e retorna um ponteiro para um conjunto disjunto de tamanho n. */
DisjointSet newDisjointSet(unsigned int n);

/* Retorna o antecessor mais alto do elemento índice index. */
unsigned int set_find(DisjointSet set, unsigned int index);

/* Une dois elementos sob o mesmo antecessor. */
void set_union(DisjointSet set, unsigned int a, unsigned int b);

/* Libera a memória utilizada por set. */
void set_free(DisjointSet set);

#endif