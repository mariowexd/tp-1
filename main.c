#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "tp1.h"
#include "ej3.h"
#include "ej45.h"
#include "mod.h"

int main(int argc, char* argv[]){
    
    char nombre_txt[255];
    char nombre_mid[255];
    char nombre_wav[255];
    size_t canal = 0;
    int frec_muestreo = 44100;
    int pulsos_p_segundo = 2; // todavia no nos dieron un valor por defecto a usar

    bool k = tomarArgumentos(argc, argv, nombre_txt, nombre_mid, nombre_wav, &canal, &frec_muestreo, &pulsos_p_segundo);
    if (k==false) return 1;
    sintetizador_t *sint = tomarSint(nombre_txt);
    if (sint==NULL){
        printf("Fallo tomarSint\n");
        return 1;
    }
    notas_t *notas = tomarNotas(nombre_mid);
    if (notas==NULL){
        destruirSint(sint);
        return 1;
    }

    printf("HAY %lu NOTAS\n", notas->n);
    /*for(size_t x=0; x<notas->n; x++){
        //printf("t0 = %d   tf = %d   a = %d   ff = %d\n", notas->t0[x], notas->tf[x], notas->a[x], notas->ff[x]);
    }*/
    tramo_t *tramo = muestrearTramo(sint, notas, frec_muestreo);
    if (tramo==NULL){
        destruirNotas(notas);
        destruirSint(sint);
    }
    for(size_t x=0; notas->n; x++){
        printf("%f\n", tramo->v[x]);
    }
    //destruirTramo(tramo);*/
    destruirNotas(notas);
    destruirSint(sint);
    return 0;
}
