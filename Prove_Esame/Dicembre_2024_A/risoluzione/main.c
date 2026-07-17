#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "header.h"

#define NUM_CONSUMI 10

int main() {

    /*
        TODO: inizializzazione segmenti di memoria e strutture condivisi
    */
    key_t chiave_shm = IPC_PRIVATE;
    int shm_id = shmget(chiave_shm, sizeof(MonitorProdCons), IPC_CREAT|0664);
    if(shm_id < 0) die("Erorre SHMGET\n");

    MonitorProdCons * m = (MonitorProdCons *) shmat(shm_id, NULL, 0);
    if( m == (void*)-1) die("Errore SHMAT\n");

    inizializza_vettore(m);

    key_t chiave_bb = IPC_PRIVATE;
    int shm_bb = shmget(chiave_bb, sizeof(BufferBabbo), IPC_CREAT|0664);
    if (shm_bb < 0) die("Erorre SHMGET BB\n");

    BufferBabbo * bb = (BufferBabbo *) shmat(shm_bb, NULL, 0);
    if(bb == (void*)-1) die("Errrore SMHAT BB\n");

    inizializza_buffer(bb);

    pid_t pid_p, pid_c1, pid_c2, pid_b;

    /*
        TODO: creazione di 4 figli: un Produttore, un Consumatore1, un Consumatore2 e BabboNatale
    */
    pid_p = fork();
    if(pid_p <0) die("Errore Fork Prod\n");
    else if(pid_p == 0){
        printf("[MAIN] Avvio processo Produttore...\n");
        Produttore(m);
        exit(0);
    }

    pid_c1 = fork();
    if(pid_c1 <0) die("Errore Fork Cons 1\n");
    else if(pid_c1 == 0){
        printf("[MAIN] Avvio processo Consumatore 1...\n");
        Consumatore1(m, bb);
        exit(0);
    }

    pid_c2 = fork();
    if(pid_c2 <0) die("Errore Fork Cons 2\n");
    else if(pid_c2 == 0){
        printf("[MAIN] Avvio processo Consumatore 2...\n");
        Consumatore2(m, bb);
        exit(0);
    }


    pid_b = fork();
    if(pid_b <0) die("Errore Fork Babb\n");
    else if(pid_b == 0){
        printf("[MAIN] Avvio processo Babbo Natale...\n");
        BabboNatale(bb);
        exit(0);
    }

    /*
        TODO: attesa terminazione figli
    */
    while(wait(NULL)> 0);

    /*
        TODO: distruzione strutture
    */
    rimuovi_vettore(m);
    shmctl(shm_id, IPC_RMID, NULL);

    rimuovi_buffer(bb);
    shmctl(shm_bb, IPC_RMID, NULL);

    return 0;
}