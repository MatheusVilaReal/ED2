#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "disjointset.h"
#include "hashtable.h"
#include "linkedlist.h"
#include "graph.h"
#include "rectmath.h"
#include "priorityqueue.h"

#define INFINITY DBL_MAX / 2
#define NONE NULL

/**************
*Parte privada*
**************/

typedef struct Vertex{

    LinkedList /*<Edge>*/ adjList;

    char* id;

    double x,
           y;

    Status_t status;

    unsigned int index;

} Vertex_t;

typedef struct Edge{

    char* destination,
        * origin,
        * name,
        * ldir,
        * lesq;

    weight_t vm,
             cmp;

} Edge_t;

typedef struct Graph{

    HashTable vertices_h;
    Vertex_t** vertices_a;

    unsigned int numVertices,
                 numEdges,
                 v_size;

} Graph_t;

static Vertex_t* newVertex(char* id, double x, double y);

static Edge_t* newEdge(char* vertex, char* origin, char* name, char* ldir, char* lesq, weight_t cmp,
    weight_t vm);

static int compareEdges(const void* edge_1, const void* edge_2);

static bool edge_compareVertices(Object edge_1, Object edge_2);

static void initDijkstra(Graph_t* g, PriorityQueue q, char* s, weight_t* weights, solution_t prev);

static void relax(PriorityQueue q, Vertex_t* v, Vertex_t* v_2, Edge_t* e, weight_t* weights, 
    solution_t prev, int weight_choice);

static void util_DFS(Graph graph, Vertex_t* v, LinkedList f);

static void util_depthFirstTravel(Graph graph, Vertex_t* v, double f);

static void util_BFS(Graph_t* g, Vertex_t* s, Filter_t filter_fn);

static void resetVisitStatus(Graph graph);

static void freeVertex(Object vertex);

static void freeEdge(Object edge);

static void printVertex(Object vertex);

static void printEdge(Object edge);

/**************
*Parte pública*
**************/

Graph newGraph(unsigned int n){

    Graph_t* graph = (Graph_t*) malloc(sizeof(Graph_t));

    graph->numVertices = n;
    graph->numEdges = 0;
    
    graph->vertices_h = newHashTable(n);

    graph->vertices_a = (Vertex_t**) malloc((n + 1) * sizeof(Vertex_t*));

    for(unsigned int i = 0; i < n + 1; i++)
        graph->vertices_a[i] = NULL;

    graph->v_size = 0;

    return graph;
}

Graph graph_minSpanningTree(Graph graph){

    if(graph == NULL)
        return NULL;

    Graph_t* g = (Graph_t*) graph,
           * msp = newGraph(g->numVertices);
    
    Vertex_t** vArray = (Vertex_t**) calloc(g->numVertices + 1, sizeof(Vertex_t*)),
             * v = NULL,
             * origin = NULL,
             * dest = NULL;
    
    Edge_t** edges = (Edge_t**) graph_edgesToArray(g);

    DisjointSet vertices = newDisjointSet(g->numVertices);

    unsigned long index = 0,
                  edgeCount = 0,
                  m = 0,
                  n = 0;

    for(Iterator it = graph_it_start(g); it != NULL; it = graph_it_next(g, it)){

        vArray[index] = it;

        graph_addVertex(msp, ((Vertex_t*) it)->id, ((Vertex_t*) it)->x, ((Vertex_t*) it)->y);

        ++index;
    }

    resetVisitStatus(g);

    qsort(edges, g->numEdges, sizeof(Edge_t*), compareEdges);

    index = 0;
    edgeCount = 0;
    
    while(index < (g->numVertices - 1) && edgeCount < g->numEdges){

        dest = hashtable_get(g->vertices_h, edges[edgeCount]->destination);
        origin = hashtable_get(g->vertices_h, edges[edgeCount]->origin);

        m = 0;
        n = 0;

        while(vArray[m] && strcmp(dest->id, vArray[m]->id))
            ++m;

        while(vArray[n] && strcmp(origin->id, vArray[n]->id))
            ++n;

        if(set_find(vertices, m) != set_find(vertices, n)){

            graph_addEdge(msp, edges[edgeCount]->origin, edges[edgeCount]->destination, 
                edges[edgeCount]->name, edges[edgeCount]->ldir, edges[edgeCount]->lesq, 
                    edges[edgeCount]->cmp, edges[edgeCount]->vm);

            graph_addEdge(msp, edges[edgeCount]->destination, edges[edgeCount]->origin, 
                edges[edgeCount]->name, edges[edgeCount]->ldir, edges[edgeCount]->lesq, 
                    edges[edgeCount]->cmp, edges[edgeCount]->vm);
                    
            set_union(vertices, set_find(vertices, m), set_find(vertices, n));

            ++index;
        }

        ++edgeCount;
    }

    free(edges);
    free(vArray);

    set_free(vertices);

    return msp;
}

