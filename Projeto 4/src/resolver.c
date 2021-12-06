#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "hashtable.h"
#include "block.h"
#include "rectmath.h"
#include "linkedlist.h"
#include "strinput.h"
#include "path.h"
#include "qry.h"
#include "resolver.h"

#define BASE_SIZE_A 2
#define OPACITY 1
#define ASSINATURA "\n###################################################\naluno: MATHEUS PIRES VILA REAL\t\tn° 202000560352"
#define GRAPH_NODE_RADIUS 3
#define STREET_WIDTH 1.5
#define STREET_COLOR "black"
#define STREET_OPACITY 1
#define FONT_SIZE 20
#define TEXT_FILL "black"
#define TEXT_STROKE "white"
#define TEXT_STROKE_WIDTH 0.8
#define MARGIN 5

// /**************
// *Parte privada*
// **************/

static void drawBlocks(Iterator it, SvgFile svg);

static void drawStreets(Graph graph, SvgFile svg, char* color, double width, int streetarrow);

// /**************
// *Parte pública*
// **************/

int solveGeo(City city, Env env){

    FILE* geo = fopen(env_getGeoFile(env), "r");

    if(geo == NULL){

        fprintf(stderr, "ERRO\nNao foi possivel abrir arquivo geo.\nVerifique o diretorio especific\
                        ado e tente novamente.\n");
    
        return -1;
    }

    char word[20] = {""},
         cep[20] = {""},
         fill[20] = {""},
         stroke[20] = {""};

    double x = 0, 
           y = 0, 
           w = 0, 
           h = 0,
           sw = 0;

    unsigned int nx = 0;


    do{

        fgetword(word, sizeof(word), geo);

        if(strcmp(word, "cq") == 0){
            
            fscanf(geo, " %lfpx %s %s ", &sw, fill, stroke);

        } else if(strcmp(word, "q") == 0){

            fgetword(cep, sizeof(cep), geo);

            fscanf(geo, " %lf %lf %lf %lf ", &x, &y, &w, &h);

            Block new = newBlock(cep, fill, stroke, x, y, w, h, sw);

            tree_add(city_getBlocks(city), new, x, x + w, x);

            hashtable_add(city_getCeps(city), cep, new);
        
        } else if(strcmp(word, "nx") == 0){

            fscanf(geo, " %u ", &nx);

            city_setCeps(city, nx);
        }

    } while(strcmp(word, ""));

    fclose(geo);

    return 0;
}

int solveVia(City city, Env env){

    FILE* via = fopen(env_getViaFile(env), "r");

    if(via == NULL){

        fprintf(stderr, "ERRO\nNao foi possivel abrir arquivo via.\nVerifique o diretorio especific\
                        ado e tente novamente.\n");
    
        return -1;
    }

    char id[20] = {""},
         i[20] = {""},
         j[20] = {""},
         ldir[20] = {""},
         lesq[20] = {""},
         name[20] = {""},
         ch = 0;

    weight_t cmp = 0,
             vm = 0;

    double x = 0,
           y = 0;

    int nv = 100;

    fscanf(via, " %d ", &nv);
    
    city_setStreets(city, nv);

    do{

        ch = fgetc(via);

        switch(ch){

            case 'v':

                fscanf(via, " %s %lf %lf ", id, &x, &y);

                Vertex new = graph_addVertex(city_getStreets(city), id, x, y);

                tree_add(city_getCrossings(city), new, x, x + GRAPH_NODE_RADIUS, 
                    x - GRAPH_NODE_RADIUS);

                break;

            case 'e':

                fscanf(via, " %s %s %s %s %lf %lf %s ", i, j, ldir, lesq, &cmp, &vm, name);

                graph_addEdge(city_getStreets(city), i, j, name, ldir, lesq, cmp, vm);

                break;

            default:
                break;
        }

    } while(ch != EOF);

    fclose(via);

    return 0;
}

int solveSvg(City city, SvgFile svg, SvgFile queries){

    if(svg){
    
        drawBlocks(tree_it_start(city_getBlocks(city)), svg);

        if(city_getStreets(city))
            drawStreets(city_getStreets(city), svg, STREET_COLOR, STREET_WIDTH, 1);

        if(queries != NULL)
            svg_append(svg, queries);

    } else return -1;

    return 0;
}

