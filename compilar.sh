gcc -Wall -std=c99 -pedantic -g -c tp1.c
gcc -Wall -std=c99 -pedantic -g -c main.c
gcc -Wall -std=c99 -pedantic -g -lm tp1.o main.o -o tp1