Graph graph_transpose(Graph graph){

    if(graph == NULL)
        return NULL;

    Graph_t* g = (Graph_t*) graph,
           * transposed = newGraph(g->numVertices);

    Edge_t* e = NULL;

    for(unsigned int i = 0; i < g->numVertices; i++){

        graph_addVertex(transposed, g->vertices_a[i]->id, g->vertices_a[i]->x, g->vertices_a[i]->y);
    }

    resetVisitStatus(transposed);

    for(unsigned int i = 0; i < transposed->numVertices; i++){

        for(Iterator it = list_it_start(g->vertices_a[i]->adjList); it != NULL; it = list_it_next(it)){

            e = list_it_getObject(it);

            graph_addEdge(transposed, e->destination, e->origin, e->name, e->ldir, e->lesq, e->cmp,
                e->vm);
        }
    }

    return transposed;
}

Iterator graph_it_start(Graph graph){

    if(graph == NULL)
        return 0;

    Graph_t* g = (Graph_t*) graph;

    return g->vertices_a[0];
}

Iterator graph_it_next(Graph graph, Iterator it){

    if(graph == NULL)
        return 0;

    Graph_t* g = (Graph_t*) graph;
    Vertex_t* v = (Vertex_t*) it;

    return g->vertices_a[v->index + 1];
}

LinkedList graph_getAdjcencies(Graph graph, char* i){

    if(graph == NULL)
        return NULL;

    Graph_t* g = (Graph_t*) graph;

    Vertex_t* vi = hashtable_get(g->vertices_h, i);

    return vi->adjList;
}

LinkedList graph_it_getAdjacencies(Iterator it){

    if(it == NULL)
        return NULL;

    Vertex_t* v = (Vertex_t*) it;

    return v->adjList;
}

LinkedList graph_DFS(Graph graph){

    if(graph == NULL)
        return NULL;

    Graph_t* g = (Graph_t*) graph;

    LinkedList time = newLinkedList();

    resetVisitStatus(g);

    util_DFS(g, g->vertices_a[0], time);

    return time;
}

Vertex graph_get(Graph graph, char* id){

    if(graph == NULL)
        return NULL;

    Graph_t* g = (Graph_t*) graph;

    return hashtable_get(g->vertices_h, id);
}

Vertex graph_it_getVertex(Iterator it){

    if(it == NULL)
        return NULL;

    return (Vertex) it;
}

Vertex graph_addVertex(Graph graph, char* id, double x, double y){

    if(graph == NULL)
        return NULL;

    Graph_t* g = (Graph_t*) graph;
    Vertex_t* v = newVertex(id, x, y);

    hashtable_add(g->vertices_h, id, v);

    if(g->v_size != g->numVertices){

        g->vertices_a[g->v_size] = v;

        v->index = g->v_size++;
    }

    return v;
}

Edge* graph_edgesToArray(Graph graph){

    if(graph == NULL)
        return NULL;

    Graph_t* g = (Graph_t*) graph;

    Edge_t** array = (Edge_t**) calloc(g->numEdges + 1, sizeof(Edge_t*));

    LinkedList adjacencies = NULL;

    unsigned int index = 0;

    for(unsigned int i = 0; g->vertices_a[i] != NULL; i++){

        adjacencies = g->vertices_a[i]->adjList;

        for(Iterator n = list_it_start(adjacencies); n != NULL; n = list_it_next(n))
            array[index++] = (Edge_t*) list_it_getObject(n);
    }

    array[index] = NULL;

    return (Edge*) array;
}

char* graph_it_getId(Iterator it){

    if(it == NULL)
        return "";

    Vertex_t* v = (Vertex_t*) it;

    return v->id;
}

