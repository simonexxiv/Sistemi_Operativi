#ifndef _HEADER_PRODCONS
#define _HEADER_PRODCONS

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define LIBERO 1
#define OCCUPATO 0

struct buffer {
    
    int valore;

    /* TBD: Aggiungere altre variabili per la sincronizzazione */
    int stato; 
    pthread_mutex_t mutex;
    pthread_cond_t cv_cons;
    pthread_cond_t cv_prod;
};

void buffer_init(struct buffer * b);
void buffer_produci(struct buffer * b, int valore);
int buffer_consuma(struct buffer * b);
void buffer_destroy(struct buffer * b);

static inline void die(const char *msg){
    perror(msg);
    exit(1);
}

#endif