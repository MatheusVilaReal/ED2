#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buildings.h"
#include "avltree.h"
#include "list.h"
#include "hashtable.h"
#include "qry.h"
#include "svg.h"
#include "regionmath.h"
#include "dot.h"

#define BLUE "#0099FF"
#define DARK_BLUE "#000066"
#define PINK "#FF33CC"
#define DARK_PINK "#660033"
#define LAVENDER "#AB37C8"
#define DARK_LAVENDER "#AA0044"

/**************
*Parte privada*
**************/

static int distanceFromBorder = 1;          /* Buffer de espaçamento para o texto não *
                                             *          ficar amontoado               */

static Node util_searchCepOnTree(Node node, char* cep);

static Node searchCepOnTree(Tree tree, char* cep);

static Data searchCepOnList(List list, char* cep);

static void freeFromHashTable(HashTable table, HashTable toDel, void (*freeData)(void*));

static void hashTableIntoFile(FILE* stream, HashTable table, char* (*packager_f)(void*));

static void getFullyInternalBlocks(List blocks, List blocklist, double x, double y, double w, double h);

static void getInternalBlocks(List blocks, List blocklist, double x, double y, double w, double h);

static void spaceSearch_full(Node node, List blockList, double x, double y, double w, double h);

static void spaceSearch(Node node, List blockList, double x, double y, double w, double h);

static void dummy(void* ptr){}

/**************
*Parte pública*
**************/

void oloc(HashTable blocks, HashTable tenancies, char* id, char* cep, char side, char* num,
          char* compl, double area, double price){

    Block block = searchKey(blocks, cep);
    Tenancy tenancy = newTenancy(id, cep, side, num, compl, area, price);

    insertTenancy(tenancy, block);

    insertHashNode(tenancies, id, tenancy);
}

void olocP(Tree tree, Svg qrySvg, FILE* txt, double x, double y, double w, double h){

    List blocksInsideRegion = newList();
    Node buffer = NULL, *buffer_2 = NULL, buffer_3 = NULL;
    HashTable tenancies = NULL;

    fprintf(txt, "oloc? %lf %lf %lf %lf\n", x, y, w, h);

    drawRect(qrySvg, x, y, w, h, "transparent", 0, "red", 0.8, 20, 30);

    spaceSearch(getRoot(tree), blocksInsideRegion, x, y, w, h);

    buffer = getHead(blocksInsideRegion);

    while(buffer){

        tenancies = getBlockTenancies(getListNodeData(buffer));

        buffer_2 = getNodeArray(tenancies);

        for(unsigned int i = 0; i < TABLE_SIZE; i++){

            buffer_3 = buffer_2[i];

            while(buffer_3){
                if(isPointInternal(getTenancyX(getHashNodeData(buffer_3)), getTenancyY(getHashNodeData(buffer_3)), x, y, w, h)){

                    fprintf(txt, "id: %s cep: %s face: %c num: %s cpl: %s ar: %lf v: %lf\n",
                            getTenancyId(getHashNodeData(buffer_3)), getTenancyCep(getHashNodeData(buffer_3)),
                            getTenancySide(getHashNodeData(buffer_3)), getTenancyNum(getHashNodeData(buffer_3)),
                            getTenancyCompl(getHashNodeData(buffer_3)), getTenancyArea(getHashNodeData(buffer_3)),
                            getTenancyPrice(getHashNodeData(buffer_3)));

                    drawText(qrySvg, "*", getTenancyX(getHashNodeData(buffer_3)), getTenancyY(getHashNodeData(buffer_3)) + 20,
                            40, "white", 1, "black", 0.8, 1);
                }
                
                buffer_3 = getHashNodeNext(buffer_3);    
            }
        }

        buffer = getNext(buffer);
    }

    fputc('\n', txt);

    freeList(blocksInsideRegion, dummy);
}

