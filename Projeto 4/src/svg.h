#ifndef SVG_H
#define SVG_H

#include "types.h"

typedef void* SvgFile;

/* Aloca em memória e retorna um ponteiro para um Svg. */
SvgFile newSvgFile();

/* Adiciona os elementos de svg ao final de dest. */
SvgFile svg_append(SvgFile dest, const SvgFile svg);

/* Retorna cor hexadecimal aleatória no formato #FFFFFF, e escreve em dest, de tamanho buf_size. */
char* svg_randomColor(char* dest, unsigned long buf_size);

/* Desenha um retângulo em svg. */
void svg_drawRect(SvgFile svg, double x, double y, double w, double h, char* fill, double fill_op, 
    char* stroke, double stroke_op, double stroke_width, int dasharray);

/* Desenha um elipse em svg. */
void svg_drawEllipse(SvgFile svg, double cx, double cy, double rx, double ry, char* fill, 
    double fill_op, char* stroke, double stroke_op, double stroke_width, int dasharray);

/* Desenha um círculo em svg. */
void svg_drawCircle(SvgFile svg, double x, double y, double r, char* fill, double fill_op, 
    char* stroke, double stroke_op, double stroke_width, int dasharray, int underlay);

/* Desenha uma linha em svg. */
void svg_drawLine(SvgFile svg, double x_1, double y_1, double x_2, double y_2, double width, 
    char* color, int arrowhead, int dasharray);

/* Desenha um caminho em svg. */
void svg_drawPath(SvgFile svg, char* d, double width, char* fill, char* stroke, int id,
    int arrowhead);

/* Desenha um texto em svg. */
void svg_drawText(SvgFile svg, char* text, double x, double y, double font_size, char* fill, 
    double fill_op, char* stroke, double stroke_op, double stroke_width);

/* Desenha um retângulo animado em svg. */
void svg_drawCar(SvgFile svg, double w, double h, char* fill, double fill_op, char* stroke, 
    double stroke_op, double stroke_width, int duration, int id);

/* Imprime svg em stdout. */
void svg_display(SvgFile svg);

/* Salva svg no arquivo de caminho path. */
void svg_write(SvgFile svg, char* path);

/* Libera a memória utilizada por svg. */
void svg_free(SvgFile svg);

#endif