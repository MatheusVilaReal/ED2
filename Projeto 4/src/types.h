#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>
#include <float.h>

/* Tipo de referência a nós */
typedef void* Node;

/* Tipo de referência a objetos */
typedef void* Object;

/* Tipo de referência a iteradores */
typedef void* Iterator;

/* Tipo de referência a funções comparadoras */
typedef bool (*Compare_t)(Object, Object);

/* Tipo de referência a funções destrutoras */
typedef void (*Destroyer_t)(Object);

/* Tipo de referência a funções impressoras */
typedef void (*Printer_t)(Object);

/* Tipo de referência a funções de filtro */
typedef void (*Filter_t)(Object);

#endif