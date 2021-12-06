#include <stdlib.h>
#include <string.h>

#include "strinput.h"

char* fgetword(char* str, unsigned long buf_size, FILE* stream){

    unsigned long i = 0;
    char ch = 0;

    --buf_size;

    while((ch = fgetc(stream)) != EOF && !strchr(" ,:;\n\0", ch) && i < buf_size)
        str[i++] = ch;

    str[i] = '\0';

    return str;
}

char* getword(char* str, unsigned long buf_size){

    return fgetword(str, buf_size, stdin);
}