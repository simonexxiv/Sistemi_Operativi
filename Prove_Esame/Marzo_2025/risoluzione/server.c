#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <unistd.h>
#include <pthread.h>

#include "header_msg.h"
#include "header_prodcons.h"

struct parametri_thread_1 {

    /* TBD: Inserire in questa struttura dati i parametri per il thread 1,
            includendo il buffer b1, e gli identificativi delle code di messaggi
    */
    struct buffer * b1;
    int coda_mess;
    int coda_ok;
    int coda_req;
};

struct parametri_thread_2 {
    /* TBD: Inserire in questa struttura dati i parametri per il thread 2,
            includendo il buffer b1 e il buffer b2
    */
    struct buffer * b1;
    struct buffer * b2;
};

struct parametri_thread_3 {
    /* TBD: Inserire in questa struttura dati i parametri per il thread 3,
            includendo il buffer b2
    */
    struct buffer * b2;
};

void * thread_1(void *);
void * thread_2(void *);
void * thread_3(void *);

int main() {

    /* TBD: Ottenere gli identificativi delle code di messaggi */
    key_t key_mess = ftok(".", 'A');
    int coda_mess = msgget(key_mess, 0);
    if(coda_mess < 0) die("Errore MSGGET Messaggio\n");

    key_t key_req = ftok(".", 'B');
    int coda_req = msgget(key_req, 0);
    if(coda_req < 0) die("Errore MSGGET Request\n");

    key_t key_ok = ftok(".", 'C');
    int coda_ok = msgget(key_ok, 0);
    if(coda_ok < 0) die("Errore MSGGET Ok\n");

    struct buffer * b1 = malloc(sizeof(struct buffer));/* TBD: Inizializzare il puntatore alla struttura dati */
    struct buffer * b2 = malloc(sizeof(struct buffer));/* TBD: Inizializzare il puntatore alla struttura dati */

    buffer_init(b1);
    buffer_init(b2);

    struct parametri_thread_1 * p1 = malloc(sizeof(struct parametri_thread_1));/* TBD: Inizializzare il puntatore alla struttura dati */

    /* TBD: Inizializzare i valori nella struttura "p1" */
    p1->b1 = b1;
    p1->coda_mess = coda_mess;
    p1->coda_ok = coda_ok;
    p1->coda_req = coda_req;

    struct parametri_thread_2 * p2 = malloc(sizeof(struct parametri_thread_2));/* TBD: Inizializzare il puntatore alla struttura dati */

    /* TBD: Inizializzare i valori nella struttura "p2" */
    p2->b1 = b1;
    p2->b2 = b2;

    struct parametri_thread_3 * p3 = malloc(sizeof(struct parametri_thread_3)); /* TBD: Inizializzare il puntatore alla struttura dati */

    /* TBD: Inizializzare i valori nella struttura "p3" */
    p3->b2 = b2;


    /* TBD: Creare i 3 thread, passandogli le 3 strutture dati con i parametri */
    pthread_t threads[3];
    
    pthread_create(&threads[0], NULL, thread_1, (void*)p1);
    pthread_create(&threads[1], NULL, thread_2, (void*)p2);
    pthread_create(&threads[2], NULL, thread_3, (void*)p3);

    /* TBD: Attendere la terminazione dei 3 thread */
    for(int i = 0; i < 3; i++){
        pthread_join(threads[i], NULL);
    }

    buffer_destroy(b1);
    buffer_destroy(b2);

    free(b1);
    free(b2);
    free(p1);
    free(p2);
    free(p3);

    pthread_exit(0);

}


void * thread_1(void * x) {

    /* TBD: Completare il passaggio dei parametri */
    struct parametri_thread_1 * p1 = (struct parametri_thread_1 *) x;

    struct rts rts;
    struct ots ots;
    struct richiesta r;


    for(int i=0; i<3; i++) {

        /* TBD: Ricevere il messaggio "request to send" */
        int ret = msgrcv(p1->coda_req, (void*)&rts, sizeof(rts)-sizeof(long), 0, 0);

        printf("[THREAD 1] Ricevuto messaggio RTS\n");



        /* TBD: Inviare il messaggio "ok to send" */
        ots.tipo = MESSAGGIO;

        ret = msgsnd(p1->coda_ok, (void*)&ots, sizeof(ots)-sizeof(long), 0);

        printf("[THREAD 1] Inviato messaggio OTS\n");



        /* TBD: Ricevere il messaggio con il valore */
        ret = msgrcv(p1->coda_mess, (void*)&r, sizeof(r)-sizeof(long), MESSAGGIO, 0);

        int valore = r.valore/* TBD */;

        printf("[THREAD 1] Ricevuto messaggio MSG con valore %d\n", valore);


        buffer_produci(p1->b1/* TBD */, valore);

        printf("[THREAD 1] Inserito valore %d nel buffer 1\n", valore);
    }

    pthread_exit(NULL);
    
}

void * thread_2(void * x) {

    /* TBD: Completare il passaggio dei parametri */
    struct parametri_thread_2 * p2 = (struct parametri_thread_2 *) x;

    int valore;

    for(int i=0; i<3; i++) {

        valore = buffer_consuma(p2->b1/* TBD */);


        printf("[THREAD 2] Prelevato valore %d dal buffer 1\n", valore);


        sleep(2);


        valore = valore * 2;

        buffer_produci(p2->b2/* TBD */, valore);

        printf("[THREAD 2] Inserito valore %d nel buffer 2\n", valore);
    }

    pthread_exit(NULL);
}

void * thread_3(void * x) {

    /* TBD: Completare il passaggio dei parametri */
    struct parametri_thread_3 * p3 = (struct parametri_thread_3 *) x;

    int valore;

    for(int i=0; i<3; i++) {

        valore = buffer_consuma(p3->b2/* TBD */);

        printf("[THREAD 3] Prelevato valore %d dal buffer 2\n", valore);

        sleep(1);

        valore = valore + 1;

        printf("[THREAD 3] Valore finale: %d\n", valore);

    }

    pthread_exit(NULL);
}
