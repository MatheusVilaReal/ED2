#include "rectmath.h"


bool isInternal(double x, double region_x1, double region_x2){

    return (x >= region_x1) && (x <= region_x2);
}

bool isRectInternal(double x_1, double y_1, double w_1, double h_1, double x_2, double y_2, double w_2, double h_2){

    if(x_1 + w_1 >= x_2 + w_2 && x_1 <= x_2)
        if(y_1 + h_1 >= y_2 + h_2 && y_1 <= y_2)
            return true;

    return false;
}

bool isPointInternal(double x, double y, double r_x, double r_y, double r_w, double r_h){

    if(x < r_x || x > r_x + r_w)
        return false;

    if(y < r_y || y > r_y + r_h)
        return false;

    return true;
}

bool doRectsOverlap(double x_1, double y_1, double w_1, double h_1, 
                    double x_2, double y_2, double w_2, double h_2){

    if(x_1 > x_2+w_2 || x_2 > x_1+w_1) 
        return false;

    if(y_1 > y_2+h_2 || y_2 > y_1+h_1)
        return false;

    return true;
}