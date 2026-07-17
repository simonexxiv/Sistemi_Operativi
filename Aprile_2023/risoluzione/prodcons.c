#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "prodcons.h"
#include "monitor_signal_continue.h"

void inizializza(MonitorPC * m) {

    printf("Inizializzazione monitor\n");

    /* TBD: Inizializzazione */
    init_monitor(&m->monitor, 4);
    m->num_liberi = DIM;
    m->num_occupati_tipo1 = 0;
    m->num_occupati_tipo2 = 0;

    for(int i = 0; i < DIM; i++){
        m->stato[i] = 0;
    }

}


void rimuovi(MonitorPC * m) {

    printf("Rimozione monitor\n");

    /* TBD: Rimozione */
    remove_monitor(&m->monitor);

}


void produci_tipo_1(MonitorPC * m, int valore) {

    int index = 0;

    /* TBD: Aggiungere la sincronizzazione, 
            con la selezione dell'indice su cui produrre
            (alg. con vettore di stato) */
    enter_monitor(&m->monitor);

    while(m->num_liberi == 0){
        wait_condition(&m->monitor, CV_PROD_1);
    }

    while(index < DIM && m->stato[index] != LIBERO) index++;

    m->stato[index] = IN_USO;
    m->num_liberi--;

    leave_monitor(&m->monitor); 

    printf("Produzione di tipo 1 in corso (valore=%d, index=%d)\n", valore, index);

    sleep(1);
    m->vettore[index] = valore;

    enter_monitor(&m->monitor);

    m->num_occupati_tipo1++;
    m->stato[index] = OCCUPATO1;

    signal_condition(&m->monitor, CV_CONS_1);
 
    leave_monitor(&m->monitor);

}


void produci_tipo_2(MonitorPC * m, int valore) {

    int index = 0;

    /* TBD: Aggiungere la sincronizzazione, 
            con la selezione dell'indice su cui produrre
            (alg. con vettore di stato) */
    enter_monitor(&m->monitor);

    while(m->num_liberi == 0){
        wait_condition(&m->monitor, CV_PROD_2);
    }

    while(index < DIM && m->stato[index] != LIBERO) index++;

    m->stato[index] = IN_USO;
    m->num_liberi--;

    leave_monitor(&m->monitor); 

    printf("Produzione di tipo 2 in corso (valore=%d, index=%d)\n", valore, index);

    sleep(1);
    m->vettore[index] = valore;

    enter_monitor(&m->monitor);

    m->num_occupati_tipo2++;
    m->stato[index] = OCCUPATO2;

    signal_condition(&m->monitor, CV_CONS_2);
 
    leave_monitor(&m->monitor);
}


void consuma_tipo_1(MonitorPC * m, int * valore) {

    int index = 0;

    /* TBD: Aggiungere la sincronizzazione, 
        con la selezione dell'indice su cui consumare
        (alg. con vettore di stato) */
        
    enter_monitor(&m->monitor);

    while(m->num_occupati_tipo1 == 0){
        wait_condition(&m->monitor, CV_CONS_1);
    }

    while(index < DIM && m->stato[index] != OCCUPATO1)index++;

    m->stato[index] = IN_USO;
    m->num_occupati_tipo1--;

    leave_monitor(&m->monitor);

    sleep(1);
    *valore = m->vettore[index];

    printf("Consumazione di tipo 1 (valore=%d, index=%d)\n", *valore, index);

    enter_monitor(&m->monitor);

    m->num_liberi++;
    m->stato[index] = LIBERO;

    signal_condition(&m->monitor, CV_PROD_1);
    signal_condition(&m->monitor, CV_PROD_2);

    leave_monitor(&m->monitor);
}


void consuma_tipo_2(MonitorPC * m, int * valore) {

    int index = 0;

    /* TBD: Aggiungere la sincronizzazione, 
        con la selezione dell'indice su cui consumare
        (alg. con vettore di stato) */
        
    enter_monitor(&m->monitor);

    while(m->num_occupati_tipo2 == 0){
        wait_condition(&m->monitor, CV_CONS_2);
    }

    while(index < DIM && m->stato[index] != OCCUPATO2)index++;

    m->stato[index] = IN_USO;
    m->num_occupati_tipo2--;

    leave_monitor(&m->monitor);

    sleep(1);
    *valore = m->vettore[index];

    printf("Consumazione di tipo 2 (valore=%d, index=%d)\n", *valore, index);

    enter_monitor(&m->monitor);

    m->num_liberi++;
    m->stato[index] = LIBERO;

    signal_condition(&m->monitor, CV_PROD_2);
    signal_condition(&m->monitor, CV_PROD_1);

    leave_monitor(&m->monitor);
}