#ifndef _circolare_
#define _circolare_

#include "monitor_hoare.h"
#include <stdio.h>
#include <stdlib.h>

#define VARCOND_PRODUTTORI 0
#define VARCOND_CONSUMATORI 1

#define MESSAGGIO 1

#define LIBERO 0
#define OCCUPATO 1

typedef struct {

    int buffer;

    /* TBD: Introdurre ulteriori variabili per la gestione del buffer
            e per la sincronizzazione */

    int pieno;
    
    Monitor m;

} MonitorBufferSingolo;


void init_monitor_buffer_singolo(MonitorBufferSingolo * b);
void produzione_buffer_singolo(MonitorBufferSingolo * b, int val);
int consumazione_buffer_singolo(MonitorBufferSingolo * b);
void remove_monitor_buffer_singolo(MonitorBufferSingolo * b);


typedef struct {

    /* TBD: Definire una struttura dati e le costanti per lo scambio di messaggi */
    long tipo;

    int valore;

} messaggio;

static inline void die(const char* msg){
    perror(msg);
    exit(1);
}

#endif