#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <time.h>

#include "procedure.h"
#include "semafori.h"

void cliente (posto* teatro, int * disponibilita, int sem_id){

    //Preparo la casualità
    sleep(rand()%6); //__esigenza di traccia__

    int n = rand() % 4 + 1;
    int array [4];

    //Wait prima della sezione critica, controllo disponibilità
    Wait_Sem(sem_id, MUTEX);

    if(*disponibilita < n){
        printf("Disponibiltà esaurita...\n");
        Signal_Sem(sem_id, MUTEX);
        exit(1);
    }

    //Fase di prenotazione temporanea
    int i = 0, j = 0;

    while (j < DIM_POSTI && i < n){

        if (teatro[j].stato == LIBERO){
            
            teatro[j].stato = IN_AGGIORNAMENTO; //Prenotazione soft
            array[i] = j;                       //Salvo il numero del posto nel mio array
            i++;

        }

        j++;

    }

    //A questo punto sono stati riservati i posti e lo modifico
    *disponibilita -= n;   

    Signal_Sem (sem_id, MUTEX); //Rilascio sezione critica

    sleep(1);                   //Simula il tempo di pagamento


    //Faccio partire la seconda sezione critica
    Wait_Sem (sem_id, MUTEX);

    //Conferma definitiva
    for(int i = 0; i < n; i++){

        teatro[array[i]].stato = OCCUPATO;
        teatro[array[i]].id_cliente = getpid(); //Firmo il posto con il pid 

    }

    printf("Cliente PID=%d ha prenotato %d posti\n", getpid(), n);

    //Esco dalla sezione critica
    Signal_Sem (sem_id, MUTEX);

    exit(0); //chiudere SEMPRE con exit

}