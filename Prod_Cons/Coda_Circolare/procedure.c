#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>

#include "procedure.h"
#include "semafori.h"

void produttore(struct prodcons *p, int sem_id){

//Wait iniziali
Wait_Sem(sem_id, SPAZIO_DISPONIBILE);

Wait_Sem(sem_id, MUTEX_P);

sleep(2);

//Produzione e Assegnazione
p->buffer[p->testa] = rand() % 100; 
printf("Il valore prodotto è %d\n", p->buffer[p->testa]);

p->testa = (p->testa+1) % DIM_BUFFER;


//Signal finali
Signal_Sem(sem_id, MUTEX_P);

Signal_Sem(sem_id, MESSAGGIO_DISPONIBILE);

}

void consumatore(struct prodcons *p, int sem_id){

//Wait iniziali
Wait_Sem(sem_id, MESSAGGIO_DISPONIBILE);

Wait_Sem(sem_id, MUTEX_C);

sleep(2);

//Consumazione
printf("Il valore consumato è %d\n", p->buffer[p->coda]);
p->coda = (p->coda+1) % DIM_BUFFER;


//Signal finali
Signal_Sem(sem_id, MUTEX_C);

Signal_Sem(sem_id, SPAZIO_DISPONIBILE);

}