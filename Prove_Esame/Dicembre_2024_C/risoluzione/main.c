#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include "header.h"

int main() {

    /*
        TODO: creazione code di messaggi
    */
    key_t chiave_richieste = ftok(".", 'A');
    int coda_richieste = msgget(chiave_richieste, IPC_CREAT|0664);

    key_t chiave_risposte = ftok(".", 'B');
    int coda_risposte = msgget(chiave_risposte, IPC_CREAT|0664);

    if (coda_richieste == -1 || coda_risposte == -1) {
        perror("Errore nella creazione delle code di messaggi");
        exit(1);
    }

    /*
        TODO: avvio client e server in eseguibili distinti
    */
    pid_t pid = fork();
    if (pid < 0) die("Errore fork\n");
    else if(pid == 0){
        printf("[MAIN] Avvio client...\n");
        execl("./client", "client", NULL);
        die("Execl");
    }

    pid = fork();
    if (pid < 0) die("Errore fork\n");
    else if(pid == 0){
        printf("[MAIN] Avvio server...\n");
        execl("./server", "server", NULL);
        die("Execl");
    }


    /*
        TODO: attendo terminazione figli
    */
    while(wait(NULL)> 0);

    msgctl(coda_richieste, IPC_RMID, NULL);
    msgctl(coda_risposte, IPC_RMID, NULL);

    return 0;
}