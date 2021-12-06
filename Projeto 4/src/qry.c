#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "qry.h"
#include "rectmath.h"
#include "block.h"
#include "time.h"

#define O_LINE_COLOR "red"
#define O_LINE_WIDTH 3
#define FONT_SIZE 20
#define TEXT_FILL "black"
#define TEXT_STROKE "white"
#define TEXT_STROKE_WIDTH 0.8
#define ORIGIN_FILL "orange"
#define ORIGIN_STROKE "red"
#define DEST_FILL "olive"
#define DEST_STROKE "seagreen"
#define CAR_FILL "grey"
#define CAR_STROKE "darkgrey"
#define CAR_SIZE 15
#define DURATION 13
#define MARKER_TEXT_FILL "white"
#define MARKER_TEXT_STROKE "transparent"
#define MARGIN 5
#define LAVENDER "#AB37C8"
#define DARK_LAVENDER "#AA0044"
#define DASH_RECT_COLOR "red"
#define DASH_RECT_STROKE 4
#define DASH_RECT_DASH 2
#define ROOT_FILL "transparent"
#define ROOT_STROKE "red"

typedef void (*Remover_t)(void*, void*, FILE*, double, double, double, double);

// /**************
// *Parte privada*
// **************/

static Graph genSubGraph(Graph graph, Iterator node, double x, double y, double w, double h);

static Vertex findNearestVertex(City city, Graph graph, char* cep, char side, int num);

static char* backtrackSolution(Graph graph, FILE* txt, Vertex origin, Vertex destination,
    solution_t solution);

static void removeInternalBlocks(BinaryTree tree, Iterator node, FILE* txt, double x, double y, 
    double w, double h);

static void removeInternalCrossings(Graph graph, BinaryTree tree, Iterator node, FILE* txt,
    double x, double y, double w, double h);

static void cutGraph(Graph graph, Graph subgraph, Iterator node, double x, double y, double w, 
    double h);

static void drawSCCs(Graph graph, SvgFile svg, Iterator v, char* color, double limiar);

static void reportEdge(Object o);

static FILE* file = NULL;

static int pathCount = 0;

// /**************
// *Parte pública*
// **************/

Graph qry_rv(City city, SvgFile svg, FILE* txt, double x, double y, double w, double h, double f){

    fprintf(txt, "rv %lf %lf %lf %lf %lf\n", x, y, w, h, f);

    svg_drawRect(svg, x, y, w, h, "transparent", 0, DASH_RECT_COLOR, 1, DASH_RECT_STROKE,
        DASH_RECT_DASH);

    Graph subgraph = genSubGraph(city_getStreets(city), tree_it_start(city_getCrossings(city)),
        x, y, w, h);

    Graph msp = graph_minSpanningTree(subgraph);

    graph_free(subgraph);

    graph_DFS_updateF(msp, f);

    file = txt;

    graph_BFS(msp, reportEdge);

    fputc('\n', txt);

    return msp;
}

void qry_cx(City city, SvgFile svg, FILE* txt, double limiar){

    Graph transposed = graph_transpose(city_getStreets(city));

    LinkedList times = graph_DFS(city_getStreets(city));

    for(Iterator it = graph_it_start(city_getStreets(city)); it != NULL; 
        it = graph_it_next(city_getStreets(city), it)){

        if(graph_it_getVisitStatus(it) == OPEN)
            list_prepend(times, it);
    }

    Vertex v = NULL;

    char color[8];

    fprintf(txt, "cx %lf\n", limiar);

    file = txt;

    for(Iterator it = list_it_start(times); it != NULL; it = list_it_next(it)){
        
        v = graph_get(transposed, graph_it_getId(list_it_getObject(it)));

        svg_randomColor(color, sizeof(color));        

        if(graph_it_getVisitStatus(v) == OPEN)
            fprintf(txt, "\tRegião de cor %s\n", color);

        drawSCCs(transposed, svg, v, color, limiar);
    }

    list_free(times, NULL);

    fputc('\n', txt);

    graph_free(transposed);
}

