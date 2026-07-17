#include "header.h"

int prodotto_scalare(int *v1, int *v2, int n) {

    int sum = 0;

    for(int i=0; i<n; i++) {

        sum += v1[i] * v2[i];
    }

    return sum;
}



void init_input(VettoreInput *v) {

    /* TBD: Completare l'inizializzazione */
    v->stato_v1 = 0;
    v->stato_v2 = 0;
    for(int i = 0; i < DIM1; i++){
        v->primo_vettore[i] = 0;
    }

    for(int i = 0; i < DIM2; i++){
        v->secondo_vettore[i] = 0;
    }

    init_monitor(&v->m, 2);
}

void init_intermedio(VettoreIntermedio *v) {

    /* TBD: Completare l'inizializzazione */
    init_monitor(&v->m, 2);

    v->stato = 0;

    for(int i = 0; i < DIM5; i++){
        v->vettore[i] = 0;
    }
}

void remove_input(VettoreInput *v) {

    /* TBD: Completare la de-inizializzazione */
    remove_monitor(&v->m);

    for(int i = 0; i < DIM1; i++){
        v->primo_vettore[i] = 0;
    }

    for(int i = 0; i < DIM2; i++){
        v->secondo_vettore[i] = 0;
    }

}

void remove_intermedio(VettoreIntermedio *v) {

    /* TBD: Completare la de-inizializzazione */

    remove_monitor(&v->m);

    for(int i = 0; i < DIM5; i++){
        v->vettore[i] = 0;
    }
}

void inserisci_input_primo_vettore(VettoreInput *v, int *val) {

    /* TBD: Completare la sincronizzazione */
    enter_monitor(&v->m);

    // se la prima metà è piena aspetto
    if(v->stato_v1 == PIENO){
        wait_condition(&v->m, CV_FULL);
    }

    // inserico la prima metà
    for(int i=0; i<DIM1; i++) {

        v->primo_vettore[i] = val[i];
    }

    v->stato_v1 = PIENO;

    // ora controllo se anche la seconda metà è piena, se lo è svegliamo P3
    if(v->stato_v2 == PIENO){
        signal_condition(&v->m, CV_EMPTY);
    }

    leave_monitor(&v->m);

}

void inserisci_input_secondo_vettore(VettoreInput *v, int *val) {

    /* TBD: Completare la sincronizzazione */
    enter_monitor(&v->m);

    // se la seconda metà è piena aspetto
    if(v->stato_v2 == PIENO){
        wait_condition(&v->m, CV_FULL);
    }

    //inserisco la seconda metà
    for(int i=0; i<DIM2; i++) {

        v->secondo_vettore[i] = val[i];
    }

    v->stato_v2 = PIENO;

    if(v->stato_v1 == PIENO){
        signal_condition(&v->m, CV_EMPTY);
    }

    leave_monitor(&v->m);

}

void preleva_input(VettoreInput *v, int *val) {

    /* TBD: Completare la sincronizzazione */

    enter_monitor(&v->m);

    // aspetta finche non si riempino entrambi
    if(v->stato_v1 == VUOTO && v->stato_v2 == VUOTO){
        wait_condition(&v->m, CV_EMPTY);
    }

    for(int i=0; i<DIM1; i++) {

        val[i] = v->primo_vettore[i];
    }

    for(int i=0; i<DIM2; i++) {

        val[i+DIM1] = v->secondo_vettore[i];
    }

    v->stato_v1 = VUOTO;
    v->stato_v2 = VUOTO;

    //svegliamo entrambi i produttori 
    signal_condition(&v->m, CV_FULL);
    signal_condition(&v->m, CV_FULL);

    leave_monitor(&v->m);

}

void inserisci_intermedio_vettore(VettoreIntermedio *v, int *val) {

    /* TBD: Completare la sincronizzazione */
    enter_monitor(&v->m);

    if(v->stato == PIENO){
        wait_condition(&v->m, CV_FULL);
    }

    for(int i=0; i<DIM5; i++) {

        v->vettore[i] = val[i];
    }

    v->stato = PIENO;

    signal_condition(&v->m, CV_EMPTY);

    leave_monitor(&v->m);

}

void preleva_intermedio(VettoreIntermedio *v, int *val) {

    /* TBD: Completare la sincronizzazione */

    enter_monitor(&v->m);

    if (v->stato == VUOTO){
        wait_condition(&v->m, CV_EMPTY);
    }

    for(int i=0; i<DIM5; i++) {

        val[i] = v->vettore[i];
    }

    v->stato = VUOTO;

    signal_condition(&v->m, CV_FULL);

    leave_monitor(&v->m);

}
