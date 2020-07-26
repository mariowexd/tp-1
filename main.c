#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "tp1.h"
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

    printf("El nombre del txt es %s\nEl nombre del mid es %s\nEl nombre del wav es %s\nEl canal es%lu\nLa frec es %d\nLos pps son %d",
            nombre_txt, nombre_mid, nombre_wav, canal, frec_muestreo, pulsos_p_segundo); //Para probar que tomarArgumentos anda ok


    return 0;
}
