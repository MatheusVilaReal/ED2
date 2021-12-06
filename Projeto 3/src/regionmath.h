#ifndef REGIONMATH_H
#define REGIONMATH_H

#include "types.h"

bool isInternal(double x, double region_x1, double region_x2);

bool isRectInternal(double x_1, double y_1, double w_1, double h_1, double x_2, double y_2, double w_2, double h_2);

bool isPointInternal(double x, double y, double r_x, double r_y, double r_w, double r_h);

bool doRectsOverlap(double x_1, double y_1, double w_1, double h_1, 
                    double x_2, double y_2, double w_2, double h_2);

#endif