void qry_o(City city, SvgFile svg, char* cep, char side, int num){

    char address[100] = {""};

    double x = -1,
           y = -1;

    city_setR(city, cep, side, num);

    x = city_r_getX(city);
    y = city_r_getY(city);

    if(x == -1 || y == -1){

        fputs("ERRO: Falha na atualização de r.\nAbortando.", stderr);

        return;
    }

    city_setShortestPath(city, NULL);

    city_setFastestPath(city, NULL);

    snprintf(address, sizeof(address), "r: %s/%c/%d", cep, side, num);

    svg_drawLine(svg, x, y, x, 0, O_LINE_WIDTH, O_LINE_COLOR, 0, 0);
    svg_drawText(svg, address, x + MARGIN, 3 * MARGIN, FONT_SIZE, TEXT_FILL, 1, TEXT_STROKE, 1,
        TEXT_STROKE_WIDTH);
}

void qry_catac(City city, SvgFile svg, FILE* txt, double x, double y, double w, double h){

    fprintf(txt, "catac %lf %lf %lf %lf\n", x, y, w, h);

    svg_drawRect(svg, x, y, w, h, LAVENDER, 0.5, DARK_LAVENDER, 0.5, 3, 0);

    removeInternalBlocks(city_getBlocks(city), tree_it_start(city_getBlocks(city)), txt, 
        x, y, w, h);
    removeInternalCrossings(city_getStreets(city), city_getCrossings(city), 
        tree_it_start(city_getCrossings(city)), txt, x, y, w, h);

    city_setShortestPath(city, NULL);

    city_setFastestPath(city, NULL);

    fputc('\n', txt);
}

void qry_p(City city, SvgFile svg, FILE* txt, char* cep, char side, int num, char* cmc, char* cmr){

    char* path = NULL;

    Vertex origin = findNearestVertex(city, city_getStreets(city), city_r_getCep(city), 
        city_r_getSide(city), city_r_getNum(city));

    Vertex destination = findNearestVertex(city, city_getStreets(city), cep, side, num);

    if(city_getShortestPath(city) == NULL){
        
        solution_t shortestPath = 
            graph_dijkstra(city_getStreets(city), graph_it_getId(origin), CMP);
        
        city_setShortestPath(city, shortestPath);
    }

    if(city_getFastestPath(city) == NULL){
        
        solution_t fastestPath = graph_dijkstra(city_getStreets(city), graph_it_getId(origin), VM);

        city_setFastestPath(city, fastestPath);
    }
    fprintf(txt, "p? %s %c %d %s %s\n", cep, side, num, cmc, cmr);

    fputs("\tCaminho mais curto:\n", txt);
    path = backtrackSolution(city_getStreets(city), txt, origin, destination, 
        city_getShortestPath(city));

    if(path){
    
        svg_drawPath(svg, path, 4.5, "transparent", cmc, pathCount, 0);
        svg_drawCar(svg, CAR_SIZE, CAR_SIZE - 2, CAR_FILL, 1, CAR_STROKE, 1, 3, DURATION, pathCount++);
    
    } else
        svg_drawLine(svg, graph_it_getX(origin), graph_it_getY(origin), graph_it_getX(destination), 
            graph_it_getY(destination), 5, "red", 0, 4);

    free(path);

    fputs("\n\tCaminho mais rápido:\n", txt);
    path = backtrackSolution(city_getStreets(city), txt, origin, destination, 
        city_getFastestPath(city));

    if(path){
    
        svg_drawPath(svg, path, 3.5, "transparent", cmr, pathCount, 0);
        svg_drawCar(svg, CAR_SIZE, CAR_SIZE - 2, CAR_FILL, 1, CAR_STROKE, 1, 3, DURATION, pathCount++);
    
    } else
        svg_drawLine(svg, graph_it_getX(origin), graph_it_getY(origin), graph_it_getX(destination), 
            graph_it_getY(destination), 5, "red", 0, 4);

    free(path);

    svg_drawCircle(svg, graph_it_getX(origin), graph_it_getY(origin), 10, ORIGIN_FILL, 0.8, 
        ORIGIN_STROKE, 1, 3, 0, 0);
    svg_drawText(svg, "I", graph_it_getX(origin), graph_it_getY(origin), 17, 
        MARKER_TEXT_FILL, 1, MARKER_TEXT_STROKE, 1, 0.8);

    svg_drawCircle(svg, graph_it_getX(destination), graph_it_getY(destination), 10, DEST_FILL, 1, 
        DEST_STROKE, 0.8, 3, 0, 0);
    svg_drawText(svg, "F", graph_it_getX(destination), graph_it_getY(destination), 16, 
        MARKER_TEXT_FILL, 1, MARKER_TEXT_STROKE, 1, 0.8);

    fputc('\n', txt);
}

