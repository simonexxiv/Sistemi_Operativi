#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

#include "prodcons.h"


void produttore_1(MonitorPC * m)
{
	srand(getpid());

	for(int i=0; i<4; i++) {

		int valore = rand() % 10;

		/* TBD: chiamare il metodo di produzione tipo 1 */
        produci_tipo_1(m, valore);

		sleep(1);
	}

}


void produttore_2(MonitorPC * m)
{
    srand(getpid());

	for(int i=0; i<4; i++) {

		int valore = rand() % 10;

		/* TBD: chiamare il metodo di produzione tipo 2 */
        produci_tipo_2(m, valore);

		sleep(1);
	}

}


void consumatore_1(MonitorPC * m)
{

	for(int i=0; i<8; i++) {

		int valore;

		/* TBD: chiamare il metodo di consumazione tipo 1 */
        consuma_tipo_1(m, &valore);

		sleep(1);
	}

}


void consumatore_2(MonitorPC * m)
{
	for(int i=0; i<8; i++) {

		int valore;

		/* TBD: chiamare il metodo di consumazione tipo 2 */
        consuma_tipo_2(m, &valore);

		sleep(1);
	}
}

int main(int argc, char *argv[])
{

	/* TBD: creare e inizializzare l'oggetto monitor (metodo "inizializza") */
    int shm_id = shmget(IPC_PRIVATE, sizeof(MonitorPC), IPC_CREAT|0664);
    if (shm_id < 0 ) die("errore shmget\n");

    MonitorPC * m = (MonitorPC * ) shmat(shm_id, NULL, 0);
    if(m == (void*)-1) die("Errore shmat\n");

    inizializza(m);

    pid_t pid;
    int i = 0;
    
	for(i=0; i<2; i++) {

		/* TBD: avviare 2 produttori di tipo 1 */
        pid = fork();
        if(pid < 0) die("Errore fork\n");
        else if(pid == 0){
            printf("[MAIN] Avvio produttore tipo 1, numero %d\n", i+1);
            produttore_1(m);
            exit(0);
        }
	}

	for(i=0; i<2; i++) {

		/* TBD: avviare 2 produttori di tipo 2 */
        pid = fork();
        if(pid < 0) die("Errore fork\n");
        else if(pid == 0){
            printf("[MAIN] Avvio produttore tipo 2, numero %d\n", i+1);
            produttore_2(m);
            exit(0);
        }
	}

	/* TBD: avviare consumatore di tipo 1 */
    pid = fork();
    if(pid < 0) die("Errore fork\n");
    else if(pid == 0){
        printf("[MAIN] Avvio consumatore tipo 1\n");
        consumatore_1(m);
        exit(0);
    }

	/* TBD: avviare consumatore di tipo 2 */
    pid = fork();
    if(pid < 0) die("Errore fork\n");
    else if(pid == 0){
        printf("[MAIN] Avvio consumatore tipo 2\n");
        consumatore_2(m);
        exit(0);
    }

    while(wait(NULL)> 0);

	/* TBD: rimuovere l'oggetto monitor */
    rimuovi(m);

    shmctl(shm_id, IPC_RMID, NULL);

    return 0;

}