int solveQry(City city, SvgFile svg, SvgFile queries, Env env){

    FILE* qry = fopen(env_getQryFile(env), "r");

    if(qry == NULL){

        fputs("ERRO\nNao foi possivel abrir arquivo de consulta.\nVerifique o diretorio "
        "especificado e tente novamente.\n", stderr);

        return -1;
    }

    FILE* txt = fopen(env_getTxtPath(env), "w+");

    if(txt == NULL){

        fputs("ERRO\nNao foi possivel gerar arquivo de texto.\nVerifique o diretorio "
        "especificado e tente novamente.\n", stderr);

        return -1;
    }

    char word[20] = {""},
         cep[20] = {""},
         cmc[20] = {""},
         cmr[20] = {""},
         side = 0;

    double x = 0, 
           y = 0, 
           w = 0, 
           h = 0,
           f = 0,
           limiar = 0;

    int num = 0;

    do{

        fgetword(word, sizeof(word), qry);

        if(strcmp(word, "@o?") == 0){

            fscanf(qry, " %s %c %d ", cep, &side, &num);

            qry_o(city, queries, cep, side, num);

        } else if(strcmp(word, "catac") == 0){

            fscanf(qry, " %lf %lf %lf %lf ", &x, &y, &w, &h);

            qry_catac(city, queries, txt, x, y, w, h);

        } else if(strcmp(word, "rv") == 0){

            fscanf(qry, " %lf %lf %lf %lf %lf ", &x, &y, &w, &h, &f);

            Graph msp = qry_rv(city, queries, txt, x, y, w, h, f);

            drawStreets(msp, queries, svg_randomColor(cmc, sizeof(cmc)), 5, 0);

            Vertex root = graph_it_start(msp);

            svg_drawText(queries, "/", graph_it_getX(root), graph_it_getY(root), 17, "white", 1,
                "transparent", 1, 0.8);

            graph_free(msp);

        } else if(strcmp(word, "cx") == 0){

            fscanf(qry, " %lf ", &limiar);

            qry_cx(city, svg, txt, limiar);

        } else if(strcmp(word, "p?") == 0){

            fscanf(qry, " %s %c %d %s %s ", cep, &side, &num, cmc, cmr);

            qry_p(city, queries, txt, cep, side, num, cmc, cmr);
        }

    } while(strcmp(word, ""));

    fputs(ASSINATURA, txt);

    fclose(qry);
    fclose(txt);

    return 0;
}

/**************************************************************************************************/

static void drawBlocks(Iterator it, SvgFile svg){

    if(it == NULL)
        return;

    LinkedList objects = tree_it_getObjects(it);
    Block block = NULL;

    for(Iterator i = list_it_start(objects); i != NULL; i = list_it_next(i)){

        block = list_it_getObject(i);

        svg_drawRect(svg, block_getX(block), block_getY(block), block_getW(block), 
            block_getH(block), block_getFill(block), OPACITY, block_getStroke(block), OPACITY, 
            block_getStrokeWidth(block), false);

        svg_drawText(svg, block_getCep(block), block_getX(block) + MARGIN * 5, 
            block_getY(block) + 3 * MARGIN, FONT_SIZE, TEXT_FILL, 1, TEXT_STROKE, 1, 
            TEXT_STROKE_WIDTH);
    }

    drawBlocks(tree_it_nextLeft(it), svg);
    drawBlocks(tree_it_nextRight(it), svg);
}

static void drawStreets(Graph graph, SvgFile svg, char* color, double width, int streetarrow){

    LinkedList adjacencies = NULL;

    char* vertex = NULL;

    double x = 0,
           y = 0,
           x_2 = 0,
           y_2 = 0;

    for(Iterator it = graph_it_start(graph); it != NULL; it = graph_it_next(graph, it)){

        x = graph_it_getX(it);
        y = graph_it_getY(it);

        adjacencies = graph_it_getAdjacencies(it);

        for(Iterator n = list_it_start(adjacencies); n != NULL; n = list_it_next(n)){

            vertex = graph_edge_getVertex(list_it_getObject(n));

            x_2 = graph_it_getX(graph_get(graph, vertex));
            y_2 = graph_it_getY(graph_get(graph, vertex));
            
            svg_drawLine(svg, x, y, x_2, y_2, width, color, streetarrow, 0);
        }

        svg_drawCircle(svg, x, y, GRAPH_NODE_RADIUS * width / STREET_WIDTH, color, 
            STREET_OPACITY, color, STREET_OPACITY, 0, 0, 0);
    }
}