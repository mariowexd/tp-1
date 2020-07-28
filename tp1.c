#include "modulacion.h"

#define NOMBRE_MAX 255
#define CANT_ARG 6
#define CANT_ARG_TXT 3
#define ARG_MAX 12
#define CANT_CHAR_RENGLON 10
#define CANT_ARM_MAX 4

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

void destruirNotas(nota_t notas){
    free(notas->t0);
    free(notas->tf);
    free(notas->a);
    free(notas->ff);
    free(notas);
}
void destruirArmonicos(armo_t *armos){
    free(armos->v);
    free(armos);
}

int tomarFrecuencia(nota_t *, octava){
}

armo_t *tomarArmonicos(char *nombre){
    FILE *archivo = fopen(nombre, "rt");
    if (archivo==NULL){
        printf("Archivo sintetizador no encontrado");
        return NULL;
    }

    size_t naux = 0;
    char aux1[CANT_ARM_MAX];
    fgets(aux1, CANT_ARM_MAX, archivo);
    naux = atoi(aux1);

    armo_t *armos = malloc(sizeof(armo_t));
    armos->v = malloc(naux*sizeof(float));

    armos->n = naux;
    char aux2[CANT_CHAR_RENGLON];
    size_t cant_chars = 0;
    for(size_t x=0; x<naux; x++){
        cant_chars = CANT_CHAR_RENGLON+1+(x+1)/10;
        fgets(aux2, cant_chars, archivo);
        armos->v[x] = atof(aux2+2);
        printf("%f\n", armos->v[x]);
    }
    fclose(archivo);
    return armos;
}

notas_t tomarNotas(char *nombre_mid){
    notas_t *notas;
    //Leo encabezado
    formato_t formato;
    uint16_t numero_pistas;
    uint16_t pulsos_negra;
    
    FILE *f = fopen(nombre, "rb");
    if (archivo==NULL){
        printf("Archivo sintetizador no encontrado");
        return NULL;
    }
    
    if(! leer_encabezado(f, &formato, &numero_pistas, &pulsos_negra))
        return NULL;
    
    //Leo e itero las pistas
    for(uint16_t pista = 0; pista < numero_pistas; pista++) {
        uint32_t tamagno_pista;
        if(! leer_pista(f, &tamagno_pista))
            return NULL;
        //Leo los evetos
        evento_t evento;
        char canal;
        int longitud;
        uint32_t tiempo;
        
        while(1) {
            //iterador (lo incrementamos al final del ciclo)
            size_t i = 0;
            //incremento n va de la mano con i
            notas.n[i] += 1; //NO, NOTAS ES UN PUNTERO IRIA ASI notas->n+=1 APARTE N NO ES UN PUNTERO FIJATE
            //Guardo el tiempo
            uint32_t delta_tiempo;
            leer_tiempo(f, &delta_tiempo);
            tiempo += delta_tiempo;
      
            if(! leer_evento(f, &evento, &canal, &longitud, buffer))
                return NULL;

            else if(evento == METAEVENTO && canal == 0xF) {
                if(buffer[METAEVENTO_TIPO] == METAEVENTO_FIN_DE_PISTA)
                    break;
                descartar_metaevento(f, buffer[METAEVENTO_LONGITUD]);
            }

            else if(evento == NOTA_ENCENDIDA){
                notas.t0[i] = tiempo;
                size_t l = i;
            }
            
            else if(evento == NOTA_APAGADA)
                notas.tf[l] = tiempo - notas.t0[l];

            

        }

    } 
    fclose(f);
}
