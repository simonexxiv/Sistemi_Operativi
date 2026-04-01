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

int i = 0;

// TODO: Wait su Spazio e Wait su Mutex per entrare in sezione critica
Wait_Sem(sem_id, SPAZIO_DISPONIBILE);
Wait_Sem(sem_id, MUTEX_P);

// TODO: Cercare un buffer libero (BUFFER_VUOTO) nel vettore stato[]
while( i < DIM_BUFFER && p->stato[i]!= BUFFER_VUOTO) i++;

// TODO: Marcare il buffer trovato come BUFFER_INUSO
p->stato[i] = BUFFER_INUSO;

// TODO: Uscire dalla sezione critica!!
Signal_Sem(sem_id, MUTEX_P);

// TODO: Produzione, generare un valore casuale e inserirlo 
sleep(2);
 
p->buffer[i] = rand() % 100;
printf("Il valore prodotto è %d\n", p->buffer[i]);


// TODO: Marcare il buffer come BUFFER_PIENO
p->stato[i] = BUFFER_PIENO;


// TODO: Segnalare messaggio disponibile
Signal_Sem(sem_id, MESSAGGIO_DISPONIBILE);
   

}


void consumatore(struct prodcons *p, int sem_id){

int i = 0;

// TODO: Wait su Messaggio e Wait su Mutex per entrare in sezione critica
Wait_Sem(sem_id, MESSAGGIO_DISPONIBILE);
Wait_Sem(sem_id, MUTEX_C);

// TODO: Cercare un buffer pieno nel vettore stato[]
while( i < DIM_BUFFER && p->stato[i]!= BUFFER_PIENO) i++;

// TODO: Marcare il buffer trovato come BUFFER_INUSO
p->stato[i] = BUFFER_INUSO;

// TODO: Uscire dalla sezione critica!!
Signal_Sem(sem_id, MUTEX_C);

// TODO: Consumazione
sleep(2);
 
printf("Il valore consumato è %d\n", p->buffer[i]);

// TODO: Marcare il buffer come BUFFER_VUOTO
p->stato[i] = BUFFER_VUOTO;

// TODO: Segnalare messaggio disponibile
Signal_Sem(sem_id, SPAZIO_DISPONIBILE);
   

}