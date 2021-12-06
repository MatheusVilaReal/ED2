#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

#define MAGIC_HASH 5381UL

/**************
*Parte privada*
**************/

typedef struct Node{

    char* key;
    Object o;

    struct Node* next;

} Node_t;

typedef struct HashTable{

    unsigned long length;

    unsigned int size;

    Node_t* nodeArray[];

} Hash_t;

static Node_t* newNode(char* key, Object o);

static void freeNode(Node node, Destroyer_t o_free);

/**************
*Parte pública*
**************/

HashTable newHashTable(unsigned int n){

    Hash_t* hashTable = (Hash_t*) malloc(sizeof(Hash_t) + sizeof(Node_t*) * n);

    hashTable->size = n;
    hashTable->length = 0;

    for(unsigned long i = 0; i < n; i++)
        hashTable->nodeArray[i] = NULL;

    return hashTable;
}

Iterator hashtable_it_start(HashTable table){

    Hash_t* t = (Hash_t*) table;
    Node_t* it = t->nodeArray[0];

    unsigned int index = 0;

    while(it == NULL && ++index < t->size){

        it = t->nodeArray[index];
    }

    return it;
}

Iterator hashtable_it_next(HashTable table, Iterator iterator){

    if(iterator == NULL || table == NULL)
        return NULL;

    Hash_t* t = (Hash_t*) table;
    Node_t* it = (Node_t*) iterator;

    unsigned int index = hashCode(it->key, t->size);

    if(it->next)
        return it->next;
    else{

        while(++index < t->size){

            it = t->nodeArray[index];

            if(it != NULL)
                return it;
        }
    }

    return NULL;
}

Object* hashtable_toArray(HashTable table){

    if(table == NULL)
        return NULL;

    Hash_t* t = (Hash_t*) table;
    Object* array = (Object*) calloc(sizeof(Object), t->length + 1);

    unsigned long index = 0;

    for(Iterator it = hashtable_it_start(t); it != NULL; it = hashtable_it_next(t, it))
        array[index++] = ((Node_t*) it)->o;

    return array;
}

Object hashtable_get(HashTable table, char* key){

    if(key == NULL)
        return NULL;

    Hash_t* hashTable = (Hash_t*) table;

    Node_t* buffer = hashTable->nodeArray[hashCode(key, hashTable->size)];

    while(buffer && strcmp(key, buffer->key))
        buffer = buffer->next;

    if(buffer)
        return buffer->o;
    else
        return NULL;
}

Object hashtable_it_getObject(Iterator it){

    if(it == NULL)
        return NULL;

    Node_t* this = (Node_t*) it;

    return this->o;
}

char* hashtable_getKey(Node node){

    if(node == NULL)
        return NULL;

    Node_t* this = (Node_t*) node;

    return this->key;
}

unsigned int hashtable_size(HashTable table){

    if(table == NULL)
        return 0;

    Hash_t* t = (Hash_t*) table;

    return t->size;
}

unsigned int hashCode(char* key, unsigned long size){

    if(key == NULL)
        return 0;

    unsigned long hash = MAGIC_HASH;
    char ch;

    while((ch = *key++))
        hash = ((hash << 5) + hash) + ch;

    return (hash % size);
}

void hashtable_add(HashTable table, char* key, Object o){

    if(table == NULL)
        return;

    Hash_t* hashTable = (Hash_t*) table;
    Node_t* toAdd = (Node_t*) newNode(key, o),
          * buffer;

    buffer = hashTable->nodeArray[hashCode(toAdd->key, hashTable->size)];

    if(buffer == NULL)
        hashTable->nodeArray[hashCode(toAdd->key, hashTable->size)] = toAdd;
        
    else{

        while(buffer->next)
            buffer = buffer->next;

        buffer->next = toAdd;
    }

    ++hashTable->length;
}

void hashtable_remove(HashTable table, char* key, Destroyer_t o_free){

    Hash_t* hashTable = (Hash_t*) table;

    Node_t* buffer = hashTable->nodeArray[hashCode(key, hashTable->size)],
          * buffer_2 = NULL;

    while(buffer && strcmp(key, buffer->key)){

        buffer_2 = buffer;
        buffer = buffer->next;
    }

    if(buffer == NULL)
        return;

    if(buffer_2)
        buffer_2->next = buffer->next;
    else
        hashTable->nodeArray[hashCode(key, hashTable->size)] = buffer->next;

    freeNode(buffer, o_free);
}

void hashtable_print(HashTable table, Printer_t o_print){

    if(table == NULL)
        return;

    Node_t* buffer = NULL;
    Hash_t* hashTable = (Hash_t*) table;

    for(unsigned int i = 0; i < hashTable->size; i++){
        
        printf("[%u] -> ", i);

        buffer = hashTable->nodeArray[i];

        while(buffer){

            puts("{ ");

            if(o_print)
                o_print(buffer->o);
            else
                printf("%s", buffer->key);

            puts(" }, ");

            buffer = buffer->next;
        }

        printf("\n");
    }
}

void hashtable_free(HashTable table, Destroyer_t o_free){

    Hash_t* hashTable = (Hash_t*) table;
    Node_t* node, *buffer;

    for(unsigned long i = 0; i < hashTable->size; i++){

        node = hashTable->nodeArray[i];

        while(node){

            buffer = node->next;

            freeNode(node, o_free);

            node = buffer;
        }
    }

    free(hashTable);
}

/****************************************************************************/

static Node_t* newNode(char* key, Object o){

    Node_t* node = (Node_t*) malloc(sizeof(Node_t));

    node->key = (char*) calloc(strlen(key) + 1, sizeof(char));
    node->key = strncpy(node->key, key, strlen(key) + 1);

    node->o = o;

    node->next = NULL;

    return node;
}

static void freeNode(Node node, Destroyer_t o_free){

    Node_t* toFree = (Node_t*) node;

    free(toFree->key);

    if(o_free)
        o_free(toFree->o);

    free(toFree);
}