/**************************************************************************************************/
 
static Graph genSubGraph(Graph graph, Iterator node, double x, double y, double w, double h){

    Graph subgraph = newGraph(graph_size(graph));
    LinkedList adjList = NULL;
    Edge e = NULL;
    Vertex v = NULL;

    cutGraph(graph, subgraph, node, x, y, w, h);

    for(Iterator it = graph_it_start(subgraph); it != NULL; it = graph_it_next(subgraph, it)){

        adjList = graph_getAdjcencies(graph, graph_it_getId(it));

        for(Iterator n = list_it_start(adjList); n != NULL; n = list_it_next(n)){

            e = list_it_getObject(n);
            v = graph_get(graph, graph_edge_getVertex(e));

            if(isPointInternal(graph_it_getX(v), graph_it_getY(v), x, y, w, h))
                graph_addEdge(subgraph, graph_edge_getOrigin(e), graph_edge_getVertex(e), 
                    graph_edge_getName(e), graph_edge_getLdir(e), graph_edge_getLesq(e),
                    graph_edge_getCmp(e), graph_edge_getVm(e));
        }
    }

    return subgraph;
}

static Vertex findNearestVertex(City city, Graph graph, char* cep, char side, int num){

    if(graph == NULL || cep == NULL)
        return NULL;

    Vertex buffer = NULL,
           nearest = NULL;

    Edge e = NULL;

    double buf_x = 0,
           buf_y = 0,
           minDist = DBL_MAX,
           x = city_getAddressX(city, cep, side, num),
           y = city_getAddressY(city, cep, side, num),
           distance = 0;

    for(Iterator n = graph_it_start(graph); n != NULL; n = graph_it_next(graph, n)){
        
        for(Iterator it = list_it_start(graph_it_getAdjacencies(n)); it != NULL;
            
            it = list_it_next(it)){

            e = list_it_getObject(it);

            if(strcmp(cep, graph_edge_getLdir(e)) == 0 || strcmp(cep, graph_edge_getLesq(e)) == 0){

                buffer = graph_get(graph, graph_edge_getVertex(e));

                for(int i = 0; i < 2; i++){
                
                    buf_x = graph_it_getX(buffer);
                    buf_y = graph_it_getY(buffer);

                    distance = hypot(buf_x - x, buf_y - y);

                    if(distance < minDist){

                        minDist = distance;
                        nearest = buffer;
                    }

                    buffer = graph_get(graph, graph_edge_getOrigin(e));
                }
            }
        }
    }
    return nearest;
}

