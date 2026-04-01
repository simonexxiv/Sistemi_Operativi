#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include "procedure.h"
#include "semafori.h"

int main() {

    

    // 1. Allocazione Memoria Condivisa (shmget)
    key_t chiave = IPC_PRIVATE;
    int shm_id = shmget(chiave, sizeof(struct condivisa), IPC_CREAT | 0664);

    if(shm_id < 0){
        perror("SHM Error");
        exit(1);
    }

    struct condivisa *s = (struct condivisa*) shmat(shm_id, NULL, 0);

    // 2. Allocazione Array di Semafori (semget - servono 2 semafori)
    key_t chiavesem = IPC_PRIVATE;
    int sem_id = semget(chiavesem, 2, IPC_CREAT | 0664);

    if (sem_id < 0){
        perror("SEM Error");
        exit(1);
    }


    // TODO: 3. Inizializzazione Semafori (semctl con SETVAL)
    semctl(sem_id, SPAZIO_DISPONIBILE, SETVAL, 1);
    semctl(sem_id, MESSAGGIO_DISPONIBILE, SETVAL, 0);


    // 4. Creazione Processi
    pid_t pid;

    //CONSUMATORE
    pid = fork();
    if (pid < 0){ perror("Fork Error"); exit(1); }
    if (pid == 0){
        printf("Figlio Consumatore Avviato\n");
        consumatore(s, sem_id);
        exit(0);
    }

    //PRODUTTORE
    pid = fork();
    if (pid < 0){ perror("Fork Error"); exit(1); }
    if (pid == 0){
        printf("Figlio Produttore Avviato\n");
        produttore(s, sem_id);
        exit(0);
    }


    // 5. Attesa terminazione figli
    wait(NULL);
    printf("Primo figlio terminato.\n");

    wait(NULL);
    printf("Secondo figlio terminato.\n");


    // TODO: 6. Rimozione Risorse (shmctl e semctl con IPC_RMID)
    // Non dimenticarlo o sporcherai la memoria della VM!
    shmctl(shm_id, IPC_RMID, NULL);
    semctl(sem_id, 0, IPC_RMID);


    printf("Main: Risorse rimosse. Fine programma.\n");
    return 0;
}