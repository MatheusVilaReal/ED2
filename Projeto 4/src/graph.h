#ifndef GRAPH_H
#define GRAPH_H

#include "types.h"

/* Status de visitação. */
typedef enum Status{
    
    OPEN,
    VISITED,
    CLOSED

} Status_t;

/* Opções de peso. */
enum WeightOption{

    CMP,
    VM
};

/* Tipos de referência. */
typedef void* Graph,
            * Vertex,
            * Edge;

typedef double weight_t;

typedef char** solution_t;

/* Aloca em memória e retorna um ponteiro para um novo objeto grafo. */
Graph newGraph(unsigned int n);

/* Aloca em memória e retorna um ponteiro para o grafo transposto de graph. Se graph igual a NULO 
retorna NULO. */
Graph graph_transpose(Graph graph);

/* Retorna um iterador para o grafo graph. Se graph igual a NULO, retorna NULO. */
Iterator graph_it_start(Graph graph);

/* Retorna o próximo elemento do iterador it. Se graph ou it nulos, retorna NULO. */
Iterator graph_it_next(Graph graph, Iterator it);

/* Retorna o vértice armazenado em graph de id id. Se não encontrado retorna NULO. */
Vertex graph_get(Graph graph, char* id);

/* Retorna o vértice apontado pelo iterador it. */
Vertex graph_it_getVertex(Iterator it);

/* Adiciona um novo vértice em graph. Se graph igual a NULO, nada é feito. */
Vertex graph_addVertex(Graph graph, char* id, double x, double y);

/* Retorna um vetor de tamanho dinâmico, terminado em NULO, contendo todo as arestas do grafo 
graph. */
Edge* graph_edgesToArray(Graph graph);

/* Aloca em memória e retorna um ponteiro para a árvore geradora mínima do grafo graph. */
Graph graph_minSpanningTree(Graph graph);

/* Retorna o id do vértice apontado pelo iterador it. Se it igual a NULO, retorna NULO. */
char* graph_it_getId(Iterator it);

/* Retorna o id do vértice de destino da aresta apontado pelo iterador it. Se it igual a NULO, 
retorna NULO. */
char* graph_edge_getVertex(Iterator it);

/* Retorna o id do vértice de origem da aresta apontado pelo iterador it. Se it igual a NULO, 
retorna NULO. */
char* graph_edge_getOrigin(Iterator it);

/* Retorna o nome da aresta apontado pelo iterador it. Se it igual a NULO, retorna NULO. */
char* graph_edge_getName(Iterator it);

/* Retorna o atributo ldir da aresta apontado pelo iterador it. Se it igual a NULO, 
retorna NULO. */
char* graph_edge_getLdir(Iterator it);

/* Retorna o atributo lesq da aresta apontado pelo iterador it. Se it igual a NULO, 
retorna NULO. */
char* graph_edge_getLesq(Iterator it);

/* Retorna o status de visita do vértice apontado pelo iterador it. Se it igual a NULO, 
retorna CLOSED. */
Status_t graph_it_getVisitStatus(Iterator it);

/* Retorna o atributo comprimento da aresta apontado pelo iterador it. Se it igual a NULO, 
retorna -1. */
weight_t graph_edge_getCmp(Iterator it);

/* Retorna o atributo velocidade média da aresta apontado pelo iterador it. Se it igual a NULO, 
retorna -1. */
weight_t graph_edge_getVm(Iterator it);

/* Retorna a coordenada x do vértice apontado pelo iterador it. Se it igual a NULO, 
retorna -1. */
double graph_it_getX(Iterator it);

/* Retorna a coordenada y do vértice apontado pelo iterador it. Se it igual a NULO, 
retorna -1. */
double graph_it_getY(Iterator it);

/* Executa o algoritmo de Dijkstra em graph. Retorna o conjunto dos antecessores mais próximos
de cada vértice a partir de s. Para escolher qual peso usar, weight_choice = CMP ou VM. */
solution_t graph_dijkstra(Graph graph, char* s, int weight_choice);

/* Retorna o índice de inserção do vértice apontado pelo iterador it. Se it igual a NULO, 
retorna 0. */
unsigned int graph_it_getIndex(Iterator it);

/* Retorna o tamanho do grafo graph. Se it igual a NULO, 
retorna 0. */
unsigned int graph_size(Graph graph);

/* Retorna o número de arestas do grafo graph. Se it igual a NULO, 
retorna 0. */
unsigned int graph_getNumEdges(Graph graph);

/* Retorna VERDADEIRO se i e j forem vértices adjacentes. Caso contrário, retorna FALSO. */
bool graph_areAdjacent(Graph graph, char* i, char* j);

/* Insere um novo vértice em graph. */
void graph_addEdge(Graph graph, char* i, char* j, char* name, char* ldir, char* lesq, weight_t cmp,
    weight_t vm);

/* Define o status de visita do vértice apontado por it para status. */
void graph_it_setVisitStatus(Iterator it, Status_t status);

/* Remove o vértice de id id do grafo graph. */
void graph_removeVertex(Graph graph, char* id);

/* Busca em profundidade atualizando so valores de vm de acordo com parâmetro f. */
void graph_DFS_updateF(Graph graph, double f);

/* Busca em largura. A função filter_fn é executada em todos os vértices percorridos. */
void graph_BFS(Graph graph, Filter_t filter_fn);

/* Imprime o grafo graph. */
void graph_print(Graph graph);

/* Libera memória das estruturas de graph. */
void graph_free(Graph graph);

#ifdef LIST_H

/* Retorna a lista de adjacências do vértice apontado por it. */
void* graph_it_getAdjacencies(Iterator it);

/* Retorna a lista de adjacências do vértice de id i. */
void* graph_getAdjcencies(Graph graph, char* i);

/* Realiza busca em profundidade em graph. */
void* graph_DFS(Graph graph);

#endif

#endif