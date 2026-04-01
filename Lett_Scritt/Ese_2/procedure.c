#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>

#include "header.h"
#include "semafori.h"

// TODO: Procedure di INIZIO e FINE Lettura

void InizioLettura(int sem_id, Buffer *buff){

    Wait_Sem(sem_id, MUTEXL);

    //Incremento numero lettori
    buff->numlettori = buff->numlettori + 1;

    //Se si tratta del primo lettore blocca gli scrittori
    if(buff->numlettori == 1) Wait_Sem(sem_id, SYNCH);

    Signal_Sem(sem_id, MUTEXL);

}

void FineLettura(int sem_id, Buffer * buff){

    Wait_Sem(sem_id, MUTEXL);

    //Decremento numero lettori
    buff->numlettori = buff->numlettori -1;

    //Se si tratta dell'ultimo allra dai la signal agli scrittori
    if(buff->numlettori == 1) Signal_Sem(sem_id, SYNCH);

    Signal_Sem(sem_id, MUTEXL);

}

// TODO: Procedure di INIZIO e FINE Scrittura

void InizioScrittura(int sem_id, Buffer *buff){

    Wait_Sem(sem_id, MUTEXS);

    //Incremento numero lettori
    buff->numscrittori = buff->numscrittori + 1;

    //Se si tratta del primo lettore blocca gli scrittori
    if(buff->numscrittori == 1) Wait_Sem(sem_id, SYNCH);

    //Rilascia il mutex per far entrare altri scrittori per potersi mettere in attesa
    Signal_Sem(sem_id, MUTEXS);

    //Blocco eventuali scrittori per la scrittura vera e propria
    Signal_Sem(sem_id, MUTEX);

}

void FineScrittura(int sem_id, Buffer *buff){

    //Rilascio il mutex per gli scrittori che devono scrivere
    Signal_Sem(sem_id, MUTEX);

    Wait_Sem(sem_id, MUTEXS);



    //Incremento numero lettori
    buff->numscrittori = buff->numscrittori - 1;

    //Se si tratta del primo lettore blocca gli scrittori
    if(buff->numscrittori == 0) Signal_Sem(sem_id, SYNCH);

    //Rilascia il mutex per far entrare altri scrittori per potersi mettere in attesa
    Signal_Sem(sem_id, MUTEXS);

}

void Scrittore(int sem_id, Buffer *buff){

    InizioScrittura(sem_id, buff);

    struct timeval t1;
    struct timeval t2;
    gettimeofday(&t1,&t2);    //valore diverso ad ogni scrittura
    buff->messaggio =t1.tv_usec;
	sleep(1);
    printf("Valore scritto: [%ld] \n", buff->messaggio);

    FineScrittura(sem_id, buff);

}

void Lettore(int sem_id, Buffer *buff){

    InizioLettura(sem_id, buff);

    sleep(1);
    printf("Valore letto = [%ld], numero lettori = %d \n", buff->messaggio, buff->numlettori);

    FineLettura(sem_id, buff);

}