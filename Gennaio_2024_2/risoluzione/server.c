#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "header_msg.h"
#include "header_sem.h"

void server(lettscritt * p, int id_sem) {
    for(int i=0; i<10; i++) {
        sleep(1);
        int valore = leggi(id_sem, p);
        printf("[SERVER WORKER %d] Letto valore %d\n", getpid(), valore);
    }
}

int main() {
    /* Ottenere gli identificativi delle code di messaggi */
    key_t chiave_richieste = ftok(".", 'A');
    int coda_richieste = msgget(chiave_richieste, 0);
    if(coda_richieste < 0){
        perror("Errore msgget\n");
        exit(1);
    }

    key_t chiave_risposte = ftok(".", 'B');
    int coda_risposte = msgget(chiave_risposte, 0);
    if(coda_risposte < 0){
        perror("Errore msgget\n");
        exit(1);
    }

    int sem[NUM_BUFFER];
    int shm[NUM_BUFFER];
    lettscritt * ls[NUM_BUFFER];

    for(int i=0; i<NUM_BUFFER; i++) {
        shm[i] = shmget(IPC_PRIVATE, sizeof(lettscritt), IPC_CREAT|0664);
        ls[i] = (lettscritt*) shmat(shm[i], NULL, 0);
        ls[i]->buffer = 0;
        ls[i]->num_lettori = 0;

        printf("[SERVER FRONTEND %d] Creato buffer %d (p=%p, id=%d)\n", getpid(), i, ls[i], shm[i]);

        sem[i] = semget(IPC_PRIVATE, 2, IPC_CREAT|0664);
        printf("[SERVER FRONTEND %d] Creato vettore semafori %d (id=%d)\n", getpid(), i, sem[i]);

        semctl(sem[i], SYNCH, SETVAL, 1);
        semctl(sem[i], MUTEXL, SETVAL, 1);
    }

    for(int i=0; i<NUM_SERVER; i++) {
        pid_t pid = fork();
        if(pid < 0){
            perror("Errore fork\n");
            exit(1);
        }else if (pid == 0) {
            printf("[SERVER] Avvio Server Worker...\n");
            server(ls[i % NUM_BUFFER], sem[i % NUM_BUFFER]);
            exit(0);
        }
    }

    // CORREZIONE 1: Cicliamo per NUM_RICHIESTE * 2 (4 richieste * 2 client = 8)
    for(int i=0; i<NUM_RICHIESTE * 2; i++) {

        /* Effettuare la ricezione del messaggio dal client */
        messaggio_richiesta m;
        int ret = msgrcv(coda_richieste, (void*)&m, sizeof(messaggio_richiesta)-sizeof(long), MESSAGGIO, 0);
        if(ret < 0){
            perror("Errore msgrcv server \n");
            exit(1);
        }

        int buffer = m.id_buffer;
        int value = m.valore;

        printf("[SERVER FRONTEND %d] Ricevuto richiesta (valore=%d, buffer=%d) da Client %d\n", getpid(), value, buffer, m.pid_client);

        /* Scrittura sincronizzata nel buffer */
        scrivi(sem[buffer], ls[buffer], value);

        // CORREZIONE 2: Sblocchiamo il client specifico inviando l'ACK di ritorno sulla coda risposte!
        messaggio_risposta risp;
        risp.tipo = m.pid_client; // Usiamo il PID del client come canale di comunicazione mirato
        risp.valore = 1;          // Valore di conferma (ACK)

        ret = msgsnd(coda_risposte, (void*)&risp, sizeof(messaggio_risposta)-sizeof(long), 0);
        if(ret < 0){
            perror("Errore msgsnd server risposta\n");
            exit(1);
        }
    }

    /* Attendere la terminazione dei processi figli e deallocare le risorse */
    printf("[SERVER FRONTEND %d] Ricezione completata. Attesa terminazione worker...\n", getpid());
    
    for(int i=0; i<NUM_SERVER; i++) {
        wait(NULL);
    }

    for(int i=0; i<NUM_BUFFER; i++) {
        shmdt(ls[i]);
        shmctl(shm[i], IPC_RMID, NULL);
        semctl(sem[i], 0, IPC_RMID);
    }

    printf("[SERVER FRONTEND %d] Risorse deallocate. Fine processo.\n", getpid());
    return 0;
}