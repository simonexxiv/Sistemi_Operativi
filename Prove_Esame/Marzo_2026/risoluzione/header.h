#ifndef _HEADER_H
#define _HEADER_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define X 1
#define Y 2

#define LIBERO 0
#define OCCUPATO 1

typedef struct{
    int buffer_x;
    int buffer_y;

    int stato_x;
    int stato_y;

    /*TBD: Completare aggiungendo ulteriori variabili per la sincronizzazione*/
    pthread_mutex_t mutex;
    pthread_cond_t cv_cons;
    pthread_cond_t cv_prod;

}coppia_buffer;

typedef struct{
    
    /*TBD: Completare la struttura del messaggio*/
    long type;
    int value;

}messaggio_sensore;

typedef struct{
    
    /*TBD: Completare la struttura del messaggio*/
    long type;
    int value;

}messaggio_attuatore;

static inline void die(const char* msg){
    perror(msg);
    exit(1);
}

void sensore_x(int coda_sensori);
void sensore_y(int coda_sensori);
void planner(int coda_sensori, int coda_attuatore);
void attuatore(int coda_attuatore);

void init_coppia_buffer(coppia_buffer * b);
void remove_coppia_buffer(coppia_buffer * b);
void produci(coppia_buffer * buffer, int tipo, int valore);
void consuma(coppia_buffer * buffer, int * valore_x, int * valore_y);

#endif