void hom(Tree tree, Svg qrySvg, FILE* txt, double x, double y, double w, double h){

    List blocksInsideRegion = newList();
    Node buffer = NULL, *buffer_2 = NULL, buffer_3 = NULL;
    HashTable residents = NULL;

    fprintf(txt, "hom %lf %lf %lf %lf\n", x, y, w, h);

    drawRect(qrySvg, x, y, w, h, "transparent", 0, "red", 0.8, 20, 30);

    spaceSearch(getRoot(tree), blocksInsideRegion, x, y, w, h);

    buffer = getHead(blocksInsideRegion);

    while(buffer){

        residents = getBlockResidents(getListNodeData(buffer));

        buffer_2 = getNodeArray(residents);

        for(unsigned int i = 0; i < TABLE_SIZE; i++){

            buffer_3 = buffer_2[i];

            while(buffer_3){
                if(isPointInternal(getPersonX(getHashNodeData(buffer_3)), getPersonY(getHashNodeData(buffer_3)), x, y, w, h) &&
                   getPersonSex(getHashNodeData(buffer_3)) == 'M'){

                    fprintf(txt, "cpf: %s nome: %s sobrenome: %s sexo: %c nasc: %s\n"
                                "cep: %s face: %c num: %s compl: %s",
                            getPersonCpf(getHashNodeData(buffer_3)), getPersonName(getHashNodeData(buffer_3)),
                            getPersonSurname(getHashNodeData(buffer_3)), getPersonSex(getHashNodeData(buffer_3)),
                            getPersonBirthdate(getHashNodeData(buffer_3)), getPersonCep(getHashNodeData(buffer_3)),
                            getPersonSide(getHashNodeData(buffer_3)), getPersonNum(getHashNodeData(buffer_3)),
                            getPersonCompl(getHashNodeData(buffer_3)));

                    drawEllipse(qrySvg, getPersonX(getHashNodeData(buffer_3)), getPersonY(getHashNodeData(buffer_3)), 15, 7.5,
                                BLUE, 0.8, DARK_BLUE, 0.8, 3, 0);
                }

                buffer_3 = getHashNodeNext(buffer_3);
            }
        }

        buffer = getNext(buffer);
    }

    fputc('\n', txt);

    freeList(blocksInsideRegion, dummy);
}

void mul(Tree tree, Svg qrySvg, FILE* txt, double x, double y, double w, double h){

    List blocksInsideRegion = newList();
    Node buffer = NULL, *buffer_2 = NULL, buffer_3 = NULL;
    HashTable residents = NULL;

    fprintf(txt, "mul\n %lf %lf %lf %lf\n", x, y, w, h);

    drawRect(qrySvg, x, y, w, h, "transparent", 0, "red", 0.8, 20, 30);

    spaceSearch(getRoot(tree), blocksInsideRegion, x, y, w, h);

    buffer = getHead(blocksInsideRegion);

    while(buffer){

        residents = getBlockResidents(getListNodeData(buffer));

        buffer_2 = getNodeArray(residents);

        for(unsigned int i = 0; i < TABLE_SIZE; i++){

            buffer_3 = buffer_2[i];

            while(buffer_3){
                if(isPointInternal(getPersonX(getHashNodeData(buffer_3)), getPersonY(getHashNodeData(buffer_3)), x, y, w, h) &&
                   getPersonSex(getHashNodeData(buffer_3)) == 'F'){

                    fprintf(txt, "cpf: %s nome: %s sobrenome: %s sexo: %c nasc: %s\n"
                                "cep: %s face: %c num: %s compl: %s",
                            getPersonCpf(getHashNodeData(buffer_3)), getPersonName(getHashNodeData(buffer_3)),
                            getPersonSurname(getHashNodeData(buffer_3)), getPersonSex(getHashNodeData(buffer_3)),
                            getPersonBirthdate(getHashNodeData(buffer_3)), getPersonCep(getHashNodeData(buffer_3)),
                            getPersonSide(getHashNodeData(buffer_3)), getPersonNum(getHashNodeData(buffer_3)),
                            getPersonCompl(getHashNodeData(buffer_3)));

                    drawEllipse(qrySvg, getPersonX(getHashNodeData(buffer_3)), getPersonY(getHashNodeData(buffer_3)), 15, 7.5,
                                PINK, 0.8, DARK_PINK, 0.8, 3, 0);
                }

                buffer_3 = getHashNodeNext(buffer_3);
            }
        }

        buffer = getNext(buffer);
    }

    fputc('\n', txt);

    freeList(blocksInsideRegion, dummy);
}

