build: 
	gcc -Wall -m32 -std=c99 main.c functii_stiva.c functii_coada.c -o tema2
run:
	./tema2
clean:
	rm -rf tema2
