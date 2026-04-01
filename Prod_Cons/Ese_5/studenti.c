#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <time.h>      
#include <sys/wait.h> 

#include "procedure.h"

static void die(const char * msg){
    perror(msg);
    exit(1);
}

int main(){

    // TODO: Allocare la shared memory
    key_t chiave = ftok(".", 'A');

    int shm_id = shmget(chiave, sizeof(coda_stampa), IPC_CREAT|0664);
    if (shm_id < 0) die("SHM Error");

    // TODO: Attaccare la shared memory alla memoria condivisa
    coda_stampa * p = (coda_stampa*) shmat(shm_id, NULL, 0);
    if(p == (void*)-1) die("SHMAT Error");

    // TODO: Inizializzare i valori
    p->coda = 0;
    p->testa = 0;
    p->studenti_vivi = NUM_STUDENTI;

    // TODO: Allocare i semafori e inizializzarli
    key_t chiave_sem = ftok(".", 'B');

    int sem_id = semget(chiave_sem, 4, IPC_CREAT|0664);
    if (sem_id < 0) die("SEM Error");

    semctl(sem_id, SPAZIO_DISPONIBILE, SETVAL, DIM_BUFFER);
    semctl(sem_id, MESSAGGIO_DISPONIBILE, SETVAL, 0);
    semctl(sem_id, MUTEX_STUDENTI, SETVAL, 1);
    semctl(sem_id, MUTEX_ADDETTI, SETVAL, 1);

    // TODO: Creare figli studenti
    for(int i = 0; i < NUM_STUDENTI; i++){

        pid_t pid = fork();
        if (pid < 0) die("FORK Error");
        if (pid == 0){

            srand(time(NULL)*getpid());
            studente(p, sem_id);
            exit(0);
            
        }
    }

    // TODO: Attendere terminazione figli
    for(int i = 0; i < NUM_STUDENTI; i++){
        wait(NULL);
    }

    return 0;

}