void del(Tree tree, HashTable ceps, HashTable inhabitants, Svg qrySvg, FILE* txt, char* cep){

    fprintf(txt, "del %s\n", cep);

    Node nodeToDel = searchCepOnTree(tree, cep);

    if(nodeToDel == NULL){

        fputs("ERRO: quadra não encontrada.\n", txt);

        return;
    }
    
    Block toDel = searchKey(ceps, cep);
    
    drawLine(qrySvg, getBlockX(toDel) + getBlockW(toDel) / 2, getBlockY(toDel) + getBlockH(toDel) / 2,
             getBlockX(toDel) + getBlockW(toDel) / 2, 0, 5, "red");
    drawText(qrySvg, getBlockCep(toDel), 15 + getBlockX(toDel) + getBlockW(toDel) / 2, 40, 50, "white", 0.8, "black", 0.8, 2.5);

    fputs("locações: ", txt);
    hashTableIntoFile(txt, getBlockTenancies(toDel), packTenancy);

    fputs("residentes: ", txt);
    hashTableIntoFile(txt, getBlockResidents(toDel), packPerson);

    fputc('\n', txt);

    delNode(ceps, getBlockCep(toDel), dummy);
    freeFromHashTable(getBlockResidents(toDel), inhabitants, freePerson);

    if(getListSize(getNodeData(nodeToDel)) > 1)
        deleteNode(getNodeData(nodeToDel), toDel, freeBlock);
    else
        delTreeNode(tree, getNodeKey(nodeToDel), freeBlock);
}

void mP(HashTable ceps, FILE* txt, char* cep){

    Block block = searchKey(ceps, cep);

    fprintf(txt, "m? %s\n", cep);

    if(block)
        hashTableIntoFile(txt, getBlockResidents(block), packPerson);
    else
        fputs("ERRO: quadra não encontrada\n\n", txt);

    fputc('\n', txt);
}

void dmP(HashTable inhabitants, FILE* txt, char* cpf){

    Person person = searchKey(inhabitants, cpf);

    fprintf(txt, "dm? %s\n", cpf);

    if(person)
        fprintf(txt, "%s", packPerson(person));
    else
        fputs("ERRO: pessoa não encontrada\n\n", txt);

    fputc('\n', txt);
}

void mud(HashTable ceps, HashTable inhabitants, Svg qrySvg, FILE* txt, char* cpf, char* cep, char side,
         char* num, char* compl){

    fprintf(txt, "mud %s %s %c %s %s\n", cpf, cep, side, num, compl);

    Person person = searchKey(inhabitants, cpf);

    if(person == NULL){

        fputs("ERRO: pessoa não encontrada\n\n", txt);

        return;
    }

    Block oldHome = searchKey(ceps, getPersonCep(person)),
          newHome = searchKey(ceps, cep);

    double newX = 0,
           newY = 0,
           n = atof(num);

    switch(side){

        case 'N':
           
            newX = getBlockX(newHome) + n;
            newY = getBlockY(newHome) + getBlockH(newHome);

            break;

        case 'S':
            
            newX = getBlockX(newHome) + n;
            newY = getBlockY(newHome); 

            break;

        case 'L':

            newX = getBlockX(newHome);
            newY = getBlockY(newHome) + n;

            break;

        case 'O':

            newX = getBlockX(newHome) + getBlockW(newHome);
            newY = getBlockY(newHome) + n;

            break;

        default:
            break;
    }

    drawCircle(qrySvg, getPersonX(person), getPersonY(person), 10, "red", 0.8, "white", 0.8, 5, 0);

    drawCircle(qrySvg, newX, newY, 10, "blue", 0.8, "white", 0.8, 5, 0);

    drawLine(qrySvg, getPersonX(person), getPersonY(person), newX, newY, 5, "red");

    char* address = genAddress_short(getPersonSide(person), getPersonNum(person), getPersonCompl(person));

    delNode(getBlockResidents(oldHome), address, dummy);

    free(address);

    char* package = packPerson(person);

    fprintf(txt, "morador: %s", package);
    fprintf(txt, "endereço antigo - cep: %s lado: %c num: %s compl: %s\n", getPersonCep(person), getPersonSide(person),
                                                                           getPersonNum(person), getPersonCompl(person));
    fprintf(txt, "endereço novo - cep: %s lado: %c num: %s compl: %s\n", cep, side, num, compl);

    fputc('\n', txt);

    setPersonCep(person, cep);
    setPersonSide(person, side);
    setPersonNum(person, num);
    setPersonCompl(person, compl);

    insertPerson(person, newHome, side, num, compl);

    free(package);
}

