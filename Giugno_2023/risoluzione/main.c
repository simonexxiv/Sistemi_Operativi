#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <time.h>

#include "prodcons.h"

void Produttore(MonitorBufferSingolo* b,int id_coda);
void Consumatore(MonitorBufferSingolo* b,int id_coda);

int main() {
	/* TBD: Allocare l'oggetto MonitorBufferSingolo in memoria condivisa */;
    key_t chiave_shm = IPC_PRIVATE;
    int shm_id = shmget(chiave_shm, sizeof(MonitorBufferSingolo), IPC_CREAT|0664);
    if(shm_id < 0) die("Errore shmget\n");

    MonitorBufferSingolo * b = (MonitorBufferSingolo*) shmat(shm_id, NULL, 0);
    if(b == (void*)-1) die("Errore SHmat\n");
    
    /* TBD: Inizializzare l'oggetto MonitorBufferSingolo */
    init_monitor_buffer_singolo(b);

    /* TBD: Allocare una coda di messaggi */
    key_t chaive_coda = ftok(".", 'A');
    int id_coda =   msgget(chaive_coda, IPC_CREAT|0664); 
    if(id_coda < 0) die("Errore MSGGET\n");
     

	int pid;
    
    /* TBD: Creare un processo figlio, che esegua la funzione "Consumatore()" */
    pid = fork();
    if(pid < 0) die("Errorr fork\n");
    else if (pid == 0){
        printf("[MAIN] Eseguo Consumatore...\n");
        Consumatore(b, id_coda);
        exit(0);
    }
     

    /* TBD: Creare un processo figlio, che esegua la funzione "Produttore()" */
    pid = fork();
    if(pid < 0) die("Errorr fork\n");
    else if (pid == 0){
        printf("[MAIN] Eseguo Produttore...\n");
        Produttore(b, id_coda);
        exit(0);
    }

    /* TBD: Attendere la terminazione dei processi figli */
    while(wait(NULL) > 0);


    /* TBD: Deallocare l'oggetto monitor */
    remove_monitor_buffer_singolo(b);

    shmctl(shm_id, IPC_RMID, NULL);

    /* TBD: Deallocare la coda di messaggi */
    msgctl(id_coda, IPC_RMID, NULL);
}


void Produttore(MonitorBufferSingolo* b,int id_coda) {

    int numero_elementi = rand() % 10;

    printf("[%d][PRODUTTORE] Totale elementi da inviare: %d\n",getpid(),numero_elementi);

    /* TBD: Effettuare una produzione su MonitorBufferSingolo con
            la funzione "produzione_buffer_singolo", inserendo "numero_elementi"
     */
    produzione_buffer_singolo(b, numero_elementi);
    
    for(int i=0; i<numero_elementi; i++) {
    
        sleep(1);

        int val = rand() % 10;

        printf("[%d] [PRODUTTORE] Invio del valore: %d\n", getpid(),val);

		messaggio m;
		m.tipo = MESSAGGIO;
		m.valore = val;
        /* TBD: Inviare il valore "val" tramite la coda di messaggi */
        int ret = msgsnd(id_coda, (void*)&m, sizeof(messaggio)-sizeof(long), 0);
        if(ret <0) die("Errorr msgsend\n");

    }
}

void Consumatore(MonitorBufferSingolo* b,int id_coda) {

    /* TBD: Effettuare una consumazione su MonitorBufferSingolo con
            la funzione "consumazione_buffer_singolo", prelevando "numero_elementi"
     */
    int numero_elementi = consumazione_buffer_singolo(b);
    
    for(int i=0; i<numero_elementi; i++) {

        /* TBD: Ricevere il valore "val" tramite la coda di messaggi */
        messaggio m;

        int ret = msgrcv(id_coda, (void*)&m, sizeof(messaggio)-sizeof(long), 0,0);

        int val =  m.valore /* TBD */;

        printf("[%d][CONSUMATORE] Ricezione del valore: %d\n", getpid(),val);
    }
}