#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include "header.h"


int main(){
	//ottenimento e attach shared memory
	key_t key_shm;
	key_shm = ftok(".", 'A')/*TODO*/;

	int ds_shm = shmget(key_shm, sizeof(struct Coda), 0); /*TODO*/
    if(ds_shm < 0) die("Errore shget\n");

	struct Coda * c;
	c = (struct Coda *) shmat(ds_shm, NULL, 0);/*TODO*/
    if(c == (void*)-1) die("Errore Shmat\n");
	
	//ottenimento array semafori

	key_t key_sem;
	key_sem = ftok(".", 'B')/*TODO*/;

	int ds_sem = semget(key_sem, 4, 0); /*TODO*/;
    if (ds_sem < 0) die("Errore semget\n");
    
	printf("[Client %d] - invio richieste...\n",getpid());

	int i = 0;
	int num_prenotazioni;	


	while(i < NUM_REQ){
		num_prenotazioni = rand() % 5 +1;//TODO numero casuale
		//TODO invio richiesta tramite funzione "produci"

        produci(c, ds_sem, num_prenotazioni);
	
		sleep(1);
		i++;
	}
	//TODO invio richiesta di terminazione (-1)
    produci(c, ds_sem, -1);

	printf("[Client %d] - Fine processo\n",getpid());

	return 0;
}