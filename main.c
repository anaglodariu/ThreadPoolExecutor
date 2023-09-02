/*GLODARIU Ana - 311CB*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header_stiva.h"
#include "header_coada.h"
#include "struct.h"
#define N 50


int main(int argc, char *argv[])
{
    void *s = NULL, *cw = NULL,*cr = NULL, *cf = NULL, *caux = NULL;
    void *st = NULL; /*stiva pentru task id disponibile*/
    void *caux1 = NULL;
    st = InitS(sizeof(unsigned short));
    if (st == NULL) {
        return 0;
    } 

    cw = InitC(sizeof(Task)); /*coada de waiting*/
    if (cw == NULL) {
        DistrS(&st);
        return 0;
    }

    cr = InitC(sizeof(Task)); /*coada de running*/
    if (cr == NULL) {
        DistrS(&st);
        DistrC(&cw);
        return 0;
    }

    cf = InitC(sizeof(Task)); /*coada de running*/
    if (cf == NULL) {
        DistrS(&st);
        DistrC(&cw);
        DistrC(&cf);
        return 0;
    }

    int R = 0; /*timp de rulare total*/

    unsigned char id1 = 0; /*pentru comanda get thread*/
    unsigned short id2 = 0; /*pentru comanda get task*/

    int nr = 0; /*nr de taskuri*/
    unsigned int time = 0; /*timp de executie*/
    unsigned char p = 0; /*prioritate*/

    int T = 0; /*running time*/
    int Q = 0; /*cuanta de timp*/
    unsigned char C = 0; /*nr de cores*/

    int ok = 0; /*il folosesc la citirea primei linii din fisier*/
    int ok1 = 0; /*il folosesc la verificarea daca cw
    este vida inainte de a adauga noi elemente in ele*/
    int ok2 = 0;
    int l = 0;
    unsigned short k = 1; /*ultima valoarea id task folosit*/

    FILE *in, *out = NULL;
    
    if (argc == 3) {
        in = fopen(argv[1], "rt");
        if (in == NULL) {
            return 0;
        }
        out = fopen(argv[2], "wt");
        if (out == NULL) {
            return 0;
        }
        char *line = NULL;
        line = malloc(N * sizeof(char));
        while (fgets(line, N, in) != NULL) {
            l = strlen(line);
            if (line[l - 1] == '\n') {
                line[l - 1] = '\0';
            }
            char *cuv= NULL;
            cuv = strtok(line, " ");
            if (ok == 0) { /*pentru citirea lui Q*/
                ok = 1; 
                Q = atoi(cuv);
            } else if (ok == 1 && (cuv[0] >= '0' && cuv[0] <= '9')) {
                C = (unsigned char)atoi(cuv); /*pentru citirea lui C*/
                /*2C = nr maxim de threaduri*/
                s = Create_ThreadPool(2 * C, sizeof(Thread)); 
            } else if (strstr(cuv, "add") != NULL) {
                /*citim comanda add_tasks*/
                cuv = strtok(NULL, " ");
                nr = atoi(cuv);
                cuv = strtok(NULL, " ");
                time = (unsigned int)atoi(cuv);
                cuv = strtok(NULL, " ");
                p = (unsigned char)atoi(cuv);
                ok1 = 0;
                if (VidaC(cw) == 0) {
                    ok1 = 1; /*cw e vida*/
                }
                while (nr > 0) {
                    Task *task = (Task *)malloc(sizeof(Task));
                    if (task == NULL) {
                        return 0;
                    }
                    void *elem = malloc(sizeof(unsigned short));
                    if (elem == NULL) {
                        free(task);
                        return 0;
                    }
                    /*nu primesc inca un thread, sunt in coada de waiting*/
                    task->id1.id = 0; 
                    if (VidaS(st) == 1) {
                        Pop(st, elem);
                        task->id2 = *(unsigned short *)elem;
                    } else {
                        task->id2 = k; 
                        k++;
                    }
                    /*ii dam taskului un id, pe care il adaugam intr-o stiva de 
                    task id ordonata cresc (st) doar dupa ce l-am folosit si
                    nu mai avem nevoie de el, pt a fi preluat de taskuri noi*/
                    /*daca stiva este vida si noi trebuie sa mai adaugam 
                    taskuri in running, atunci punem valoarea k, pe care o
                    crestem cand st e vida*/
                    task->p = p;
                    task->t = time;
                    task->rt = time;
                    int rez = Add(cw, task, &ok1);
                    if (rez == 1) { /*task creat cu succes*/
                        fprintf(out, "Task created successfully : ID %u.\n",
                        task->id2);
                    } 
                    nr--;
                    free(task);
                    free(elem);
                }
            } else if (strcmp(cuv, "print") == 0) {
                /*citim comenzile de tip print*/
                /*citim comanda print waiting*/
                cuv = strtok(NULL, " ");
                if (strcmp(cuv, "waiting") == 0) {
                    fprintf(out, "====== Waiting queue =======\n");
                    if (VidaC(cw) == 0) {
                        fprintf(out, "[]\n");
                    } else {
                        void *element = malloc(sizeof(Task));
                        if (element == NULL) {
                            return 0;
                        }
                        caux1 = InitC(sizeof(Task)); 
                        if (caux1 == NULL) {
                            free(element);                      
                            return 0;
                        }
                        fprintf(out, "[");
                        while (ExtrC(cw, element)) {
                            Task *elem = (Task *)element;
                            IntrC(caux1, elem);
                            if (VidaC(cw) == 0) {
                                fprintf(out, "(%u: priority = %hhu, " 
                                "remaining_time = %u)", 
                                elem->id2, elem->p, elem->t);
                            } else {
                                fprintf(out, "(%u: priority = %hhu, "
                                "remaining_time = %u),\n", 
                                elem->id2, elem->p, elem->t);
                            }
                        }
                        fprintf(out, "]\n");
                        ConcatC(cw, caux1);
                        DistrC(&caux1);
                        free(element);
                    }
                } else if (strcmp(cuv, "running") == 0) {
                    /*citim comanda print running*/
                    fprintf(out, "====== Running in parallel =======\n");
                    if (VidaC(cr) == 0) {
                        fprintf(out, "[]\n");
                    } else {
                        void *element = malloc(sizeof(Task));
                        if (element == NULL) {
                            return 0;
                        }
                        caux1 = InitC(sizeof(Task)); 
                        if (caux1 == NULL) {
                            free(element);
                            return 0;
                        }
                        fprintf(out, "[");
                        while (ExtrC(cr, element)) {
                            Task *elem = (Task *)element;
                            IntrC(caux1, elem);
                            if (VidaC(cr) == 0) {
                                fprintf(out, "(%u: priority = %hhu, " 
                                "remaining_time = %u, running_thread = %hhu)",
                                elem->id2, elem->p, elem->rt, elem->id1.id);
                            } else {
                                fprintf(out, "(%u: priority = %hhu, "
                                "remaining_time = %u, running_thread = %hhu),"
                                "\n", elem->id2, elem->p, elem->rt,
                                elem->id1.id);
                            }
                        }
                        fprintf(out, "]\n");
                        ConcatC(cr, caux1);
                        DistrC(&caux1);
                        free(element);
                    }
                } else if (strcmp(cuv, "finished") == 0) {
                    /*citim comanda print finished*/
                    fprintf(out, "====== Finished queue =======\n");
                    if (VidaC(cf) == 0) {
                        fprintf(out, "[]\n");
                    } else {
                        void *element = malloc(sizeof(Task));
                        if (element == NULL) {
                            return 0;
                        }
                        caux1 = InitC(sizeof(Task)); 
                        if (caux1 == NULL) {
                            free(element);
                            return 0;
                        }
                        fprintf(out, "[");
                        while (ExtrC(cf, element)) {
                            Task *elem = (Task *)element;
                            IntrC(caux1, elem);
                            if (VidaC(cf) == 0) {
                                fprintf(out, "(%u: priority = %hhu, " 
                                "executed_time = %u)", 
                                elem->id2, elem->p, elem->t);
                            } else {
                                fprintf(out, "(%u: priority = %hhu, "
                                "executed_time = %u),\n", 
                                elem->id2, elem->p, elem->t);
                            }
                        }
                        fprintf(out, "]\n");
                        ConcatC(cf, caux1);
                        DistrC(&caux1);
                        free(element);
                    }
                }
            } else if (strstr(cuv, "task") != NULL) {
                /*citim comanda get_task*/
                cuv = strtok(NULL, " ");
                id2 = (unsigned short)atoi(cuv);
                int rez = 0;
                int flag = Find_task(cw, cr, cf, id2, &rez);
                if (flag == 1) {
                    fprintf(out, "Task %u is waiting (remaining_time = %d).\n",
                    id2, rez);
                } else if (flag == 2) {
                    fprintf(out, "Task %u is running (remaining_time = %d).\n",
                    id2, rez);
                } else if (flag == 3){
                    fprintf(out, "Task %u is finished (executed_time = %d).\n",
                    id2, rez);
                } else {
                    fprintf(out, "Task %u not found.\n", id2);
                }
            } else if (strstr(cuv, "thread") != NULL) {
                /*citim comanda get_thread*/
                cuv = strtok(NULL, " ");
                id1 = (unsigned char)atoi(cuv);
                int rez1 = -1, rez2 = -1;
                if (id1 < 2 * C) { /*daca exista threadul in stiva*/
                    Find_thread(cr, id1, &rez1, &rez2);
                    if (rez1 == -1) {
                        fprintf(out, "Thread %hhu is idle.\n", id1);
                    } else {
                        fprintf(out, "Thread %hhu is running task %d "
                        "(remaining_time = %d).\n", id1, rez2, rez1);
                    }
                }
            } else if (strcmp(cuv, "run") == 0) {
                /*citim comanda run*/
                cuv = strtok(NULL, " ");
                T = atoi(cuv);
                int TIME = 0; /*cat timp ruleaza taskurile intr-un ciclu*/
                int RTIME = 0; /*restul de timp cat ruleaza taskurile 
                intr-un ciclu daca T>Q si T nu este multiplu de Q*/
                int ciclu = 0; /*numara cate cicluri sunt*/
                ok2 = 0; /*il folosesc atunci cand T>Q 
                si T nu este multiplu de Q*/
                fprintf(out, "Running tasks for %d ms...\n", T);
                if (Q >= T) {
                    ciclu = 1;
                    TIME = T;
                } else {
                    ciclu = T / Q;
                    if (T % Q != 0) {
                        ok2 = 1;
                        RTIME = T - Q * ciclu;
                        ciclu++;
                    }
                    TIME = Q;
                }
                for (int i = 1; i <= ciclu; i++) {
                    /*daca am threaduri disponibile in stiva si daca am taskuri
                    in coada de waiting, atunci pot baga taskuri in
                    coada de running*/
                    if (i == ciclu && ok2 == 1) {
                        TIME = RTIME; /*timpul ramas in ultimul ciclu*/
                        /*daca T>Q si T nu este multiplu de Q*/
                    }
                    void *elem1 = malloc(sizeof(Task));
                    if (elem1 == 0) {
                        return 0;
                    }
                    void *elem2 = malloc(sizeof(Thread));
                    if (elem2 == 0) {
                        free(elem1);
                        return 0;
                    }
                    while(VidaC(cw) == 1 && VidaS(s) == 1) {
                        ExtrC(cw, elem1);
                        Pop(s, elem2);
                        Task *task = (Task *)elem1;
                        Thread *thread = (Thread *)elem2;
                        task->id1.id = thread->id;/*taskul primeste un thread*/
                        IntrC(cr, task); /*si ajunge un coada de running*/
                    }
                    free(elem1);
                    free(elem2);
                    caux = InitC(sizeof(Task));
                    if (caux == NULL) {
                        return 0;
                    }
                    void *elem3 = malloc(sizeof(Task));
                    if (elem3 == NULL) {
                        DistrC(&caux);
                        return 0;
                    }
                    int ok3 = 0;
                    while (VidaC(cr) == 1) { /*ma folosesc de o coada auxiliara
                        pt a parcurge cr, pentru a verifica daca dupa fiecare
                        ciclu exista taskuri care s-au terminat*/
                        ok3 = 1;
                        ExtrC(cr, elem3);
                        Task *task1 = (Task *)elem3;
                        if (task1->rt <= TIME) {
                            IntrC(cf, task1);  /*daca taskul s-a terminat
                            il punem in coada de finished*/
                            Push(s, &task1->id1); 
                            /*bag threadul folosit inapoi in stiva*/
                            AddS(st, &task1->id2); /*taskul a ajuns in
                            finished, deci pun task id-ul disponibil
                            in stiva st*/
                        } else {
                            task1->rt = task1->rt - TIME;
                            IntrC(caux, task1); 
                            /*daca nu, va ramane in coada de running*/
                        }
                    }
                    ConcatC(cr, caux); /*refac coada cr*/
                    DistrC(&caux); /*distrug coada aux si o initializez*/
                    free(elem3);   /*cu NULL, pt a o refolosi*/
                    if (ok3 == 0 && VidaC(cw) == 0) {
                        break;
                        /*daca cr si cw sunt vide, nu mai
                        adaugam TIME la timpul total*/
                    } else {
                        R = R + TIME;
                    }
                }
                if (VidaS(s) == 1) {
                    /*daca dupa runnul curent s-au eliberat thread id-uri,
                    completam coada cr cu urmatoarele taskuri din waiting 
                    ce vor astepta sa fie rulate la urmatorul run*/
                    void *elem1 = malloc(sizeof(Task));
                        if (elem1 == 0) {
                        return 0;
                    }
                    void *elem2 = malloc(sizeof(Thread));
                    if (elem2 == 0) {
                        free(elem1);
                        return 0;
                    }
                    while(VidaC(cw) == 1 && VidaS(s) == 1) {
                        ExtrC(cw, elem1);
                        Pop(s, elem2);
                        Task *task = (Task *)elem1;
                        Thread *thread = (Thread *)elem2;
                        task->id1.id = thread->id;
                        IntrC(cr, task);
                    }
                    free(elem1);
                    free(elem2);
                }

            } else if (strcmp(cuv, "finish") == 0) {
                /*citim comanda finish*/
                fprintf(out, "Total time: %d", R);
            }
            cuv = strtok(NULL, " ");
        }
        free(line);
    }
    
    DistrS(&s);
    DistrS(&st);
    DistrC(&cw);
    DistrC(&cr);
    DistrC(&cf);
    fclose(in);
    fclose(out);
    return 0;
}
