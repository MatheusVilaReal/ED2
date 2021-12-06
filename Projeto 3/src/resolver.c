#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "regionmath.h"
#include "hashtable.h"
#include "list.h"
#include "city.h"
#include "environment.h"
#include "avltree.h"
#include "buildings.h"
#include "strinput.h"
#include "path.h"
#include "svg.h"
#include "qry.h"
#include "resolver.h"

#define BASE_SIZE_A 2
#define OPACITY 0.8F
#define ASSINATURA "\n###################################################\naluno: MATHEUS PIRES VILA REAL\t\tn° 202000560352"

/**************
*Parte privada*
**************/

static void drawRectBranch(Node node, Svg svg);

/**************
*Parte pública*
**************/

int solveGeo(City city, Env env){

    FILE* geo = fopen(getGeoFile(env), "r");

    if(geo == NULL){

        fprintf(stderr, "ERRO\nNao foi possivel abrir arquivo geo.\nVerifique o diretorio especificado e tente novamente.\n");
    
        return -1;
    }

    Node buffer = NULL;
    char* word = NULL, *cep = NULL;

    char def_rectFill[20], def_rectStroke[20];
    double def_rectSW = 0;

    double x = 0, y = 0, w = 0, h = 0;

    while((word = getword(word, geo)) && strcmp(word, "")){

        if(strcmp(word, "cq") == 0){
            
            fscanf(geo, " %lfpx %s %s ", &def_rectSW, def_rectFill, def_rectStroke);

        } else if(strcmp(word, "q") == 0){

            cep = getword(cep, geo);

            fscanf(geo, " %lf %lf %lf %lf ", &x, &y, &w, &h);

            Block toInsert = newBlock(cep, x, y, w, h, def_rectSW, def_rectFill, def_rectStroke);

            if((buffer = searchTree(getCityBlocks(city), x))){
                
                List nodeList = getNodeData(buffer);
                double maxX = x, largestListX = maxX;
                Node node = getHead(nodeList);

                while(node){

                    largestListX = getBlockX(getListNodeData(node)) + getBlockW(getListNodeData(node));

                    if(largestListX > maxX)
                        maxX = largestListX;

                    node = getNext(node);
                }

                appendList(nodeList, toInsert);

                setNodeMaxX(buffer, largestListX);

            } else{

                List newEntry = newList();
                appendList(newEntry, toInsert);

                insertNode(getCityBlocks(city), newEntry, x, x + w, x);
            }

            insertHashNode(getCityCepMap(city), getBlockCep(toInsert), toInsert);

            buffer = NULL;
        }
    }

    free(cep);  
    free(word);

    fclose(geo);

    return 0;
}

int solvePm(City city, Env env){

    FILE* pm = fopen(getPmFile(env), "r");

    if(pm == NULL){

        fprintf(stderr, "ERRO\nNao foi possivel abrir arquivo pm.\nVerifique o diretorio especificado e tente novamente.\n");
    
        return -1;
    }

    char cpf[15] = {""}, name[50] = {""}, surname[50] = {""}, bdate[11] = {""}, cep[50] = {""},
         num[10] = {""}, compl[15] = {""};
    char ch = 0, side = 0, sex = 'M';

    while((ch = fgetc(pm)) && ch != EOF){

        switch(ch){
            
            case 'p':

                fscanf(pm, " %s %s %s %c %s ", cpf, name, surname, &sex, bdate);

                insertHashNode(getCityInhabitants(city), cpf, newPerson(cpf, name, surname, sex, bdate));

                break;

            case 'm':

                fscanf(pm, " %s %s %c %s %s ", cpf, cep, &side, num, compl);

                insertPerson(searchKey(getCityInhabitants(city), cpf), searchKey(getCityCepMap(city), cep), side, num, compl);

                break;

            default:
                break;
        }
    }

    fclose(pm);

    return 0;
}

int solveSvg(City city, Svg svgFile, Svg querries){

    if(svgFile){
    
        drawRectBranch(getRoot(getCityBlocks(city)), svgFile);

        if(querries != NULL)
            catSvg(svgFile, querries);

    } else return -1;

    return 0;
}

