#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "linkedlist.h"
#include "svg.h"

#define HEADER "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n<svg\n\txmlns=\"http:\
//www.w3.org/2000/svg\"\n\txmlns:xlink=\"http://www.w3.org/1999/xlink\"\n\theight=\"%lf\" width=\"%\
lf\"\n>\n"
#define DEFS "<defs>\n\t<marker id=\"streetarrow\" markerWidth=\"5\" markerHeight=\"3.5\" refX=\"6\
\" refY=\"1.75\" orient=\"auto\" markerUnits=\"strokeWidth\">\n\t\t<polygon points=\"0 0, 5 1.75, 0\
 3.5\" />\n\t</marker>\t<marker id=\"patharrow\" markerWidth=\"5\" markerHeight=\"3.5\" refX=\"0\" \
 refY=\"1.75\" orient=\"auto\" markerUnits=\"strokeWidth\">\n\t\t<polygon points=\"0 0, 5 1.75, 0 3\
 .5\" />\n\t</marker>\n</defs>\n\n"
#define FOOTER "\n</svg>"
#define SPACING 5
#define ANIMATION_DUR 5
#define HEX_COLOR_MAX 0xFFFFFF

/**************
*Parte privada*
**************/

typedef struct SvgFile{

    LinkedList /*<char*>*/ body;

    bool hasFooter;

} SvgFile_t;

static double canvas_x = 0,
              canvas_y = 0;

static void printSvgElement(Object toPrint);

/**************
*Parte pública*
**************/

SvgFile newSvgFile(){

    SvgFile_t* svg = (SvgFile_t*) malloc(sizeof(SvgFile_t));

    svg->body = newLinkedList();

    svg->hasFooter = false;

    return svg;
}

SvgFile svg_append(SvgFile dest, const SvgFile svg){

    if(dest == NULL || svg == NULL)
        return NULL;

    SvgFile_t* receiver = (SvgFile_t*) dest;
    const SvgFile_t* toCopy = (const SvgFile_t*) svg;

    char* o = NULL,
        * p = NULL;
    
    for(Iterator it = list_it_start(toCopy->body); it != NULL; it = list_it_next(it)){

        o = list_it_getObject(it);

        p = (char*) calloc(strlen(o) + 1, sizeof(char));
        snprintf(p, strlen(o) + 1, "%s", o);

        list_append(receiver->body, p);
    }

    return receiver;
}

char* svg_randomColor(char* dest, unsigned long buf_size){

    unsigned int hex = rand();
    hex %= HEX_COLOR_MAX;
    
    snprintf(dest, buf_size, "#%0X", hex);

    return dest;
}

void svg_drawRect(SvgFile svg, double x, double y, double w, double h, char* fill, double fill_op, 
    char* stroke, double stroke_op, double stroke_width, int dasharray){

    SvgFile_t* s = (SvgFile_t*) svg;

    char buffer[1024];
    int size = 0;

    if(x + w > canvas_x)
        canvas_x = x + w;

    if(y + h > canvas_y)
        canvas_y = y + h;

    if(dasharray)
        size = snprintf(buffer, sizeof(buffer), "\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" \
            height=\"%lf\"\n\tstyle=\"fill: %s; fill-opacity: %lf; stroke: %s; stroke-opacity: %lf;\
             stroke-width: %lf; stroke-dasharray: %d\"/>\n", 
            x, y, w, h, fill, fill_op, stroke, stroke_op, stroke_width, dasharray);
    else
        size = snprintf(buffer, sizeof(buffer), "\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" \
            height=\"%lf\"\n\tstyle=\"fill: %s; fill-opacity: %lf; stroke: %s; stroke-opacity: %lf;\
             stroke-width: %lf\"/>\n", 
            x, y, w, h, fill, fill_op, stroke, stroke_op, stroke_width);

    char* o = (char*) calloc(size + 1, sizeof(char));
    snprintf(o, size + 1, "%s", buffer);

    list_append(s->body, o);
}

