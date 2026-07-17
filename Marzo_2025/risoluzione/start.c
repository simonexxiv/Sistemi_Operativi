#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>


#include "header_prodcons.h"

int main() {

    /* TBD: Creare le code di messaggi per la send sincrona*/
    key_t key_mess = ftok(".", 'A');
    int coda_mess = msgget(key_mess, IPC_CREAT|0664);
    if(coda_mess < 0) die("Errore MSGGET Messaggio\n");

    key_t key_req = ftok(".", 'B');
    int coda_req = msgget(key_req, IPC_CREAT|0664);
    if(coda_req < 0) die("Errore MSGGET Request\n");

    key_t key_ok = ftok(".", 'C');
    int coda_ok = msgget(key_ok, IPC_CREAT|0664);
    if(coda_ok < 0) die("Errore MSGGET Ok\n");

    /* TBD: Creare un processo figlio, ed eseguire il programma "server" */
    pid_t pid = fork();
    if(pid < 0) die("Errore Fork Server\n");
    else if(pid == 0){

        execl("./server", "server", NULL);
        die("execl server\n");
    }

    printf("[START] Processo Server avviato...\n");

    /* TBD: Creare un processo figlio, ed eseguire il programma "client" */
    pid = fork();
    if(pid < 0) die("Errore Frok Client\n");
    else if (pid == 0){

        execl("./client", "client", NULL);
        die("ececl client\n");
    }

    printf("[START] Processo Client avviato...\n");
    

    /* TBD: Attendere la terminazione dei processi figli */
    while(wait(NULL) > 0);


    /* TBD: Eliminare le code di messaggi */
    msgctl(coda_mess, IPC_RMID, NULL);
    msgctl(coda_req, IPC_RMID, NULL);
    msgctl(coda_ok, IPC_RMID, NULL);

    return 0;

}