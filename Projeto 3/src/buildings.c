#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buildings.h"
#include "hashtable.h"

/**************
*Parte privada*
**************/

typedef struct Person{

    char* cpf, *name, *surname, sex, *bdate;
    char* cep, side, *num, *compl;

    double x, y;

} Person_t;

typedef struct Tenancy{

    struct Block* location;
    Person_t* tenant;

    char* id;
    char* cep, side, *num, *compl;

    double area, price;
    double x, y;

} Tenancy_t;

typedef struct Block{

    double x, y, w, h, sw;

    char* cep, *cfill, *cstrk;

    HashTable /*<Person>*/ residents;
    HashTable /*<Tenancy>*/ tenancies;

} Block_t;

static void dummy(void* ptr){}

/**************
*Parte pública*
**************/

Person newPerson(char* cpf, char* name, char* surname, char sex, char* bdate){

    Person_t* person = (Person_t*) malloc(sizeof(Person_t));

    person->cpf = (char*) calloc(sizeof(char), strlen(cpf) + 1);
    person->name = (char*) calloc(sizeof(char), strlen(name) + 1);
    person->surname = (char*) calloc(sizeof(char), strlen(surname) + 1);
    person->bdate = (char*) calloc(sizeof(char), strlen(bdate) + 1);

    person->cep = (char*) calloc(sizeof(char), 1);
    person->num = (char*) calloc(sizeof(char), 1);
    person->compl = (char*) calloc(sizeof(char), 1);

    person->side = 0;
    person->sex = sex;

    snprintf(person->cpf, strlen(cpf) + 1, cpf);
    snprintf(person->name, strlen(name) + 1, name);
    snprintf(person->surname, strlen(surname) + 1, surname);
    snprintf(person->bdate, strlen(bdate) + 1, bdate);

    person->x = 0;
    person->y = 0;

    return person;
}

Block newBlock(char* cep, double x, double y, double w, double h, double sw, char* cfill, char* cstrk){

    Block_t* block = (Block_t*) malloc(sizeof(Block_t));

    block->x = x;
    block->y = y;
    block->w = w;
    block->h = h;
    block->sw = sw;

    block->cep = (char*) calloc(sizeof(char), strlen(cep) + 1);
    block->cfill = (char*) calloc(sizeof(char), strlen(cfill) + 1);
    block->cstrk = (char*) calloc(sizeof(char), strlen(cstrk) + 1);

    snprintf(block->cep, strlen(cep) + 1, "%s", cep);
    snprintf(block->cfill, strlen(cfill) + 1, "%s", cfill);
    snprintf(block->cstrk, strlen(cstrk) + 1, "%s", cstrk);

    block->tenancies = newHashTable();
    block->residents = newHashTable();

    return block;
}

Tenancy newTenancy(char* id, char* cep, char side, char* num, char* compl, double area, double price){

    Tenancy_t* tenancy = (Tenancy_t*) malloc(sizeof(Tenancy_t));

    tenancy->location = NULL;
    tenancy->tenant = NULL;

    tenancy->id = (char*) calloc(sizeof(char), strlen(id) + 1);
    tenancy->cep = (char*) calloc(sizeof(char), strlen(cep) + 1);
    tenancy->num = (char*) calloc(sizeof(char), strlen(num) + 1);
    tenancy->compl = (char*) calloc(sizeof(char), strlen(compl) + 1);

    snprintf(tenancy->id, strlen(id) + 1, "%s", id);
    snprintf(tenancy->cep, strlen(cep) + 1, "%s", cep);
    snprintf(tenancy->num, strlen(num) + 1, "%s", num);
    snprintf(tenancy->compl, strlen(compl) + 1, "%s", compl);

    tenancy->area = area;
    tenancy->price = price;
    tenancy->side = side;

    tenancy->x = 0;
    tenancy->y = 0;

    return tenancy;
}

Person getTenancyTenant(Tenancy tenancy){

    if(tenancy == NULL)
        return 0;

    Tenancy_t* toGet = (Tenancy_t*) tenancy;

    return toGet->tenant;
}

HashTable getBlockTenancies(Block block){

    if(block == NULL)
        return NULL;

    Block_t* toGet = (Block_t*) block;

    return toGet->tenancies;    
}

HashTable getBlockResidents(Block block){

    if(block == NULL)
        return NULL;

    Block_t* toGet = (Block_t*) block;

    return toGet->residents;
}

char* getPersonCpf(Person person){

    if(person == NULL)
        return NULL;

    Person_t* toGet = (Person_t*) person;

    return toGet->cpf;
}

char* getPersonName(Person person){

    if(person == NULL)
        return NULL;

    Person_t* toGet = (Person_t*) person;

    return toGet->name;
}

char* getPersonSurname(Person person){

    if(person == NULL)
        return NULL;

    Person_t* toGet = (Person_t*) person;

    return toGet->surname;
}

