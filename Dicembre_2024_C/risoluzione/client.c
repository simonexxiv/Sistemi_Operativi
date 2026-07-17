#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "header.h"

void bambino(int queue_req, int queue_res) {
	int i;
	for (i=0; i < NUM_LETTERE; i++) {
		/*
        	TODO: invio del messaggio
    	*/
        MessaggioRichiesta msg;
    	
		/* TODO */msg.numero_regali = rand() % 5 + 1;
        msg.tipo = MESSAGGIO;

        int ret = msgsnd(queue_req, (void*)&msg, sizeof(MessaggioRichiesta)-sizeof(long), 0);
        if(ret < 0) die("Errore MSGSEND\n");

        printf("Invio lettera a Babbo Natale chiedendo %d regali\n", msg.numero_regali);

		/*
        	TODO: attesa del messaggio di risposta
    	*/
        MessaggioRisposta m;

        ret = msgrcv(queue_res, (void*)&m, sizeof(MessaggioRisposta)-sizeof(long), 0, 0);

		printf("Ho ricevuto la conferma di ricezione!\n");
		sleep(1);
	}
}

int main() {
	int pid;
	/*
        TODO: recupero code di messaggi
    */
    key_t chiave_richieste = ftok(".", 'A');
    int coda_richieste = msgget(chiave_richieste, 0);

    key_t chiave_risposte = ftok(".", 'B');
    int coda_risposte = msgget(chiave_risposte, 0);

	int i;
	for (int i = 0; i < NUM_BAMBINI; i++) {
		/*
        	TODO: completare creazione figli 
    	*/	
            pid = fork();
            if(pid < 0) die("Errore fork\n");
            else if (pid == 0){

                
                srand(time(NULL)*getpid());
			    printf("Bambino %d sta scrivendo la letterina...\n",getpid());
                bambino(coda_richieste, coda_risposte);
                exit(0);

            }   
	}

	/*
        TODO: attesa terminazione figli
    */
    while (wait(NULL)> 0);

	msgctl(coda_richieste,IPC_RMID,0);
	msgctl(coda_risposte,IPC_RMID,0);

	return 0;
}