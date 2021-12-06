#include <stdlib.h>

#include "hashtable.h"
#include "city.h"
#include "avltree.h"
#include "list.h"
#include "buildings.h"

/**************
*Parte privada*
**************/

typedef struct City{

    // Tree blocks;
    // HashTable cepMap;
    // HashTable inhabitants;
    // HashTable tenancies;

} City_t;

/**************
*Parte pública*
**************/

City newCity(){

    City_t* city = (City_t*) malloc(sizeof(City_t));

    // city->blocks = newTree();
    // city->cepMap = newHashTable();
    // city->inhabitants = newHashTable();
    // city->tenancies = newHashTable();

    return city;
}

void destroyCity(City city){

    if(city == NULL)
        return;

    City_t* toDestroy = (City_t*) city;

    // chopTree(toDestroy->blocks, freeList, dummy);
    // freeHashTable(toDestroy->tenancies, dummy);
    // freeHashTable(toDestroy->inhabitants, freePerson);
    // freeHashTable(toDestroy->cepMap, freeBlock);

    free(toDestroy);
}