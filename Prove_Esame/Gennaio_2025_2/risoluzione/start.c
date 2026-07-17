#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#include "header.h"

int main() {

    /* TBD: Creare le code di messaggi */
    key_t chiave_richiesta = ftok(".", 'A');
    int coda_richieste = msgget(chiave_richiesta, IPC_CREAT|0664);
    if(coda_richieste < 0) die("Errore MSGGET coda richieste\n");

    key_t chiave_risposte = ftok(".", 'B');
    int coda_risposte = msgget(chiave_risposte, IPC_CREAT|0664);
    if(coda_risposte < 0) die("Errore MSGET coda risposte\n");

    key_t chiave_richieste_serv_db = ftok(".", 'C');
    int coda_richieste_serv_db = msgget(chiave_richieste_serv_db, IPC_CREAT|0664);
    if(coda_richieste_serv_db < 0) die("Errore MSGET coda richiste server db\n");

    key_t chiave_risposte_serv_db = ftok(".", 'D');
    int coda_risposte_serv_db = msgget(chiave_risposte_serv_db, IPC_CREAT|0664);
    if(coda_risposte_serv_db < 0) die("Errore MSGET coda risposte\n");



    for(int i=0; i<PROCESSI_CLIENT; i++) {

        pid_t pid = fork();
        if (pid < 0) die("Errore Fork\n");
        else if ( pid == 0){

            printf("[START] Creazione processo Client [%d]\n", i+1);
            execl("./client", "client", NULL);
            die("Execl");
        }
         
        
        /* TBD: Creare i processi client */

    }


    for(int i=0; i<PROCESSI_SERVER; i++) {
        
        /* TBD: Creare i processi server */
        pid_t pid = fork();
        if (pid < 0) die("Errore Fork\n");
        else if ( pid == 0){

            printf("[START] Creazione processo Server [%d]\n", i+1);
            execl("./server", "server", NULL);
            die("Execl");
        }
         
    }


    /* TBD: Creare il processo database */
    pid_t pid = fork();
    if (pid < 0) die("Errore Fork\n");
    else if ( pid == 0){

            printf("[START] Creazione processo Database\n");
            execl("./database", "database", NULL);
            die("Execl");
    }

   
    /* TBD: Attendere e gestire la terminazione */
    while(wait(NULL) > 0);

    msgctl(coda_richieste, IPC_RMID, NULL);
    msgctl(coda_risposte, IPC_RMID, NULL);
    msgctl(coda_richieste_serv_db, IPC_RMID, NULL); 
    msgctl(coda_risposte_serv_db, IPC_RMID, NULL);  

    return 0;

}