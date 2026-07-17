#include "header.h"
#include <sys/msg.h>
#include <unistd.h>

void sensore_x(int coda_sensori){

    printf("[Sensore X] Avviato...\n");

    srand(getpid());

    for(int i = 0; i < 10; i++){
        
        int valore = rand()%10;

        printf("[Sensore X] Invio %d\n", valore);

        /*TBD: Inviare un messaggio con il valore sulla coda dei sensori, di tipo "X"*/
        messaggio_sensore m;
        m.value = valore;
        m.type = X;

        int ret = msgsnd(coda_sensori, (void*)&m, sizeof(messaggio_sensore)-sizeof(long), 0);
        if(ret <0 ) die("Errore msgsend\n");

        sleep(1);
    }
}

void sensore_y(int coda_sensori){

    printf("[Sensore Y] Avviato...\n");

    srand(getpid());

    for(int i = 0; i < 10; i++){
        
        int valore = rand()%10;

        printf("[Sensore Y] Invio %d\n", valore);

        /*TBD: Inviare un messaggio con il valore sulla coda dei sensori, di tipo "Y"*/
        messaggio_sensore m;
        m.value = valore;
        m.type = Y;

        int ret = msgsnd(coda_sensori, (void*)&m, sizeof(messaggio_sensore)-sizeof(long), 0);
        if(ret <0 ) die("Errore msgsend\n");

        sleep(1);
    }
}

void attuatore(int coda_attuatore) {
    printf("[Attuatore] Avviato...\n");

    for(int i = 0; i < 10; i++) {
        messaggio_attuatore m;
        
        // Riceve qualsiasi messaggio sulla coda dell'attuatore
        int ret = msgrcv(coda_attuatore, (void*)&m, sizeof(messaggio_attuatore)-sizeof(long), 0, 0);
        if(ret < 0) die("Errore msgrcv Attuatore\n");

        printf("[Attuatore] COMANDO RICEVUTO dal Planner: Esegui azione con potenza %d\n", m.value);
    }
    printf("[Attuatore] Ho completato i 10 comandi. Termino.\n");
}