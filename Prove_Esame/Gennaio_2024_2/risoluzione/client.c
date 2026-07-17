#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>

#include "header_msg.h"


int main() {

    srand(getpid());


    /* TBD: Ottenere gli identificativi delle code di messaggi */
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

    for(int i=0; i<NUM_RICHIESTE; i++) {

        int buffer = rand() % NUM_BUFFER;

        int value = rand() % 10;


        /* TBD: Inviare un messaggio contenente "value" e "buffer".

                È richiesto che l'invio sia effettuato
                tramite send sincrona.
         */
        
        messaggio_richiesta m;
        m.id_buffer = buffer;
        m.pid_client = getpid();
        m.tipo = MESSAGGIO;
        m.valore = value;

        int ret = msgsnd(coda_richieste, (void*)&m, sizeof(messaggio_richiesta)-sizeof(long), 0);
        if(ret < 0){
            perror("Errore msgsend client\n");
            exit(1);
        }


        messaggio_risposta msg;
        ret = msgrcv(coda_risposte, (void*)&msg, sizeof(messaggio_risposta)-sizeof(long), getpid(), 0);
        if(ret < 0){
            perror("Errore msgrcv client\n");
            exit(1);
        }

        printf("[CLIENT %d] Ricevuto ACK. Sbloccato!\n", getpid());

        sleep(2);

    }


}