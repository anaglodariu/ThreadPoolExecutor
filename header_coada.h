/*GLODARIU Ana - 311CB*/
#include <stdio.h>
#include <stdlib.h>

typedef struct celc
{
    struct celc *urm;
    void *info;
} TCelC, *ACelC;

typedef struct coada
{
    size_t dime; /*dimensiune element*/
    ACelC ic, sc; /* adresa inceput/sfarsit lista*/
} TCoada, *AC;

void *InitC(size_t);
int IntrC(void *, void *);
int ExtrC(void *, void *);
void DistrC(void **);
void DistrugeLC(ACelC *);
int Add(void *, void *, int *);
int ConcatC(void *, void *);
int VidaC(void *);
int Find_task(void *, void *, void *, unsigned short, int *);
void Find_thread(void *, unsigned char, int *, int *);
