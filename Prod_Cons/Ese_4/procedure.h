#ifndef PROCEDURE_H
#define PROCEDURE_H

//Definisco i semafori che occorrono
#define MUTEX 0

//Definisco il num max di posti
#define DIM_POSTI 80

//Definisco il numero di figli
#define NUM_FIGLI 50

//Definisco la struct della traccia
typedef struct {
    unsigned int id_cliente; //PID del cliente 
    unsigned int stato; //0 = libero, 1 = occupato, 2 = in aggiornamento
} posto;

//Definizione stato
#define LIBERO 0
#define OCCUPATO 1
#define  IN_AGGIORNAMENTO 2

//Definisco la struct della shared memory
struct shm_data{
    posto teatro[DIM_POSTI];
    int disponibilita;
};

//Funzione cliente 
void cliente(posto *teatro, int* disponibilita, int sem_id);

#endif

