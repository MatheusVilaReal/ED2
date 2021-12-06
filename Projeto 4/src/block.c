#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "block.h"

/**************
*Parte privada*
**************/

typedef struct Block{

    char* cep,
        * fill,
        * stroke;

    double x,
           y,
           w,
           h,
           stroke_width;

} Block_t;

/**************
*Parte pública*
**************/

Block newBlock(char* cep, char* fill, char* stroke, double x, double y, double w, double h,
    double stroke_width){

    Block_t* block = (Block_t*) malloc(sizeof(Block_t));

    block->cep = (char*) calloc(strlen(cep) + 1, sizeof(char));
    block->fill = (char*) calloc(strlen(fill) + 1, sizeof(char));
    block->stroke = (char*) calloc(strlen(stroke) + 1, sizeof(char));

    snprintf(block->cep, strlen(cep) + 1, "%s", cep);
    snprintf(block->fill, strlen(fill) + 1, "%s", fill);
    snprintf(block->stroke, strlen(stroke) + 1, "%s", stroke);

    block->x = x;
    block->y = y;
    block->w = w;
    block->h = h;
    block->stroke_width = stroke_width;

    return block;
}

char* block_report(Block block, char* dest, unsigned long buffer_size){

    if(block == NULL || dest == NULL)
        return NULL;

    Block_t* s = (Block_t*) block;

    snprintf(dest, buffer_size, "cep: %s, cfill: %s, cstrk: %s, x: %lf, y: %lf, w: %lf, h: %lf, \
                                sw: %lf\n",
                          s->cep, s->fill, s->stroke, s->x, s->y, s->w, s->h, s->stroke_width);

    return dest;
}

char* block_getCep(Block block){

    if(block == NULL)
        return NULL;

    Block_t* b = (Block_t*) block;

    return b->cep;
}

char* block_getFill(Block block){

    if(block == NULL)
        return NULL;

    Block_t* b = (Block_t*) block;

    return b->fill;
}

char* block_getStroke(Block block){

    if(block == NULL)
        return NULL;

    Block_t* b = (Block_t*) block;

    return b->stroke;
}

double block_getX(Block block){

    if(block == NULL)
        return -1;

    Block_t* s = (Block_t*) block;

    return s->x;
}

double block_getY(Block block){

    if(block == NULL)
        return -1;

    Block_t* s = (Block_t*) block;

    return s->y;
}

double block_getW(Block block){

    if(block == NULL)
        return -1;

    Block_t* s = (Block_t*) block;

    return s->w;
}

double block_getH(Block block){

    if(block == NULL)
        return -1;

    Block_t* s = (Block_t*) block;

    return s->h;
}

double block_getStrokeWidth(Block block){

    if(block == NULL)
        return -1;

    Block_t* s = (Block_t*) block;

    return s->stroke_width;
}

void block_free(Block block){

    if(block == NULL)
        return;

    Block_t* b = (Block_t*) block;

    free(b->cep);
    free(b->fill);
    free(b->stroke);

    free(b);
}