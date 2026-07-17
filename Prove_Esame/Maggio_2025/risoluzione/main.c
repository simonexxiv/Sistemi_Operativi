#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "header.h"

#define NUM_CLIENT 3
#define NUM_SERVER 1

int main(){

	//ottenimento e attach shared memory
	key_t key_shm;
	key_shm = ftok(".", 'A') /*TODO*/;
    if( key_shm < 0) die("Errore FTOK SHM\n");

	int ds_shm = shmget(key_shm, sizeof(struct Coda), IPC_CREAT|0664) /*TODO*/;
    if( ds_shm < 0) die("Errore SHMGET\n");

	struct Coda * c;
	c = shmat(ds_shm, NULL, 0) /*TODO*/;
    if( c == (void*)-1) die("Errore SHMAT\n");
	
	//ottenimento array semafori
	key_t key_sem;
	key_sem = ftok(".", 'B') /*TODO*/;

	int ds_sem = semget(key_sem, 3, IPC_CREAT|0664) /*TODO*/;
	
	//TODO inizializzazione coda e semafori
    c->coda = 0;
    c->testa = 0;

    semctl(ds_sem, SPAZIO_DISPONIBILE, SETVAL, DIM_BUFFER);
    semctl(ds_sem, MESSAGGIO_DISPONIBILE, SETVAL, 0);
    semctl(ds_sem, MUTEX, SETVAL, 1);


	int i = 0;
	int pid;
	int status;
	
	for( i = 0; i < NUM_CLIENT+NUM_SERVER; i++){
		
		sleep(1);

		pid = fork();

		if(pid == 0){

			if(i == NUM_CLIENT+NUM_SERVER -1){
				//TODO aggiungere exec per avviare il server
                printf("[PADRE] - Avvio server\n");
                execl("./server", "server", NULL);
                die("execl");
			}else{
				//TODO aggiungere exec per avviare il client
                srand(getpid() * time(NULL));
                printf("[PADRE] - Avvio Client\n");
                execl("./client", "client", NULL);
				exit(-1);
			}
		}
	}

	for(i = 0; i < NUM_CLIENT+NUM_SERVER;i++){

		wait(&status);

		if (WIFEXITED(status)) {
    			printf("[PADRE] - Figlio terminato con stato %d\n",status);
  		}
	}

	//TODO rimozione array semafori e memoria condivisa
    semctl(ds_sem, 0, IPC_RMID);
    shmctl(ds_shm, IPC_RMID, NULL);

	printf("[PADRE] - Fine elaborazione...\n");

	return 0;	
}