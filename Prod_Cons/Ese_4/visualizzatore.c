#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "procedure.h"  
#include "semafori.h"

static void die(const char * msg){
    perror(msg);
    exit(1);
}

int main(){

    // TODO: Importare la memoria e collegarla
    key_t chiave = ftok(".", 'A');
    int shm_id = shmget(chiave, sizeof(struct shm_data ), 0664);
    if(shm_id < 0) die("SHM Error Visualizzatore");

    struct shm_data* p = (struct shm_data *) shmat(shm_id, NULL, 0);
    if (p == (void*)-1) die("SHMAT Error Visualizzatore");


    // TODO: Connettersi ai semafori
    key_t chiave_sem = ftok(".", 'B');
    int sem_id = semget(chiave_sem, 1, 0);
    if (sem_id < 0) die("SEMGET Error Visualizzatore");


    while(1){

        sleep(1);

        Wait_Sem(sem_id, MUTEX);

        // TODO: Stampare a video la situazione aggiornata dei posti:
        // per ogni posto: numero, stato (LIBERO/OCCUPATO/IN AGGIORNAMENTO), 
        // id_cliente se occupato

        for (int i=0;i<DIM_POSTI;i++){
            printf("Posto N %d :", i);
            if (p->teatro[i].stato==0){
               printf("LIBERO\n");
            } else if (p->teatro[i].stato == 1){
                printf("OCCUPATO da %d\n", p->teatro[i].id_cliente);
            } else{
                printf("IN AGGIORNAMENTO\n");
            }
        }

        printf("\n\n");

        // TODO: Verificare se disponibilita == 0
        // Se sì, uscire dal ciclo
        if (p->disponibilita==0) {
            Signal_Sem(sem_id, MUTEX);
            break;
        }

        Signal_Sem(sem_id, MUTEX);

    }

    return 0;

}