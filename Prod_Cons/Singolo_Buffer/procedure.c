#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>

#include "procedure.h"
#include "semafori.h"

void produttore(struct condivisa *s, int sem) {
    printf("Produttore: Fermo prima di wait...\n");

    // TODO: 1. Attendere che il buffer sia libero
    Wait_Sem(sem, SPAZIO_DISPONIBILE);


    // TODO: 2. Scrivere un valore nel buffer 
    s->valore = rand() % 100;
    printf("Produttore: Ho scritto il valore %d\n", s->valore);


    // TODO: 3. Segnalare che il messaggio è pronto
    Signal_Sem(sem, MESSAGGIO_DISPONIBILE);
}

void consumatore(struct condivisa *s, int sem) {
    printf("Consumatore: Fermo prima di wait...\n");

    int valore_consumato;
    // TODO: 1. Attendere che il messaggio sia pronto 
    Wait_Sem(sem, MESSAGGIO_DISPONIBILE);


    // TODO: 2. Leggere il valore dal buffer e stamparlo
    valore_consumato = s->valore; 
    printf("Consumatore: Valore consumato %d\n", valore_consumato);

    // TODO: 3. Segnalare che lo spazio è di nuovo libero 
    Signal_Sem(sem, SPAZIO_DISPONIBILE);
}