char* graph_edge_getVertex(Iterator it){

    if(it == NULL)
        return NULL;

    Edge_t* e = (Edge_t*) it;

    return e->destination;
}

char* graph_edge_getOrigin(Iterator it){

    if(it == NULL)
        return NULL;

    Edge_t* e = (Edge_t*) it;

    return e->origin;
}

char* graph_edge_getName(Iterator it){

    if(it == NULL)
        return NULL;

    Edge_t* e = (Edge_t*) it;

    return e->name;
}

char* graph_edge_getLdir(Iterator it){

    if(it == NULL)
        return NULL;

    Edge_t* e = (Edge_t*) it;

    return e->ldir;
}

char* graph_edge_getLesq(Iterator it){

    if(it == NULL)
        return NULL;

    Edge_t* e = (Edge_t*) it;

    return e->lesq;
}

Status_t graph_it_getVisitStatus(Iterator it){

    if(it == NULL)
        return CLOSED;

    Vertex_t* v = (Vertex_t*) it;

    return v->status;
}

weight_t graph_edge_getCmp(Iterator it){

    if(it == NULL)
        return -1;

    Edge_t* e = (Edge_t*) it;

    return e->cmp;
}

weight_t graph_edge_getVm(Iterator it){

    if(it == NULL)
        return -1;

    Edge_t* e = (Edge_t*) it;

    return e->vm;
}

double graph_it_getX(Iterator it){

    if(it == NULL)
        return -1;

    Vertex_t* v = (Vertex_t*) it;

    return v->x;
}

double graph_it_getY(Iterator it){

    if(it == NULL)
        return -1;

    Vertex_t* v = (Vertex_t*) it;

    return v->y;
}

solution_t graph_dijkstra(Graph graph, char* s, int weight_choice){

    if(graph == NULL)
        return NULL;

    Graph_t* g = (Graph_t*) graph;

    PriorityQueue q = newPriorityQueue();

    LinkedList adjList = NULL;

    Vertex_t* v = NULL,
            * v_2 = NULL;

    Edge_t* e = NULL;

    weight_t* weights = (weight_t*) malloc(sizeof(weight_t) * g->numVertices);
    solution_t prev = (solution_t) calloc(g->numVertices, sizeof(solution_t));

    initDijkstra(g, q, s, weights, prev);
    
    while(queue_size(q)){

        v = queue_pop(q);

        adjList = graph_it_getAdjacencies(v);

        for(Iterator it = list_it_start(adjList); it != NULL; it = list_it_next(it)){

            e = list_it_getObject(it);
            v_2 = graph_get(g, graph_edge_getVertex(e));

            if(v_2->status == OPEN)
                relax(q, v, v_2, e, weights, prev, weight_choice);
        }

        v->status = VISITED;
    }        

    free(weights);

    queue_free(q, NULL);

    return prev;
}

unsigned int graph_it_getIndex(Iterator it){

    if(it == NULL)
        return 0;

    Vertex_t* v = (Vertex_t*) it;

    return v->index;
}

unsigned int graph_size(Graph graph){

    if(graph == NULL)
        return 0;

    Graph_t* g = (Graph_t*) graph;

    return g->numVertices;
}

unsigned int graph_getNumEdges(Graph graph){

    if(graph == NULL)
        return 0;

    Graph_t* g = (Graph_t*) graph;

    return g->numEdges;
}

bool graph_areAdjacent(Graph graph, char* i, char* j){

    if(graph == NULL)
        return false;

    Graph_t* g = (Graph_t*) graph;

    Vertex_t* vi = hashtable_get(g->vertices_h, i);
    Vertex_t* vj = hashtable_get(g->vertices_h, j);

    bool output = false;

    if(vi && vj){

        Edge_t* buffer = newEdge(j, i, "", "", "", 0, 0);

        output = list_indexOf(vi->adjList, buffer, edge_compareVertices) > -1;

        freeEdge(buffer);
    }

    return output;
}

void graph_addEdge(Graph graph, char* i, char* j, char* name, char* ldir, char* lesq, weight_t cmp,
    weight_t vm){

    if(graph == NULL)
        return;

    Graph_t* g = (Graph_t*) graph;

    Vertex_t* vi = hashtable_get(g->vertices_h, i);
    Vertex_t* vj = hashtable_get(g->vertices_h, j);

    if(vi && vj){

        list_append(vi->adjList, newEdge(j, i, name, ldir, lesq, cmp, vm));
    
        ++g->numEdges;
    }
}

