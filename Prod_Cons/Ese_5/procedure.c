#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include "procedure.h"
#include "semafori.h"

void studente (coda_stampa *coda, int sem_id){

    printf("Avvio Studente [PID = %d]\n", getpid());

    // TODO: Attendere spazio disponibile 
    Wait_Sem(sem_id, SPAZIO_DISPONIBILE);

    // TODO: Entrare in sezione critica 
    Wait_Sem(sem_id, MUTEX_STUDENTI);


    // TODO: Inserire documento in buffer
    int r = rand()%10000 + 1000;
    coda->buffer[coda->testa] = r;

    printf("Aggiornato Buffer a %d con %d\n", coda->testa, coda->buffer[coda->testa]);

    // TODO: Aggiornare indice testa
    coda->testa = (coda->testa + 1) % DIM_BUFFER;

    // TODO: Aggiorna studenti rimasti 
    coda->studenti_vivi--;

    // TODO: Uscire da sezione critica 
    Signal_Sem(sem_id, MUTEX_STUDENTI);

    // TODO: Segnalare presenza di nuovo documento 
    Signal_Sem(sem_id, MESSAGGIO_DISPONIBILE);

    // TODO: Terminare
    exit(0);

}

void addetto (coda_stampa *coda, int sem_id){

    printf("Avvio Addetto [PID = %d]\n", getpid());

    while(1){
    
    // TODO: Attendere documento
    Wait_Sem(sem_id, MESSAGGIO_DISPONIBILE);

    // TODO: Entrare in sezione critica
    Wait_Sem(sem_id, MUTEX_ADDETTI);

    // TODO: Prelevare documento dal buffer
    int arch = coda->buffer[coda->coda];

    // TODO: Aggiornare indice coda
    coda->coda = (coda->coda + 1) % DIM_BUFFER;

    printf("Addetto %d: Documento archiviato %d\n", getpid(), arch);

    // TODO: Uscire da sezione critica 
    Signal_Sem(sem_id, MUTEX_ADDETTI);

    // TODO: Segnalare spazio disponibile
    Signal_Sem(sem_id, SPAZIO_DISPONIBILE);

    // TODO: Simulare archiviazione documento
    sleep(1);

    // Se era l'ultimo messaggio e gli studenti sono morti, esco
    if(coda->studenti_vivi == 0 && coda->testa == coda->coda) {
        // Prima di uscire, faccio una Signal per il collega che è ancora bloccato
        Signal_Sem(sem_id, MESSAGGIO_DISPONIBILE); 
        printf("Addetto %d: Buffer vuoto. Passo la sveglia e esco.\n", getpid());
        break; 
    }

    }

}