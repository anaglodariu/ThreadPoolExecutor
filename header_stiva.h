/*GLODARIU Ana - 311CB*/
#include <stdio.h>
#include <stdlib.h>

typedef struct celst
{
    struct celst *urm;
    void *info;
} TCelSt, *ACelSt;

typedef struct stiva
{
    size_t dime; /*dimensiune element*/
    ACelSt vf; /* adresa celulei din varf*/
} TStiva, *ASt;

void *InitS(size_t);
int Push(void *, void *);
int Pop(void *, void *);
void DistrugeLSt(ACelSt *);
void *Create_ThreadPool(unsigned char, size_t);
void DistrS(void **);
int VidaS(void *);
int AddS(void *, void *);