char getPersonSex(Person person){

    if(person == NULL)
        return ' ';

    Person_t* toGet = (Person_t*) person;

    return toGet->sex;
}

char* getPersonBirthdate(Person person){

    if(person == NULL)
        return NULL;

    Person_t* toGet = (Person_t*) person;

    return toGet->bdate;
}

char* getPersonCep(Person person){

    if(person == NULL)
        return NULL;

    Person_t* toGet = (Person_t*) person;

    return toGet->cep;
}

char* getPersonNum(Person person){

    if(person == NULL)
        return NULL;

    Person_t* toGet = (Person_t*) person;

    return toGet->num;
}

char* getPersonCompl(Person person){

    if(person == NULL)
        return NULL;

    Person_t* toGet = (Person_t*) person;

    return toGet->compl;
}

char* getBlockCep(Block block){

    if(block == NULL)
        return NULL;

    Block_t* toGet = (Block_t*) block;

    return toGet->cep;
}

char* getBlockFillColor(Block block){

    if(block == NULL)
        return NULL;

    Block_t* toGet = (Block_t*) block;

    return toGet->cfill;
}

char* getBlockStrokeColor(Block block){

    if(block == NULL)
        return NULL;

    Block_t* toGet = (Block_t*) block;

    return toGet->cstrk;
}

char* genAddress(char* cep, char side, char* num, char* compl){

    char* address = NULL;

    address = (char*) calloc(sizeof(char), strlen(cep) + 1 + strlen(num) + strlen(compl) + 3);

    snprintf(address, strlen(cep) + 1 + strlen(num) + strlen(compl) + 3, "%s/%c/%s/%s", cep, side, num, compl);
    
    return address;
}

char* genAddress_short(char side, char* num, char* compl){

    char* address = NULL;

    address = (char*) calloc(sizeof(char), 1 + strlen(num) + strlen(compl) + 3);

    snprintf(address, 1 + strlen(num) + strlen(compl) + 3, "%c/%s/%s", side, num, compl);
    
    return address;
}

char* getTenancyId(Tenancy tenancy){

    if(tenancy == NULL)
        return 0;

    Tenancy_t* toGet = (Tenancy_t*) tenancy;

    return toGet->id;
}

char* getTenancyCep(Tenancy tenancy){

    if(tenancy == NULL)
        return 0;

    Tenancy_t* toGet = (Tenancy_t*) tenancy;

    return toGet->cep;
}

char* getTenancyNum(Tenancy tenancy){

    if(tenancy == NULL)
        return 0;

    Tenancy_t* toGet = (Tenancy_t*) tenancy;

    return toGet->num;
}

char* getTenancyCompl(Tenancy tenancy){

    if(tenancy == NULL)
        return 0;

    Tenancy_t* toGet = (Tenancy_t*) tenancy;

    return toGet->compl;
}

char* packBlock(Block block){

    if(block == NULL)
        return NULL;

    Block_t* toPack = (Block_t*) block;
    
    Node* tenancies = getNodeArray(toPack->tenancies),
        * residents = getNodeArray(toPack->residents),
        buffer = NULL;

    char* packedStr = (char*) calloc(sizeof(char), 1024),
        * toCat = NULL,
        * bufferStr = NULL;

    snprintf(packedStr, 1024, "cep: %s cfill: %s cstrk: %s x: %lf y: %lf w: %lf h: %lf\n",
                          toPack->cep, toPack->cfill, toPack->cstrk, toPack->x, toPack->y, toPack->w, toPack->h);

    bufferStr = realloc(packedStr, strlen(packedStr) + 1);

    if(bufferStr)
        packedStr = bufferStr;

    for(unsigned int i = 0; i < TABLE_SIZE; i++){

        buffer = tenancies[i];

        while(buffer){

            toCat = packTenancy(getHashNodeData(buffer));

            bufferStr = realloc(packedStr, strlen(packedStr) + strlen(toCat) + 1);

            if(bufferStr)
                packedStr = bufferStr;

            strcat(packedStr, toCat);

            free(toCat);

            buffer = getHashNodeNext(buffer);
        }
    }

    for(unsigned int i = 0; i < TABLE_SIZE; i++){

        buffer = residents[i];

        while(buffer){

            toCat = packPerson(getHashNodeData(buffer));

            bufferStr = realloc(packedStr, strlen(packedStr) + strlen(toCat) + 1);

            if(bufferStr)
                packedStr = bufferStr;

            strcat(packedStr, toCat);

            free(toCat);

            buffer = getHashNodeNext(buffer);
        }
    }

    return packedStr;
}