void loc(HashTable tenancies, HashTable inhabitants, Svg qrySvg, FILE* txt, char* id, char* cpf){

    Tenancy destiny = searchKey(tenancies, id);
    Person tenant = searchKey(inhabitants, cpf);

    char* pkgTenancy = packTenancy(destiny),
        * pkgTenant = packPerson(tenant);

    drawLine(qrySvg, getTenancyX(destiny), getTenancyY(destiny), getTenancyX(destiny), 0, 5, "red");

    drawText(qrySvg, pkgTenancy, getTenancyX(destiny) + 15, 40 * distanceFromBorder, 50, "white", 0.8, "black", 0.8, 2.5);
    drawText(qrySvg, pkgTenant, getTenancyX(destiny) + 15, 40 * distanceFromBorder + 50, 50, "white", 0.8, "black", 0.8, 2.5);

    distanceFromBorder += 3;

    fprintf(txt, "loc %s %s\n", id, cpf);

    fprintf(txt, "%s", pkgTenancy);
    
    if(tenant)
        fprintf(txt, "%s", pkgTenant);
    
    fputc('\n', txt);

    free(pkgTenancy);
    free(pkgTenant);
}

void dloc(HashTable tenancies, Svg qrySvg, FILE* txt, char* id){

    Tenancy tenancy = searchKey(tenancies, id);

    char* pkgTenancy = packTenancy(tenancy);

    drawLine(qrySvg, getTenancyX(tenancy), getTenancyY(tenancy), getTenancyX(tenancy), 0, 5, "red");

    drawText(qrySvg, pkgTenancy, getTenancyX(tenancy) + 15, 40 * distanceFromBorder++, 50, "white", 0.8, "black", 0.8, 2.5);

    fprintf(txt, "dloc %s\n", id);

    fprintf(txt, "%s", pkgTenancy);

    fputc('\n', txt);

    free(pkgTenancy);

    setTenancyArea(tenancy, 0);
    setTenancyPrice(tenancy, 0);
}

void locP(HashTable tenancies, Svg qrySvg, FILE* txt, char* id){

    Tenancy tenancy = searchKey(tenancies, id);

    char* pkgTenancy = packTenancy(tenancy);

    fprintf(txt, "loc? %s\n", id);

    if(getTenancyArea(tenancy) == 0 && getTenancyPrice(tenancy) == 0){
        
        drawText(qrySvg, "#", getTenancyX(tenancy), getTenancyY(tenancy), 40, "white", 1, "black", 0.8, 1);

        fprintf(txt, "[REMOVIDA ou INEXISTENTE]\n", id);
    
    } else if(getTenancyTenant(tenancy) == NULL){
         
            drawText(qrySvg, "$", getTenancyX(tenancy), getTenancyY(tenancy), 40, "white", 1, "black", 0.8, 1);
        
            fprintf(txt, "[DISPONÍVEL] %s", pkgTenancy);

        } else{
         
            drawText(qrySvg, "*", getTenancyX(tenancy), getTenancyY(tenancy), 40, "white", 1, "black", 0.8, 1);

            Person person = getTenancyTenant(tenancy);

            char* pkgPerson = packPerson(person);

            fprintf(txt, "[LOCADA] %s", pkgTenancy);
            fprintf(txt, "%s", pkgPerson);

            free(pkgPerson);
        }

    fputc('\n', txt);

    free(pkgTenancy);
}

void catac(Tree tree, HashTable ceps, HashTable inhabitants, Svg qrySvg,
           FILE* txt, double x, double y, double w, double h){
    
    List blocksInsideRegion = newList();
    Block block = NULL;
    Node buffer = NULL, nodeToDel = NULL;

    char* pkgBlock = NULL;

    fprintf(txt, "catac %lf %lf %lf %lf\n", x, y, w, h);

    drawRect(qrySvg, x, y, w, h, LAVENDER, 0.5, DARK_LAVENDER, 0.5, 3, 0);

    spaceSearch_full(getRoot(tree), blocksInsideRegion, x, y, w, h);

    buffer = getHead(blocksInsideRegion);

    while(buffer){

        block = getListNodeData(buffer);
        nodeToDel = searchTree(tree, getBlockX(block));

        pkgBlock = packBlock(block);
        fprintf(txt, "%s\n", pkgBlock);

        delNode(ceps, getBlockCep(block), dummy);
        freeFromHashTable(getBlockResidents(block), inhabitants, freePerson);

        if(getListSize(getNodeData(nodeToDel)) > 1)
            deleteNode(getNodeData(nodeToDel), block, freeBlock);
        else
            delTreeNode(tree, getNodeKey(nodeToDel), freeBlock);

        buffer = getNext(buffer);

        free(pkgBlock);
    }

    fputc('\n', txt);

    freeList(blocksInsideRegion, dummy);
}

