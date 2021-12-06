#ifndef CITY_H
#define CITY_H

#include "binarytree.h"
#include "hashtable.h"
#include "graph.h"
#include "types.h"

typedef void* City;

/* Aloca em memória e retorna um ponteiro para um objeto do tipo cidade. */
City newCity();

/* Retorna a árvore de quadras armazenada em city. Se city igual a NULO, retorna NULO. */
BinaryTree city_getBlocks(City city);

/* Retorna a árvore de esquinas armazenada em city. Se city igual a NULO, retorna NULO. */
BinaryTree city_getCrossings(City city);

/* Retorna o grafo de ruas armazenado em city. Se city igual a NULO, retorna NULO. */
Graph city_getStreets(City city);

/* Retorna a tabela de espalhamento de quadras armazenada em city. Se city igual a NULO, retorna 
NULO. */
HashTable city_getCeps(City city);

/* Retorna um conjunto solução, contendo o caminho mais curto calculado, armazenado em city. Se city
igual a NULO, retorna NULO. */
solution_t city_getShortestPath(City city);

/* Retorna um conjunto solução, contendo o caminho mais curto calculado, armazenado em city. Se city
igual a NULO, retorna NULO. */
solution_t city_getFastestPath(City city);

/* Retorna o CEP do registrador de origem r, armazenado em city. Se city
igual a NULO, retorna NULO. */
char* city_r_getCep(City city);

/* Retorna a coordenada x do registrador de origem r, armazenado em city. Se city
igual a NULO, retorna -1. */
double city_r_getX(City city);

/* Retorna a coordenada y do registrador de origem r, armazenado em city. Se city
igual a NULO, retorna -1. */
double city_r_getY(City city);

/* Retorna a coordenada x do endereço CEP/Side/Num, na cidade city. 
Se city igual a NULO, retorna -1. */
double city_getAddressX(City city, char* cep, char side, int num);

/* Retorna a coordenada y do endereço CEP/Side/Num, na cidade city. 
Se city igual a NULO, retorna -1. */
double city_getAddressY(City city, char* cep, char side, int num);

/* Retorna o número do registrador de origem r, armazenado em city. Se city
igual a NULO, retorna -1. */
int city_r_getNum(City city);

/* Retorna o lado do registrador de origem r, armazenado em city. Se city
igual a NULO, retorna -1. */
char city_r_getSide(City city);

/* Inicializa o grafo de ruas de city com um novo grafo contendo n vértices. Se city igual a NULO
nada é feito.*/
void city_setStreets(City city, unsigned int n);

/* Inicializa a tabela de espalhamento de quadras com uma nova tabela de tamanho n. Se city igual a
NULO, nada é feito. */
void city_setCeps(City city, unsigned int n);

/* Atribui os elementos cep, side e num ao registrador r contido em city. Se city igual a NULO,
nada é feito. */
void city_setR(City city, char* cep, char side, int num);

/* Atribui o caminho mais curto shortestPath ao caminho mais curto calculado, armazenado em city. 
Se city igual a NULO, nada é feito. */
void city_setShortestPath(City city, solution_t shortestPath);

/* Atribui o caminho mais rápido fastestPath ao caminho mais curto calculado, armazenado em city. 
Se city igual a NULO, nada é feito. */
void city_setFastestPath(City city, solution_t fastestPath);

/* Libera memória da estrutura city alocada em memória. */
void city_free(City city);

#endif