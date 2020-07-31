#ifndef _tp1_h_
#define _tp1_h_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "tp1.h"
#include "ej45.h"
#include "ej3.h"

//TDA NOTA
typedef struct {
    uint32_t *t0;
    uint32_t *tf;
    uint32_t *a;
    int *ff; //frecuencia fundamental
    size_t n;
}notas_t;

//TDA SINTETIZADOR
typedef struct{
    float **v;
    size_t n;
    //0<t<0.05 como el ataque, 0.05<t<0.25 como el sostenido y t>0.25 como el decaimiento:
    //Agregarle punteros a funciones
}sintetizador_t;

//TDA TRAMOFIMAL
/*typedef struct{
    float *v;
    size_t n;
}tramoFinal_t;
*/


bool tomarArgumentos(size_t , char *[], char *, char *, char *, size_t *, int *, int *);

void destruirNotas(notas_t *);

void destruirSint(sintetizador_t *);

int tomarFrecuencia(nota_t , signed char);

sintetizador_t *tomarSint(char *);

notas_t *tomarNotas(char *);

tramo_t *muestrearTramo(sintetizador_t, notas_t , int);

#endif
