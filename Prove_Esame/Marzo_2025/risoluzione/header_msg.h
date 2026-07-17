#ifndef _HEADER_H
#define _HEADER_H

#include <sys/ipc.h>
#include <sys/msg.h>

/* TBD: Completare la definizione delle strutture per i messaggi.
        È possibile definire in questo file delle costanti con la direttiva "define" 
 */

#define REQUEST_TO_SEND 1
#define OK_TO_SEND 1
#define MESSAGGIO 1

struct richiesta {
    /* TBD */
    long tipo;
    int valore;
};

struct rts {
    /* TBD */
    long tipo;

};

struct ots {
    /* TBD */
    long tipo;
};

#endif