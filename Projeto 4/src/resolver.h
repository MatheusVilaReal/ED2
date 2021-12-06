#ifndef RESOLVER_H
#define RESOLVER_H

#include "city.h"
#include "environment.h"
#include "svg.h"
#include "types.h"

int solveGeo(City city, Env env);

int solveVia(City city, Env env);

int solveQry(City city, SvgFile svg, SvgFile queries, Env env);

int solveSvg(City city, SvgFile svg, SvgFile queries);

#endif