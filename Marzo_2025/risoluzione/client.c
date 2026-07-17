#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>


#include "header_msg.h"
#include "header_prodcons.h"

int main() {

    /* TBD: Ottenere gli identificativi delle code di messaggi */
    key_t key_mess = ftok(".", 'A');
    int coda_mess = msgget(key_mess, 0);
    if(coda_mess < 0) die("Errore MSGGET Messaggio\n");

    key_t key_req = ftok(".", 'B');
    int coda_req = msgget(key_req, 0);
    if(coda_req < 0) die("Errore MSGGET Request\n");

    key_t key_ok = ftok(".", 'C');
    int coda_ok = msgget(key_ok, 0);
    if(coda_ok < 0) die("Errore MSGGET Ok\n");


    srand(getpid());

    for(int i=0; i<3; i++) {

        struct richiesta r;
        struct rts rts;
        struct ots ots;

        /* TBD: Inviare il messaggio "request to send" */
        rts.tipo = MESSAGGIO;

        int ret = msgsnd(coda_req, (void*)&rts, sizeof(rts)-sizeof(long), 0);
        if(ret <0) die("MsgSEND Error RTS\n");

        printf("[CLIENT] Inviato messaggio RTS\n");

        
        /* TBD: Ricevere il messaggio "ok to send" */
        ret = msgrcv(coda_ok, (void*)&ots, sizeof(ots)-sizeof(long), 0, 0);
        if(ret <0) die("MSGRCV Error Client\n");

        printf("[CLIENT] Ricevuto messaggio OTS\n");

        int valore = rand() % 10;

        /* TBD: Inviare un messaggio con il valore */
        r.tipo = MESSAGGIO;
        r.valore = valore;

        ret = msgsnd(coda_mess, (void*)&r, sizeof(struct richiesta)-sizeof(long), 0);
        if (ret < 0) die("Errore MSGSEND RIchiesta CLient\n");

        printf("[CLIENT] Inviato messaggio MSG con valore %d\n", valore);

    }
}