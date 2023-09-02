in folderul zip : 
	- in header_stiva.h am declarate functiile folosite in functii_stiva.c + structura pentru stiva implementata cu liste
	- in header_coada.h am declarate functiile folosite in functii_coada.c + structura pentru coada implementata cu liste
	- in struct.h am declarat structurile task si thread din enunt
 - cu functia Create_ThreadPool imi creez stiva cu threaduri, facand push la elemente (structuri de tip thread) 
 - cand avem comanda de add tasks
	- folosesc un ok1 pentru a vedea daca, inainte sa adaugam taskuri in coada de waiting in ordinea ceruta in cerinta, ea e vida (care e un caz separat)
	- cand coada nu e vida, adaugam noi elemente folosind functia de comparare (compare)
 - pentru atribuire de id a taskurilor, am creat o noua stiva (st), in care sunt puse id-uri dupa ce au fost folosite (taskul s-a terminat). Stiva continand id taskurile disponibile (valoarea minima fiind in varful stivei)
 - in variabila k am valoarea maxima pe care o poate lua un id -> atunci cand nu avem id-uri disponibile in stiva (st) (adica e vida), il cresc pe k
 - la comezile de print folosesc mereu o coada auxiliara si refac coada initiala cu o functie de concatenare (ConcatC)
 - la comanda de get task, caut taskul pe rand in cele 3 cozi (returnez 1 daca e in cw, 2 daca e in cr, 3 daca e in cf si 0 daca nu exista)


ana@DESKTOP-RPATT8R:~/sda/tema2$ ./check.sh 
gcc -g -Wall -m32 -std=c99 main.c functii_stiva.c functii_coada.c -o tema2

                     = Tema 2 SD ThreadPoolExecutor =

[test1.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test2.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test3.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test4.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test5.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test6.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test7.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test8.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test9.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test10.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test11.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test12.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test13.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test14.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test15.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test16.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test17.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test18.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test19.out]...........................................failed  [ 0/135]

[test20.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test21.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test22.out]...........................................passed  [05/135]
[VALGRIND: PASSED]....................................passed

[test23.out]...........................................failed  [ 0/135]

[test24.out]...........................................failed  [ 0/135]

[test25.out]...........................................failed  [ 0/135]

[test26.out]...........................................failed  [ 0/135]

[test27.out]...........................................failed  [ 0/135]


                                           Total: [105/135]

                                           Bonus: [15/20]
