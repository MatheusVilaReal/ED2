#ifndef BUILD_ENV_H
#define BUILD_ENV_H

#include "types.h"

Env setEnv(int argc, char* argv[]);

char* getGeoFile(Env env);

char* getBsd(Env env);

char* getQryFile(Env env);

char* getPmFile(Env env);

char* getBed(Env env);

char* getSvgPath(Env env);

char* getQrySvgPath(Env env);

char* solveSvgPath(Env env);

char* getTxtPath(Env env);

char* solveQrySvgPath(Env env);

char* solveTxtPath(Env env);

bool isQueryActive(Env env);

void freeEnv(Env env);

#endif