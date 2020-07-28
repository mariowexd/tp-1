#ifndef _tp1_h_
#define _tp1_h_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "tp1.h"

//TDA ARMONICOS
typedef struct {
    float *v;
    size_t n;
} armo_t;

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
}sintetizador_t;

//TDA MUESTRAS
typedef struct{
}muestras_t;


bool tomarArgumentos(size_t , char *[], char *, char *, char *, size_t *, int *, int *);

void destruirNotas(nota_t *);

void destruirArmonicos(armo_t *);

int tomarFrecuencia(nota_t *, octava);

armo_t *tomarArmonicos(char *);

nota_t *tomarNotas(char *);

#endif