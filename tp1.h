#ifndef _tp1_h_
#define _tp1_h_

typedef struct {
    float *v;
    size_t n;
} armo_t;

bool tomarArgumentos(size_t , char *[], char *, char *, char *, size_t *, int *, int *);
armo_t *tomarArmonicos(char *);
void destruirArmonicos(armo_t *);
#endif