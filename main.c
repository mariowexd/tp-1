#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "tp1.h"
#include "ej3.h"
#include "ej4_5.h"
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

    sintetizador_t *sint = tomarSint(nombre_txt);
    if (sint==NULL) return 1;

    notas_t *notas = tomarNotas(nombre_mid); //Esta esta por hacer
    if (nombre_mid==NULL) return 1; 

    destruirNotas(notas);
    destruirSint(sint);
    return 0;
}
