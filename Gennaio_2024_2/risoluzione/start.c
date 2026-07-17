#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>


int main() {


    /* TBD: Predisporre le code di messaggi UNIX per la send sincrona */
    key_t chiave_richieste = ftok(".", 'A');
    int coda_richieste = msgget(chiave_richieste, IPC_CREAT|0664);
    if(coda_richieste < 0){
        perror("Errore msgget\n");
        exit(1);
    }

    key_t chiave_risposte = ftok(".", 'B');
    int coda_risposte = msgget(chiave_risposte, IPC_CREAT|0664);
    if(coda_risposte < 0){
        perror("Errore msgget\n");
        exit(1);
    }

    /* TBD: Creare un processo figlio, che esegua l'eseguibile "server" */
    pid_t pid = fork();
    if (pid < 0){
        perror("Errore fork\n");
        exit(1);
    }else if (pid == 0) {
        printf("[MAIN] Avvio server\n");
        execl("./server", "server", NULL);
        perror("Errore execl\n");
        exit(1);
    }

    sleep(1);


    /* TBD: Creare un processo figlio, che esegua l'eseguibile "client" */
    for(int i = 0; i < 2; i++){
    pid = fork();
    if (pid < 0){
        perror("Errore fork\n");
        exit(1);
    }else if (pid == 0) {
        printf("[MAIN] Avvio client\n");
        execl("./client", "client", NULL);
        perror("Errore execl\n");
        exit(1);
    }
    }


    /* TBD: Attendere la terminazione dei figli, deallocare le code di messaggi */
    while(wait(NULL)> 0);

    msgctl(coda_richieste, IPC_RMID, NULL);
    msgctl(coda_risposte, IPC_RMID, NULL);

    printf("[MAIN] Fine Programma...\n");

    return 0;
}