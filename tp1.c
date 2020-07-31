#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "mod.h"
#include "ej3.h"
#include "tp1.h"
#include "ej45.h"
#define NOMBRE_MAX 255
#define CANT_ARG 6
#define CANT_ARG_TXT 3
#define ARG_MAX 12
#define CANT_CHAR_RENGLON 11
#define CANT_ARM_MAX 4
#define LA_4_FREC 440
#define LA_4_VAL 69
#define RAIZ_12_2 1.05946309435929

#define METAEVENTO_FIN_DE_PISTA 0x2F
#define EVENTO_MAX_LONG 10

enum {EVNOTA_NOTA, EVNOTA_VELOCIDAD};

enum {METAEVENTO_TIPO, METAEVENTO_LONGITUD};

bool tomarArgumentos(size_t n, char *v[], char *txt, char *mid, char *wav, size_t *c, int *f, int *r)
{
    if (n > ARG_MAX)
        return false;
    bool lecturas[CANT_ARG_TXT] = {false, false, false};
    for (size_t x = 0; x < (n - 1); x++)
    {
        if (strcmp(v[x], "-s") == 0)
        {
            lecturas[0] = true;
            strcpy(txt, v[x + 1]);
            //x +=2;
        }
        if (strcmp(v[x], "-i") == 0)
        {
            lecturas[1] = true;
            strcpy(mid, v[x + 1]);
            //x+=2;
        }
        if (strcmp(v[x], "-o") == 0)
        {
            lecturas[2] = true;
            strcpy(wav, v[x + 1]);
            //x+=2;
        }
        if (strcmp(v[x], "-c") == 0)
        {
            *c = atoi(v[x + 1]);
            //x+=2;
        }
        if (strcmp(v[x], "-f") == 0)
        {
            *f = atoi(v[x + 1]);
            //x+=2;
        }
        if (strcmp(v[x], "-r") == 0)
        {
            *r = atoi(v[x + 1]);
            //x+=2;
        }
    }
    if (lecturas[0] == false || lecturas[1] == false || lecturas[2] == false)
        return false;
    return true;
}
int tomarFrecuencia(nota_t nota, signed char octava){
    int n = nota+12*(octava+1);
    n = n - LA_4_VAL;
    float factor = pow(RAIZ_12_2, n);
    return LA_4_FREC*factor;
}
sintetizador_t *tomarSint(char *nombre){
    FILE *archivo = fopen(nombre, "rt");
    if (archivo==NULL){
        printf("Archivo sintetizador no encontrado");
        return NULL;
    }
    size_t naux = 0;
    char aux1[CANT_ARM_MAX];
    fgets(aux1, CANT_ARM_MAX, archivo);
    naux = atoi(aux1);
    
    sintetizador_t *sint =malloc(sizeof(sintetizador_t));
    sint->v = (float **)malloc(naux*sizeof(float *));
    for(size_t x = 0; x<naux; x++){
        sint->v[x] = (float *)malloc(2*sizeof(float));
    }

    sint->n = naux;
    char aux2[CANT_CHAR_RENGLON];

    for(size_t x=0; x<naux; x++){
        size_t cant_chars = CANT_CHAR_RENGLON+1+(x+1)/10;
        fgets(aux2, cant_chars, archivo);
        sint->v[x][0] = x+1;
        sint->v[x][1] = atof(aux2+2);
        //printf("%f   %f\n", sint->v[x][0], sint->v[x][1]);
    }

    fclose(archivo);
    return sint;
}