static char* backtrackSolution(Graph graph, FILE* txt, Vertex origin, Vertex destination, 
    solution_t solution){

    LinkedList pathList = newLinkedList();
    
    Vertex v = destination;

    Edge e = NULL;

    const char* randomTxt[] = {"\t\tSiga em %s\n", "\t\tAvance em %s\n", "\t\tVire na rua %s\n", 
                            "\t\tPermaneça na rua %s\n", "\t\tSiga no sentido de %s\n",
                            "\t\tVire à esquerda em %s\n", "\t\tVire à direita em %s\n"};

    char* path = NULL,
        * s = graph_it_getId(origin),
          buffer[1024];

    unsigned long size = 4096,
                  index = 0;

    list_append(pathList, v);

    do{

        v = graph_get(graph, solution[graph_it_getIndex(v)]);

        if(v == NULL){

            fputs("\t\tERRO: Destino inacessível\n", txt);

            return NULL;
        }

        list_prepend(pathList, v);

    } while(v && strcmp(graph_it_getId(v), s));

    path = (char*) calloc(sizeof(char), 4096);

    strcpy(path, "M ");
    
    for(Iterator it = list_it_start(pathList); it != NULL; it = list_it_next(it)){

        v = list_it_getObject(it);

        index += snprintf(buffer, sizeof(buffer), "%lf %lf, ", graph_it_getX(v), graph_it_getY(v));

        for(Iterator n = list_it_start(graph_it_getAdjacencies(v)); n != NULL; n = list_it_next(n)){

            e = list_it_getObject(n);

            if(strcmp(graph_edge_getVertex(e), 
                graph_it_getId(list_it_getObject(list_it_next(it)))) == 0){

                fprintf(txt, randomTxt[rand() % 7], graph_edge_getName(e));
            }
        }

        strcat(path, buffer);

        if(index >= size - 50){

            size *= 2;

            path = realloc(path, size);
        }
    }

    fputs("\tDESTINO\n", txt);

    path[index] = '\0';

    list_free(pathList, NULL);

    return realloc(path, strlen(path) + 1);
}

static void removeInternalBlocks(BinaryTree tree, Iterator node, FILE* txt, double x, double y, 
    double w, double h){

    if(node == NULL)
        return;

    if(isInternal(tree_it_getKey(node), x, x + w)){

        Iterator it = list_it_start(tree_it_getObjects(node));
        Block block = NULL;

        unsigned long index = 0;

        while(it != NULL){

            block = list_it_getObject(it);

            fprintf(txt, "\tQuadra: CEP = \"%s\"; x = %lf; y = %lf; w = %lf; h = %lf\n",
            block_getCep(block), block_getX(block), block_getY(block), block_getW(block), 
            block_getH(block));

            if(isRectInternal(x, y, w, h, block_getX(block), block_getY(block), block_getW(block), 
                block_getH(block))){

                it = list_it_next(it);

                list_remove(tree_it_getObjects(node), index, block_free);
            
            } else{
            
                it = list_it_next(it);
            
                ++index;
            }
        }

        if(list_size(tree_it_getObjects(node)) == 0)
            tree_remove(tree, tree_it_getKey(node), block_free);
    }

    if(tree_it_getMaxX(tree_it_nextLeft(node)) >= x && tree_it_getMinX(tree_it_nextLeft(node)) <= 
        x + w)
        removeInternalBlocks(tree, tree_it_nextLeft(node), txt, x, y, w, h);

    if(tree_it_getMaxX(tree_it_nextRight(node)) >= x && tree_it_getMinX(tree_it_nextRight(node)) <= 
        x + w)
        removeInternalBlocks(tree, tree_it_nextRight(node), txt, x, y, w, h);
}

