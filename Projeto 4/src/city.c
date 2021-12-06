#include <stdio.h>
#include <stdlib.h>

#include "block.h"
#include "city.h"

/**************
*Parte privada*
**************/

typedef struct Address{

    char cep[100],
         side;

    int num;

} Address_t;

typedef struct City{

    BinaryTree blocks,
               crossings;

    HashTable ceps;

    Graph streets;

    Address_t r;

    solution_t shortestPath,
               fastestPath;

} City_t;

/**************
*Parte pública*
**************/

City newCity(){

    City_t* city = (City_t*) malloc(sizeof(City_t));

    city->blocks = newBinaryTree();
    city->crossings = newBinaryTree();

    city->ceps = NULL;

    city->streets = NULL;

    snprintf(city->r.cep, sizeof(city->r.cep), "Rolandia");             // ;)

    city->r.side = 0;
    city->r.num = 0;

    city->shortestPath = NULL;
    city->fastestPath = NULL;

    return city;
}

BinaryTree city_getBlocks(City city){

    if(city == NULL)
        return NULL;

    City_t* c = (City_t*) city;

    return c->blocks;
}

BinaryTree city_getCrossings(City city){

    if(city == NULL)
        return NULL;

    City_t* c = (City_t*) city;

    return c->crossings;
}

Graph city_getStreets(City city){

    if(city == NULL)
        return NULL;

    City_t* c = (City_t*) city;

    return c->streets;
}

HashTable city_getCeps(City city){

    if(city == NULL)
        return NULL;

    City_t* c = (City_t*) city;

    return c->ceps;
}

solution_t city_getShortestPath(City city){

    if(city == NULL)
        return NULL;

    City_t* c = (City_t*) city;

    return c->shortestPath;
}

solution_t city_getFastestPath(City city){

    if(city == NULL)
        return NULL;

    City_t* c = (City_t*) city;

    return c->fastestPath;
}

char* city_r_getCep(City city){

    if(city == NULL)
        return NULL;

    City_t* c = (City_t*) city;

    return c->r.cep;
}

double city_r_getX(City city){

    if(city == NULL)
        return -1;

    City_t* c = (City_t*) city;

    return city_getAddressX(city, c->r.cep, c->r.side, c->r.num);
}

double city_r_getY(City city){

    if(city == NULL)
        return -1;

    City_t* c = (City_t*) city;

    return city_getAddressY(city, c->r.cep, c->r.side, c->r.num);
}

double city_getAddressX(City city, char* cep, char side, int num){

    if(city == NULL)
        return -1;

    City_t* c = (City_t*) city;

    double x = 0,
           x_buffer = block_getX(hashtable_get(c->ceps, cep)),
           w = block_getW(hashtable_get(c->ceps, cep));

    if(x_buffer == -1)
        return -1;

    switch(side){

        case 'N':
           
            x = x_buffer + (double) num;

            break;

        case 'S':
            
            x = x_buffer + (double) num;
            
            break;

        case 'L':

            x = x_buffer;
            
            break;

        case 'O':

            x = x_buffer + w;
            
            break;

        default:
            break;
    }

    return x;
}

double city_getAddressY(City city, char* cep, char side, int num){

    if(city == NULL)
        return -1;

    City_t* c = (City_t*) city;

    double y = 0,
           y_buffer = block_getY(hashtable_get(c->ceps, cep)),
           h = block_getH(hashtable_get(c->ceps, cep));

    if(y_buffer == -1)
        return -1;

    switch(side){

        case 'N':
           
            y = y_buffer + h;

            break;

        case 'S':
            
            y = y_buffer; 

            break;

        case 'L':

            y = y_buffer + (double) num;

            break;

        case 'O':

            y = y_buffer + (double) num;

            break;

        default:
            break;
    }

    return y;
}

int city_r_getNum(City city){

    if(city == NULL)
        return -1;

    City_t* c = (City_t*) city;

    return c->r.num;
}

char city_r_getSide(City city){

    if(city == NULL)
        return -1;

    City_t* c = (City_t*) city;

    return c->r.side;
}

void city_setStreets(City city, unsigned int n){

    if(city == NULL)
        return;

    City_t* c = (City_t*) city;

    c->streets = newGraph(n);
}

void city_setCeps(City city, unsigned int n){

    if(city == NULL)
        return;

    City_t* c = (City_t*) city;

    c->ceps = newHashTable(n);    
}

void city_setR(City city, char* cep, char side, int num){

    if(city == NULL)
        return;

    City_t* c = (City_t*) city;

    snprintf(c->r.cep, sizeof(c->r.cep), "%s", cep);

    c->r.side = side;
    c->r.num = num;
}

void city_setShortestPath(City city, solution_t shortestPath){

    if(city == NULL)
        return;

    City_t* c = (City_t*) city;

    free(c->shortestPath);

    c->shortestPath = shortestPath;
}

void city_setFastestPath(City city, solution_t fastestPath){

    if(city == NULL)
        return;

    City_t* c = (City_t*) city;

    free(c->fastestPath);

    c->fastestPath = fastestPath;
}

void city_free(City city){

    if(city == NULL)
        return;

    City_t* c = (City_t*) city;

    tree_free(c->blocks, block_free);

    tree_free(c->crossings, NULL);

    hashtable_free(c->ceps, NULL);

    graph_free(c->streets);

    free(c->shortestPath);

    free(c->fastestPath);

    free(c);
}