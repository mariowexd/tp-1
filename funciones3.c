#include "ej3.h"

size_t calcular_n(double t0, double tf, int f_m){
    size_t n = (tf-t0)*f_m+0.5;
    return n;
}

void tramo_destruir(tramo_t *t){
    free(t->v);
    free(t);
}
tramo_t *_tramo_crear(double t0, double tf, int f_m){

    tramo_t *tramo1 = malloc(sizeof(tramo_t));
    if (tramo1 == NULL){
        return NULL;
        printf("No hay memoria");
    }
    size_t n = calcular_n(t0, tf, f_m);
    tramo1->v   = malloc(sizeof(float)*n);
    if (tramo1->v == NULL){
        tramo_destruir(tramo1);
        return NULL;
    }
    for (size_t x=0; x<n; x++){
        tramo1->v[x]=0;
    }
    tramo1->n   = n;
    tramo1->t0  = t0;
    tramo1->f_m = f_m;

    return tramo1;
}

tramo_t *tramo_clonar(const tramo_t *t){
    double tf= t->t0 + (double)(t->n)/(t->f_m);
    tramo_t *tramoClon = _tramo_crear(t->t0, tf, t->f_m);
    if (tramoClon==NULL){
        printf("no se pudo copiar");
        return NULL;
    }
    for(size_t x=0; x<tramoClon->n; x++){
        tramoClon->v[x]=t->v[x];
    }
    return tramoClon;
}

void muestrear_senoidal(float v[], size_t n, double t0, int f_m, float f, float a){
    for(int i=0; i<n; i++) {
        v[i] += a*sin(2*PI*f*((double)i/f_m+t0));
    }
}

void muestrear_armonicos(float v[], size_t n, double t0, int f_m, float f, float a, const float fa[][2], size_t n_fa){
    for(int x=0; x<n_fa; x++){
        float frecArmo = f * fa[x][0];
        float amplArmo = a * fa[x][1];
        muestrear_senoidal(v, n, t0, f_m, frecArmo, amplArmo);
    }
}

tramo_t *tramo_crear_muestreo(double t0, double tf, int f_m, float f, float a, const float fa[][2], size_t n_fa){  //reciclaje de MuestrearArmonicos!
    tramo_t *tramo1 = _tramo_crear(t0, tf, f_m);
    if (tramo1 == NULL) return NULL;
    muestrear_armonicos(tramo1->v, tramo1->n, t0, f_m, f, a, fa, n_fa);
    return tramo1;

}

bool tramo_redimensionar(tramo_t *t, double tf){
    size_t nNuevo=(tf-t->t0)*t->f_m;
    float *aux = malloc(sizeof(float)*nNuevo);
    if (aux==NULL) return false;
    for(size_t x = 0; x < t->n; x++){
        aux[x] = t->v[x];
    }
    for(size_t x = t->n;  x<nNuevo; x++){
        aux[x] = 0;
    }
    t->n = nNuevo;
    free(t->v);
    t->v = aux;
    return true;
}

bool tramo_extender(tramo_t *destino, tramo_t *extension){
    if ((destino->t0)>(extension->t0)){
        printf("El t0 de destino es mayor que el t0 de extension");
        return false;
    }
    if ((destino->f_m)!=(extension->f_m)){
        printf("Las frecuencias de muestreo de los tramos son diferentes");
        return false;
    }
    double tfdest= destino->t0 + (double)(destino->n)/(destino->f_m);
    double tfexte= extension->t0 + (double)(extension->n)/(extension->f_m);
    size_t deltaN = ((double)(extension->t0)-(destino->t0))*destino->f_m;

    if (tfexte > tfdest){
        bool k = tramo_redimensionar(destino, tfexte);
        if (k==0) return false;
    }

    for(size_t x=0; x<extension->n; x++){
        destino->v[x+deltaN] += extension->v[x];
    }
    return true;
}


void imprimir_muestras(const float v[], size_t n, double t0, int f_m){
    for(int i=t0*f_m; i<n; i++){
        float fi = (float)i;
        float ff = (float)f_m;
        printf("%f,%f\n", fi/ff, v[i]);
    }
}
