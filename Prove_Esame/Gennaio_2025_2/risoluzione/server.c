#include <stdio.h>
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

    key_t chiave_richieste_serv_db = ftok(".", 'C');
    int coda_richieste_serv_db = msgget(chiave_richieste_serv_db, 0);
    if(coda_richieste_serv_db < 0) die("Errore MSGET coda richiste server db\n");

    key_t chiave_risposte_serv_db = ftok(".", 'D');
    int coda_risposte_serv_db = msgget(chiave_risposte_serv_db, 0);
    if(coda_risposte_serv_db < 0) die("Errore MSGET coda risposte\n");


    for(int i=0; i<NUMERO_RICHIESTE; i++) {

        /* TBD: Ricevere un messaggio di richiesta dal client */
        messaggio_richiesta m;
        
        int ret = msgrcv(coda_richieste, (void*)&m, sizeof(messaggio_richiesta)-sizeof(long), 0, 0);
        if (ret < 0) die("[SERVER] Errore MSGrcv clietne server\n");

        int tipo_richiesta = m.tipo/* TBD */;
        int valore = m.valore/* TBD */;
        int posizione = m.posizione/* TBD */;
        int pid_client = m.pid_mittente/* TBD */;

        printf("[SERVER %d] Inoltro richiesta (tipo=%d, pid=%d, posizione=%d, valore=%d)\n", getpid(), tipo_richiesta, pid_client, posizione, valore);

        sleep(1);


        /* TBD: Inviare un messaggio di richiesta al database, contenente lo stesso tipo di richiesta (LETTURA/SCRITTURA), posizione, e valore */
        m.pid_mittente = getpid();

        ret = msgsnd(coda_richieste_serv_db, (void*)&m, sizeof(messaggio_richiesta)-sizeof(long), 0);
        if(ret < 0) die("[SERVER] Errrore msgsend serv db\n");


        /* TBD: Ricevere un messaggio di risposta dal database */
        messaggio_risposta mess;

        ret = msgrcv(coda_risposte_serv_db, (void*)&mess, sizeof(messaggio_risposta)-sizeof(long), getpid(), 0);
        if( ret < 0) die("Errore msgrcv db severv\n");

        int valore_risposta = mess.valore/* TBD */;

        printf("[SERVER %d] Inoltro risposta (pid=%d, valore=%d)\n", getpid(), pid_client, valore_risposta);



        /* TBD: Inviare il messaggio di risposta al client, contenente lo stesso valore risposta */
        mess.tipo = pid_client;

        ret = msgsnd(coda_risposte, (void*)&mess, sizeof(messaggio_risposta)-sizeof(long), 0);
        if(ret < 0) die("Errore msgsend server client\n");



    }

}