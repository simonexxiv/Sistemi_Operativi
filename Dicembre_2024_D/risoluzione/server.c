#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include "header.h"

int main() {

    /*
        TODO: recupero code e inizializzazione strutture
    */
    key_t chiave_richieste = ftok(".", 'A');
    int coda_richieste = msgget(chiave_richieste, 0);

    key_t chiave_risposte = ftok(".", 'B');
    int coda_risposte = msgget(chiave_risposte, 0);

    int shm_id = shmget(IPC_PRIVATE, sizeof(MonitorLettScritt), IPC_CREAT|0664);
    if(shm_id < 0){
        perror("Errore shmget\n");
        exit(1);
    }

    MonitorLettScritt * ls = (MonitorLettScritt *) shmat(shm_id, NULL, 0);

    inizializza_monitor(ls);

    pid_t pids, pidl;

    for (int i=0; i<NUM_BAMBINI*NUM_LETTERE; i++) {
        /*
            TODO: creare figli
        */
        pids = fork();
        if (pids < 0){
            perror("Server Errore Fork\n");
            exit(1);
        }else if(pids == 0){
            sleep(rand()%3 + 1);
            Scrittore(coda_richieste, ls);
            exit(0);
        }
            

    }

    for (int i=0; i<NUM_BAMBINI*NUM_LETTERE; i++) {
        /*
            TODO: creare figli
        */
        pidl = fork();
        if (pidl < 0){
            perror("Server Errore Fork\n");
            exit(1);
        }else if(pidl == 0){
            sleep(rand()%3 + 1);
            Lettore(coda_risposte, ls);
            exit(0);
        }

    }

    /*
        TODO: attesa terminazione figli
    */
    while(wait(NULL)> 0);
    
    /*
        TODO: distruzione risorse
    */
    distruggi_monitor(ls);

    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}