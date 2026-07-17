#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "header.h"

void ProcessoP1(VettoreInput*); /* TBD: Completare il passaggio dei parametri */
void ProcessoP2(VettoreInput*); /* TBD: Completare il passaggio dei parametri */
void ProcessoP3(VettoreInput*, MatriceParametri1, VettoreIntermedio*);/* TBD: Completare il passaggio dei parametri */
void ProcessoP4(VettoreIntermedio *, MatriceParametri2 ); /* TBD: Completare il passaggio dei parametri */

int main() {

    srand(getpid());

    int shm_id_input = shmget(IPC_PRIVATE, sizeof(VettoreInput*), IPC_CREAT|0664);
    if(shm_id_input < 0) die("SHMGET\n");

    VettoreInput * input = (VettoreInput*) shmat(shm_id_input, NULL, 0); 
    if(input == (void*)-1) die("errore shmat input\n");
    /* TBD: Allocare e inizializzare il vettore di input */

    init_input(input);

    int shm_id_intermedio = shmget(IPC_PRIVATE, sizeof(VettoreIntermedio*), IPC_CREAT|0664);
    if(shm_id_intermedio < 0) die("SHMGET intermedio\n");

    VettoreIntermedio * intermedio = (VettoreIntermedio*) shmat(shm_id_intermedio, NULL, 0);
    if(intermedio == (void*)-1) die("errore shmat intermedio\n");
    /* TBD: Allocare e inizializzare il vettore di risultati intermedio */

    init_intermedio(intermedio);

    MatriceParametri1 matrice_1;

    printf("Matrice 1: \n");

    for(int i=0; i<DIM3; i++) {

        for(int j=0; j<DIM4; j++) {

            matrice_1[i][j] = rand() % 10;

            printf("[%d][%d]=%d ", i, j, matrice_1[i][j]);
        }

        printf("\n");
    }

    printf("\n");



    MatriceParametri2 matrice_2;

    printf("Matrice 2: \n");

    for(int i=0; i<DIM6; i++) {

        for(int j=0; j<DIM7; j++) {

            matrice_2[i][j] = rand() % 10;

            printf("[%d][%d]=%d ", i, j, matrice_2[i][j]);
        }

        printf("\n");

    }

    printf("\n");



    /* TBD: Avviare i processi P1, P2, P3, P4 */
    pid_t pid_p1 = fork();
    if(pid_p1 < 0) die("errore fork\n");
    else if (pid_p1 == 0){

        printf("[MAIN] Avvio processo P1...\n");
        ProcessoP1(input);
        exit(0);

    }

    pid_t pid_p2 = fork();
    if(pid_p2 < 0) die("errore fork\n");
    else if (pid_p2 == 0){

        printf("[MAIN] Avvio processo P2...\n");
        ProcessoP2(input);
        exit(0);

    }

    pid_t pid_p3 = fork();
    if(pid_p3 < 0) die("errore fork\n");
    else if (pid_p3 == 0){

        printf("[MAIN] Avvio processo P3...\n");
        ProcessoP3(input, matrice_1, intermedio);
        exit(0);

    }

    pid_t pid_p4 = fork();
    if(pid_p4 < 0) die("errore fork\n");
    else if (pid_p4 == 0){

        printf("[MAIN] Avvio processo P4...\n");
        ProcessoP4(intermedio, matrice_2);
        exit(0);

    }

    /* TBD: Attendere la terminazione dei processi P1, P2, P3, P4 */
    while(wait(NULL) > 0);


    /* TBD: De-inizializzare e deallocare i vettori */
    remove_input(input);
    remove_intermedio(intermedio);

    shmctl(shm_id_input, IPC_RMID, NULL);
    shmctl(shm_id_intermedio, IPC_RMID, NULL);

    return 0;

}


void ProcessoP1(VettoreInput* input /* TBD: Completare il passaggio dei parametri */) {

    for(int i=0; i<3; i++) {

        printf("[P1] Inserimento vettore input: ");

        int val[DIM1];

        for(int j=0; j<DIM1; j++) {

            val[j] = rand() % 10;

            printf("[%d]=%d ", j, val[j]);
        }

        printf("\n");

        inserisci_input_primo_vettore(input, val/* TBD: Completare il passaggio dei parametri */);

        sleep(1);
    }

}

void ProcessoP2(VettoreInput * input/* TBD: Completare il passaggio dei parametri */) {

    for(int i=0; i<3; i++) {

        printf("[P2] Inserimento vettore input: ");

        int val[DIM2];

        for(int j=0; j<DIM2; j++) {

            val[j] = rand() % 10;

            printf("[%d]=%d ", DIM1+j, val[j]);
        }

        printf("\n");

        inserisci_input_secondo_vettore(input, val/* TBD: Completare il passaggio dei parametri */);

        sleep(1);
    }
}

void ProcessoP3(VettoreInput* input, MatriceParametri1 matrice_1, VettoreIntermedio * intermedio /* TBD: Completare il passaggio dei parametri */) {

    for(int i=0; i<3; i++) {

        int val[DIM1+DIM2];

        preleva_input(input, val/* TBD: Completare il passaggio dei parametri */);

        printf("[P3] Prelevamento vettore input: ");

        for(int j=0; j<DIM1+DIM2; j++) {

            printf("[%d]=%d ", j, val[j]);
        }

        printf("\n");



        int risultato[DIM5];

        for(int i=0; i<DIM3; i++) {

            risultato[i] = prodotto_scalare(val, matrice_1[i], DIM4/* TBD: Completare il passaggio dei parametri */);
        
        }


        printf("[P3] Inserimento vettore intermedio: ");

        for(int j=0; j<DIM5; j++) {

            printf("[%d]=%d ", j, risultato[j]);
        }

        printf("\n");

        inserisci_intermedio_vettore(intermedio, risultato/* TBD: Completare il passaggio dei parametri */);

        sleep(1);
    }

}

void ProcessoP4(VettoreIntermedio * intermedio, MatriceParametri2 matrice_2) {

    for(int i=0; i<3; i++) {

        int val[DIM5];

        preleva_intermedio(intermedio, val);

        printf("[P4] Prelevamento vettore intermedio: ");

        for(int j=0; j<DIM5; j++) {

            printf("[%d]=%d ", j, val[j]);
        }

        printf("\n");


        Output risultato;

        for(int i=0; i<DIM6; i++) {

            risultato[i] = prodotto_scalare(val, matrice_2[i], DIM7);
        
        }



        printf("[P4] Risultato: ");

        for(int j=0; j<DIM8; j++) {

            printf("[%d]=%d ", j, risultato[j]);
        }

        printf("\n");

    }


}