int solveQry(City city, Svg querries, Env env){

    char* qryPath = getQryFile(env);

    FILE* qry = fopen(qryPath, "r");

    if(qry == NULL){

        fprintf(stderr, "ERRO\nNao foi possivel abrir arquivo de consulta.\nVerifique o diretorio especificado e tente novamente.\n");

        return -1;
    }

    char* txtPath = getTxtPath(env), *word = NULL;

    double x = 0, y = 0, w = 0, h = 0, area = 0, price = 0;
    
    char cpf[15] = {""}, cep[50] = {""}, num[10] = {""}, 
         compl[15] = {""}, id[50] = {""}, sfx[50] = {""},
         side = '\0';

    FILE* txt = fopen(txtPath, "w+");

    if(txt == NULL){

        fprintf(stderr, "ERRO\nNao foi possivel gerar arquivo de texto.\nVerifique o diretorio especificado e tente novamente.\n");

        return -1;
    }

    while((word = getword(word, qry)) && strcmp(word, "\0")){

        if(strcmp(word, "oloc?") == 0){

            fscanf(qry, " %lf %lf %lf %lf ", &x, &y, &w, &h);

            olocP(getCityBlocks(city), querries, txt, x, y, w, h);

        } else if(strcmp(word, "oloc") == 0){

            fscanf(qry, " %s %s %c %s %s %lf %lf ", id, cep, &side, num, compl, &area, &price);

            oloc(getCityCepMap(city), getCityTenancies(city), id, cep, side, num, compl, area, price);
        
        } else if(strcmp(word, "hom") == 0){

            fscanf(qry, " %lf %lf %lf %lf ", &x, &y, &w, &h);

            hom(getCityBlocks(city), querries, txt, x, y, w, h);
        
        } else if(strcmp(word, "mul") == 0){

            fscanf(qry, " %lf %lf %lf %lf ", &x, &y, &w, &h);

            mul(getCityBlocks(city), querries, txt, x, y, w, h);
        
        } else if(strcmp(word, "del") == 0){

            fscanf(qry, " %s ", cep);

            del(getCityBlocks(city), getCityCepMap(city), getCityInhabitants(city), querries, txt, cep);
        
        } else if(strcmp(word, "m?") == 0){

            fscanf(qry, " %s ", cep);

            mP(getCityCepMap(city), txt, cep); 

        } else if(strcmp(word, "dm?") == 0){

            fscanf(qry, " %s ", cpf);

            dmP(getCityInhabitants(city), txt, cpf);
        
        } else if(strcmp(word, "mud") == 0){

            fscanf(qry, " %s %s %c %s %s ", cpf, cep, &side, num, compl);

            mud(getCityCepMap(city), getCityInhabitants(city), querries, txt, cpf, cep, side, num, compl);
        
        } else if(strcmp(word, "loc") == 0){

            fscanf(qry, " %s %s ", id, cpf);

            loc(getCityTenancies(city), getCityInhabitants(city), querries, txt, id, cpf);
        
        } else if(strcmp(word, "dloc") == 0){

            fscanf(qry, " %s ", id);

            dloc(getCityTenancies(city), querries, txt, id);
        
        } else if(strcmp(word, "loc?") == 0){

            fscanf(qry, " %s ", id);

            locP(getCityTenancies(city), querries, txt, id);
        
        } else if(strcmp(word, "catac") == 0){

            fscanf(qry, " %lf %lf %lf %lf ", &x, &y, &w, &h);

            catac(getCityBlocks(city), getCityCepMap(city), getCityInhabitants(city),
                  querries, txt, x, y, w, h);
        
        } else if(strcmp(word, "dmpt") == 0){

            char path[PATH_MAX / 2],
                 pathSuffix[PATH_MAX],
                 dotPath[PATH_MAX];


            fscanf(qry, " %s ", sfx);

            fprintf(txt, "dmpt %s\n", sfx);

            rmSuffix(path, getQrySvgPath(env), PATH_MAX);

            snprintf(pathSuffix, PATH_MAX, "%s-%s", path, sfx);

            addSuffix(dotPath, pathSuffix, ".dot", PATH_MAX);

            dmpt(getCityBlocks(city), getGeoFile(env), getPmFile(env), getQryFile(env), dotPath);

            fputc('\n', txt);
        }
    }

    fputs(ASSINATURA, txt);

    free(word);

    fclose(qry);
    fclose(txt);

    return 0;
}

/*************************************************************************************************/

static void drawRectBranch(Node node, Svg svg){

    if(node == NULL)
        return;

    Block block = NULL;
    Node listNode = getHead(getNodeData(node));

    while(listNode){

        block = getListNodeData(listNode);

        drawRect(svg, getBlockX(block), getBlockY(block), getBlockW(block), getBlockH(block),
                 getBlockFillColor(block), OPACITY, getBlockStrokeColor(block), OPACITY, getBlockSW(block),
                 false);

        listNode = getNext(listNode);
    }

    drawRectBranch(getLeftChild(node), svg);
    drawRectBranch(getRightChild(node), svg);
}