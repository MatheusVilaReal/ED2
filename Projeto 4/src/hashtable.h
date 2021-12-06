#ifndef HASHTABLE_H
#define HASHTABLE_H

#define TABLE_SIZE 100U

#include "types.h"

typedef void* HashTable;

/* Aloca em memória e retorna um ponteiro para uma hashtable de tamanho n. */
HashTable newHashTable(unsigned int n);

/* Retorna um ponteiro para iterar sobre os elementos armazenados em table. Se table igual a NULO,
retorna NULO. */
Iterator hashtable_it_start(HashTable table);

/* Retorna o elemento seguinte ao apontado por iterator. Se igual a NULO, retorna NULO. */
Iterator hashtable_it_next(HashTable table, Iterator iterator);

/* Retorna um vetor de tamanho variável terminado em NULO contendo todos os elementos da hashtable
table. Se table igual a NULO, retorna NULO. */
Object* hashtable_toArray(HashTable table);

/* Retorna o objeto armazenado no elemento apontado por it. Se it igual a NULO, retorna NULO. */
Object hashtable_it_getObject(Iterator it);

/* Retorna o primeiro objeto encontrado em key com a chave key. Se não for possível concluir a
busca, retorna NULO. */
Object hashtable_get(HashTable table, char* key);

/* Retorna a chave associada ao elemento node. Se node igual a NULO, retorna NULO. */
char* hashtable_getKey(Node node);

/* Retorna o tamanho da hashtable table. */
unsigned int hashtable_size(HashTable table);

/* Função de espalhamento. Calcula um valor de hash para a string key, limitado pelo tamanho
size. */
unsigned int hashCode(char* key, unsigned long size);

/* Insere o objeto o na hashtable table, associado à chave key. */
void hashtable_add(HashTable table, char* key, Object o);

/* Remove a primeira ocorrência encontrada de key na hashtable table. A função o_free libera memória
utilizada pelo objeto. Se o_free igual a NULO, nada é feito. */
void hashtable_remove(HashTable table, char* key, Destroyer_t o_free);

/* Imprime a hashtable table, usando a função especial o_print para imprimir os objetos armazenados.
Se o_print igual a NULO, uma impressão genérica é feita. */
void hashtable_print(HashTable table, Printer_t o_print);

/* Libera a memória utilizada por table, e o_free libera os objetos armazenados em table. Se
o_print igual a NULO, nada é feito. */
void hashtable_free(HashTable table, Destroyer_t o_free);

#endif