char* packTenancy(Tenancy tenancy){

    if(tenancy == NULL)
        return NULL;

    Tenancy_t* toPack = (Tenancy_t*) tenancy;

    char* packedStr = (char*) calloc(sizeof(char), 4096);

    int length = snprintf(packedStr, 4096, "id: %s cep: %s lado: %c num: %s compl: %s area: %.2lf m² preco: R$ %.2lf\n",
                          toPack->id, toPack->cep, toPack->side, toPack->num, toPack->compl, toPack->area, toPack->price);

    return (char*) realloc(packedStr, length + 1);
}

char* packPerson(Person person){

    if(person == NULL)
        return NULL;

    Person_t* toPack = (Person_t*) person;

    char* packedStr = (char*) calloc(sizeof(char), 4096);

    int length = snprintf(packedStr, 4096, "cpf: %s nome: %s sobrenome: %s sexo: %c nasc: %s\n",
                          toPack->cpf, toPack->name, toPack->surname, toPack->sex, toPack->bdate);

    return (char*) realloc(packedStr, length + 1);
}

double getTenancyX(Tenancy tenancy){

    if(tenancy == NULL)
        return 0;

    Tenancy_t* toGet = (Tenancy_t*) tenancy;

    return toGet->x;
}

double getTenancyY(Tenancy tenancy){

    if(tenancy == NULL)
        return 0;

    Tenancy_t* toGet = (Tenancy_t*) tenancy;

    return toGet->y;
}

double getTenancyArea(Tenancy tenancy){

    if(tenancy == NULL)
        return 0;

    Tenancy_t* toGet = (Tenancy_t*) tenancy;

    return toGet->area;
}

double getTenancyPrice(Tenancy tenancy){

    if(tenancy == NULL)
        return 0;

    Tenancy_t* toGet = (Tenancy_t*) tenancy;

    return toGet->price;
}

double getPersonX(Person person){

    if(person == NULL)
        return 0;

    Person_t* toGet = (Person_t*) person;

    return toGet->x;
}

double getPersonY(Person person){

    if(person == NULL)
        return 0;

    Person_t* toGet = (Person_t*) person;

    return toGet->y;
}

double getBlockX(Block block){

    if(block == NULL)
        return 0;

    Block_t* toGet = (Block_t*) block;

    return toGet->x;
}

double getBlockY(Block block){

    if(block == NULL)
        return 0;

    Block_t* toGet = (Block_t*) block;

    return toGet->y;
}

double getBlockW(Block block){

    if(block == NULL)
        return 0;

    Block_t* toGet = (Block_t*) block;

    return toGet->w;
}

double getBlockH(Block block){

    if(block == NULL)
        return 0;

    Block_t* toGet = (Block_t*) block;

    return toGet->h;
}

double getBlockSW(Block block){

    if(block == NULL)
        return 0;

    Block_t* toGet = (Block_t*) block;

    return toGet->sw;
}

char getTenancySide(Tenancy tenancy){

    if(tenancy == NULL)
        return 0;

    Tenancy_t* toGet = (Tenancy_t*) tenancy;

    return toGet->side;
}

char getPersonSide(Person person){

    if(person == NULL)
        return ' ';

    Person_t* toGet = (Person_t*) person;

    return toGet->side;
}

bool isTenancyAvailable(Tenancy tenancy){

    if(tenancy == NULL)
        return false;

    Tenancy_t* toGet = (Tenancy_t*) tenancy;

    return toGet->tenant == NULL;
}

void insertPerson(Person person, Block block, char side, char *num, char *compl){
    
    if(person == NULL || block == NULL)
        return;

    Block_t* destiny = (Block_t*) block;
    Person_t* toInsert = (Person_t*) person;

    toInsert->cep = (char*) realloc(toInsert->cep, strlen(destiny->cep) + 1);
    toInsert->num = (char*) realloc(toInsert->num, strlen(num) + 1);
    toInsert->compl = (char*) realloc(toInsert->compl, strlen(compl) + 1);

    snprintf(toInsert->cep, strlen(destiny->cep) + 1, "%s", destiny->cep);
    snprintf(toInsert->num, strlen(num) + 1, "%s", num);
    snprintf(toInsert->compl, strlen(compl) + 1, "%s", compl);

    toInsert->side = side;

    double n = atof(num);

    switch(side){

        case 'N':
           
            toInsert->x = destiny->x + n;
            toInsert->y = destiny->y + destiny->h;

            break;

        case 'S':
            
            toInsert->x = destiny->x + n;
            toInsert->y = destiny->y; 

            break;

        case 'L':

            toInsert->x = destiny->x;
            toInsert->y = destiny->y + n;

            break;

        case 'O':

            toInsert->x = destiny->x + destiny->w;
            toInsert->y = destiny->y + n;

            break;

        default:
            break;
    }

    char* address = genAddress_short(side, num, compl);

    insertHashNode(destiny->residents, address, toInsert);

    free(address);
}