static void removeInternalCrossings(Graph graph, BinaryTree tree, Iterator node, FILE* txt,
    double x, double y, double w, double h){

    if(node == NULL)
        return;

    if(isInternal(tree_it_getKey(node), x, x + w)){

        Iterator it = list_it_start(tree_it_getObjects(node));
        Vertex vertex = NULL;

        unsigned long index = 0;

        while(it != NULL){

            vertex = list_it_getObject(it);

            if(isPointInternal(graph_it_getX(vertex), graph_it_getY(vertex), x, y, w, h)){

                it = list_it_next(it);

                fprintf(txt, "\tVértice: Id = \"%s\"; x = %lf; y = %lf\n", graph_it_getId(vertex),
                graph_it_getX(vertex), graph_it_getY(vertex));

                list_remove(tree_it_getObjects(node), index, NULL);

                graph_removeVertex(graph, graph_it_getId(vertex));
            
            } else{
            
                it = list_it_next(it);
            
                ++index;
            }
        }

        if(list_size(tree_it_getObjects(node)) == 0)
            tree_remove(tree, tree_it_getKey(node), NULL);
    }

    if(tree_it_getMaxX(tree_it_nextLeft(node)) >= x && tree_it_getMinX(tree_it_nextLeft(node)) <= 
        x + w)
        removeInternalCrossings(graph, tree, tree_it_nextLeft(node), txt, x, y, w, h);

    if(tree_it_getMaxX(tree_it_nextRight(node)) >= x && tree_it_getMinX(tree_it_nextRight(node)) <= 
        x + w)
        removeInternalCrossings(graph, tree, tree_it_nextRight(node), txt, x, y, w, h);
}

static void cutGraph(Graph graph, Graph subgraph, Iterator node, double x, double y, double w,
    double h){

    if(node == NULL)
        return;

    if(isInternal(tree_it_getKey(node), x, x + w)){

        Iterator it = list_it_start(tree_it_getObjects(node));
        Vertex vertex = NULL;

        while(it != NULL){

            vertex = list_it_getObject(it);

            if(isPointInternal(graph_it_getX(vertex), graph_it_getY(vertex), x, y, w, h)){

                graph_addVertex(subgraph, graph_it_getId(vertex), graph_it_getX(vertex), 
                    graph_it_getY(vertex));
            }
            
            it = list_it_next(it);
        }
    }

    if(tree_it_getMaxX(tree_it_nextLeft(node)) >= x && tree_it_getMinX(tree_it_nextLeft(node)) <= 
        x + w)
        cutGraph(graph, subgraph, tree_it_nextLeft(node), x, y, w, h);

    if(tree_it_getMaxX(tree_it_nextRight(node)) >= x && tree_it_getMinX(tree_it_nextRight(node)) <= 
        x + w)
        cutGraph(graph, subgraph, tree_it_nextRight(node), x, y, w, h);
}

static void drawSCCs(Graph graph, SvgFile svg, Iterator v, char* color, double limiar){

    if(v == NULL)
        return; 

    if(graph_it_getVisitStatus(v) == VISITED)
        return;

    Edge e = NULL;

    graph_it_setVisitStatus(v, VISITED);

    fprintf(file, "\t\tid: %s; x: %lf; y: %lf\n", graph_it_getId(v), graph_it_getX(v), 
        graph_it_getY(v));

    svg_drawEllipse(svg, graph_it_getX(v), graph_it_getY(v), 40, 40, color, 0.5, color, 0, 3, 0);

    for(Iterator n = list_it_start(graph_it_getAdjacencies(v)); n != NULL; n = list_it_next(n)){

        e = list_it_getObject(n);

        if(graph_edge_getVm(e) > limiar){
            
            drawSCCs(graph, svg, graph_get(graph, graph_edge_getVertex(e)), 
            color, limiar);

        }else{

            svg_drawLine(svg, graph_it_getX(graph_get(graph, graph_edge_getVertex(e))), 
                graph_it_getY(graph_get(graph, graph_edge_getVertex(e))), graph_it_getX(v), 
                graph_it_getY(v), 3, "red", 0, 0);
        }
    }
}

static void reportEdge(Object o){

    fprintf(file, "\tnome: %s; cmp: %lf; vm: %lf; ldir: %s; lesq: %s\n", 
        graph_edge_getName(o), graph_edge_getCmp(o), graph_edge_getVm(o), graph_edge_getLdir(o),
            graph_edge_getLesq(o));
}

