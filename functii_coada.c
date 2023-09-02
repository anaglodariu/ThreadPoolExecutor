/*GLODARIU Ana - 311CB*/
#include "header_coada.h"
#include "header_stiva.h"
#include "struct.h"
#include <string.h>

void *InitC(size_t d)
{
    AC c = NULL;
    c = (AC)malloc(sizeof(TCoada));
    if (c == NULL) {
        return NULL;
    }

    c->dime = d;
    c->ic = c->sc = NULL;

    return (void *)c;
}

int IntrC(void *c, void *ae)
{
    ACelC aux = (ACelC)malloc(sizeof(TCelC));
    if (aux == NULL) {
        return 0;
    }
    aux->info = malloc(((TCoada *)c)->dime);
    if (aux->info == NULL) {
        free(aux);
        return 0;
    }
    memcpy(aux->info, ae, ((TCoada *)c)->dime);
    aux->urm = NULL;
    /*daca coada e vida*/
    if (((TCoada *)c)->ic == NULL && ((TCoada *)c)->sc == NULL) {
        ((TCoada *)c)->ic = ((TCoada *)c)->sc = aux;
    } else {
        ((TCoada *)c)->sc->urm = aux;
        ((TCoada *)c)->sc = aux;
    }
    return 1;
}

int ExtrC(void *c, void *ae)
{
    if (((TCoada *)c)->ic == NULL && ((TCoada *)c)->sc == NULL) {
        return 0;
    }
    ACelC aux = ((TCoada *)c)->ic;
    ((TCoada *)c)->ic = aux->urm;
    if (((TCoada *)c)->ic == NULL) {
        ((TCoada *)c)->sc = NULL;
        /*mai era un singur element in coada*/
    }
    memcpy(ae, aux->info, ((TCoada *)c)->dime);
    free(aux->info);
    free(aux);
    return 1;
}

void DistrugeLC(ACelC *al) {
    while(*al != NULL) {
        ACelC aux = *al; /* adresa celulei eliminate */
        *al = aux->urm; /* deconecteaza celula din lista */
        free(aux->info); /* elib.spatiul ocupat de element*/
        free(aux);  /* elibereaza spatiul ocupat de celula */
    }
}

int VidaC(void *c) {
    if (((TCoada *)c)->ic == NULL && ((TCoada *)c)->sc == NULL) {
       return 0;  
    }
    return 1;
}

void DistrC(void **c) 
{
    ACelC l = ((TCoada *)*c)->ic;
    ((TCoada *)*c)->ic = ((TCoada *)*c)->sc = NULL;
    DistrugeLC(&l);
    free(*c);
    *c = NULL;
}

int compare(void *task1, void *task2) 
{
    Task *elem1 = (Task *)task1;
    Task *elem2 = (Task *)task2;
    if (elem1->p > elem2->p) {
        return 1; 
    } else if (elem1->p == elem2->p){
        if (elem1->t < elem2->t) {
            return 1;
        } else if (elem1->t == elem2->t) {
            if (elem1->id2 < elem2->id2) {
                return 1;
            }
        }
    }
    return 0;
}
int ConcatC(void *cd, void *cs) 
{ /*concatenam ce avem in coada sursa in coada destinatie*/
    if (((TCoada *)cs)->ic == NULL && ((TCoada *)cs)->sc == NULL) {
        return 0; /*daca coada sursa este vida*/
    }
    if (((TCoada *)cd)->ic == NULL && ((TCoada *)cd)->sc == NULL) {
        /*daca coada destinatie este vida*/
        ((TCoada *)cd)->ic = ((TCoada *)cs)->ic;
        ((TCoada *)cd)->sc = ((TCoada *)cs)->sc;
    } else {
        ((TCoada *)cd)->sc->urm = ((TCoada *)cs)->ic;
        ((TCoada *)cd)->sc = ((TCoada *)cs)->sc;
    }
    ((TCoada *)cs)->sc = ((TCoada *)cs)->ic = NULL;
    return 1;
}

