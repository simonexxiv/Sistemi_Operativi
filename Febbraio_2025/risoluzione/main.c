#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/msg.h>

#include "header_msg.h"

int main() {

    key_t chiave_collettore = ftok(".", 'C');/*TODO*/;
    int id_queue_collettore = msgget(chiave_collettore, IPC_CREAT|0664)/*TODO*/;

    key_t chiave_server = ftok(".", 'S');/*TODO*/;
    int id_queue_server = msgget(chiave_server, IPC_CREAT|0664)/*TODO*/;

    //TODO: creazione ed eseuzione processo server
    pid_t pid = fork();
    if( pid < 0) die("Errore Fork Server\n");
    else if ( pid == 0){

        printf("[MAIN] Avvio Processo Sensore...\n");
        execl("./server", "server", NULL);
        die("execl server\n");
    }
     

    //TODO: creazione processo collettore
    pid = fork();
    if( pid < 0) die("Errore Fork Collettorre\n");
    else if ( pid == 0){

        printf("[MAIN] Avvio Processo Collettore...\n");
        collettore(id_queue_collettore, id_queue_server);
        exit(0);

    }

    //TODO: creazione processi sensori, ciascuno con il proprio ID
    for(int i = 0; i < NUM_SENSORI; i++){
        pid = fork();
        if( pid < 0) die("Errore Fork Sensore\n");
        else if ( pid == 0){

            printf("[MAIN] Avvio Processo Sensore %d...\n", i+1);
            sensore(i+1, id_queue_collettore);
            exit(0);
        }
    }
    

    //TODO: attesa dei processi figli
    while(wait(NULL) > 0);
    printf("[MAIN] Processi figli terminati...\n");
    

    //TODO: rimozione risorse IPC
    msgctl(id_queue_collettore, IPC_RMID, NULL);
    msgctl(id_queue_server, IPC_RMID, NULL);
    
    return 0;
}