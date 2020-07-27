#ifndef _ej3_h_
#define _ej3_h_

typedef struct {
    float *v;
    size_t n;
    double t0;
    int f_m;
} tramo_t;

size_t calcular_n(double, double, int);
void tramo_destruir(tramo_t *);
tramo_t *_tramo_crear(double, double, int);
tramo_t *tramo_clonar(const tramo_t *);
void muestrear_senoidal(float, size_t, double, int, float, float);
void muestrear_armonicos(float, size_t, double, int float, float, const float [][], size_t);
tramo_t *tramo_crear_muestreo(double, double, int, float, float, const float [][], size_t);
bool tramo_redimensionar(tramo_t *, double);
bool tramo_extender(tramo_t *, tramo_t *);
void imprimir_muestras(const float [], size_t, double, int);
#endif
