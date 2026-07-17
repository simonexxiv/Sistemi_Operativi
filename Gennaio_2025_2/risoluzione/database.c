
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <unistd.h>

#include "header.h"

typedef struct {

    int * vettore;

    /* TBD: Aggiungere altre variabili per la sincronizzazione */
    pthread_mutex_t mutex;

} risorsa_database;


typedef struct {

    /* TBD: Utilizzare questa struttura dati per il
            passaggio dei parametri dal thread principale ai
            thread worker.
     */
    int coda_richieste_serv_db;
    int coda_risposte_serv_db;

    risorsa_database * r;

} parametri_worker;


void * worker(void * x);


#define THREAD_WORKER 3


int main() {

    // risorsa gestita dal database (vettore di interi)

    risorsa_database * r = malloc(sizeof(risorsa_database));

    r->vettore = malloc(sizeof(int) * DIM_VETTORE);

    for(int i=0; i<DIM_VETTORE; i++) {
        r->vettore[i] = 0;
    }


    /* TBD: Completare l'inizializzazione delle variabili di sincronizzazione */
    pthread_mutex_init(&r->mutex, NULL);


    /* TBD: Ottenere gli identificativi delle code di messaggi */
    key_t chiave_richieste_serv_db = ftok(".", 'C');
    int coda_richieste_serv_db = msgget(chiave_richieste_serv_db, 0);
    if(coda_richieste_serv_db < 0) die("Errore MSGET coda richiste server db\n");

    key_t chiave_risposte_serv_db = ftok(".", 'D');
    int coda_risposte_serv_db = msgget(chiave_risposte_serv_db, 0);
    if(coda_risposte_serv_db < 0) die("Errore MSGET coda risposte\n");



    /* TBD: Creare 3 thread worker, e utilizzare la struttura "parametri_worker"
            per passare ai thread il puntatore alla risorsa e gli
            identificativi delle code di messaggi.
     */
    pthread_t threads[THREAD_WORKER];
    parametri_worker param[THREAD_WORKER];

    for(int i=0; i<THREAD_WORKER; i++) {

        param[i].coda_richieste_serv_db = coda_richieste_serv_db;
        param[i].coda_risposte_serv_db = coda_risposte_serv_db;
        param[i].r = r;

        pthread_create(&threads[i], NULL, worker, (void*)&param[i]);
        /* TBD */
    }

    /* TBD: Attendere la terminazione dei thread worker */
    for(int i = 0; i < THREAD_WORKER; i++){
        pthread_join(threads[i], NULL);
    }

    free(r->vettore);
    free(r);

}



void * worker(void * x) {

    /* TBD: Completare il passaggio dei parametri */

    

    parametri_worker * p = (parametri_worker*) x;
    risorsa_database * r = p->r/* TBD */;
    
    for(int i=0; i<NUMERO_RICHIESTE; i++) {

        /* TBD: Ricevere un messaggio di richiesta */
        messaggio_richiesta m;

        int ret = msgrcv(p->coda_richieste_serv_db, (void*)&m, sizeof(messaggio_richiesta)-sizeof(long), 0, 0);
        if(ret < 0) die("Errore msgrcv db\n");

        int tipo_richiesta = m.tipo/* TBD */;
        int valore = m.valore/* TBD */;
        int posizione = m.posizione/* TBD */;
        int pid_server = m.pid_mittente/* TBD */;

        printf("[DB] Ricevuta richiesta (tipo=%d, pid=%d, posizione=%d, valore=%d)\n", tipo_richiesta, pid_server, posizione, valore);


        int valore_risposta;

        /* TBD: Aggiungere la sincronizzazione per l'accesso alla risorsa condivisa */
        pthread_mutex_lock(&r->mutex);

        if(tipo_richiesta == LETTURA/* TBD: Verificare se è una richiesta di LETTURA */) {

            valore_risposta = r->vettore[posizione];

        }
        else if(tipo_richiesta == SCRITTURA/* TBD: Verificare se è una richiesta di SCRITTURA */) {

            r->vettore[posizione] = valore;
            valore_risposta = 0;

        }
        else {
            printf("[DB] Tipo richiesta non valido\n");
        }

        pthread_mutex_unlock(&r->mutex);



        /* TBD: Inviare un messaggio di risposta, includendo la variabile "valore_risposta" */
        messaggio_risposta mess;

        mess.valore = valore_risposta;
        mess.tipo = pid_server;

        ret = msgsnd(p->coda_risposte_serv_db, (void*)&mess, sizeof(messaggio_risposta)-sizeof(long), 0);
        if (ret < 0) die("Errore\n");

        printf("[DB] Invio risposta (tipo=%d, pid=%d, posizione=%d, valore=%d)\n", tipo_richiesta, pid_server, posizione, valore_risposta);
    }

    pthread_exit(NULL);
    return NULL;

}