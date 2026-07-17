#include "header.h"
#include "monitor_signal_continue.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>

void inizializza_monitor(MonitorLettScritt *ls) {
	/*
        TODO: completare
    */
    ls->lettori_wait = 0;
    ls->scrittori_wait = 0;
    ls->num_scrittori = 0;
    ls->num_lettori = 0;
    ls->totale_regali = 0;

    init_monitor(&ls->m, 2);
}

void distruggi_monitor(MonitorLettScritt *ls) {
	/*
        TODO: completare
    */
    remove_monitor(&ls->m);
}

void inizio_lettura(MonitorLettScritt *ls) {
	/*
        TODO: completare
    */
    enter_monitor(&ls->m);

    while(ls->num_scrittori > 0 || ls->scrittori_wait > 0){
        ls->lettori_wait++;
        wait_condition(&ls->m, CV_LETT);
        ls->lettori_wait--;
    }

    ls->num_lettori++;

    signal_condition(&ls->m, CV_LETT);

    leave_monitor(&ls->m);
}

void fine_lettura(MonitorLettScritt *ls) {
	/*
        TODO: completare
    */
    enter_monitor(&ls->m);

    ls->num_lettori--;

    if(ls->num_lettori == 0){
        signal_condition(&ls->m, CV_SCRITT);
    }

    leave_monitor(&ls->m);
}

void inizio_scrittura(MonitorLettScritt *ls) {
	/*
        TODO: completare
    */
    enter_monitor(&ls->m);

    while(ls->num_lettori > 0 || ls->num_scrittori > 0){
        ls->scrittori_wait++;
        wait_condition(&ls->m, CV_SCRITT);
        ls->scrittori_wait--;
    }

    ls->num_scrittori++;

    signal_condition(&ls->m, CV_SCRITT);

    leave_monitor(&ls->m);
}

void fine_scrittura(MonitorLettScritt *ls) {
	/*
        TODO: completare
    */
    enter_monitor(&ls->m);

    ls->num_scrittori--;

    if(ls->scrittori_wait > 0){
        signal_condition(&ls->m, CV_SCRITT);
    }else {
        signal_all(&ls->m, CV_LETT);
    }

    leave_monitor(&ls->m);
}

void Lettore(int codaresp, MonitorLettScritt *ls) {

	/*
        TODO: completare
    */
    inizio_lettura(ls);



    printf("[Lettore] Il totale regali richiesti finora è: %d\n", ls->totale_regali);

    fine_lettura(ls);


    /*
        TODO: invio messaggio conferma di ricezione 
    */
    MessaggioRisposta m;
    m.tipo = MESSAGGIO;

    int ret = msgsnd(codaresp, (void*)&m, sizeof(MessaggioRisposta)-sizeof(long), 0);
    if(ret < 0) {
        perror("Errore send procedure\n");
        exit(1);
    }

    printf("[Lettore] Ho inviato la conferma di ricezione\n");
}

void Scrittore(int codareq, MonitorLettScritt *ls) {
    /*
        TODO: attesa messaggio
    */
    MessaggioRichiesta m;

    int ret = msgrcv(codareq, (void*)&m, sizeof(MessaggioRichiesta)-sizeof(long), 0, 0);

    printf("[Scrittore %d] Ricevuta richiesta per %d regali\n", getpid(), m.numero_regali);

	/*
        TODO: completare scrittura
    */
    inizio_scrittura(ls);

    ls->totale_regali += m.numero_regali;

    printf("[Scrittore %d] Adesso il totale dei regali richiesti è: %d\n",getpid(), ls->totale_regali);

    fine_scrittura(ls);

}