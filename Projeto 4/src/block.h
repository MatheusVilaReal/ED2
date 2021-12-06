#ifndef BLOCK_H
#define BLOCK_H

#include "types.h"

typedef void* Block;

/* Aloca em memória em retorna um ponteiro para um objeto do tipo quadra. */
Block newBlock(char* cep, char* fill, char* stroke, double x, double y, double w, double h,
    double stroke_width);

/* Grava informações sobre a quadra block na string dest, de tamanho buffer_size. Se block for NULO,
nada é feito. */
char* block_report(Block block, char* dest, unsigned long buffer_size);

/* Retorna o CEP armazenado na quadra block. Se block igual a NULO, retorna NULO. */
char* block_getCep(Block block);

/* Retorna a cor de preenchimento da quadra block. Se block igual a NULO, retorna NULO. */
char* block_getFill(Block block);

/* Retorna a cor de contorno da quadra block. Se block igual a NULO, retorna NULO. */
char* block_getStroke(Block block);

/* Retorna a coordenada x da quadra block. Se block igual a NULO, retorna -1. */
double block_getX(Block block);

/* Retorna a coordenada y da quadra block. Se block igual a NULO, retorna -1. */
double block_getY(Block block);

/* Retorna o comprimento w da quadra block. Se block igual a NULO, retorna -1. */
double block_getW(Block block);

/* Retorna o comprimento h da quadra block. Se block igual a NULO, retorna -1. */
double block_getH(Block block);

/* Retorna a espessura do contorno da quadra block. Se block igual a NULO, retorna -1. */
double block_getStrokeWidth(Block block);

/* Libera memória da quadra block. */
void block_free(Block block);

#endif