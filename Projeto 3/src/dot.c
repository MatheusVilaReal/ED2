#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dot.h"
#include "avltree.h"
#include "list.h"
#include "buildings.h"

#define SHAPE "record"
#define HEIGHT 0.5F
#define STYLE "filled"
#define FILLCOLOR "lavender"
#define FONTNAME "Courier New"

/**************
*Parte privada*
**************/

typedef struct Dot{

    char* body;

    bool hasFooter;

} Dot_t;

static char* packNode(Node node, unsigned short height);

static void declareNodes(Node node, Dot dot, unsigned short height);

static void drawConnections(Node node, Dot dot);

/**************
*Parte pública*
**************/

Dot newDot(char* geo, char* pm, char* qry){

    Dot_t* dot = (Dot_t*) malloc(sizeof(Dot_t));

    dot->body = (char*) calloc(sizeof(char), strlen(geo) + strlen(pm) + strlen(qry) + 1024);

    int length = snprintf(dot->body, strlen(geo) + strlen(pm) + strlen(qry) + 1024,
                             "digraph Ed2{\n\n"
                             "\tlabel = \" aluno: MATHEUS PIRES VILA REAL\tn° 202000560352 \\ %s \\ %s \\ %s\"\n"
                             "\n\t//\tFormato e escala dos nós\n"
                             "\tnode[shape = %s, height = %lf, style = \"%s\", fillcolor = \"%s\"\n"
                             "\t\t fontname = \"%s\"]\n\n",
                             geo, pm, qry, SHAPE, HEIGHT, STYLE, FILLCOLOR, FONTNAME
                         );

    char* buffer = (char*) realloc(dot->body, length + 1);

    if(buffer)
        dot->body = buffer;

    dot->hasFooter = false;

    return dot;
}

void drawNode(Dot dot, char* id, char* label){

    Dot_t* toDraw = (Dot_t*) dot; 

    char buf_1[10000];

    int allocLen_1 = snprintf(buf_1, sizeof(buf_1), "\t%s [label = \"<f0> | <f1> %s | <f2>\"]\n",
                              id, label);

    char *buf = realloc(toDraw->body, strlen(toDraw->body) + allocLen_1 + 1);

    if(buf){

        toDraw->body = buf;

        strncat(toDraw->body, buf_1, allocLen_1);
    }
}

void connectNodes(Dot dot, char* id_1, char* id_2, int side){

    Dot_t* toDraw = (Dot_t*) dot;

    char buf_1[1024];

    int allocLen_1 = snprintf(buf_1, 1024, "\n\"%s\":f%d->\"%s\":f1", id_1, side, id_2);

    char *buf = realloc(toDraw->body, strlen(toDraw->body) + allocLen_1 * sizeof(char) + 1);

    if(buf){

        toDraw->body = buf;

        strncat(toDraw->body, buf_1, allocLen_1);
    }
}

void treeToDot(Tree tree, Dot dot){

    declareNodes(getRoot(tree), dot, 0);

    drawConnections(getRoot(tree), dot);
}

void addFooter(Dot dot){

    if(dot == NULL)
        return;

    Dot_t* toAdd = (Dot_t*) dot;

    char* buffer = (char*) realloc(toAdd->body, strlen(toAdd->body) + 3);

    if(buffer == NULL)
        return;
    
    toAdd->body = buffer;

    strncat(toAdd->body, "\n}", 3);

    toAdd->hasFooter = true;
}

void writeDot(Dot dot, char* path){

    if(dot == NULL)
        return;

    Dot_t* toWrite = (Dot_t*) dot;

    FILE* file = fopen(path, "w+");

    if(toWrite && file){

        if(toWrite->hasFooter == false)
            addFooter(dot);

        fwrite(toWrite->body, sizeof(char), strlen(toWrite->body), file);
        
    } else
        return;

    fclose(file);
}

void freeDot(Dot dot){

    if(dot == NULL)
        return;

    Dot_t* this = (Dot_t*) dot;

    free(this->body);
    free(this);
}

/*************************************************************************************************/

static char* packNode(Node node, unsigned short height){

    char* packedStr = (char*) calloc(sizeof(char), 2048),
        * buffer = NULL;
    char newStr[4096];

    int length = snprintf(packedStr, 1024, "x: %.2lf\\nminX: %.2lf\\nmaxX: %.2lf \\nAltura: %u Balanc: %d\\n",
                          getNodeKey(node), getNodeMinX(node), getNodeMaxX(node), height, getBranchBalance(node));

    int i = 0;

    Node listNode = getHead(getNodeData(node));
    
    while(listNode && i < 5){

        // length = snprintf(newStr, sizeof(newStr), "x: %lf y: %lf cep: %s\\n", getBlockX(getNodeData(listNode)), getBlockY(getNodeData(listNode)),
        //          getBlockCep(getNodeData(listNode)));

        length = snprintf(newStr, sizeof(newStr), "cep: %s\\n",
                 getBlockCep(getNodeData(listNode)));

        buffer = (char*) realloc(packedStr, strlen(packedStr) + length + 1);

        if(buffer)
            packedStr = buffer;

        strcat(packedStr, newStr);

        listNode = getNext(listNode);

        ++i;
    }
    
    return packedStr;
}

static void declareNodes(Node node, Dot dot, unsigned short height){

    if(node == NULL)
        return;

    char id[20];
    snprintf(id, 20, "%lu", getNodeIndex(node));

    char* packedBlock = packNode(node, height);

    drawNode(dot, id, packedBlock);

    free(packedBlock);

    declareNodes(getLeftChild(node), dot, ++height);
    declareNodes(getRightChild(node), dot, height);
}

static void drawConnections(Node node, Dot dot){

    // if(node == NULL)
    //     return;

    char id_1[10],
         id_2[10];

    if(getLeftChild(node)){
        
        snprintf(id_1, sizeof(id_1), "%lu", getNodeIndex(node));
        snprintf(id_2, sizeof(id_2), "%lu", getNodeIndex(getLeftChild(node)));

        connectNodes(dot, id_1, id_2, 0);

        drawConnections(getLeftChild(node), dot);
    }
    
    if(getRightChild(node)){
        
        snprintf(id_1, sizeof(id_1), "%lu", getNodeIndex(node));
        snprintf(id_2, sizeof(id_2), "%lu", getNodeIndex(getRightChild(node)));

        connectNodes(dot, id_1, id_2, 2);

        drawConnections(getRightChild(node), dot);
    }
}