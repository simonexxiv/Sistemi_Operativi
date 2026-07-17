#include "monitor_fifo.h"
#include "monitor_signal_continue.h"

#include <stdio.h>
#include <unistd.h>

void inizializza(MonitorProdConsFIFO * p) {

    /* TBD: Inizializzare la struttura dati */
    p->numero_liberi = DIM;
    p->numero_occupati = 0;
    p->ultimo_produttore_arrivato = 0;
    p->ultimo_produttore_avviato = 0;

    for(int i = 0; i < DIM; i++){
        p->stato[i] = 0;
    }

    init_monitor(&p->m, 2);
}

void produci(MonitorProdConsFIFO * p, int valore) {

    /* TBD: Completare con la sincronizzazione */
    enter_monitor(&p->m);

    int arrivo = p->ultimo_produttore_arrivato;

    p->ultimo_produttore_arrivato++;

    printf("[PRODUTTORE %d] Il mio turno di arrivo è %d\n", getpid(), arrivo);


    // ... gestire le condizioni di attesa ...
    while(p->numero_liberi == 0 || arrivo != p->ultimo_produttore_avviato){
        printf("[PRODUTTORE %d] Sospeso...\n", getpid());
        wait_condition(&p->m, CV_PROD);
        printf("[PRODUTTORE %d] Ripreso...\n", getpid());
    }

    printf("[PRODUTTORE %d] Il mio turno di produzione è %d\n", getpid(), p->ultimo_produttore_avviato);

    p->ultimo_produttore_avviato++;

    int i = 0;
    while( i < DIM && p->stato[i] != LIBERO) i++;

    p->numero_liberi--;
    p->stato[i] = IN_USO;

    leave_monitor(&p->m);

    // PRODUZIONE VERA E PROPRIA
    p->buffer[i/* TBD */] = valore;

    printf("[PRODUTTORE %d] Prodotto valore %d in pos %d\n", getpid(), valore, i);

    sleep(1);

    enter_monitor(&p->m);

    p->numero_occupati++;
    p->stato[i] = OCCUPATO;

    signal_condition(&p->m, CV_CONS);

    leave_monitor(&p->m);

}

int consuma(MonitorProdConsFIFO * p) {

    /* TBD: Completare con la sincronizzazione */
    enter_monitor(&p->m);

    while(p->numero_occupati == 0){
        printf("[CONSUMATORE %d] Sospeso...\n", getpid());
        wait_condition(&p->m, CV_CONS);
        printf("[CONSUMATORE %d] Ripreso...\n", getpid());
    }

    int i = 0;
    while(i < DIM && p->stato[i] != OCCUPATO) i++;

    p->numero_occupati--;
    p->stato[i] = IN_USO;

    leave_monitor(&p->m);

    printf("[CONSUMATORE %d] Consumazione in corso\n", getpid());

    int valore = p->buffer[i/* TBD */];

    sleep(1);

    /* Nota: Occorre riattivare -tutti- i produttori in attesa.
             Il numero di produttori in attesa è dato dalla seguente differenza.
     */

    int produttori_in_attesa = p->ultimo_produttore_arrivato - p->ultimo_produttore_avviato;
    enter_monitor(&p->m);

    p->numero_liberi++;
    p->stato[i] = LIBERO;

    signal_all(&p->m, CV_PROD);

    leave_monitor(&p->m);

    return valore;
}

void distruggi(MonitorProdConsFIFO * p) {

    /* TBD: Disattivare l'oggetto monitor */
    remove_monitor(&p->m);
}