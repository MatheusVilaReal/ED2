#ifndef DOT_H
#define DOT_H

#include "types.h"

Dot newDot(char* geo, char* pm, char* qry);

void drawNode(Dot dot, char* id, char* label);

void connectNodes(Dot dot, char* id_1, char* id_2, int side);

void treeToDot(Tree tree, Dot dot);

void writeDot(Dot dot, char* path);

void freeDot(Dot dot);

#endif