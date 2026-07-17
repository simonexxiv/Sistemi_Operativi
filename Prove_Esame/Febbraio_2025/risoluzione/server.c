#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>

#include "header_prodcons.h"
#include "header_msg.h"
#include "semafori.h"


void ricevi_da_collettore(int id_queue_server, BufferProdCons * b);
void visualizza(BufferProdCons * b);
void produci_valore(BufferProdCons * b, int valore);
int consuma_valore(BufferProdCons * b);

int main () {

    int sem_key = IPC_PRIVATE/*TODO*/;
    int sem_id = semget(sem_key, 4, IPC_CREAT|0664);/*TODO*/;
    if(sem_id < 0) die("Errore semget\n");

    //TODO inizializzazione semafori
    semctl(sem_id, SPAZIO_DISPONIBILE, SETVAL, DIM_BUFF);
    semctl(sem_id, MESSAGGIO_DISPONIBILE, SETVAL, 0);
    semctl(sem_id, MUTEXC, SETVAL, 1);
    semctl(sem_id, MUTEXP, SETVAL, 1);

    //TODO inizializzazione struttura condivisa
    int shm_id = shmget(IPC_PRIVATE, sizeof(BufferProdCons), IPC_CREAT|0664);
    if(shm_id < 0) die("Errore SHMGET\n");

    BufferProdCons * b = (BufferProdCons*) shmat(shm_id, NULL, 0);/*TODO*/;
    if(b == (void*)-1) die("Errore SHMAT\n");

    b->coda = 0;
    b->testa = 0;
    b->sem_id = sem_id;

    key_t chiave_server = ftok(".", 'S');/*TODO*/;
    int id_queue_server = msgget(chiave_server, 0)/*TODO*/;


    //TODO: Creazione processo "ricevi_da_collettore"
    pid_t pid = fork();
    if(pid < 0) die("Errore Fork RIcevi da collettore\n");
    else if(pid == 0){
        printf("[SERVER] Avvio processo Ricevi da collettore\n");
        ricevi_da_collettore(id_queue_server, b);
        exit(0);
    }
    
    //TODO: Creazione 2 processi "visualizza"
    for(int i = 0; i< 2; i++){
        pid = fork();
        if(pid < 0) die("[SERVER] Errore Visualiazzatore\n ");
        else if( pid == 0){
            printf("[SERVER] Creazione Processo Visualiazzatore %d\n", i+1);
            visualizza(b);
            exit(0);
        }
    }

    //TODO: attesa processi figli
    while(wait(NULL) > 0);

    //TODO: rimozione risorse IPC
    semctl(b->sem_id, 0, IPC_RMID);
    shmctl(shm_id, IPC_RMID, NULL);
    msgctl(id_queue_server, IPC_RMID, NULL);

    return 0;
}

void ricevi_da_collettore(int id_queue_server, BufferProdCons * b){

    for(int i=0; i<NUM_MESSAGGI_PER_SENSORE; i++) {

        //TODO: ricezione messaggio con media da collettore
        messaggio_collettore m;
        int ret = msgrcv(id_queue_server, (void*)&m, sizeof(messaggio_collettore)-sizeof(long), MESSAGGIO, 0);

        int media = m.valore/*TODO*/;

        printf("[SERVER] Ricevuto Media: %d\n", media);

        produci_valore(b, media);

    }

}

void visualizza(BufferProdCons * b) {

    for(int i=0; i<NUM_MESSAGGI_PER_SENSORE/2; i++) {
        sleep(2);
        int media = consuma_valore(b);

        printf("[SERVER] Visualizzo valore medio corrente=%d\n", media);
    
    }

}


void produci_valore(BufferProdCons * b, int valore) {

    //TODO: implementare produttore
    Wait_Sem(b->sem_id, SPAZIO_DISPONIBILE);
    Wait_Sem(b->sem_id, MUTEXP);

    b->valore[b->testa] = valore;
    b->testa = (b->testa +1) % DIM_BUFF;

    printf("[SERVER-PRODUTTORE] Prodotto Valore %d\n", valore);

    Signal_Sem(b->sem_id, MUTEXP);
    Signal_Sem(b->sem_id, MESSAGGIO_DISPONIBILE);

}

int consuma_valore(BufferProdCons * b) {

    //TODO: implementare consumatore
    Wait_Sem(b->sem_id, MESSAGGIO_DISPONIBILE);
    Wait_Sem(b->sem_id, MUTEXC);

    int valore = b->valore[b->coda];
    b->coda = (b->coda +1) % DIM_BUFF;

    printf("[SERVER-CONSUMATORE] Consumato Valore %d\n", valore);

    Signal_Sem(b->sem_id, MUTEXC);
    Signal_Sem(b->sem_id, SPAZIO_DISPONIBILE);

    return valore;

}