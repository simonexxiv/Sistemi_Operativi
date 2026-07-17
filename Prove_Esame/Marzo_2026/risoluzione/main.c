#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header.h"

int main(){

    /*TBD: Allocare le due code di messaggi */
    key_t chiave_coda_sensore = ftok(".", 'A');
    int coda_sensore = msgget(chiave_coda_sensore, IPC_CREAT|0664);
    if (coda_sensore < 0) die("errore msgget\n");

    key_t chiave_coda_attuatore = ftok(".", 'B');
    int coda_attuatore = msgget(chiave_coda_attuatore, IPC_CREAT|0664);
    if(coda_attuatore < 0) die("errore msgget attu\n");

    pid_t pid = fork();
    if( pid < 0) die("Errore fork\n");
    else if ( pid == 0){
        printf("[MAIN] Avvio processo sensore_x\n");
        sensore_x(coda_sensore);
        exit(0);
    }

    pid = fork();
    if( pid < 0) die("Errore fork\n");
    else if ( pid == 0){
        printf("[MAIN] Avvio processo sensore_y\n");
        sensore_y(coda_sensore);
        exit(0);
    }

    pid = fork();
    if( pid < 0) die("Errore fork\n");
    else if ( pid == 0){
        printf("[MAIN] Avvio processo planner\n");
        planner(coda_sensore, coda_attuatore );
        exit(0);
    }

    pid = fork();
    if( pid < 0) die("Errore fork\n");
    else if ( pid == 0){
        printf("[MAIN] Avvio processo attuatore\n");
        attuatore(coda_attuatore);
        exit(0);
    }

    /*TBD: Avviare 4 processi figli, facendogli eseguire le funzioni:
            - sensore_x()
            - sensore_y()
            - planner()
            - attuatore()
    */

    /*TBD: Attendere la terminazione dei processi figli e deallocare le risorse*/
    while(wait(NULL) > 0);

    msgctl(coda_attuatore, IPC_RMID, NULL);
    msgctl(coda_sensore, IPC_RMID, NULL);

    return 0;
}