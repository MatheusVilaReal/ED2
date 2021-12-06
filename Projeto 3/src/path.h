#ifndef PATH_H
#define PATH_H

#include <linux/limits.h>

#include "types.h"

#define SLASH '/'

char* getPath(char* dest, const char* dir, unsigned long rest);

char* getFilename(char* dest, const char* dir, unsigned long rest);

char* getSuffix(char* dest, const char* filename, unsigned long rest);

char* catPathFile(char* dest, const char* dir, const char* filename, unsigned long rest);

char* addSuffix(char* dest, const char* filename, const char* suffix, unsigned long rest);

char* rmSuffix(char* dest, const char* filename, unsigned long rest);

int setRequiredParams(int argc, char** argv, char** bsd, char** geoName);

bool cmpSuffix(char* filename, const char* suffix);

bool hasSlash(const char* path);

#endif