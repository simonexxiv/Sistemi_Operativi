#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <pthread.h>
#include "header.h"

#define NUM_WORKER 3

static struct Coda * coda;
static int ds_sem;
static pthread_t threads[NUM_WORKER];
static int end = 0;

// struttura per passaggio parametri ai thread
typedef struct {
	int id;
	Count* count;
} parametri;

void init_buffer(Count* c){
	//TODO aggiungere codice per inizializzare Count
    pthread_mutex_init(&c->mutex, NULL);
    c->num_prenotazioni = 0;
}

void remove_buffer(Count* c){
	//TODO aggiungere codice per rimuovere Count
    pthread_mutex_destroy(&c->mutex);
}


void *worker(void* arg){
    printf("[WORKER] - start del thread...\n");
    parametri* par = (parametri*) arg;

    Count * c = par->count;
    int id = par->id;
    int res;  
    
    while (1) { // Ciclo continuo, esce esplicitamente al -1
        res = consuma(coda, ds_sem);

        if(res == -1){
            printf("WORKER [%d] Ricevuta richiesta di fine.\n", id);
            break; // Questo worker termina correttamente
        }

        pthread_mutex_lock(&c->mutex);
        c->num_prenotazioni = c->num_prenotazioni + res;
        printf("WORKER [%d] Aggiorno il conteggio %d.\n", id, c->num_prenotazioni);
        pthread_mutex_unlock(&c->mutex);
    }

    pthread_exit(NULL);
}



int main(){

	int i,k;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

    //ottenimento e attach shared memory
    int key_shm;
    key_shm = ftok(".", 'A');
    if(key_shm < 0) die("Errore FTOK SHM\n");

    int ds_shm = shmget(key_shm, sizeof(struct Coda), 0);
    if(ds_shm < 0) die("Errore SHMGET\n");

    coda = shmat(ds_shm, NULL, 0);
    if (coda == (void*)-1) die("Errore SHMAT");
	
	//ottenimento array semafori
    int key_sem;
    key_sem = ftok(".", 'B');
    if (key_sem < 0) die("Errore Ftok SEM\n");

    ds_sem = semget(key_sem, 3, 0);
    if(ds_sem < 0) die("Errore SEM\n");


	//TODO Creazione e Inizializzazione struttura locale
	Count* count = (Count*) malloc(sizeof(Count))/*TODO*/;

    init_buffer(count);
	
	parametri param[NUM_WORKER];

	//Avvio dei thread
	for(i = 0; i < NUM_WORKER;i++){
		printf("[SERVER] - creo thread %d...\n",i);
		//TODO creazione threads	
        param[i].id = i;
        param[i].count = count;
        pthread_create(&threads[i], &attr, worker, (void*)&param[i]);
	}
	
	for(k=0; k < NUM_WORKER; k++) {
	  //TODO join 
      pthread_join(threads[k], NULL);
	}
	
	//TODO rimozione struttura locale
    remove_buffer(count);
    free(count);

	printf("[SERVER] - bye bye...\n");
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);
}