void svg_drawEllipse(SvgFile svg, double cx, double cy, double rx, double ry, char* fill, 
    double fill_op, char* stroke, double stroke_op, double stroke_width, int dasharray){

    SvgFile_t* s = (SvgFile_t*) svg;

    char buffer[1024];
    int size = 0;

    if(cx + rx > canvas_x)
        canvas_x = cx + rx;

    if(cy + ry > canvas_y)
        canvas_y = cy + ry;

    if(dasharray)
        size = snprintf(buffer, sizeof(buffer), "\t<ellipse cx=\"%lf\" cy=\"%lf\" rx=\"%lf\" \
        ry=\"%lf\"\n\tstyle=\"fill: %s; fill-opacity: %lf; stroke: %s; stroke-opacity: %lf; \
        stroke-width: %lf; stroke-dasharray: %d\"/>\n", 
        cx, cy, rx, ry, fill, fill_op, stroke, stroke_op, stroke_width, dasharray);
    else
        size = snprintf(buffer, sizeof(buffer), "\t<ellipse cx=\"%lf\" cy=\"%lf\" rx=\"%lf\" \
        ry=\"%lf\"\n\tstyle=\"fill: %s; fill-opacity: %lf; stroke: %s; stroke-opacity: %lf; \
        stroke-width: %lf;\"/>\n", 
        cx, cy, rx, ry, fill, fill_op, stroke, stroke_op, stroke_width);

    char* o = (char*) calloc(size + 1, sizeof(char));
    snprintf(o, size + 1, "%s", buffer);

    list_append(s->body, o);
}

void svg_drawCircle(SvgFile svg, double x, double y, double r, char* fill, double fill_op, 
    char* stroke, double stroke_op, double stroke_width, int dasharray, int underlay){

    SvgFile_t* s = (SvgFile_t*) svg;

    char buffer[1024];
    int size = 0;

    if(x + r > canvas_x)
        canvas_x = x + r;

    if(y + r > canvas_y)
        canvas_y = y + r;
    
    if(dasharray)
        size = snprintf(buffer, sizeof(buffer), "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" "
        "style=\"fill: %s; fill-opacity: %lf; stroke: %s; stroke-opacity: %lf; stroke-width: %lfpx\
        \">\n\t</circle>\n", 
        x, y, r, fill, fill_op, stroke, stroke_op, stroke_width);
    else
        size = snprintf(buffer, sizeof(buffer), "\t<circle cx=\"%lf\" cy=\"%lf\" r=\"%lf\" "
        "style=\"fill: %s; fill-opacity: %lf; stroke: %s; stroke-opacity: %lf; stroke-width: %lfpx\
        \">\n\t</circle>\n", 
        x, y, r, fill, fill_op, stroke, stroke_op, stroke_width);

    char* o = (char*) calloc(size + 1, sizeof(char));
    snprintf(o, size + 1, "%s", buffer);

    if(!underlay)
        list_append(s->body, o);
    else
        list_prepend(s->body, o);
}

void svg_drawLine(SvgFile svg, double x_1, double y_1, double x_2, double y_2, double width, 
    char* color, int streetarrow, int dasharray){

    SvgFile_t* s = (SvgFile_t*) svg;

    char buffer[1024];

    int size = 0;

    if(dasharray){

        if(streetarrow)
            size = snprintf(buffer, sizeof(buffer), "\n\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" \
            y2=\"%lf\" style=\"fill: %s; stroke: %s; stroke-width: %lf; stroke-opacity: 1; \
            stroke-dasharray: %d\" marker-end=\"url(#streetarrow)\"/>\n",
            x_1, y_1, x_2, y_2, color, color, width, dasharray);
        else
            size = snprintf(buffer, sizeof(buffer), "\n\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" \
            y2=\"%lf\" style=\"fill: %s; stroke: %s; stroke-width: %lf; stroke-opacity: 1; \
            stroke-dasharray: %d\"/>\n",
            x_1, y_1, x_2, y_2, color, color, width, dasharray);

    } else{
        
        if(streetarrow)
            size = snprintf(buffer, sizeof(buffer), "\n\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" \
            y2=\"%lf\" style=\"fill: %s; stroke: %s; stroke-width: %lf; stroke-opacity: 1\" \
            marker-end=\"url(#streetarrow)\"/>\n",
            x_1, y_1, x_2, y_2, color, color, width);
        else
            size = snprintf(buffer, sizeof(buffer), "\n\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" \
            y2=\"%lf\" style=\"fill: %s; stroke: %s; stroke-width: %lf; stroke-opacity: 1\"/>\n",
            x_1, y_1, x_2, y_2, color, color, width);
    }
    char* o = (char*) calloc(size + 1, sizeof(char));
    snprintf(o, size + 1, "%s", buffer);

    list_append(s->body, o);
}

