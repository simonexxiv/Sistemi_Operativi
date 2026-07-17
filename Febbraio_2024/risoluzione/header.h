#ifndef _HEADER_
#define _HEADER_

#include "monitor_hoare.h"
#include <stdio.h>
#include <stdlib.h>

#define DIM1 4
#define DIM2 3

#define DIM3 3
#define DIM4 7

#define DIM5 3

#define DIM6 2
#define DIM7 3

#define DIM8 2

#define CV_FULL 0       // produttori in attesa di spazio (post scriptum: mettere due cv_full per p1 e p2)
#define CV_EMPTY 1      // consumatori in attesa di dati

#define VUOTO 0
#define PIENO 1

typedef struct {

    int primo_vettore[DIM1];
    int secondo_vettore[DIM2];

    /* TBD: Definire ulteriori variabili per la sincronizzazione */
    Monitor m;

    int stato_v1;  // 0 = vuoto, 1 = pieno (primo vettore);
    int stato_v2; // 0 = vuoto, 1 = pieno (secondo vettore);

} VettoreInput;

typedef int MatriceParametri1[DIM3][DIM4];


typedef struct {

    int vettore[DIM5];

    /* TBD: Definire ulteriori variabili per la sincronizzazione */
    Monitor m;

    int stato; // 0 = vuoto, 1 = pieno;
    
} VettoreIntermedio;

typedef int MatriceParametri2[DIM6][DIM7];

typedef int Output[DIM8];


int prodotto_scalare(int *v1, int *v2, int n);

void init_input(VettoreInput *v);
void init_intermedio(VettoreIntermedio *v);
void remove_input(VettoreInput *v);
void remove_intermedio(VettoreIntermedio *v);

void inserisci_input_primo_vettore(VettoreInput *v, int *val);
void inserisci_input_secondo_vettore(VettoreInput *v, int *val);
void preleva_input(VettoreInput *v, int *val);

void inserisci_intermedio_vettore(VettoreIntermedio *v, int *val);
void preleva_intermedio(VettoreIntermedio *v, int *val);

static inline void die(const char* msg){
    perror(msg);
    exit(1);
}

#endif