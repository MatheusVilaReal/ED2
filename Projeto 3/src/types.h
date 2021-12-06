#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

typedef void* Node, *Data;

typedef int (*SearchFunction_t)(Data, char*);

typedef void* Tree;

typedef void* List;

typedef void* HashTable;

typedef void* Block, *Tenancy, *Person;

typedef void (*FreeData_t)(Data);

typedef void* Env;

typedef void* City;

typedef void* Svg;

typedef void* Dot;

#endif