int Add(void *cw, void *task, int *ok)
{
    int rez = 0;
    Task *elem = (Task *)task;
    if ((*ok) == 1) { /*daca cw e vida, punem primul element in ea*/
        /*pentru a construi cw in ordinea ceruta, trebuie sa am cu ce compara*/
        rez = IntrC(cw, elem);
        if (rez == 1) {
            (*ok) = 0;
            return 1; /*task creat cu succes*/
        }
    }
    void *caux = InitC(sizeof(Task));
    if (caux == NULL) {
        return 0;
    }
    int ok1 = 0;
    void *element = malloc(sizeof(Task));
    if (element == NULL) {
        DistrC(&caux);
        return 0;
    }
    while (ExtrC(cw, element)) {
        /*odata ce functia compare a intors 1, nu mai intru iar in ea*/
        if (compare(elem, element) == 1 && ok1 == 0) {
            rez = IntrC(caux, elem);
            if (rez == 0) {
                DistrC(&caux);
                free(element);
                return 0; 
            }
            rez = IntrC(caux, element);
            if (rez == 0) {
                DistrC(&caux);
                free(element);
                return 0; 
            }
            ok1 = 1;
        } else {
            rez = IntrC(caux, element);
            if (rez == 0) {
                DistrC(&caux);
                free(element);
                return 0; 
            }
        }
    }
    if (ok1 == 0) { /*daca functia de compare nu a intors niciodata 1,
        elementul trebuie pus la finalul cozii*/
        rez = IntrC(caux, elem);
        if (rez == 0) {
            DistrC(&caux);
            free(element);
            return 0; 
        }
    }
    rez = ConcatC(cw, caux);
    DistrC(&caux);
    free(element);
    if (rez == 1) {
        return 1; /*task creat cu succes*/
    }
    return 0;
}

int Find_task(void *cw, void *cr, void *cf, unsigned short id2, int *rez) 
{
    int ok = 0;
    void *elem = malloc(sizeof(Task));
    if (elem == NULL) {
        return 0;
    }
    void *cwaux = InitC(sizeof(Task));
    if (cwaux == NULL) {
        free(elem);
        return 0;
    }
    while (ExtrC(cw, elem)) {
        IntrC(cwaux, elem);
        Task *element = (Task *)elem;
        if (element->id2 == id2) {
            *rez = element->t;
            ok = 1;
        }
    }
    ConcatC(cw, cwaux);
    DistrC(&cwaux);
    if (ok == 1) {
        free(elem);
        return 1;
    }
    void *craux = InitC(sizeof(Task));
    if (craux == NULL) {
        free(elem);
        return 0;
    }
    while (ExtrC(cr, elem)) {
        IntrC(craux, elem);
        Task *element = (Task *)elem;
        if (element->id2 == id2) {
            *rez = element->rt;
            ok = 2;
        }
    }
    ConcatC(cr, craux);
    DistrC(&craux);
    if (ok == 2) {
        free(elem);
        return 2;
    }
    void *cfaux = InitC(sizeof(Task));
    if (cfaux == NULL) {
        free(elem);
        return 0;
    }
    while (ExtrC(cf, elem)) {
        IntrC(cfaux, elem);
        Task *element = (Task *)elem;
        if (element->id2 == id2 && ok != 3) {
            *rez = element->t;
            ok = 3;
        }
    }
    ConcatC(cf, cfaux);
    DistrC(&cfaux);
    free(elem);
    if (ok == 3) {
        return 3;
    }
    return 0;
}
void Find_thread(void *cr, unsigned char id1, int *rez1, int *rez2) {
    void *elem = malloc(sizeof(Task));
    if (elem == NULL) {
        return;
    }
    void *craux = InitC(sizeof(Task));
    if (craux == NULL) {
        free(elem);
        return;
    }
    while (ExtrC(cr, elem)) {
        IntrC(craux, elem);
        Task *element = (Task *)elem;
        if (element->id1.id == id1) {
            *rez1 = element->rt;
            *rez2 = element->id2;
            break;
        }
    }
    ConcatC(cr, craux);
    DistrC(&craux);
    free(elem);
}
