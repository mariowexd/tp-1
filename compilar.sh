gcc -Wall -std=c99 -pedantic -g -c tp1.c
gcc -Wall -std=c99 -pedantic -g -c mod.c -lm
gcc -Wall -std=c99 -pedantic -g -c ej45.c -lm
gcc -Wall -std=c99 -pedantic -g -c ej3.c -lm
gcc -Wall -std=c99 -pedantic -g -c  main.c

gcc -Wall -std=c99 -pedantic -g tp1.o mod.o ej3.o ej45.o main.o -o tp1 -lm
