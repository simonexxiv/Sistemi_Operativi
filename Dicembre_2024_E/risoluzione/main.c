
#include <complex.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <sys/wait.h>
#include "header.h"
#include "time.h"

int main() {
/*
    TODO: completare la creazione della memoria condivisa e inizializzazione struttura
*/
int shm_id = shmget(IPC_PRIVATE, sizeof(MonitorPC), IPC_CREAT|0664);
if( shm_id < 0){
    perror("Errore shm\n");
    exit(1);
}

MonitorPC * m = (MonitorPC *) shmat(shm_id, NULL, 0);

inizializza_coda(m);

/*
    TODO: completare la creazione della coda di messaggi
*/
key_t chiave_coda = ftok(".", 'A');
int coda_risposte = msgget(chiave_coda, IPC_CREAT|0664);
    

/*
    TODO: figlio consumatore
*/

pid_t pid = fork();
if (pid < 0){
    perror("errore fork\n");
    exit(1);
}else if(pid == 0){
    printf("[MAIN] Avvio consumatore\n");
    Consumatore(m, coda_risposte);
    exit(0);
}

    pid_t pidp;
    for (int i = 0; i < NUM_BAMBINI; i++) {
        /*
            TODO: completare la creazione dei figli produttori
        */
        pidp = fork();
        srand(time(NULL)*getpid());

        if(pidp < 0){
            perror("errore fork\n");
            exit(1);
        }else if (pidp == 0) {

            int id = rand() % 11;
            Produttore(m, id, coda_risposte);
            exit(0);
        }
            

        sleep(rand() % 3 + 1);
    }

    /*
        TODO: attesa terminazione processi e rimozione strutture
    */
    while(wait(NULL)> 0);

    msgctl(coda_risposte, IPC_RMID, NULL);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}