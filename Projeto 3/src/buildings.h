#ifndef BUILDINGS_H
#define BUILDINGS_H

#include "types.h"

Block newBlock(char* cep, double x, double y, double w, double h, double sw, char* cfill, char* cstrk);

Tenancy newTenancy(char* id, char* cep, char face, char* num, char* compl, double area, double price);

void insertTenancy(Tenancy tenancy, Block block);

void insertPerson(Person person, Block block, char side, char* num, char* compl);

HashTable getBlockTenancies(Block block);

HashTable getTenancyResidents(Block block, char* address);

HashTable getBlockResidents(Block block);

char* getBlockCep(Block block);

char* getBlockFillColor(Block block);

char* getBlockStrokeColor(Block block);

double getBlockX(Block block);

double getBlockY(Block block);

double getBlockW(Block block);

double getBlockH(Block block);

double getBlockSW(Block block);

char* packBlock(Block block);

Person getTenancyTenant(Tenancy tenancy);

double getTenancyX(Tenancy tenancy);

double getTenancyY(Tenancy tenancy);

char* getTenancyId(Tenancy tenancy);

char* getTenancyCep(Tenancy tenancy);

char getTenancySide(Tenancy tenancy);

char* getTenancyCompl(Tenancy tenancy);

char* getTenancyNum(Tenancy tenancy);

double getTenancyPrice(Tenancy tenancy);

double getTenancyArea(Tenancy tenancy);

char* genAddress(char* cep, char side, char* num, char* compl);

char* genAddress_short(char side, char* num, char* compl);

int checkCep(Block block, char* cep);

void printBlock(Block block);

void freeBlock(Block block);

void freeTenancy(Tenancy tenancy);

bool isTenancyAvailable(Tenancy tenancy);

void locTenancy(Tenancy tenancy, Person person);

void setTenancyTenant(Tenancy tenancy, Person tenant);

void setTenancyArea(Tenancy tenancy, double area);

void setTenancyPrice(Tenancy tenancy, double price);

Person newPerson(char* cpf, char* name, char* surname, char sex, char* bdate);

double getPersonX(Person person);

double getPersonY(Person person);

char* getPersonName(Person person);

char* getPersonSurname(Person person);

char* getPersonCpf(Person person);

char getPersonSex(Person person);

char* getPersonBirthdate(Person person);

char* getPersonCep(Person person);

char getPersonSide(Person person);

char* getPersonNum(Person person);

char* getPersonCompl(Person person);

void setPersonCpf(Person person, char* cpf);

void setPersonCep(Person person, char* cep);

void setPersonSide(Person person, char side);

void setPersonNum(Person person, char* num);

void setPersonCompl(Person person, char* compl);

char* packBlock(Block block);

char* packPerson(Person person);

char* packTenancy(Tenancy tenancy);

void freePerson(Person person);

#endif