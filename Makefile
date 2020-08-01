all: tp1

tp1.o: ej3.h tp1.h ej45.h mod.h
	gcc -Wall -std=c99 -pedantic -g -c tp1.c

mod.o: mod.h
	gcc -Wall -std=c99 -pedantic -g -c mod.c -lm

ej3.o: ej3.h
	gcc -Wall -std=c99 -pedantic -g -c ej45.c -lm

ej45.o: ej45.h
	gcc -Wall -std=c99 -pedantic -g -c ej3.c -lm

main.o: ej3.h tp1.h ej45.h mod.h
	gcc -Wall -std=c99 -pedantic -g -c  main.c

tp1: tp1.o mod.o ej3.o ej45.o main.o 
	gcc -Wall -std=c99 -pedantic -g tp1.o mod.o ej3.o ej45.o main.o -o tp1 -lm

