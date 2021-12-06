#ifndef QRY_H
#define QRY_H

#include "types.h"
#include "svg.h"
#include "city.h"
#include "linkedlist.h"

/* rv */
Graph qry_rv(City city, SvgFile svg, FILE* txt, double x, double y, double w, double h, double f);

/* cx */
void qry_cx(City city, SvgFile svg, FILE* txt, double limiar);

/* @o */
void qry_o(City city, SvgFile svg, char* cep, char side, int num);

/* catac */
void qry_catac(City city, SvgFile svg, FILE* txt, double x, double y, double w, double h);

/* p? */
void qry_p(City city, SvgFile svg, FILE* txt, char* cep, char side, int num, char* cmc, char* cmr);

#endif