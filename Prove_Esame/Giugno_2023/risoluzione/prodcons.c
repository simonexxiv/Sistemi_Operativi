
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>

#include "prodcons.h"


void init_monitor_buffer_singolo(MonitorBufferSingolo * b) {

    /* TBD: Inizializzare il monitor */
    b->pieno = LIBERO;
    init_monitor(&b->m, 2);
}

void produzione_buffer_singolo(MonitorBufferSingolo * b, int val) {

    /* TBD: Completare la produzione sul buffer singolo con sincronizzazione */
    enter_monitor(&b->m);

    if(b->pieno == OCCUPATO){
        wait_condition(&b->m, VARCOND_PRODUTTORI);
    }

    b->buffer = val;
    b->pieno = OCCUPATO;

    signal_condition(&b->m, VARCOND_CONSUMATORI);

    leave_monitor(&b->m);
}

int consumazione_buffer_singolo(MonitorBufferSingolo * b) {

    int val;

    enter_monitor(&b->m);

    /* TBD: Completare la consumazione dal buffer singolo con sincronizzazione */
    if(b->pieno == LIBERO){
        wait_condition(&b->m, VARCOND_CONSUMATORI);
    }

    val = b->buffer;
    b->pieno = LIBERO;

    signal_condition(&b->m, VARCOND_PRODUTTORI);

    leave_monitor(&b->m);
	
    return val;
}

void remove_monitor_buffer_singolo(MonitorBufferSingolo * b) {

    /* TBD: Disattivare l'oggetto monitor */
    remove_monitor(&b->m);
}