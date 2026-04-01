#ifndef PROCEDURE_H
#define PROCEDURE_H

#define NUM_STUDENTI 30
#define NUM_ADDETTI 5

#define DIM_BUFFER 10

typedef struct {
    int buffer[DIM_BUFFER];
    int testa;                  //testa: dove scrivono gli studenti
    int coda;                   //coda: dove legono gli addetti
    int studenti_vivi;
} coda_stampa;

// TODO: Implementare i define per un buffer circolare
#define SPAZIO_DISPONIBILE 0
#define MESSAGGIO_DISPONIBILE 1
#define MUTEX_STUDENTI 2
#define MUTEX_ADDETTI 3

//Prototipi delle funzioni
void studente(coda_stampa * coda, int sem_id );
void addetto(coda_stampa* coda, int sem_id);

#endif 