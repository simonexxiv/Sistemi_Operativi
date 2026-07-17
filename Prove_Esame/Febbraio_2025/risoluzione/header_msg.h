#ifndef _HEADER_
#define _HEADER_

#include <stdio.h>
#include <stdlib.h>
#define MESSAGGIO 1

typedef struct {

    //TODO completare struttura messaggio
    long tipo;
    int valore;

} messaggio_sensore;

typedef struct {

    //TODO completare struttura messaggio
    long tipo;
    int valore;

} messaggio_collettore;


#define NUM_MESSAGGI_PER_SENSORE 5
#define NUM_SENSORI 3

void sensore(int id_sensore, int id_queue_collettore);
void collettore(int id_queue_collettore, int id_queue_server);

static inline void die(const char* msg){
    perror(msg);
    exit(1);
}

#endif