void setPersonCpf(Person person, char* cpf){

    if(person == NULL)
        return;

    Person_t* toSet = (Person_t*) person;

    char* buffer = (char*) realloc(toSet->cpf, strlen(cpf) + 1);

    if(buffer)
        toSet->cpf = buffer;
    else
        return;

    snprintf(toSet->cpf, strlen(cpf) + 1, "%s", cpf);
}

void setPersonCep(Person person, char* cep){

    if(person == NULL)
        return;

    Person_t* toSet = (Person_t*) person;

    char* buffer = (char*) realloc(toSet->cep, strlen(cep) + 1);

    if(buffer)
        toSet->cep = buffer;
    else
        return;

    snprintf(toSet->cep, strlen(cep) + 1, "%s", cep);
}

void setPersonSide(Person person, char side){

    if(person == NULL)
        return;

    Person_t* toSet = (Person_t*) person;

    toSet->side = side;
}

void setPersonNum(Person person, char* num){

    if(person == NULL)
        return;

    Person_t* toSet = (Person_t*) person;

    char* buffer = (char*) realloc(toSet->num, strlen(num) + 1);

    if(buffer)
        toSet->num = buffer;
    else
        return;

    snprintf(toSet->num, strlen(num) + 1, "%s", num);
}

void setPersonCompl(Person person, char* compl){

    if(person == NULL)
        return;

    Person_t* toSet = (Person_t*) person;

    char* buffer = (char*) realloc(toSet->compl, strlen(compl) + 1);

    if(buffer)
        toSet->compl = buffer;
    else
        return;

    snprintf(toSet->compl, strlen(compl) + 1, "%s", compl);
}

void freePerson(Person person){

    Person_t* toFree = (Person_t*) person;

    free(toFree->cpf);
    free(toFree->name);
    free(toFree->surname);
    free(toFree->bdate);

    free(toFree->cep);
    free(toFree->num);
    free(toFree->compl);

    free(toFree);
}

void printBlock(Block block){

    if(block == NULL)
        return;

    Block_t* toPrint = (Block_t*) block;

    printf("%s %lf %lf %lf %lf %lf\n", toPrint->cep, toPrint->x, toPrint->y, toPrint->w, toPrint->h, toPrint->sw);
    printHashTable(toPrint->residents);
}

void setTenancyTenant(Tenancy tenancy, Person tenant){

    if(tenancy == NULL)
        return;

    Tenancy_t* this = (Tenancy_t*) tenancy;

    this->tenant = tenant;
}

void setTenancyArea(Tenancy tenancy, double area){

    if(tenancy == NULL)
        return;

    Tenancy_t* this = (Tenancy_t*) tenancy;

    this->area = area;
}

void setTenancyPrice(Tenancy tenancy, double price){

    if(tenancy == NULL)
        return;

    Tenancy_t* this = (Tenancy_t*) tenancy;

    this->price = price;
}

void insertTenancy(Tenancy tenancy, Block block){

    if(tenancy == NULL || block == NULL)
        return;

    Block_t* destiny = (Block_t*) block;
    Tenancy_t* toInsert = (Tenancy_t*) tenancy;

    double n = atof(toInsert->num);

    switch(toInsert->side){

        case 'N':
           
            toInsert->x = destiny->x + n;
            toInsert->y = destiny->y + destiny->h;

            break;

        case 'S':
            
            toInsert->x = destiny->x + n;
            toInsert->y = destiny->y; 

            break;

        case 'L':

            toInsert->x = destiny->x;
            toInsert->y = destiny->y + n;

            break;

        case 'O':

            toInsert->x = destiny->x + destiny->w;
            toInsert->y = destiny->y + n;

            break;

        default:
            break;
    }

    toInsert->location = destiny;

    char* address = genAddress_short(toInsert->side, toInsert->num, toInsert->compl);

    insertHashNode(destiny->tenancies, address, toInsert);

    free(address);
}

void locTenancy(Tenancy tenancy, Person person){

    Tenancy_t* destiny = (Tenancy_t*) tenancy;
    Block_t* block = destiny->location;

    insertPerson(person, block, getTenancySide(destiny), getTenancyNum(destiny), getTenancyCompl(destiny));

    destiny->tenant = person;
}

void freeTenancy(Tenancy tenancy){

    if(tenancy == NULL)
        return;

    Tenancy_t* toFree = (Tenancy_t*) tenancy;

    free(toFree->id);
    free(toFree->cep);
    
    free(toFree->num);
    free(toFree->compl);

    free(toFree);
}

void freeBlock(Block block){

    Block_t* toFree = (Block_t*) block;

    free(toFree->cep);
    free(toFree->cfill);
    free(toFree->cstrk);

    freeHashTable(toFree->residents, dummy);
    freeHashTable(toFree->tenancies, freeTenancy);

    free(toFree);
}