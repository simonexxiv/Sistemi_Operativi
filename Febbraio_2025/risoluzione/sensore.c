#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>

#include "header_msg.h"

void sensore(int id_sensore, int id_queue_collettore) {

    srand(getpid());

    for(int i=0; i<NUM_MESSAGGI_PER_SENSORE; i++) {


        int valore = (rand() % 10) + 10;

        //TODO: invio del valore al collettore
        messaggio_sensore m;
        m.tipo = id_sensore;
        m.valore = valore;

        int ret = msgsnd(id_queue_collettore, (void*)&m, sizeof(messaggio_sensore)-sizeof(long), 0);
        if(ret < 0) die("Errore MSGSEND Sensore\n");
    

        printf("[SENSORE] Inviato valore=%d\n", valore);


        sleep(1);
    }
}