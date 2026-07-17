
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "header.h"

CodaCircolare coda;

int main() {
    srand(time(NULL));

    /*
        TODO: allocazione buffer e inizializzazione strutture
    */   
    inizializza_coda(&coda);

    BufferBabbo * bb = (BufferBabbo*) malloc(sizeof(BufferBabbo));
    
    inizializza_buffer(bb);

    /*
        TODO: creazione thread produttori, consumatori e Babbo_Natale
    */
    pthread_t threads_prod[NPROD];
    for(int i = 0; i < NPROD; i++){
        pthread_create(&threads_prod[i], NULL, Produttore, NULL);
        // Qui non passo BB perch+ non serve nella funzione
    }

    pthread_t threads_cons[NCONS];
    for(int i = 0; i < NCONS; i++){
        pthread_create(&threads_cons[i], NULL, Consumatore, (void*)bb);
    }

    pthread_t thread_babbo;
    pthread_create(&thread_babbo, NULL, Babbo_Natale, (void*)bb);

    /*
        TODO: attesa terminazione thread produttori, consumatori e Babbo_Natale
    */
    for(int i = 0; i < NPROD; i++){
        pthread_join(threads_prod[i], NULL);
    }

    for(int i = 0; i < NCONS; i++){
        pthread_join(threads_cons[i], NULL);
    }

    pthread_join(thread_babbo, NULL);


    /*
        TODO: rimozione strutture
    */
    distruggi_coda(&coda);

    distruggi_buffer(bb);
    free(bb);

    return 0;
}

void* Produttore(void* p) {
    for (int i = 0; i < GIOCATTOLI_PER_PROD; i++) {
        sleep(2);
        int giocattolo = rand() % 100 + 1;
        /*
            TODO: produzione e terminazione
        */
        produci(&coda, giocattolo);
    }

    pthread_exit(NULL);

}

// Thread consumatore
void* Consumatore(void* p) {
     
    BufferBabbo * bb = (BufferBabbo *) p;
    for (int i=0; i<PRELIEVI_PER_CONS; i++) {
        /*
            TODO: consumo, aggiornamento e terminazione
        */
        consuma(&coda);
        aggiorna_conteggio(bb);
    }

    pthread_exit(NULL);

}

// Thread Babbo Natale
void* Babbo_Natale(void* p) {

    BufferBabbo * bb = (BufferBabbo *) p;

    for (int i=0; i<CONSEGNE; i++) {
        /*
            TODO: consegna e terminazione
        */
        consegna(bb);
    }

    pthread_exit(NULL);

}