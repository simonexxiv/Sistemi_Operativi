#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <unistd.h>

#include "header.h"
#include "semafori.h"

void InizioLettura(int sem_id, Buffer * buff){

    // TODO: Proteggere l'acceso alla variabile condivisa numlettori
    Wait_Sem(sem_id, MUTEXL);

    // TODO: Incrementare il numero di lettori attivi
    buff->numlettori = buff->numlettori + 1;
    
    // TODO: Se sono il PRIMO lettore, bloccare l'accesso agli scrittori
    if( buff->numlettori == 1) Wait_Sem (sem_id, SYNCH);
 
    // TODO: Rilasciare il mutex per permettere ad altri lettori di entrare
    Signal_Sem(sem_id, MUTEXL);

}

void FineLettura(int sem_id, Buffer * buff){

    // TODO: Proteggere il decremento dei lettori
    Wait_Sem(sem_id, MUTEXL);

    // TODO: Decrementare il numero di lettori attivi
    buff->numlettori = buff->numlettori -1;

    // TODO: Se sono l'ULTIMO lettore, liberare la risorsa agli scrittori
    if( buff->numlettori == 0) Signal_Sem(sem_id, SYNCH);

    // TODO: Rilasciare il mutex per gli altri lettori
    Signal_Sem(sem_id, MUTEXL);

}

void InizioScrittuta(int sem_id){
    Wait_Sem(sem_id, SYNCH);
}

void FineScrittura(int sem_id){
    Signal_Sem(sem_id, SYNCH);
}


void Scrittore(int sem_id, Buffer* buff){

    // TODO: Bloccare l'accesso sia ad altri scrittori che a TUTTI i lettor
    InizioScrittuta(sem_id);

    // TODO: Utilizzo di gettimeofday per generare un dato variabile (timestamp)
    struct timeval t1;
    struct timeval t2;
    gettimeofday (&t1, &t2); 

    // TODO: Scrivere il valore nel campo 'messaggio' della struttura condivisa
    msg val = t1.tv_usec;
    buff->messaggio = val;

    // TODO: Simulare il tempo di scrittura (fondamentale per testare la mutua esclusione)
    sleep(1);
    printf("Valore scritto = <%ld> \n", buff->messaggio);

    // TODO: Rilasciare la risorsa e permettere l'accesso agli altri
    FineScrittura(sem_id);

}

void Lettore(int sem_id, Buffer* buff){

    // TODO: Registrare l'ingresso del lettore e, se primo, bloccare gli scrittori
    InizioLettura(sem_id, buff);

    // TODO: Simulare il tempo di lettura
    sleep(1); 
    
    // TODO: Leggere il valore. NOTA: numlettori qui sarà >= 1
    printf("Valore letto = <%ld>, numero lettori = %d \n", buff->messaggio, buff->numlettori);
    
    // TODO: Registrare l'uscita e, se ultimo, sbloccare gli scrittori
    FineLettura(sem_id, buff);

}