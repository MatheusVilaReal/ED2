#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "svg.h"

// #define DEF_WIDTH "10000"
// #define DEF_HEIGHT "10000"
#define HEADER "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n<svg\n\txmlns=\"http://www.w3.org/2000/svg\"\n\txmlns:xlink=\"http://www.w3.org/1999/xlink\"\n\theight=\"20000\" width=\"20000\"\n>\n\n"
#define FOOTER "\n</svg>"
#define SPACING 5
#define ANIMATION_DUR 5
#define CROSS 0x86U

/**************
*Parte privada*
**************/

typedef struct Svg{

    char* body;
    bool hasFooter;

} Svg_t;

static int addHeader(Svg svg);

static int addFooter(Svg svg);

/**************
*Parte pública*
**************/

Svg newSvg(){

    Svg_t* svg = (Svg_t*) malloc(sizeof(Svg_t));

    svg->body = (char*) calloc(sizeof(char), sizeof(HEADER) + 1);
    svg->hasFooter = false;

    addHeader(svg);

    return svg;
}

Svg copySvg(Svg dest, const Svg svg){

    Svg_t* receiver = (Svg_t*) dest;
    const Svg_t* toCopy = (const Svg_t*) svg;

    receiver->body = realloc(receiver->body, strlen(toCopy->body) + 1);

    snprintf(receiver->body, strlen(toCopy->body) + 1, "%s", toCopy->body);

    receiver->hasFooter = toCopy->hasFooter;

    return receiver;
}

Svg catSvg(Svg dest, const Svg source){

    Svg_t* toCat = (Svg_t*) dest;
    Svg_t* buf = (Svg_t*) source;

    char* temp = (char*) realloc(toCat->body, (strlen(toCat->body) + strlen(buf->body) - strlen(HEADER) + 1) * sizeof(char));

    if(temp)
        toCat->body = temp;
    else
        return toCat;

    strncat(toCat->body, buf->body + sizeof(HEADER) - 1, strlen(buf->body) + 1);

    toCat->hasFooter = buf->hasFooter;

    return toCat;
}

void drawRect(Svg svg, double x, double y, double w, double h, char* fill, double fill_op, char* stroke,
              double stroke_op, double stroke_width, int dasharray){

    Svg_t* toDraw = (Svg_t*) svg;

    char buf_1[1024];
    int allocLen_1 = 0;

    if(dasharray)
        allocLen_1 = snprintf(buf_1, sizeof(buf_1), "\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\"\n"
                     "\tstyle=\"fill: %s; fill-opacity: %lf; stroke: %s; stroke-opacity: %lf; stroke-width: %lf; stroke-dasharray: %d\"/>\n", 
                     x, y, w, h, fill, fill_op, stroke, stroke_op, stroke_width, dasharray);
    else
        allocLen_1 = snprintf(buf_1, sizeof(buf_1), "\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\"\n"
                     "\tstyle=\"fill: %s; fill-opacity: %lf; stroke: %s; stroke-opacity: %lf; stroke-width: %lf\"/>\n", 
                     x, y, w, h, fill, fill_op, stroke, stroke_op, stroke_width);

    char *buf = realloc(toDraw->body, strlen(toDraw->body) + allocLen_1 + 1);

    if(buf){

        toDraw->body = buf;

        strncat(toDraw->body, buf_1, allocLen_1);
    }
}

void drawEllipse(Svg svg, double cx, double cy, double rx, double ry, char* fill, double fill_op, char* stroke,
              double stroke_op, double stroke_width, int dasharray){

    Svg_t* toDraw = (Svg_t*) svg;

    char buf_1[1024];
    int allocLen_1 = 0;

    if(dasharray)
        allocLen_1 = snprintf(buf_1, sizeof(buf_1), "\t<ellipse cx=\"%lf\" cy=\"%lf\" rx=\"%lf\" ry=\"%lf\"\n"
                     "\tstyle=\"fill: %s; fill-opacity: %lf; stroke: %s; stroke-opacity: %lf; stroke-width: %lf; stroke-dasharray: %d\"/>\n", 
                     cx, cy, rx, ry, fill, fill_op, stroke, stroke_op, stroke_width, dasharray);
    else
        allocLen_1 = snprintf(buf_1, sizeof(buf_1), "\t<ellipse cx=\"%lf\" cy=\"%lf\" rx=\"%lf\" ry=\"%lf\"\n"
                     "\tstyle=\"fill: %s; fill-opacity: %lf; stroke: %s; stroke-opacity: %lf; stroke-width: %lf;\"/>\n", 
                     cx, cy, rx, ry, fill, fill_op, stroke, stroke_op, stroke_width);

    char *buf = realloc(toDraw->body, strlen(toDraw->body) + allocLen_1 + 1);

    if(buf){

        toDraw->body = buf;

        strncat(toDraw->body, buf_1, allocLen_1);
    }
}

