#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "procedure.h"

static void die(const char *msg){
    perror(msg);
    exit(1);
}

int main(){

    // TODO: Creare chiave IPC privata
    key_t chiave = ftok(".", 'A');

    // TODO: Allocare shared memory abbastanza grande per 80 posti + disponibilita
    int shm_id = shmget(chiave, sizeof(struct shm_data), IPC_CREAT|0664);
    if (shm_id < 0) die("SHM Error");

    // TODO: Attaccare la shared memory
    struct shm_data * p = (struct shm_data *) shmat(shm_id, NULL, 0);
    if (p == (void*)-1) die("SHMAT Error");

    // TODO: Inizializzare tutti i posti a stato LIBERO e id_cliente 0
    for (int i = 0; i < DIM_POSTI; i++){
        p->teatro[i].id_cliente = 0;
        p->teatro[i].stato = LIBERO;
    }

    // TODO: Inizializzare disponibilità a 80
    p->disponibilita = DIM_POSTI;
    printf("Teatro inizializzato con %d posti disponibili\n", DIM_POSTI);

    // TODO: Istanziare semaforo
    key_t chiave_sem = ftok(".", 'B');

    int sem_id = semget(chiave_sem, 1, IPC_CREAT|0664);
    if (sem_id < 0) die("SEMGET Error");

    // TODO: Inizializzarre semaforo
    semctl(sem_id, MUTEX, SETVAL, 1);
    

    // TODO: Creare 50 processi figli con fork()
    for(int i = 0; i < NUM_FIGLI; i++){

        pid_t pid = fork();
        if(pid < 0) die("Errore Fork");
        if(pid == 0){

        printf("Avvio Figlio cliente [PID = %d]\n", getpid());
        srand(getpid()*time(NULL));

        cliente(p->teatro, &(p->disponibilita), sem_id);
        exit(1);

        }

    }

    // TODO: Attendere la terminazione di tutti i figli (wait())
    for(int i = 0; i < NUM_FIGLI; i ++){

        int status;
        pid_t term = wait(&status);

        if(WIFEXITED(status)){
            int exit_code = WEXITSTATUS(status);
            printf("[PADRE] Terminato PID figlio = %d con exit code %d\n", term, exit_code);
        }

    }

    // TODO: Deallocare risorse
    shmctl(shm_id, IPC_RMID, NULL);

    semctl(sem_id, 0, IPC_RMID);

    return 0;
    
    

}
