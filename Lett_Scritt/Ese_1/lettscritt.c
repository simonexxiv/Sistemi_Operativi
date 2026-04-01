// Lettore scrittore con starvation degli scrittori

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"

static void die(const char *text){
    perror(text);
    exit(1);
}

int main() {

    // TODO: Definizione dei parametri fondamentali
    int numlettori = NUM_LETTORI;
    int numscrittori = NUM_SCRITTORI;
    int numprocessi = numlettori + numscrittori;

    // TODO: Allocazione memoria condivisa
    key_t chiave = IPC_PRIVATE;
    int shm_id = shmget(chiave, sizeof(Buffer), IPC_CREAT|0664);
    if(shm_id < 0) die("SHM Error");

    
    // TODO: Attaccare la memoria e inizializzare i campi della struct
    Buffer* buff = (Buffer*) shmat(shm_id, NULL, 0);
    buff->numlettori = 0;
    buff->messaggio = 0;

    // TODO: Allocazione e inizializzazione semafori
    key_t chiave_sem = IPC_PRIVATE;
    int sem_id = semget(chiave_sem, 2, IPC_CREAT|0664);
    if (sem_id < 0) die("SEM Error");

    semctl(sem_id, MUTEXL, SETVAL, 1);
    semctl(sem_id, SYNCH, SETVAL, 1);

    // TODO: Generazione processi figli
    for(int k = 0; k < numprocessi; k ++){

        pid_t pid = fork();
        if(pid < 0) die("Fork Error");
        if(pid == 0){

            // Processo figlio

            if((k%2)== 0){

                printf("[Scrittore] PID = %d avviato\n", getpid());
                Scrittore(sem_id, buff);

            }else{


                printf("[Lettore] PID = %d avviato\n", getpid());
                Lettore(sem_id, buff);

            }
            
            exit(0);    //Il figlio deve terminare qui per non tornare nel ciclo for

        }
    }


    // TODO: Attesa terminazione processi figli
    int status;
    pid_t pid_finito;

    for (int k = 0; k < numprocessi; k++) {
        pid_finito = wait(&status);
        
        if (pid_finito == -1) {
            perror("Errore WAIT");
        } else {
            printf("Processo %d terminato (status: %d)\n", pid_finito, WEXITSTATUS(status));
        }
    }

    // TODO: Deallocazione delle risorse 
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);

    printf("### Simulazione conclusa. Risorse deallocate. ###\n");

    return 0;


}
