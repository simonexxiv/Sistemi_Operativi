#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

#include "monitor_fifo.h"

#define PROCESSI_PRODUTTORI 5
#define PROCESSI_CONSUMATORI 2

#define NUMERO_PRODUZIONI 4
#define NUMERO_CONSUMAZIONI 10

void consumatore(MonitorProdConsFIFO * p);
void produttore(MonitorProdConsFIFO * p);

int main() {

    /* TBD: Creare l'oggetto monitor, utilizzando la funzione "inizializza()" */
    int shm_id = shmget(IPC_PRIVATE, sizeof(MonitorProdConsFIFO), IPC_CREAT|0664);
    if( shm_id < 0) die("Errore SHMGET\n");

    MonitorProdConsFIFO * p = (MonitorProdConsFIFO *) shmat(shm_id, NULL, 0);
    if (p == (void*)-1) die("Errore SHMAT\n");

    inizializza(p);

    /* TBD: Avviare i processi produttori, facendogli eseguire la funzione "produttore()"
            e i processi consumatori, facendogli eseguire la funzione "consumatore()"
    */

    
    for(int i = 0; i < PROCESSI_PRODUTTORI; i++){

        pid_t pid = fork();
        if(pid < 0) die("Errore Fork\n");
        else if (pid == 0){
            printf("[START] Avvio processo Produttore...\n");
            produttore(p);
            exit(0);
        }

    }

    for(int i = 0; i < PROCESSI_CONSUMATORI; i++){

        pid_t pid = fork();
        if(pid < 0) die("Errore Fork\n");
        else if (pid == 0){
            printf("[START] Avvio processo Consumatore...\n");
            consumatore(p);
            exit(0);
        }

    }

    while(wait(NULL)> 0);

    /* TBD: Rimuovere l'oggetto monitor, utilizzando la funzione "distruggi()" */
    distruggi(p);

    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}


void produttore(MonitorProdConsFIFO * p) {

    srand(getpid());

    for(int i=0; i<NUMERO_PRODUZIONI; i++) {

        int valore = rand() % 100;

        printf("[PRODUTTORE %d] Produzione valore %d\n", getpid(), valore);

        /* TBD: Chiamare la funzione "produci()" del monitor */
        produci(p, valore);

    }

}

void consumatore(MonitorProdConsFIFO * p) {

    for(int i=0; i<NUMERO_CONSUMAZIONI; i++) {

        printf("[CONSUMATORE %d] Inizio consumazione\n", getpid());

        int valore;

        /* TBD: Chiamare la funzione "consuma()" del monitor */
        valore = consuma(p);

        printf("[CONSUMATORE %d] Consumazione valore %d\n", getpid(), valore);
    }

}