#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "get_args.h"
#include "path.h"
#include "environment.h"

/**************
*Parte privada*
**************/

typedef struct Environment{

    struct Required{

        char geoFile[PATH_MAX],
             bsd[PATH_MAX],
             svgPath[PATH_MAX],
             qrySvgPath[PATH_MAX],
             txtPath[PATH_MAX];
            

    } Required;

    struct Optional{

        char qryFile[PATH_MAX],
             pmFile[PATH_MAX],
             bed[PATH_MAX];

    } Optional;

} Env_t;

static bool setRequired(int argc, char* argv[], Env_t* env);

static void setOptional(int argc, char* argv[], Env_t* env);

static void setSvgPath(Env_t* env);

static void setTxtPath(Env_t* env);

/**************
*Parte pública*
**************/

Env setEnv(int argc, char* argv[]){

    Env_t* env = (Env_t*) malloc(sizeof(Env_t));
    
    setOptional(argc, argv, env);

    if(!setRequired(argc, argv, env)){
    
        freeEnv(env);

        return NULL;
    }

    setSvgPath(env);
    setTxtPath(env);

    srand(time(NULL));

    return env;
}

char* getGeoFile(Env env){

    if(env == NULL)
        return NULL;

    Env_t* toGetGeo = (Env_t*) env;

    return toGetGeo->Required.geoFile;
}

char* getBsd(Env env){

    if(env == NULL)
        return NULL;

    Env_t* toGetBsd = (Env_t*) env;

    return toGetBsd->Required.bsd;
}

char* getQryFile(Env env){

    if(env == NULL)
        return NULL;

    Env_t* toGetQry = (Env_t*) env;

    return toGetQry->Optional.qryFile;
}

char* getPmFile(Env env){

    if(env == NULL)
        return NULL;

    Env_t* toGetBed = (Env_t*) env;

    return toGetBed->Optional.pmFile;
}

char* getBed(Env env){

    if(env == NULL)
        return NULL;

    Env_t* toGetBed = (Env_t*) env;

    return toGetBed->Optional.bed;
}

char* getSvgPath(Env env){
    
    if(env == NULL)
        return NULL;

    Env_t* toGetSvg = (Env_t*) env;

    return toGetSvg->Required.svgPath;
}

char* getQrySvgPath(Env env){
    
    if(env == NULL)
        return NULL;

    Env_t* toGetSvg = (Env_t*) env;

    return toGetSvg->Required.qrySvgPath;
}

char* getTxtPath(Env env){
    
    if(env == NULL)
        return NULL;

    Env_t* toGetSvg = (Env_t*) env;

    return toGetSvg->Required.txtPath;
}

bool isQueryActive(Env env){

    if(env == NULL)
        return false;

    Env_t* this = (Env_t*) env;

    return strcmp(this->Optional.qryFile, this->Optional.bed);
}

void freeEnv(Env env){

    free(env);

    env = NULL;
}

/*************************************************************************************************/

static void setOptional(int argc, char* argv[], Env_t* env){
    
    int i = snprintf(env->Optional.bed, PATH_MAX, "%s", getArg(argc, argv, "e"));

    char* q = getArg(argc, argv, "q"), *pm = getArg(argc, argv, "pm");

    if(i)
        catPathFile(env->Optional.qryFile, env->Optional.bed, q, PATH_MAX);
    else
        
        snprintf(env->Optional.qryFile, PATH_MAX, "%s", q);

    catPathFile(env->Optional.pmFile, env->Optional.bed, pm, PATH_MAX);

}

static bool setRequired(int argc, char* argv[], Env_t* env){

    snprintf(env->Required.bsd, PATH_MAX, "%s", getArg(argc, argv, "o"));

    char* f = getArg(argc, argv, "f");

    catPathFile(env->Required.geoFile, env->Optional.bed, f, PATH_MAX);

    return (env->Required.geoFile && env->Required.bsd);
}

static void setSvgPath(Env_t* env){

    char svgName[PATH_MAX],
         geoName[PATH_MAX],
         svgNameSuffixless[PATH_MAX / 2],
         svgQryName[PATH_MAX],
         aaa[PATH_MAX / 2],
         bbb[PATH_MAX],
         qryName[PATH_MAX];

    getFilename(geoName, env->Required.geoFile, PATH_MAX);

    rmSuffix(svgNameSuffixless, geoName, PATH_MAX);

    getFilename(qryName, env->Optional.qryFile, PATH_MAX);

    rmSuffix(svgQryName, qryName, PATH_MAX);

    addSuffix(aaa, svgQryName, ".svg", PATH_MAX);

    snprintf(bbb, PATH_MAX, "%s-%s", svgNameSuffixless, aaa);

    catPathFile(env->Required.qrySvgPath, env->Required.bsd, bbb, PATH_MAX);

    addSuffix(svgName, svgNameSuffixless, ".svg", PATH_MAX);

    catPathFile(env->Required.svgPath, env->Required.bsd, svgName, PATH_MAX);
}

static void setTxtPath(Env_t* env){

    char geoName[PATH_MAX],
         qryName[PATH_MAX],
         txtName[PATH_MAX / 2],
         aaa[PATH_MAX],
         bbb[PATH_MAX],
         qryNameSuffixless[PATH_MAX / 2];

    getFilename(geoName, env->Required.geoFile, PATH_MAX);

    rmSuffix(txtName, geoName, PATH_MAX);

    getFilename(qryName, env->Optional.qryFile, PATH_MAX);

    rmSuffix(qryNameSuffixless, qryName, PATH_MAX);

    snprintf(aaa, PATH_MAX, "%s-%s", txtName, qryNameSuffixless);

    addSuffix(bbb, aaa, ".txt", PATH_MAX);

    catPathFile(env->Required.txtPath, env->Required.bsd, bbb, PATH_MAX);
}