void drawCircle(Svg svg, double x, double y, double r, char* fill, double fill_op, char* stroke,
                double stroke_op, double stroke_width, int dasharray){

    Svg_t* toDraw = (Svg_t*) svg;

    char buf_1[1024];
    int allocLen_1 = 0;
    
    if(dasharray)
        allocLen_1 = snprintf(buf_1, sizeof(buf_1), "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" "
        "style=\"fill: %s; fill-opacity: %lf; stroke: %s; stroke-opacity: %lf; stroke-width: %lfpx\">\n\t</circle>\n", 
        x, y, r, fill, fill_op, stroke, stroke_op, stroke_width);
    else
        allocLen_1 = snprintf(buf_1, sizeof(buf_1), "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" "
        "style=\"fill: %s; fill-opacity: %lf; stroke: %s; stroke-opacity: %lf; stroke-width: %lfpx\">\n\t</circle>\n", 
        x, y, r, fill, fill_op, stroke, stroke_op, stroke_width);

    char *buf = realloc(toDraw->body, strlen(toDraw->body) + allocLen_1 + 1);

    if(buf){

        toDraw->body = buf;

        strncat(toDraw->body, buf_1, allocLen_1);
    }
}

void drawLine(Svg svg, double x_1, double y_1, double x_2, double y_2, double width, char* color){

    Svg_t* toDraw = (Svg_t*) svg;

    char buf_1[1024];

    int allocLen_1 = snprintf(buf_1, sizeof(buf_1), "\n\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" "
    "style=\"stroke: %s; stroke-width: %lf; stroke-opacity: 1\"/>\n",
    x_1, y_1, x_2, y_2, color, width);

    char *buf = realloc(toDraw->body, strlen(toDraw->body) + allocLen_1 * sizeof(char) + 1);

    if(buf){

        toDraw->body = buf;

        strncat(toDraw->body, buf_1, allocLen_1);
    }
}

void drawText(Svg svg, char* text, double x, double y, double font_size, char* fill, double fill_op, char* stroke,
              double stroke_op, double stroke_width){

    Svg_t* toDraw = (Svg_t*) svg;

    char buf_1[1024];

    int allocLen_1 = snprintf(buf_1, sizeof(buf_1), "\n\t<text x=\"%lf\" y=\"%lf\" font-size=\"%lfpx\" "
    "style=\"fill: %s; fill-opacity: %lf; stroke: %s; stroke-opacity: %lf; stroke-width: %lf\">%s</text>\n",
    x, y, font_size, fill, fill_op, stroke, stroke_op, stroke_width, text);

    char *buf = realloc(toDraw->body, strlen(toDraw->body) + allocLen_1 * sizeof(char) + 1);

    if(buf){

        toDraw->body = buf;

        strncat(toDraw->body, buf_1, allocLen_1);
    }
}

void viewSvg(Svg svg){

    Svg_t* toView = (Svg_t*) svg;

    puts(toView->body);
}

void writeSvg(Svg svg, char* path){

    if(svg == NULL)
        return;

    Svg_t* toWrite = (Svg_t*) svg;

    FILE* file = fopen(path, "w+");

    if(toWrite && file){

        if(toWrite->hasFooter == false)
            addFooter(svg);

        fwrite(toWrite->body, sizeof(char), strlen(toWrite->body), file);
        
    } else
        return;

    fclose(file);
}

void freeSvg(Svg svg){

    if(svg == NULL)
        return;

    Svg_t* toFree = (Svg_t*) svg;

    free(toFree->body);
    free(toFree);
}

static int addHeader(Svg svg){

    Svg_t* toAdd = (Svg_t*) svg;

    return snprintf(toAdd->body, strlen(toAdd->body) + sizeof(HEADER) + 1, "%s", HEADER);
}

static int addFooter(Svg svg){

    Svg_t* toAdd = (Svg_t*) svg;

    char* buf = (char*) realloc(toAdd->body, strlen(toAdd->body) + sizeof(FOOTER) + 1);

    if(buf){
    
        toAdd->body = buf;
        strncat(toAdd->body, FOOTER, sizeof(FOOTER));
    }

    return sizeof(FOOTER);
}