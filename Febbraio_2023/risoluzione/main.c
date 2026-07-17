#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>

#include "header_msg.h"

int main() {

    /* TBD: Creare le code di messaggi */
    key_t chiave_collettore_1 = ftok(".", 'A');
    key_t chiave_collettore_2 = ftok(".", 'B');
    key_t chiave_server = ftok(".", 'S');

    int id_queue_collettore_1 = msgget(chiave_collettore_1, IPC_CREAT|0664);/* TBD */
    if(id_queue_collettore_1 < 0) die("Errore 1\n");


    int id_queue_collettore_2 = msgget(chiave_collettore_2, IPC_CREAT|0664)/* TBD */;
    if(id_queue_collettore_2 < 0) die("Errore 2\n");

    int id_queue_server = msgget(chiave_server, IPC_CREAT|0664) /* TBD */;
    if(id_queue_server < 0) die("Errore 3\n");

    /* TBD: Creare il processo figlio Server, 
            passandogli lo id della coda */

    pid_t pid = fork();
    if( pid < 0) die("Errore fork\n");
    else if(pid == 0){
        printf("[MAIN] Avvio processo server\n");
        server(id_queue_server);
        exit(0);
    }

    int id_collettore = 1;

    /* TBD: Creare il primo processo figlio Collettore, 
            passandogli lo "id_collettore",
            lo id della coda per ricevere dai sensori,
            e lo id della coda per inviare al server
    */
    pid = fork();
    if( pid < 0) die("Errore fork\n");
    else if(pid == 0){
        printf("[MAIN] Avvio processo Collettore 1\n");
        collettore(id_collettore, id_queue_collettore_1, id_queue_server);
        exit(0);
    }

    id_collettore = 2;
    
    /* TBD: Creare il secondo processo figlio Collettore, 
            passandogli lo "id_collettore",
            lo id della coda per ricevere dai sensori,
            e lo id della coda per inviare al server
    */
    pid = fork();
    if( pid < 0) die("Errore fork\n");
    else if(pid == 0){
        printf("[MAIN] Avvio processo Collettore 2\n");
        collettore(id_collettore, id_queue_collettore_2, id_queue_server);
        exit(0);
    }


    for(int i=0; i<NUM_SENSORI_PER_COLLETTORE; i++) {

        int id_sensore = i+1;

        /* TBD: Creare i processi figli Sensore, 
                passandogli lo "id_sensore",
                e lo id della coda per inviare al primo collettore
        */
        pid = fork();
        if( pid < 0) die("Errore fork\n");
        else if(pid == 0){
        printf("[MAIN] Avvio processo Sensore %d\n", i+1);
        sensore(id_sensore, id_queue_collettore_1);
        exit(0);
        }

    }


    for(int i=0; i<NUM_SENSORI_PER_COLLETTORE; i++) {

        int id_sensore = i+1;

        /* TBD: Creare i processi figli Sensore, 
                passandogli lo "id_sensore",
                e lo id della coda per inviare al secondo collettore
        */
        pid = fork();
        if( pid < 0) die("Errore fork\n");
        else if(pid == 0){
        printf("[MAIN] Avvio processo Sensore %d\n", i+1);
        sensore(id_sensore, id_queue_collettore_2);
        exit(0);
        }
    }



    /* TBD: Attendere la terminazione di tutti i processi figli */
    while(wait(NULL)> 0);


    /* TBD: De-allocare le code di messaggi */
    msgctl(id_queue_collettore_1, IPC_RMID, NULL);
    msgctl(id_queue_collettore_2, IPC_RMID, NULL);
    msgctl(id_queue_server, IPC_RMID, NULL);
    
    return 0;
}