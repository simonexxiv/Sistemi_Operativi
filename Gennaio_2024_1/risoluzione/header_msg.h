#ifndef _HEADER_MSG_
#define _HEADER_MSG_

#include <sys/types.h>
#define MSG_TYPE 1

typedef struct {

    /* TBD: Definire il messaggio di richiesta */
    long tipo;
    int valori;
    pid_t pid;
    

} msg_init_request;

typedef struct {

    /* TBD: Definire il messaggio di risposta */
    long tipo;
    int id_shm_send;
    int id_shm_rcv;
    int id_sem_send;
    int id_sem_rcv;

    

} msg_init_response;


#endif