#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header_sem.h"

int main() {

	
    /* TBD: Creare una coppia di code di messaggi UNIX */
    key_t key_send = ftok(".", 'A');
    key_t key_rcv = ftok(".", 'B');
    
    
    int send_queue=msgget(key_send, IPC_CREAT| 0664);
    int rcv_queue=msgget(key_rcv, IPC_CREAT | 0664);
    
    
    if(send_queue<0){
    	perror("Errore nella creazione della coda sender\n");
    	exit(1);
    }
	
	if(rcv_queue<0){
    	perror("Errore nella creazione della coda receiver\n");
    	exit(1);
    }
	

    /* TBD: Creare un processo figlio, che esegua l'eseguibile "server" */
    pid_t pid = fork();
    if(pid < 0) die("Errore Fork \n");
    else if(pid == 0){
        printf("[START] Avvio processo server\n");
        execl("./server", "server", NULL);
        die("Errore execl\n");
    }
    


    /* TBD: Creare 2 processi figli, che eseguano l'eseguibile "client" */
    pid_t pid_c;
    for(int i = 0; i < 2; i++){
        pid_c = fork();

        if(pid_c < 0) die("Errore Fork \n");
        else if(pid_c == 0){
        printf("[START] Avvio processo client\n");
        execl("./client", "client", NULL);
        die("Errore execl\n");
        }

    }
	
	
	/* TBD: Attendere la terminazione dei figli, deallocare le code di messaggi */
    while(wait(NULL)> 0);

    msgctl(send_queue, IPC_RMID, NULL);
    msgctl(rcv_queue, IPC_RMID, NULL);

    printf("[MAIN] Fine Programma\n");
    
    return 0;
}