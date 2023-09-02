/*GLODARIU Ana - 311CB*/
#include "header_stiva.h"
#include "struct.h"
#include <string.h>

void *InitS(size_t d)
{
    ASt s = NULL;
    s = (ASt)malloc(sizeof(TStiva));
    if (s == NULL) {
        return NULL;
    }

    s->dime = d;
    s->vf = NULL;

    return (void *)s;
}

int Push(void *s, void *ae) 
{
    ACelSt aux = (ACelSt)malloc(sizeof(TCelSt));
    if (aux == NULL) {
        return 0;
    }

    aux->info = malloc(((TStiva *)s)->dime);
    if (aux->info == NULL) {
        free(aux);
        return 0;
    } 

    memcpy(aux->info, ae, ((TStiva *)s)->dime);
    aux->urm = ((TStiva *)s)->vf;
    ((TStiva *)s)->vf = aux;

    return 1;
}

int VidaS(void *s) {
    if (((TStiva *)s)->vf == NULL) {
        return 0;
    }
    return 1;
}

int Pop(void *s, void *ae) 
{
    if (((TStiva *)s)->vf == NULL) {
        return 0; /*stiva vida, nu mai putem extrage*/
    }
    ACelSt aux = ((TStiva *)s)->vf;
    memcpy(ae, aux->info, ((TStiva *)s)->dime);
    ((TStiva *)s)->vf = aux->urm;
    free(aux->info);
    free(aux);
    return 1;
}

void DistrugeLSt(ACelSt *al) {
    while(*al != NULL) {
        ACelSt aux = *al; /* adresa celulei eliminate */
        if (aux == NULL) {
            return;
        }
        *al = aux->urm; /* deconecteaza celula din lista */
        free(aux->info); /* elib.spatiul ocupat de element*/
        free(aux);  /* elibereaza spatiul ocupat de celula */
    }
    return;
}

void DistrS(void **s) 
{
    ACelSt l = ((TStiva *)*s)->vf;
    DistrugeLSt(&l);
    ((TStiva *)*s)->vf = NULL;
    free(*s);
    *s = NULL;
}

void *Create_ThreadPool(unsigned char N, size_t dime) 
{
    Thread *elem = (Thread *)malloc(sizeof(Thread));
    if (elem == NULL) {
        return NULL;
    }
    void *s = InitS(dime);
    if (s == NULL) {
        free(elem);
        return NULL;
    }

    do 
    {
        N--;
        elem->id = N;
        Push(s, elem);
    } while (N > 0);
    
    free(elem);
    return s;
}

int cresc(void *id1, void *id2) {
    unsigned short elem1 = *(unsigned short *)id1;
    unsigned short elem2 = *(unsigned short *)id2;
    if (elem1 < elem2) {
        return 1;
    }
    return 0;

}
/*creare stiva de task id*/
int AddS(void *st, void *elem) {
    
    if (VidaS(st) == 0) {
        Push(st, elem);
        return 1;
    } 
    void *saux = InitS(sizeof(unsigned short));
    if (saux == NULL) {
        return 0;
    }
    void *element = malloc(sizeof(unsigned short));
    if (element == 0) {
        return 0;
    }
    int ok = 1;
    while(Pop(st, element)) {
        /*odata ce gasesc pozitia noului element nu mai fac comparatii*/
        if (cresc(elem, element) == 1 && ok == 1) {
            Push(saux, elem);
            Push(saux, element);
            ok = 0;
        } else {
            Push(saux, element);
        }
    }
    if (ok == 1) { /*daca nu a intrat niciodata in funct cresc*/
        Push(saux, elem);
    }
    void *element1 = malloc(sizeof(unsigned short));
    while(Pop(saux, element1)) {
        Push(st, element1);
    }
    DistrS(&saux);
    free(element);
    free(element1);
    return 1;
}