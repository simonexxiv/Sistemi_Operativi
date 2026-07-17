#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "header.h"

typedef struct{
    /*TBD: Utilizzare questa struttura per il passaggio dei parametri ai thread*/

    coppia_buffer * buffer;

    int coda_sensori;
    int coda_attuatore;

}parametri_thread;

void* thread_produttore( void* arg/*Completare il passaggio dei parametri*/ ){

    parametri_thread * p = (parametri_thread *) arg;
    
    for(int i = 0; i < 10; i++){
        /*TBD: Ricevere un messaggio con il valore sulla coda dei sensori, di tipo "X" (con ricezione selettiva)*/
        messaggio_sensore m;

        int ret = msgrcv(p->coda_sensori, (void*)&m, sizeof(messaggio_sensore)-sizeof(long), X, 0);
        if(ret < 0) die("Errore msgrcv produttore X\n");


        int tipo = /*TBD*/ m.type;
        int valore_x = /*TBD*/m.value;

        printf("[Planner] Ricevuto %d di tipo X\n", valore_x);

        produci(/*TBD*/p->buffer, tipo, valore_x);


        /*TBD: Ricevere un messaggio con il valore sulla coda dei sensori, di tipo "Y" (con recezione selettiva)*/
        messaggio_sensore msg;

        ret = msgrcv(p->coda_sensori, (void*)&msg, sizeof(messaggio_sensore)-sizeof(long), Y, 0);
        if(ret < 0) die("Errore msgrcv produttore Y\n");

        tipo = msg.type/*TBD*/;
        int valore_y = msg.value/*TBD*/;

        printf("[Planner] Ricevuto %d di tipo Y\n", valore_y);

        produci(/*TBD*/p->buffer, tipo, valore_y);
    }
    return NULL;
}

void* thread_consumatore( void* arg/*Completare il passaggio dei parametri*/ ){

    parametri_thread * p = (parametri_thread *) arg;
    
    for(int i = 0; i < 10; i++){
        
        int valore_x;
        int valore_y;

        consuma(p->buffer/*TBD*/, &valore_x, &valore_y);

        int valore_attuatore = valore_x + valore_y;

        printf("[Planner] Invio valorer %d all' attuatore\n", valore_attuatore);

        /*TBD: Inviare un messaggio all'attuatore, contenente "valore_attuatore"*/
        messaggio_attuatore m;
        m.value = valore_attuatore;
        m.type = 1;
        int ret = msgsnd(p->coda_attuatore, (void*)&m, sizeof(m)-sizeof(long), 0);
        if(ret < 0) die("Erroe msgsend constumatore\n");
    }
    return NULL;
}

void init_coppia_buffer(coppia_buffer *b){
    
    /*TBD: Inizializzare la struttura*/
    b->stato_x = LIBERO;
    b->stato_y = LIBERO;
    b->buffer_x = 0;
    b->buffer_y = 0;

    pthread_mutex_init(&b->mutex, NULL);
    pthread_cond_init(&b->cv_cons, NULL);
    pthread_cond_init(&b->cv_prod, NULL);
}

void remove_coppia_buffer(coppia_buffer *b){
    
    /*TBD: De-Inizializzare la struttura*/
    pthread_mutex_destroy(&b->mutex);
    pthread_cond_destroy(&b->cv_cons);
    pthread_cond_destroy(&b->cv_prod);
}

void produci(coppia_buffer *buffer, int tipo, int valore){
    
    /* TBD: Completare la sincronizzazione.
    

    Nota: Se il valore è di tipo X, il produttore deve attendere che buffer_x diventi libero.
            Se il valore è di tipo Y, il produttore deve attendere che buffer_y diventi libero.
    */

    pthread_mutex_lock(&buffer->mutex);

    if(tipo== X/*verificare se il valore è di tipo X*/){

        while(buffer->stato_x == OCCUPATO){
            pthread_cond_wait(&buffer->cv_prod, &buffer->mutex);
        }

        printf("[Planner] Produzione valore %d di tipo X\n", valore);

        buffer->buffer_x=valore;

        buffer->stato_x = OCCUPATO;
        /*Nota: il buffer_x diventa occupato*/

    }else if (tipo== Y/*verificare se il valore è di tipo Y*/) {

         while(buffer->stato_y == OCCUPATO){
            pthread_cond_wait(&buffer->cv_prod, &buffer->mutex);
        }
        
        printf("[Planner] Produzione valore %d di tipo Y\n", valore);

        buffer->buffer_y=valore;

        buffer->stato_y = OCCUPATO;

        /*Nota: il buffer_y diventa occupato*/

    }

    pthread_cond_signal(&buffer->cv_cons);

    pthread_mutex_unlock(&buffer->mutex);
}

void consuma(coppia_buffer * buffer, int * valore_x, int * valore_y){
    
    /* TBD: Completare la sincronizzazione */
    pthread_mutex_lock(&buffer->mutex);

    while(buffer->stato_x == LIBERO || buffer->stato_y == LIBERO){
        pthread_cond_wait(&buffer->cv_cons, &buffer->mutex);
    }

    *valore_x = buffer->buffer_x;
    *valore_y = buffer->buffer_y;

        printf("[Planner] Consumazione valori %d (X) e %d (Y)\n", *valore_x, *valore_y);

        /*Nota: i buffer diventano entrambi liberi*/
    buffer->stato_x = LIBERO;
    buffer->stato_y = LIBERO;

    pthread_cond_signal(&buffer->cv_prod);
    pthread_mutex_unlock(&buffer->mutex);

}


void planner(int coda_sensori, int coda_attuatore){

    printf("[Planner] Avviato...\n");

    /*TBD: Creare una struttura dati di tipo "coppia_buffer" */
    coppia_buffer * b = (coppia_buffer *) malloc(sizeof(coppia_buffer));

    init_coppia_buffer(b);

    pthread_t threads_consumatore;
    pthread_t threads_produttore;

    parametri_thread param;

    param.buffer = b;
    param.coda_attuatore = coda_attuatore;
    param.coda_sensori = coda_sensori;

    pthread_create(&threads_consumatore, NULL, thread_consumatore, (void*)&param);
    pthread_create(&threads_produttore, NULL, thread_produttore, (void*)&param);

    /*TBD: Avviare i due thread, facendogli eseguire le funzioni "thread_produttore" e "thread_consumatore",
        e passandogli i parametri necessari.
    */

    /*TBD: Attendere la terminazione dei thread e de-allocare le risorse*/
    pthread_join(threads_consumatore, NULL);
    pthread_join(threads_produttore, NULL);

    remove_coppia_buffer(b);

    free(b);
}