void dmpt(Tree tree, char* geo, char* pm, char* qry, char* path){

    Dot dot = newDot(geo, pm, qry);

    treeToDot(tree, dot);

    writeDot(dot, path);

    freeDot(dot);
}

/*************************************************************************************************/

static Node util_searchCepOnTree(Node node, char* cep){

    if(node == NULL)
        return NULL;

    Data data = searchCepOnList(getNodeData(node), cep);

    if(data)
        return node;

    Node data_2 = util_searchCepOnTree(getLeftChild(node), cep);

    if(data_2 == NULL)
        data_2 = util_searchCepOnTree(getRightChild(node), cep);

    return data_2;
}

static Node searchCepOnTree(Tree tree, char* cep){

    return util_searchCepOnTree(getRoot(tree), cep);
}

static Data searchCepOnList(List list, char* cep){

    Node node = getHead(list);

    while(node){

        if(strcmp(getBlockCep(getListNodeData(node)), cep) == 0)
            return getListNodeData(node);

        node = getNext(node);
    }

    return NULL;
}

static void freeFromHashTable(HashTable table, HashTable toDel, void (*freeData)(void*)){

    Node* nodeArray = getNodeArray(table),
          buffer = NULL;

    char* key;

    for(unsigned int i = 0; i < TABLE_SIZE; i++){

        buffer = nodeArray[i];

        while(buffer){

            key = getHashNodeKey(buffer);

            buffer = getHashNodeNext(buffer);

            delNode(toDel, key, freeData);
        }
    }
}

static void hashTableIntoFile(FILE* stream, HashTable table, char* (*packager_f)(void*)){

    Node buffer = NULL,
        *toWrite = getNodeArray(table);

    char* packedStr;

    for(unsigned int i = 0; i < TABLE_SIZE; i++){

        buffer = toWrite[i];

        while(buffer){

            packedStr = packager_f(getHashNodeData(buffer));

            fprintf(stream, "%s", packedStr);

            free(packedStr);

            buffer = getHashNodeNext(buffer);
        }
    }
}

static void getFullyInternalBlocks(List blocks, List blocklist, double x, double y, double w, double h){

    Node node = getHead(blocks);

    while(node){

        if(isRectInternal(x, y, w, h, getBlockX(getListNodeData(node)), getBlockY(getListNodeData(node)),
                          getBlockW(getListNodeData(node)), getBlockH(getListNodeData(node)))){

            appendList(blocklist, getListNodeData(node));
        }

        node = getNext(node);
    }
}

static void getInternalBlocks(List blocks, List blocklist, double x, double y, double w, double h){

    Node node = getHead(blocks);

    while(node){

        if(doRectsOverlap(getBlockX(getListNodeData(node)), getBlockY(getListNodeData(node)), 
                          getBlockW(getListNodeData(node)), getBlockH(getListNodeData(node)),
                          x, y, w, h))
            appendList(blocklist, getListNodeData(node));

        node = getNext(node);
    }
}

static void spaceSearch_full(Node node, List blockList, double x, double y, double w, double h){

    if(node == NULL)
        return;

    if(isInternal(getNodeKey(node), x, x + w))
        getFullyInternalBlocks(getNodeData(node), blockList, x, y, w, h);

    if(getNodeMaxX(getLeftChild(node)) >= x && getNodeMinX(getLeftChild(node)) <= x + w)
        spaceSearch_full(getLeftChild(node), blockList, x, y, w, h);

    if(getNodeMaxX(getRightChild(node)) >= x && getNodeMinX(getRightChild(node)) <= x + w)
        spaceSearch_full(getRightChild(node), blockList, x, y, w, h);
}

static void spaceSearch(Node node, List blockList, double x, double y, double w, double h){

    if(node == NULL)
        return;

    if(isInternal(getNodeKey(node), x, x + w))
        getInternalBlocks(getNodeData(node), blockList, x, y, w, h);

    if(getNodeMaxX(getLeftChild(node)) >= x && getNodeMinX(getLeftChild(node)) <= x + w)
        spaceSearch(getLeftChild(node), blockList, x, y, w, h);

    if(getNodeMaxX(getRightChild(node)) >= x && getNodeMinX(getRightChild(node)) <= x + w)
        spaceSearch(getRightChild(node), blockList, x, y, w, h);
}