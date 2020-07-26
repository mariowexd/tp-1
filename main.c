#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "tp1.h"
#include "modulacion.h"
#define NOMBRE_MAX 255; //longitud maxima de nombres de archivos permitido en Linux

int main(int argc, char* argv[]){
    char nombre_txt[255];
    char nombre_mid[255];
    char nombre_wav[255];
    size_t canal = 0;
    int frec_muestreo = 44100;
    int pulsos_p_segundo = 2; // todavia no nos dieron un valor por defecto a usar
    
    bool k = tomarArgumentos(argc, argv, nombre_txt, nombre_mid, nombre_wav, &canal, &frec_muestreo, &pulsos_p_segundo);
    if (k==false) return 1;

    armo_t *armonicos = tomarArmonicos(nombre_txt);
    if (armonicos==NULL) return 1;

    destruirArmonicos(armonicos);
    return 0;
}
