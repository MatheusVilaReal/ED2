#ifndef BUILD_ENV_H
#define BUILD_ENV_H

#include "types.h"

typedef void* Env;

Env env_set(int argc, char* argv[]);

char* env_getGeoFile(Env env);

char* env_getBsd(Env env);

char* env_getQryFile(Env env);

char* env_getViaFile(Env env);

char* env_getBed(Env env);

char* env_getSvgPath(Env env);

char* env_getQrySvgPath(Env env);

char* env_solveSvgPath(Env env);

char* env_getTxtPath(Env env);

char* env_solveQrySvgPath(Env env);

char* env_solveTxtPath(Env env);

bool env_isQueryActive(Env env);

void env_free(Env env);

#endif