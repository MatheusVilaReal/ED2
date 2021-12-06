#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "path.h"

char* getPath(char* dest, const char* dir, unsigned long rest){

    long pos = strlen(dir) - 1;

    while(pos >= 0 && dir[pos] != SLASH)
        --pos;

    if(pos < 2)
        return NULL;

    
    memset(dest, 0, rest);

    return strncpy(dest, dir, pos);
}

char* getFilename(char* dest, const char* dir, unsigned long rest){

    long pos = strlen(dir) - 1;

    while(pos >= 0 && dir[pos] != SLASH)
        --pos;

    

    
    

    memset(dest, 0, rest);

    
    

    return strncpy(dest, dir + pos + 1, strlen(dir) - pos);
}

char* getSuffix(char* dest, const char* filename, unsigned long rest){

    long pos = strlen(filename) - 1;

    while(pos >= 0 && filename[pos] != '.')
        --pos;

    

    
    

    memset(dest, 0, rest);

    
        

    return strncpy(dest, filename + pos + 1, strlen(filename) - pos);
}

char* catPathFile(char* dest, const char* dir, const char* filename, unsigned long rest){

    

    if(hasSlash(dir) || !strcmp(filename, ""))
        snprintf(dest, rest, "%s%s", dir, filename);
    else
        snprintf(dest, rest, "%s%c%s", dir, SLASH, filename);

    
        

    return dest;
}

char* addSuffix(char* dest, const char* filename, const char* suffix, unsigned long rest){

    bool hasPoint = 0;
    

    

    rmSuffix(dest, filename, rest);

    if(suffix[0] == '.')
        hasPoint = 1;

    if(!hasPoint)
        strncat(dest, ".", rest);

    return strncat(dest, suffix, rest);
}

char* rmSuffix(char* dest, const char* filename, unsigned long rest){

    long pos = strlen(filename) - 1;

    while(pos >= 0 && filename[pos] != '.')
        --pos;

    if(pos < 2)
        pos = strlen(filename);

    

    
    

    memset(dest, 0, rest);

    
        

    return strncpy(dest, filename, pos);
}

bool hasSlash(const char* path){

    return (path[strlen(path) - 1] == SLASH)? 1:0;
}

bool cmpSuffix(char* filename, const char* suffix){

    char toCmp[256];

    getSuffix(toCmp, filename, sizeof(toCmp));

    return !strcmp(toCmp, suffix);
}