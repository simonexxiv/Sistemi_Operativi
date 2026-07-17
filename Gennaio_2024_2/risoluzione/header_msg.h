#ifndef _HEADER_MSG_
#define _HEADER_MSG_

/* TBD: Definire le strutture dati per l'invio dei messaggi */
#define MESSAGGIO 1

#include <sys/types.h>

typedef struct{
    long tipo;
    int valore;
    int id_buffer; //0,1,2
    pid_t pid_client;
}messaggio_richiesta;

typedef struct{
    long tipo;  //sarà pari al PID del client specificato
    int valore;
}messaggio_risposta;

#define NUM_BUFFER 3
#define NUM_SERVER (NUM_BUFFER*2)
#define NUM_RICHIESTE 4

#endif