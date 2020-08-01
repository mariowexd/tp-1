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
#include "mod.h"

#define NOMBRE_MAX 255
#define CANT_ARG 6
#define CANT_ARG_TXT 3
#define ARG_MAX 12
#define CANT_CHAR_RENGLON 11
#define CANT_ARM_MAX 4
#define LA_4_FREC 440
#define LA_4_VAL 69
#define RAIZ_12_2 1.05946309435929

#define CANT_CHAR_MAX_MOD 38

#define METAEVENTO_FIN_DE_PISTA 0x2F
#define EVENTO_MAX_LONG 10

#define TIEMPO_ATAQUE 0.05
#define TIEMPO_SOSTENIDO 0.25

enum {EVNOTA_NOTA, EVNOTA_VELOCIDAD};

enum {METAEVENTO_TIPO, METAEVENTO_LONGITUD};

//TDA NOTA
typedef struct {
    uint32_t *t0;
    uint32_t *tf;
    uint32_t *a;
    int *ff; //frecuencia fundamental
    size_t n;
}notas_t;

//TDA TRAMOFIMAL
/*typedef struct{
    float *v;
    size_t n;
}tramoFinal_t;
*/


bool tomarArgumentos(size_t , char *[], char *, char *, char *, size_t *, int *, int *);

int tomarFrecuencia(nota_t , signed char);

sintetizador_t *tomarSint(char *);

notas_t *tomarNotas(char *);

tramo_t *muestrearTramo(sintetizador_t*, notas_t* , int);

void destruirNotas(notas_t *);

void destruirSint(sintetizador_t *);

//void destruirTramo(tramo_t *)

#endif
