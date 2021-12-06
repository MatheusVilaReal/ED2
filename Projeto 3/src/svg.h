#ifndef SVG_H
#define SVG_H

#include "types.h"

Svg newSvg();

Svg copySvg(Svg dest, const Svg svg);

Svg catSvg(Svg dest, const Svg source);

void drawRect(Svg svg, double x, double y, double w, double h, char* fill, double fill_op, char* stroke,
              double stroke_op, double stroke_width, int dasharray);

void drawEllipse(Svg svg, double cx, double cy, double rx, double ry, char* fill, double fill_op, char* stroke,
              double stroke_op, double stroke_width, int dasharray);

void drawRectLabel(Svg svg, double x, double y, double w, double h, char* fill, double fill_op, char* stroke, double stroke_op, double stroke_width, unsigned int sheltered);

void drawNve(Svg svg, double x, double y, double s);

void drawCircle(Svg svg, double x, double y, double r, char* fill, double fill_op, char* stroke,
                double stroke_op, double stroke_width, int dasharray);

void drawCross(Svg svg, double x, double y, double r);

void drawLine(Svg svg, double x_1, double y_1, double x_2, double y_2, double width, char* color);

void drawText(Svg svg, char* text, double x, double y, double font_size, char* fill, double fill_op, char* stroke,
              double stroke_op, double stroke_width);
              
void drawPath(Svg svg, double x_1, double y_1, double x_2, double y_2);

void viewSvg(Svg svg);

void writeSvg(Svg svg, char* path);

void freeSvg(Svg svg);

#endif