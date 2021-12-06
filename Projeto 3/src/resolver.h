#ifndef RESOLVER_H
#define RESOLVER_H

#include "types.h"

int solveGeo(City city, Env env);

int solveSvg(City city, Svg svgFile, Svg querries);

int solveQry(City city, Svg querries, Env env);

int solvePm(City city, Env env);

void drawTree(Tree tree, Svg svg, void (*draw)(Node, Svg));

#endif