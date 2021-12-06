#ifndef QRY_H
#define QRY_H

#include "types.h"

void olocP(Tree tree, Svg qry, FILE* txt, double x, double y, double w, double h);

void oloc(HashTable blocks, HashTable tenancies, char* id, char* cep, char side, char* num, char* compl, double area, double price);

void hom(Tree tree, Svg qrySvg, FILE* txt, double x, double y, double w, double h);

void mul(Tree tree, Svg qrySvg, FILE* txt, double x, double y, double w, double h);

void del(Tree tree, HashTable ceps, HashTable inhabitants, Svg qrySvg, FILE* txt, char* cep);

void mP(HashTable ceps, FILE* txt, char* cep);

void dmP(HashTable inhabitants, FILE* txt, char* cpf);

void mud(HashTable ceps, HashTable inhabitants, Svg qrySvg, FILE* txt, char* cpf, char* cep, char side, char* num, char* compl);

void loc(HashTable tenancies, HashTable inhabitants, Svg qrySvg, FILE* txt, char* id, char* cpf);

void dloc(HashTable tenancies, Svg qrySvg, FILE* txt, char* id);

void locP(HashTable tenancies, Svg qrySvg, FILE* txt, char* id);

void catac(Tree tree, HashTable ceps, HashTable inhabitants, Svg qrySvg,
           FILE* txt, double x, double y, double w, double h);

void dmpt(Tree tree, char* geo, char* pm, char* qry, char* path);

#endif