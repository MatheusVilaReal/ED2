#ifndef GETWORD_H
#define GETWORD_H

#include <stdio.h>

/* Lê uma palavra, não maior que buf_size, de stream e a escreve em str. */
char* fgetword(char* str, unsigned long buf_size, FILE* stream);

/* Lê uma palavra, não maior que buf_size, de stdin e a escreve em str. */
char* getword(char* str, unsigned long buf_size);

#endif