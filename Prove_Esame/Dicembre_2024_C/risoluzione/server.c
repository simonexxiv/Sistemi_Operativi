
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/msg.h>
#include <unistd.h>
#include "header.h"

typedef struct {
    MonitorLettScritt * ls;
    int regali_richiesti;
} ParametriScrittore;

int coda_richieste;
int coda_risposte;

void inizializza_struttura(MonitorLettScritt * ls) {
    /*
        TODO: completare
    */
    ls->lettori_wait = 0;
    ls->scrittori_wait = 0;
    ls->numero_lettori = 0;
    ls->numero_scrittori = 0;
    ls->totale_regali = 0;

    pthread_mutex_init(&ls->mutex, NULL);
    pthread_cond_init(&ls->cv_lett, NULL);
    pthread_cond_init(&ls->cv_scritt, NULL);
}

void distruggi_struttura(MonitorLettScritt *ls) {
    /*
        TODO: completare
    */
    pthread_mutex_destroy(&ls->mutex);
    pthread_cond_destroy(&ls->cv_lett);
    pthread_cond_destroy(&ls->cv_scritt);
}

void inizio_lettura(MonitorLettScritt *ls) {
    /*
        TODO: completare
    */
    pthread_mutex_lock(&ls->mutex);

    while(ls->numero_scrittori > 0){
        ls->lettori_wait++;
        pthread_cond_wait(&ls->cv_lett, &ls->mutex);
        ls->lettori_wait--;
    }

    ls->numero_lettori++;

    pthread_cond_signal(&ls->cv_lett);

    pthread_mutex_unlock(&ls->mutex);
}

void fine_lettura(MonitorLettScritt *ls) {
    /*
        TODO: completare
    */
    pthread_mutex_lock(&ls->mutex);

    ls->numero_lettori--;

    if(ls->numero_lettori == 0){
        pthread_cond_signal(&ls->cv_scritt);
    }

    pthread_mutex_unlock(&ls->mutex);
}

void inizio_scrittura(MonitorLettScritt *ls) {
    /*
        TODO: completare
    */
    pthread_mutex_lock(&ls->mutex);

    while(ls->numero_scrittori > 0 || ls->numero_lettori > 0){
        ls->scrittori_wait++;
        pthread_cond_wait(&ls->cv_scritt, &ls->mutex);
        ls->scrittori_wait--;
    }

    ls->numero_scrittori++;

    pthread_mutex_unlock(&ls->mutex);
}

void fine_scrittura(MonitorLettScritt *ls) {
    /*
        TODO: completare
    */
    pthread_mutex_lock(&ls->mutex);

    ls->numero_scrittori--;

    if(ls->scrittori_wait > 0){
        pthread_cond_signal(&ls->cv_scritt);
    }else {
        pthread_cond_broadcast(&ls->cv_lett);
    }

    pthread_mutex_unlock(&ls->mutex);
}

void *Scrittore(void *arg) {
    /*
        TODO: recupero parametri thread e inizio scrittura
    */
    ParametriScrittore * ps = (ParametriScrittore*) arg;
    MonitorLettScritt * ls = ps->ls;
    
    inizio_scrittura(ls);

    ls->totale_regali += ps->regali_richiesti;

    printf("Il totale dei regali richiesti finora è: %d\n", ls->totale_regali);

    /*
        TODO: completare
    */
    fine_scrittura(ls);

    pthread_exit(NULL);
}

void *Lettore(void *arg) {
    /*
        TODO: recupero parametri thread
    */
    MonitorLettScritt * ls = (MonitorLettScritt *) arg;

    sleep(3);

    inizio_lettura(ls);

    printf("[Lettore] Il totale di regali da consegnare è: %d\n", ls->totale_regali);

    fine_lettura(ls);


    /*
        TODO: invio messaggio di risposta
    */
    MessaggioRisposta m;
    m.tipo = MESSAGGIO;
    int ret = msgsnd(coda_risposte, (void*)&m, sizeof(MessaggioRisposta)-sizeof(long), 0);
    if(ret < 0) die("Errore mssend server\n");

    printf("Ho inviato conferma di ricezione al bambino\n");

    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));

    /*
        TODO: recupero code e inizializzazione strutture
    */
    key_t chiave_richieste = ftok(".", 'A');
    coda_richieste = msgget(chiave_richieste, 0);

    key_t chiave_risposte = ftok(".", 'B');
    coda_risposte = msgget(chiave_risposte, 0);

    MonitorLettScritt * ls = (MonitorLettScritt *) malloc(sizeof(MonitorLettScritt));

    inizializza_struttura(ls);

    ParametriScrittore parametri[NUM_BAMBINI*NUM_LETTERE];

    pthread_t threads_scrittori[NUM_BAMBINI*NUM_LETTERE];
    pthread_t threads_lettori[NUM_BAMBINI*NUM_LETTERE];
    /* TODO */
        
    MessaggioRichiesta msg;
    for (int i = 0; i < NUM_BAMBINI*NUM_LETTERE; i++) {
        msgrcv(coda_richieste,&msg,sizeof(MessaggioRichiesta)-sizeof(long),0,0);
        /*
            TODO: ricezione messaggio, inizializzazione della struttura parametri da passare al thread e creazione scrittore
        */
        
        printf("Ho ricevuto una letterina! Sono richiesti %d regali\n", msg.numero_regali);

        parametri[i].ls = ls;
        parametri[i].regali_richiesti = msg.numero_regali;

        pthread_create(&threads_scrittori[i], NULL, Scrittore, (void*)&parametri[i]);

        pthread_create(&threads_lettori[i], NULL, Lettore, (void*)ls);
    }


    /*
        TODO: attendo terminazione thread e dealloco strutture
    */
    for(int i = 0; i < NUM_BAMBINI*NUM_LETTERE; i++){
        pthread_join(threads_lettori[i], NULL);
    }

    for(int i = 0; i < NUM_BAMBINI*NUM_LETTERE;i++){
        pthread_join(threads_scrittori[i], NULL);
    }

    distruggi_struttura(ls);
    free(ls);

    return 0;

}