void graph_it_setVisitStatus(Iterator it, Status_t status){

    if(it == NULL)
        return;

    Vertex_t* v = (Vertex_t*) it;

    v->status = status;
}

void graph_removeVertex(Graph graph, char* id){

    if(graph == NULL)
        return;

    Graph_t* g = (Graph_t*) graph;

    Vertex_t* vertex = hashtable_get(g->vertices_h, id);

    Edge_t* edge = NULL;

    Iterator n = NULL;

    unsigned long index = 0;

    for(unsigned int i = 0; g->vertices_a[i] != NULL; i++){

        index = 0;

        n = list_it_start(g->vertices_a[i]->adjList);

        while(n != NULL){

            edge = (Edge_t*) list_it_getObject(n);

            if(strcmp(id, edge->destination) == 0){

                n = list_it_next(n);

                list_remove(g->vertices_a[i]->adjList, index++, freeEdge);
            
            } else{

                n = list_it_next(n);

                ++index;
            }
        }
    }

    for(unsigned int i = vertex->index; g->vertices_a[i] != NULL; i++){
        
        if(g->vertices_a[i + 1])
            g->vertices_a[i + 1]->index = i;
        g->vertices_a[i] = g->vertices_a[i + 1];
    }

    hashtable_remove(g->vertices_h, id, freeVertex);
}

void graph_DFS_updateF(Graph graph, double f){

    if(graph == NULL)
        return;

    Graph_t* g = (Graph_t*) graph;

    resetVisitStatus(g);

    util_depthFirstTravel(g, g->vertices_a[0], f);
}

void graph_BFS(Graph graph, Filter_t filter_fn){

    if(graph == NULL)
        return;

    Graph_t* g = (Graph_t*) graph;

    resetVisitStatus(g);

    util_BFS(g, g->vertices_a[0], filter_fn);
}

void graph_print(Graph graph){

    if(graph == NULL)
        return;

    Graph_t* toPrint = (Graph_t*) graph;

    hashtable_print(toPrint->vertices_h, printVertex);
}

void graph_free(Graph graph){

    if(graph == NULL)
        return;

    Graph_t* g = (Graph_t*) graph;

    hashtable_free(g->vertices_h, freeVertex);

    free(g->vertices_a);

    free(g);
}

/**************************************************************************************************/

static Vertex_t* newVertex(char* id, double x, double y){

    Vertex_t* vertex = (Vertex_t*) malloc(sizeof(Vertex_t));
    vertex->id = (char*) calloc(sizeof(char), strlen(id) + 1);

    snprintf(vertex->id, strlen(id) + 1, "%s", id);

    vertex->x = x;
    vertex->y = y;

    vertex->status = OPEN;

    vertex->adjList = newLinkedList();

    vertex->index = 0;

    return vertex;
}

static Edge_t* newEdge(char* vertex, char* origin, char* name, char* ldir, char* lesq, weight_t cmp,
    weight_t vm){

    Edge_t* edge = (Edge_t*) malloc(sizeof(Edge_t));

    edge->destination = (char*) calloc(sizeof(char), strlen(vertex) + 1);
    snprintf(edge->destination, strlen(vertex) + 1, "%s", vertex);
    edge->origin = (char*) calloc(sizeof(char), strlen(origin) + 1);
    snprintf(edge->origin, strlen(origin) + 1, "%s", origin);
    edge->name = (char*) calloc(sizeof(char), strlen(name) + 1);
    snprintf(edge->name, strlen(name) + 1, "%s", name);
    edge->ldir = (char*) calloc(sizeof(char), strlen(ldir) + 1);
    snprintf(edge->ldir, strlen(ldir) + 1, "%s", ldir);
    edge->lesq = (char*) calloc(sizeof(char), strlen(lesq) + 1);
    snprintf(edge->lesq, strlen(lesq) + 1, "%s", lesq);

    edge->cmp = cmp;
    edge->vm = vm;

    return edge;
}

static bool edge_compareVertices(Object edge_1, Object edge_2){

    if(edge_1 == NULL || edge_2 == NULL)
        return false;

    Edge_t* e = (Edge_t*) edge_1,
          * f = (Edge_t*) edge_2;

    return strcmp(e->destination, f->destination) == 0;
}

