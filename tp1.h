#ifndef _tp1_h_
#define _tp1_h_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "tp1.h"

//TDA NOTA
typedef struct {
    float *t0;
    float *tf;
    float *a;
    int *ff; //frecuencia fundamental
    size_t n;
}notas_t;

//TDA SINTETIZADOR
typedef struct{
    float *v;
    size_t n;
    //Agregarle punteros a funciones
}sintetizador_t;

//TDA MUESTRAS
typedef struct{
}muestras_t;


bool tomarArgumentos(size_t , char *[], char *, char *, char *, size_t *, int *, int *);

void destruirNotas(nota_t *);

void destruirSint(sintetizador_t *);

int tomarFrecuencia(nota_t *, octava);

sintetizador_t *tomarSint(char *);

nota_t *tomarNotas(char *);

#endif
