all: Tp1

Tp1: tp1.o  modulacion.o ej3.o ej4_5.o main.o 
	gcc -Wall -std=c99 -pedantic -g tp1.o modulacion.o ej3.o ej4_5.o main.o -o Tp1 -lm

tp1.o: ej3.h tp1.h ej4_5.h modulacion.h
	gcc -Wall -std=c99 -pedantic -g -c tp1.c

modulacion.o: modulacion.h
	gcc -Wall -std=c99 -pedantic -g -c modulacion.c -lm

ej3.o: ej3.h
	gcc	-Wall -std=c99 -pedantic -g -c ej3.c -lm

ej4_5.o: ej4_5.h
	gcc -Wall -std=c99 -pedantic -g -c ej4_5.c -lm

main.o: ej3.h tp1.h ej4_5.h modulacion.h
	gcc -Wall -std=c99 -pedantic -g -c  main.c


