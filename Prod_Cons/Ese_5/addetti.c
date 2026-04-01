#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h> 

#include "procedure.h"

static void die(const char *msg) {
    perror(msg);
    exit(1);
}

int main(){

// TODO: Collegarsi alla memoria condivisa

key_t chiave = ftok(".", 'A');

int shm_id = shmget(chiave, sizeof(coda_stampa), IPC_CREAT|0664);
if (shm_id < 0) die ("SHM Error Addetti");

coda_stampa *p = (coda_stampa *) shmat(shm_id, NULL, 0);
if (p == (void*)-1) die( "SHMAT Error Addetti");

// TODO: Collegarsi ai semafori
key_t chiave_sem = ftok(".", 'B');

int sem_id = semget( chiave_sem, 4, IPC_CREAT|0664);
if (sem_id < 0) die("SEMGET Error Addetti");

// TODO: Creare figli addetti
for (int i = 0; i < NUM_ADDETTI; i ++){

    pid_t pid = fork();
    if (pid < 0) die("FORK Error Addetti");
    if (pid == 0){

        addetto(p, sem_id);
        exit(0);

    }
}

// TODO: Attendere terminazione figli
for(int i = 0; i < NUM_ADDETTI; i++){
    wait(NULL);
}

// TODO: Stampa stato finale della shm
printf("\n--- Stato finale della coda di stampa ---\n");
printf("Testa: %d\n", p->testa);
printf("Coda: %d\n", p->coda);
printf("Studenti vivi: %d\n", p->studenti_vivi);

for (int i = 0; i < DIM_BUFFER; i++) {
    printf("Buffer[%d]: %d\n", i, p->buffer[i]);
}

// TODO: Deallocare memoria condivisa e samofori
shmctl(shm_id, IPC_RMID, NULL);
semctl(sem_id, 0, IPC_RMID);

return(0);

}