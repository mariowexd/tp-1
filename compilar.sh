gcc -Wall -std=c99 -pedantic -g -c tp1.c
gcc -Wall -std=c99 -pedantic -g -c modulacion.c -lm
gcc -Wall -std=c99 -pedantic -g -c ej3.c -lm
gcc -Wall -std=c99 -pedantic -g -c  main.c

gcc -Wall -std=c99 -pedantic -g tp1.o modulacion.o ej3.o main.o -o tp1 -lm
