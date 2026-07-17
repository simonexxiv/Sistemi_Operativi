#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include "header.h"

int main() {
    /*
        TODO: recupero code
    */
    key_t chiave_richieste = ftok(".", 'A');
    int coda_richieste = msgget(chiave_richieste, 0);

    key_t chiave_risposte = ftok(".", 'B');
    int coda_risposte = msgget(chiave_risposte, 0);

    if (coda_richieste == -1 || coda_risposte == -1) {
        perror("Errore nell'apertura delle code");
        exit(1);
    }

    for (int i = 0; i < NUM_BAMBINI; i++) {
        /* TODO */
        pid_t pid = fork();
        if(pid == 0){
            srand(time(NULL)*getpid());
            for (int j = 0; j < NUM_LETTERE; j++) {
                /*
                    TODO: costruzione e invio del messaggio
                */
                MessaggioRichiesta msg;
                msg.tipo = MESSAGGIO;

                /* TODO */msg.numero_regali = (rand() % 5) + 1;

                printf("Invio lettera a Babbo Natale chiedendo %d regali\n", msg.numero_regali);

                int ret = msgsnd(coda_richieste, (void*)&msg, sizeof(MessaggioRichiesta)-sizeof(long), 0);
                if (ret < 0) {
                    perror("Errore msgsend\n");
                    exit(1);
                }
                /*
                    TODO: attesa del messaggio di risposta
                */
                MessaggioRisposta m;

                ret = msgrcv(coda_risposte, (void*)&m, sizeof(MessaggioRisposta)-sizeof(long),0, 0);
                 if (ret < 0) {
                    perror("Errore msgrcv\n");
                    exit(1);
                }

                printf("[Bambino %d] Lettera %d: conferma ricevuta\n", i + 1, j + 1);
            }
            exit(0);
        
        }
    }

    /*
        TODO: Attesa della terminazione dei processi figli
    */
    while(wait(NULL)> 0);


    return 0;
}