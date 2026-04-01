//PROBLEMA DEI LETTORI-SCRITTORI CON ATTESA INDEFINITA DI ENTRAMBI

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <string.h>
#include "header.h"

static void die(const char* msg){
    perror(msg);
    exit(1);
}

int main(){

    int numprocessi = 10;

    // TODO: Allocazione memoria condivisa
    key_t chiave = IPC_PRIVATE;

    int shm_id = shmget(chiave, sizeof(Buffer), IPC_CREAT|0664);
    if(shm_id < 0) die("SHMGet Error\n");

    // TODO: Collegarsi alla memoria condivisa
    Buffer * p = (Buffer *) shmat(shm_id, 0, 0);
    if (p == (void*)-1 ) die("SHMAT Error\n");

    // TODO: Inizializzare le variabili
    p->messaggio = 0;
    p->numlettori = 0;
    p->numscrittori = 0;

    // TODO: Allocazione semafori e inizializzazione
    key_t chiave_sem = IPC_PRIVATE;
    int sem_id = semget(chiave_sem, 4, IPC_CREAT|0664);
    if(sem_id < 0) die("SEMGet Error");

    semctl(sem_id, MUTEXL, SETVAL, 1);
    semctl(sem_id, MUTEXS, SETVAL, 1);
    semctl(sem_id, MUTEX, SETVAL, 1);
    semctl(sem_id, SYNCH, SETVAL, 1);

    // TODO: Creazione processi figli
    for(int k = 0; k < numprocessi; k++){

        pid_t pid = fork();

        if (pid < 0) die("Fork Error");
        
        if(pid == 0){

            if((k%2)== 0){

                printf("Creazione figlio Scrittore [PID = %d]\n", getpid());
                Scrittore(sem_id, p);

            }else{

                printf("Creazione figlio Lettore [PID = %d]\n", getpid());
                Lettore(sem_id, p);

            }
            exit(0);
        }
    }


    // TODO: Attendere terminazione dei processi
    int status;
    for (int k=0; k<numprocessi; k++){
                pid_t term = wait(&status);
                if (term == -1)
                        perror("Errore Wait");
                else
                        printf ("Figlio n.ro %d è morto con status = %d\n ", term, status);
    }

    // TODO: Deallocazione memoria e semafori
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);

    return 0;

}