notas_t *tomarNotas(char *nombre_mid){
    // APERTURA DE ARCHIVO:
    FILE *f = fopen(nombre_mid, "rb");
    if(f == NULL) {
        printf("No se pudo abrir\n");
        return NULL;
    }

    // LECTURA DEL ENCABEZADO:
    formato_t formato;
    uint16_t numero_pistas;
    uint16_t pulsos_negra;

    if(! leer_encabezado(f, &formato, &numero_pistas, &pulsos_negra)) {
        fprintf(stderr, "Fallo lectura encabezado\n");
        fclose(f);
        return NULL;
    }

    //printf("Encabezado:\n\tFormato: %s\n\tNumero de pistas: %d\n\tPulsos por negra: %d\n", codificar_formato(formato), numero_pistas, pulsos_negra);

    notas_t *notas = malloc(sizeof(notas_t));
    size_t x = 0;

    // ITERAMOS LAS PISTAS:
    for(uint16_t pista = 0; pista < numero_pistas; pista++) {
        // LECTURA ENCABEZADO DE LA PISTA:
        uint32_t tamagno_pista;
        if(! leer_pista(f, &tamagno_pista)) {
            fprintf(stderr, "Fallo lectura pista\n");
            fclose(f);
            return NULL;
        }

        //printf("Pista %d:\n\tTama~no: %d\n", pista, tamagno_pista);

        evento_t evento;
        char canal;
        int longitud;
        uint32_t tiempo = 0;

        // ITERAMOS LOS EVENTOS:
        while(1) {
            uint32_t delta_tiempo;
            leer_tiempo(f, &delta_tiempo);
            tiempo += delta_tiempo;
            //printf("[%d] ", tiempo);


            // LECTURA DEL EVENTO:
            uint8_t buffer[EVENTO_MAX_LONG];
            if(! leer_evento(f, &evento, &canal, &longitud, buffer)) {
                fprintf(stderr, "Error leyendo evento\n");
                fclose(f);
                return NULL;
            }

            //printf("Evento: %s, Canal: %d", codificar_evento(evento), canal);

            // PROCESAMOS EL EVENTO:
            if(evento == METAEVENTO && canal == 0xF) {
                // METAEVENTO:
                if(buffer[METAEVENTO_TIPO] == METAEVENTO_FIN_DE_PISTA) {
                    //putchar('\n');
                    //printf("Final de la pista %d.\n", pista);
                    break;
                }

                descartar_metaevento(f, buffer[METAEVENTO_LONGITUD]);
            }
            else if (evento == NOTA_APAGADA || (evento == NOTA_ENCENDIDA && (buffer[EVNOTA_VELOCIDAD] == 0))) {
                nota_t nota;
                signed char octava;
                if(! decodificar_nota(buffer[EVNOTA_NOTA], &nota, &octava)) {
                    fprintf(stderr, "Error leyendo nota\n");
                    fclose(f);
                    return NULL;
                }
                notas->tf = realloc(notas->tf, sizeof(uint32_t)*(x));
                bool k = false;
                int frec = tomarFrecuencia(nota, octava);
                int y = x;
                while(y>=0) {
                    if(notas->ff[y]==frec){
                        notas->tf[y]=tiempo;
                        k=true;
                        break;
                    }
                    --y;
                }
            if (k==false){
                printf("Error leyendo nota\n");
            }
            }
            else if (evento == NOTA_ENCENDIDA) {
                nota_t nota;
                signed char octava;
                if(! decodificar_nota(buffer[EVNOTA_NOTA], &nota, &octava)) {
                    fprintf(stderr, "Error leyendo nota\n");
                    fclose(f);
                    return NULL;
                }
                notas->t0 = realloc(notas->t0, sizeof(uint32_t)*(x+1));
                notas->t0[x] = tiempo;
                notas->a = realloc(notas->a, sizeof(uint32_t)*(x+1));
                notas->a[x] = buffer[EVNOTA_VELOCIDAD];
                notas->ff = realloc(notas->ff, sizeof(int)*(x+1));
                notas->ff[x] = tomarFrecuencia(nota, octava);
                //printf("T0=[%d]  A=[%d]  FF=[%d]  ", notas->t0[x], notas->a[x], notas->ff[x]);
                x++;
            }
        }
    }
    //printf("notas->n = %lu\n", notas->n);
    notas->n = x;
    fclose(f);

    return notas;
}

void destruirNotas(notas_t *notas){
    free(notas->t0);
    free(notas->tf);
    free(notas->a);
    free(notas->ff);
    free(notas);
}

void destruirSint(sintetizador_t *sint){
    free(sint->v);
    free(sint);
}
