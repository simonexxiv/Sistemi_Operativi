#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#include "header.h"

int main() {

    /* TBD: Ottenere i riferimenti alle code di messaggi */
    key_t chiave_richiesta = ftok(".", 'A');
    int coda_richieste = msgget(chiave_richiesta, 0);
    if(coda_richieste < 0) die("Errore MSGGET coda richieste\n");

    key_t chiave_risposte = ftok(".", 'B');
    int coda_risposte = msgget(chiave_risposte, 0);
    if(coda_risposte < 0) die("Errore MSGET coda risposte\n");

    srand(getpid());

    for(int i=0; i<NUMERO_RICHIESTE; i++) {
        
        int tipo_richiesta;
        int valore;
        int posizione = rand() % DIM_VETTORE;

        if((rand() % 2) == 0) {

            tipo_richiesta = LETTURA/* TBD */;
            valore = 0;
        }
        else {
            
            tipo_richiesta = SCRITTURA/* TBD */;
            valore = rand() % 10;
        }

        printf("[CLIENT %d] Invio richiesta (tipo=%d, valore=%d, posizione=%d)\n", getpid(), tipo_richiesta, valore, posizione);

        /* TBD: Inviare un messaggio di richiesta */
        messaggio_richiesta m;
        m.tipo = tipo_richiesta;
        m.valore = valore;
        m.pid_mittente = getpid();
        m.posizione = posizione;

        int ret = msgsnd(coda_richieste, (void*)&m, sizeof(messaggio_richiesta)-sizeof(long), 0);
        if(ret < 0) die("Errore MSGSEND client\n");

        
        /* TBD: Ricevere il messaggio di risposta */
        messaggio_risposta mess;
        
        ret = msgrcv(coda_risposte, (void*)&mess, sizeof(messaggio_risposta)-sizeof(long), getpid(), 0);

        int valore_risposta = mess.valore/* TBD */;

        printf("[CLIENT %d] Ricezione risposta (valore=%d)\n", getpid(), valore_risposta);
    }


}