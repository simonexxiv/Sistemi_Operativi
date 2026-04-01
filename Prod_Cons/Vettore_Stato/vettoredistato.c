#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>

#include "procedure.h"
#include "semafori.h"

static void die(const char *msg){

    perror(msg);
    exit(1);

};


int main(){

// TODO: Allocare shared memory
key_t chiave = IPC_PRIVATE;

int shm_id = shmget(chiave, sizeof(struct prodcons), IPC_CREAT|0664);
if( shm_id < 0) die("SHM Error");

// TODO: Attaccare la shared memory al processo
struct prodcons *p = (struct prodcons*) shmat(shm_id, NULL, 0);
if (p==(void*)-1) die("SHMAT Error");

// TODO: Inizializzare vettore buffer a vuoto
for (int i = 0; i < DIM_BUFFER; i++){

    p->stato[i] = BUFFER_VUOTO;

}


// TODO: Allocare i semafori
key_t chiave_sem = IPC_PRIVATE;

int sem_id = semget(chiave_sem, 4, IPC_CREAT|0664);
if (sem_id < 0) die("SEM Error");

// TODO: Inizializzare i 4 semafori
semctl(sem_id, SPAZIO_DISPONIBILE, SETVAL, DIM_BUFFER);
semctl(sem_id, MESSAGGIO_DISPONIBILE, SETVAL, 0);
semctl(sem_id, MUTEX_P, SETVAL, 1);
semctl(sem_id, MUTEX_C, SETVAL, 1);


// TODO: Creare i processi figli PRODUTTORI
for(int i = 0; i < NUM_PRODUTTORI; i++){

pid_t pid = fork();

if(pid < 0) die("Fork Error");

if(pid == 0){

printf("Avvio figlio produttore [PID = %d]\n", getpid());
srand(getpid() * time(NULL));

produttore(p, sem_id);
exit(1);

}
}

// TODO: Creare i processi figli CONSUMATORI
for(int i = 0; i < NUM_CONSUMATORI; i++){

pid_t pid = fork();

if(pid < 0) die("Fork Error");

if(pid == 0){

printf("Avvio figlio consumatore [PID = %d]\n", getpid());
srand(getpid() * time(NULL));

consumatore(p, sem_id);
exit(1);

}
}

// TODO: Attendere la terminazione dei figli produttori con wait()
for(int i = 0; i < NUM_PRODUTTORI; i++){
    pid_t temp = wait(NULL);
    printf("Figlio produttore [PID = %d] terminato\n", getpid());
}

// TODO: Attendere la terminazione dei figli consumatori con wait()
for(int i = 0; i < NUM_CONSUMATORI; i++){
    pid_t temp = wait(NULL);
    printf("Figlio consumatore [PID = %d] terminato\n", getpid());
}

// TODO: Deallocare le risorse
shmctl(shm_id, IPC_RMID, NULL);
semctl(sem_id, 0, IPC_RMID);

return(0);

}