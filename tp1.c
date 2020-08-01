#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "mod.h"
#include "ej3.h"
#include "tp1.h"
#include "ej45.h"

char *fcad[] = {
    [CONSTANT]="CONSTANT",
    [LINEAR]="LINEAR",
    [INVLINEAR]="INVLINEAR",
    [SIN]="SIN",
    [EXP]="EXP",
    [INVEXP]="INVEXP",
    [QUARTCOS]="QUARTCOS",
    [QUARTSIN]="QUARTSIN",
    [HALFCOS]="HALFCOS",
    [HALFSIN]="HALFSIN",
    [LOG]="LOG",
    [INVLOG]="INVLOG",
    [TRI]="TRI",
    [PULSES]="PULSES"
};
float (*p[])(double t, float params[]) = {
    [CONSTANT] = mConstant,
    [LINEAR] = mLinear,
    [INVLINEAR] = mInvlinear,
    [SIN] = mSin,
    [EXP] = mExp,
    [INVEXP] = mInvexp,
    [QUARTCOS] = mQuartCos,
    [QUARTSIN] = mQuartSin,
    [HALFCOS] = mHalfCos,
    [HALFSIN] = mHalfSin,
    [LOG] = mLog,
    [INVLOG] = mInvlog,
    [TRI] = mTri,
    [PULSES] = mPulses
};
bool tomarArgumentos(size_t n, char *v[], char *txt, char *mid, char *wav, size_t *c, int *f, int *r){
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
    //printf("naux es%lu\n", naux);
    
    sintetizador_t *sint =malloc(sizeof(sintetizador_t));
    sint->v = (float **)malloc(naux*sizeof(float *));
    for(size_t x = 0; x<naux; x++){
        sint->v[x] = (float *)malloc(2*sizeof(float));
    }

    sint->n = naux;
    char aux2[CANT_CHAR_RENGLON];

    for(size_t x=0; x<naux; x++){
        size_t cant_chars = CANT_CHAR_RENGLON+1+(x+1)/10;
        if(fgets(aux2, cant_chars, archivo)==NULL){
            fclose(archivo);
            return NULL;
        }
        sint->v[x][0] = x+1;
        sint->v[x][1] = atof(aux2+2);
    }
    char cad[3][CANT_CHAR_MAX_MOD];
    char aux3[CANT_CHAR_MAX_MOD];
    for(size_t x=0; x<3; x++){
    size_t aux5;
        if(fgets(cad[x],CANT_CHAR_MAX_MOD+1,archivo)==NULL){
            fclose(archivo);
            return NULL;
        }
        //printf("%s",cad[x]);
        size_t y;
        for(y=0; y<CANT_CHAR_RENGLON && cad[x][y]!=' '; y++){
            aux3[y]=cad[x][y];
        }
        aux5=y;
        for(size_t l = 0; cad[x][y]!='\n' && cad[x][y]!='\0'; y++){
            if(cad[x][y]==' '){
                sint->parametros[x][l] = atof(&cad[x][y+1]);
                //printf("El parametro leido es %f\n", sint->parametros[x][l]);
                l++;
            }
            //else printf("No era espacio\n");
        }
        aux3[aux5]='\0';
        size_t i = 0;
        for(funmod_t z = 0; z<CANT_MODOS; z++){
            size_t k = strcmp(aux3,fcad[z]);
            if(k==0){
                sint->p[i] = p[z];
                //printf("%s\n", fcad[z]);
                i++;
            }
        }
    }
    fclose(archivo);
    return sint;
}
notas_t *tomarNotas(char *nombre_mid){
    FILE *f = fopen(nombre_mid, "rb");
    if(f == NULL) {
        printf("No se pudo abrir\n");
        return NULL;
    }

    formato_t formato;
    uint16_t numero_pistas;
    uint16_t pulsos_negra;

    if(! leer_encabezado(f, &formato, &numero_pistas, &pulsos_negra)) {
        fprintf(stderr, "Fallo lectura encabezado\n");
        fclose(f);
        return NULL;
    }

    notas_t *notas = malloc(sizeof(notas_t));
    notas->n = 0;
    if(notas==NULL){
        fclose(f);
        return NULL;
    }
    notas->t0 = NULL;
    notas->tf = NULL;
    notas->ff = NULL;
    notas->a = NULL;
    size_t x = 0;
    uint32_t *t0aux = NULL;
    uint32_t *tfaux = NULL;
    uint32_t *aaux = NULL;
    int *ffaux = NULL;

    for(uint16_t pista = 0; pista < numero_pistas; pista++) {
        uint32_t tamagno_pista;
        if(! leer_pista(f, &tamagno_pista)) {
            fprintf(stderr, "Fallo lectura pista\n");
            fclose(f);
            return NULL;
        }

        evento_t evento;
        char canal;
        int longitud;
        uint32_t tiempo = 0;

        while(1) {
            uint32_t delta_tiempo;
            leer_tiempo(f, &delta_tiempo);
            tiempo += delta_tiempo;
           
            uint8_t buffer[EVENTO_MAX_LONG];
            if(! leer_evento(f, &evento, &canal, &longitud, buffer)) {
                fprintf(stderr, "Error leyendo evento\n");
                fclose(f);
                return NULL;
           }

            if(evento == METAEVENTO && canal == 0xF) {
                if(buffer[METAEVENTO_TIPO] == METAEVENTO_FIN_DE_PISTA) {
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
                //float *tfaux = NULL;
                tfaux = realloc(notas->tf, sizeof(uint32_t)*(x));
                if(tfaux == NULL){
                    free(t0aux);
                    free(tfaux);
                    free(aaux);
                    free(ffaux);
                    destruirNotas(notas);
                    fprintf(stderr, "Error leyendo nota\n");
                    fclose(f);
                    return NULL;
                }
                notas->tf = tfaux;
                bool k = false;
                int frec = tomarFrecuencia(nota, octava);
                int y = x-1;
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
                //float *t0aux = NULL;
                t0aux = realloc(notas->t0, sizeof(uint32_t)*(x+1));
                if(t0aux == NULL){
                    free(t0aux);
                    free(tfaux);
                    free(aaux);
                    free(ffaux);
                    destruirNotas(notas);
                    fprintf(stderr, "Error leyendo nota\n");
                    fclose(f);
                    return NULL;
                }
                notas->t0 = t0aux;
                notas->t0[x] = tiempo;
                //float *aaux = NULL;
                aaux = realloc(notas->a, sizeof(uint32_t)*(x+1));
                if(aaux == NULL){
                    free(t0aux);
                    free(tfaux);
                    free(aaux);
                    free(ffaux);
                    destruirNotas(notas);
                    fprintf(stderr, "Error leyendo nota\n");
                    fclose(f);
                    return NULL;
                }
                notas->a = aaux;
                notas->a[x] = buffer[EVNOTA_VELOCIDAD];
                //int *ffaux = NULL;
                ffaux = realloc(notas->ff, sizeof(int)*(x+1));
                if(ffaux == NULL){
                    free(t0aux);
                    free(tfaux);
                    free(aaux);
                    free(ffaux);
                    destruirNotas(notas);
                    fprintf(stderr, "Error leyendo nota\n");
                    fclose(f);
                    return NULL;
                }
                notas->ff = ffaux;
                notas->ff[x] = tomarFrecuencia(nota, octava);
                x++;
            }
        }
    }
    notas->n = x;
    fclose(f);

    return notas;
}
/*
float *tomarAmplitud(sintetizador_t *sint, notas_t *notas, size_t x){
    double tn = notas->tf[x] - notas->t0[x];
    while(tn < TIEMPO_ATAQUE){
        return notas->a * sint->p[0];
     }
    while(TIEMPO_ATAQUE < tn < TIEMPO_SOSTENIDO){
        return notas->a *= sint->p[1];
    }
    while(tn > TIEMPO_SOSTENIDO){
        notas->a *= sint->p[2];
        return notas->a;
    }
}*/

tramo_t *muestrearTramo(sintetizador_t *sint, notas_t *notas, int f_m, int pps){
    size_t x = 0;
    double t0 = (notas->t0[x])/(double)pps;
    double tf = notas->tf[x]/(double)pps;
    float f = notas->ff[x];
    float a = notas->a[x];
    size_t n_fa = sint->n;
    tramo_t *tramo = tramo_crear_muestreo(t0, tf, f_m, f, a, (const float**)sint->v, n_fa);
    if(tramo == NULL) return NULL;
    imprimir_muestras(tramo->v, tramo->n, t0, tramo->f_m);
    return tramo;
}
void destruirTramo(tramo_t *tramo){
    free(tramo->v);
    free(tramo);
}

void destruirSint(sintetizador_t *sint){
    for(size_t x = 0; x<sint->n; x++){
        free(sint->v[x]);
    }
    free(sint->v);
    free(sint);
}

void destruirNotas(notas_t *notas){
    free(notas->t0);
    free(notas->tf);
    free(notas->a);
    free(notas->ff);
    free(notas);
}