void svg_drawPath(SvgFile svg, char* d, double width, char* fill, char* stroke, int id, 
    int streetarrow){

    if(d == NULL)
        return;

    SvgFile_t* s = (SvgFile_t*) svg;

    char* buffer = (char*) calloc(strlen(d) + 1024, sizeof(char));

    int size = 0;
    
    if(streetarrow)
        size = snprintf(buffer, strlen(d) + 1024, "\n<path d=\"%s\" id=\"%d\""
        " style=\"fill:%s; stroke: %s; stroke-width: %lf; stroke-opacity: 1\" "
        "marker-end=\"url(#patharrow)\"/>\n", d, id, fill, stroke, width);
    else
        size = snprintf(buffer, strlen(d) + 1024, "\n<path d=\"%s\" id=\"%d\" "
        "style=\"fill:%s; stroke: %s; stroke-width: %lf; stroke-opacity: 1\"/>\n", d, id, fill,
        stroke, width);

    char* o = (char*) calloc(size + 1, sizeof(char));
    snprintf(o, size + 1, "%s", buffer);

    free(buffer);

    list_append(s->body, o);
}

void svg_drawText(SvgFile svg, char* text, double x, double y, double font_size, char* fill, 
    double fill_op, char* stroke, double stroke_op, double stroke_width){

    SvgFile_t* s = (SvgFile_t*) svg;

    char buffer[1024];

    int size = snprintf(buffer, sizeof(buffer), "\n\t<text dominant-baseline=\"central\" \
    text-anchor=\"middle\" x=\"%lf\" y=\"%lf\" font-size=\"%lfpx\" style=\"fill: %s; fill-opacity: \
    %lf; stroke: %s; stroke-opacity: %lf; stroke-width: %lf\">%s</text>\n",
    x, y, font_size, fill, fill_op, stroke, stroke_op, stroke_width, text);

    char* o = (char*) calloc(size + 1, sizeof(char));
    snprintf(o, size + 1, "%s", buffer);

    list_append(s->body, o);
}

void svg_drawCar(SvgFile svg, double w, double h, char* fill, double fill_op, char* stroke, 
    double stroke_op, double stroke_width, int duration, int id){

    SvgFile_t* s = (SvgFile_t*) svg;

    char buffer[1024];
    int size = 0;

    size = snprintf(buffer, sizeof(buffer), "\t<rect x=\"\" y=\"\" width=\"%lf\" height=\"%lf\"\
    style=\"fill: %s; fill-opacity: %lf; stroke: %s; stroke-opacity: %lf; stroke-width: %lf;\">\n\t\
    <animateMotion dur=\"%ds\" repeatCount=\"indefinite\"><mpath xlink:href=\"#%d\"/>\n\t\
    </animateMotion>\n</rect>\n", 
    w, h, fill, fill_op, stroke, stroke_op, stroke_width, duration, id);

    char* o = (char*) calloc(size + 1, sizeof(char));
    snprintf(o, size + 1, "%s", buffer);

    list_append(s->body, o);
}

void svg_display(SvgFile svg){

    if(svg == NULL)
        return;

    SvgFile_t* s = (SvgFile_t*) svg;

    list_print(s->body, printSvgElement);
}

void svg_write(SvgFile svg, char* path){

    if(svg == NULL)
        return;

    SvgFile_t* s = (SvgFile_t*) svg;

    FILE* file = fopen(path, "w+");

    if(s && file){

        fprintf(file, HEADER, canvas_y, canvas_x);

        fputs(DEFS, file);

        for(Iterator it = list_it_start(s->body); it != NULL; it = list_it_next(it))
            fputs((char*) list_it_getObject(it), file);

        if(s->hasFooter == false)   
            fputs(FOOTER, file);
        
    } else
        return;

    fclose(file);
}

void svg_free(SvgFile svg){

    if(svg == NULL)
        return;

    SvgFile_t* s = (SvgFile_t*) svg;

    list_free(s->body, free);
    free(s);
}

/**************************************************************************************************/

static void printSvgElement(Object toPrint){

    puts((char*) toPrint);
}