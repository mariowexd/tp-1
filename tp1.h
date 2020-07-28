#ifndef _tp1_h_
#define _tp1_h_
//TDA ARMONICOS
typedef struct {
    float *v;
    size_t n;
} armo_t;

//TDA NOTA
typedef struct {
    float *t0;
    float *tf;
    float *a;
    int *ff; //frecuencia fundamental
    size_t n;
} notas_t

bool tomarArgumentos(size_t , char *[], char *, char *, char *, size_t *, int *, int *);
armo_t *tomarArmonicos(char *);
nota_t *tomarNotas(char *);
void destruirNotas(nota_t *);
void destruirArmonicos(armo_t *);
#endif