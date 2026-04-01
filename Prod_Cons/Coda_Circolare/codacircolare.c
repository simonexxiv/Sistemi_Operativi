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
}

int main(){

//DEFINIRE LA CHIAVE PER LA MEMORIA CONDIVISA
key_t chiave = IPC_PRIVATE;

//ALLOCARE LA MEMORIA CONDIVISA
int shm = shmget(chiave, sizeof(struct prodcons), IPC_CREAT|0664);
if (shm < 0) die("SHM Error");

//ATTACCARE SHARED MEMORY AL PROCESSO
struct prodcons *p = (struct prodcons *) shmat(shm, NULL, 0);
if (p == (void *)-1) die("SHMAT Error");

//INIZIALIZZARE TESTA E CODA
p->testa = 0;
p->coda = 0;


//DEFINIRE LA CHIAVE PER IL SET DI SEMAFORI
key_t chiave_sem = IPC_PRIVATE;

//ALLOCARE 4 SEMAFORI
int sem_id = semget(chiave_sem, 4, IPC_CREAT|0664);
if (sem_id < 0) die("SEM Error");

//INIZIALIZZARE I SEMAFORI
semctl(sem_id, SPAZIO_DISPONIBILE, SETVAL, DIM_BUFFER);
semctl(sem_id, MESSAGGIO_DISPONIBILE, SETVAL, 0);
semctl(sem_id, MUTEX_P, SETVAL, 1);
semctl(sem_id, MUTEX_C, SETVAL, 1);

// TODO: Creare NUM_CONSUMATORI processi figli con fork
    // All'interno del figlio:
    // - Stampare messaggio di avvio
    // - Inizializzare srand con getpid() * time(NULL)
    // - Chiamare la funzione consumatore
    // - Terminare il processo con exit

for(int i = 0; i < NUM_CONSUMATORI; i++){

pid_t pid = fork();
if (pid<0) die("Errore creazione Consumatori");
else if(pid == 0){

printf("Avvio [Figlio PID = %d] consumatore\n", getpid());

srand(getpid()*time(NULL));

consumatore(p, sem_id);

exit(0);
}
}

// TODO: Creare NUM_PRODUTTORI processi figli con fork
    // All'interno del figlio:
    // - Stampare messaggio di avvio
    // - Inizializzare srand con getpid() * time(NULL)
    // - Chiamare la funzione produttore
    // - Terminare il processo con exit

for(int i = 0; i < NUM_PRODUTTORI; i++){

pid_t pid = fork();
if (pid<0) die("Errore creazione Produttori");
else if(pid == 0){

printf("Avvio [Figlio PID = %d] produttore\n", getpid());

srand(getpid()*time(NULL));

produttore(p, sem_id);

exit(0);

}
}

// TODO: Attendere la terminazione di tutti i figli produttori con wait
for(int i = 0; i < NUM_PRODUTTORI; i++){

pid_t termine = wait(NULL);
printf("[Padre] Figlio produttore PID = %d terminato\n", termine);

}

// TODO: Attendere la terminazione di tutti i figli consumatori con wait
for(int i = 0; i < NUM_CONSUMATORI; i++){

pid_t termine = wait(NULL);
printf("[Padre] Figlio consumatore PID = %d terminato\n", termine);

}

// TODO: Deallocare le risorse condivise

shmctl(shm, IPC_RMID, NULL);
semctl(sem_id, 0, IPC_RMID);

return(0);


}