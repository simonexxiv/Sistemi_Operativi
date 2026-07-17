#ifndef _HEADER_PRODCONS_H_
#define _HEADER_PRODCONS_H_

#include <sys/sem.h>

#define DIM_BUFF 2
#define SPAZIO_DISPONIBILE 0
#define MESSAGGIO_DISPONIBILE 1
#define MUTEXC 2
#define MUTEXP 3

typedef struct {

    int valore[DIM_BUFF];
    //TODO: aggiungere variabili per gestione circolare con semafori
    int coda;
    int testa;

    int sem_id;

} BufferProdCons;

#endif