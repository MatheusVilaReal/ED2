#include <stdlib.h>
#include <string.h>

#include "strinput.h"

#define BASE_SIZE 2UL

char* getword(char* destination, FILE* stream){

    char ch;
    unsigned long i = 0, size = BASE_SIZE;

    char* str = (char*) malloc(sizeof(char) * size);

    if(str == NULL) 
        return str;

    while((ch = fgetc(stream)) != EOF && !strchr(" ,:;\n\0", ch)){

        str[i++] = ch;

        if(i == size){

            str = realloc(str, sizeof(char) * (size *= i));

            if(str == NULL)
                return str;
        }
    }

    str[i++] = '\0';

    if(destination)
        free(destination);

    return realloc(str, sizeof(char)*i);
}