static int compareEdges(const void* edge_1, const void* edge_2){

    const Edge_t** e = (const Edge_t**) edge_1;
    const Edge_t** f = (const Edge_t**) edge_2;

    // return ((*e)->cmp - (*f)->cmp);

    if((*e)->cmp > (*f)->cmp)
        return 1;
    else if((*e)->cmp < (*f)->cmp)
        return -1;
    else
        return 0;
}

static void initDijkstra(Graph_t* g, PriorityQueue q, char* s, double* weights, solution_t prev){

    resetVisitStatus(g);

    for(unsigned int i = 0; i < g->numVertices; i++){

        weights[i] = INFINITY;
        prev[i] = NONE;
    }

    Vertex_t* v = graph_get(g, s);

    weights[v->index] = 0;

    queue_push(q, v, 0);
}

static void relax(PriorityQueue q, Vertex_t* v, Vertex_t* v_2, Edge_t* e, weight_t* weights, 
    solution_t prev, int weight_choice){

    weight_t buffer = 0;

    if(weight_choice == VM)
        buffer = weights[v->index] + e->vm;
    else
        buffer = weights[v->index] + e->cmp;

    if(buffer < weights[v_2->index]){

        weights[v_2->index] = buffer;
        prev[v_2->index] = v->id;

        queue_push(q, v_2, buffer);
    }
}

static void util_BFS(Graph_t* g, Vertex_t* s, Filter_t filter_fn){

    if(s == NULL)
        return;

    Vertex_t* v = NULL,
            * w = NULL;

    Edge_t* e = NULL;

    PriorityQueue q = newPriorityQueue();

    s->status = VISITED;

    queue_push(q, s, 1);

    while(queue_size(q)){

        v = queue_pop(q);

        for(Iterator it = list_it_start(v->adjList); it != NULL; it = list_it_next(it)){

            e = list_it_getObject(it);
            w = hashtable_get(g->vertices_h, e->destination);        

            if(w->status == OPEN){

                filter_fn(e);

                w->status = VISITED;
                
                queue_push(q, w, 1);
            
            }
        }
    }

    queue_free(q, NULL);
}

static void util_DFS(Graph graph, Vertex_t* v, LinkedList f){

    if(v == NULL)
        return;

    if(v->status == VISITED)
        return;

    v->status = VISITED;

    list_prepend(f, v);

    Edge_t* e = NULL;

    for(Iterator n = list_it_start(v->adjList); n != NULL; n = list_it_next(n)){

        e = list_it_getObject(n);

        util_DFS(graph, graph_get(graph, e->destination), f);
    }
}

static void util_depthFirstTravel(Graph graph, Vertex_t* v, double f){

    if(v == NULL)
        return;

    if(v->status == VISITED)
        return;

    v->status = VISITED;

    Edge_t* e = NULL;

    for(Iterator n = list_it_start(v->adjList); n != NULL; n = list_it_next(n)){

        e = list_it_getObject(n);

        e->vm *= f;

        util_depthFirstTravel(graph, graph_get(graph, graph_edge_getVertex(e)), f * 2);
    }
}

static void resetVisitStatus(Graph graph){

    if(graph == NULL)
        return;

    Graph_t* g = (Graph_t*) graph;

    for(unsigned int i = 0; g->vertices_a[i] != NULL; i++)
        g->vertices_a[i]->status = OPEN;
}

static void freeVertex(Object vertex){

    if(vertex == NULL)
        return;

    Vertex_t* v = (Vertex_t*) vertex;

    free(v->id);

    list_free(v->adjList, freeEdge);
    free(v);
}

static void freeEdge(Object edge){

    if(edge == NULL)
        return;

    Edge_t* e = (Edge_t*) edge;

    free(e->destination);
    free(e->origin);
    free(e->name);
    free(e->ldir);
    free(e->lesq);

    free(e);
}

static void printVertex(Object vertex){

    if(vertex == NULL)
        return;

    Vertex_t* v = (Vertex_t*) vertex;

    printf("%s (%.2lf, %.2lf):\n", v->id, v->x, v->y);

    list_print(v->adjList, printEdge);
}

static void printEdge(Object edge){

    Edge_t* e = (Edge_t*) edge;

    printf("\"%s\":\t-->%s\n", e->name, e->destination);
}