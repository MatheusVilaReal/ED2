#ifndef BINTREE_H
#define BINTREE_H

#include "types.h"
#include "linkedlist.h"

typedef double treekey_t;
typedef void* BinaryTree;

/* Aloca e retorna um ponteiro para uma nova árvore binária do tipo AVL. */
BinaryTree newBinaryTree();

/* Retorna um iterador para todos os nós da ávore, começando na raiz. */
Iterator tree_it_start(BinaryTree tree);

/* Retorna o nó pai do iterador it. Se it for NULO, retorna NULO. */
Iterator tree_it_parent(Iterator it);

/* Retorna o filho esquerdo do iterador it. Se it for NULO, retorna NULO. */
Iterator tree_it_nextLeft(Iterator it);

/* Retorna o filho direito do iterador it. Se it for NULO, retorna NULO. */
Iterator tree_it_nextRight(Iterator it);

/* Retorna a chave de identificação "key" do nó indicado por it. Se it for, retorna 0. */
treekey_t tree_it_getKey(Iterator it);

/* Retorna o parâmetro de identificação max_X do nó indicado por it, que representa o maior valor de
X encontrado na sub-árvore com raiz em it. Se it for, retorna NULO. */
double tree_it_getMaxX(Iterator it);

/* Retorna o parâmetro de identificação min_X do nó indicado por it, que representa o menor valor de
X encontrado na sub-árvore com raiz em it. Se it for, retorna NULO. */
double tree_it_getMinX(Iterator it);

/* Retorna a altura do nó indicado por it. Se for NULO, retorna 0. */
unsigned long tree_it_getHeight(Iterator it);

/* Calcula o fator de balanceamento da sub-árvore com raiz em it. Se it for NULO, retorna 0. */
int tree_it_getBalance(Iterator it);

/* Retorna VERDADEIRO se o nó indicado por it for uma folha, se não, retorna FALSO. Se it for NULO,
retorna FALSO. */
bool tree_it_isLeaf(Iterator it);

/* Atribui maxX para o parâmetro max_X encontrado em it. Se it igual a NULO, nada é feito.*/
void tree_it_setMaxX(Iterator it, double maxX);

/* Atribui minX para o parâmetro min_X encontrado em it. Se it igual a NULO, nada é feito.*/
void tree_it_setMinX(Iterator it, double minX);

/* Insere o objeto o, indicado pela chave key, na árvore binária tree, juntamente com os parâmetros
maxX e minX. */
void tree_add(BinaryTree tree, Object o, treekey_t key, double maxX, double minX);

/* Remove o nó indicado por key da árvore tree, enquanto a função o_free realiza os procedimentos
para liberar memória. Se o_free igual a NULO, não é liberado memória. */
void tree_remove(BinaryTree tree, treekey_t key, Destroyer_t o_free);

/* Imprime a árvore binária tree. */
void tree_print(BinaryTree tree);

/* Destrói a árvore binária apontada por tree,enquanto a função o_free realiza os procedimentos
para liberar memória. Se o_free igual a NULO, não é liberado memória.  */
void tree_free(BinaryTree tree, Destroyer_t o_free);

#ifdef LIST_H

/* Retorna uma lista contendo todos os objetos de mesma chave armazenados no respectivo nó da árvore
tree. Se linkedlist.h não incluso, a função não é definida. */
LinkedList tree_get(BinaryTree tree, treekey_t key);

/* Retorna uma lista contendo todos os objetos armazenados no nó indicado pelo iterador it na árvore
tree. Se linkedlist.h não incluso, a função não é definida. */
LinkedList tree_it_getObjects(Iterator it);

#endif

/* Retorna um array de tamanho variável, terminado em NULO, contendo todos os objetos armazenadosno
respectivo nó da árvore tree. */
Object* tree_getArray(BinaryTree tree, treekey_t key);

#endif