#ifndef _HEADER_H_
#define _HEADER_H_

typedef long msg;

#define MUTEXL 0
#define MUTEXS 1
#define MUTEX 2
#define SYNCH 3

typedef struct{

    msg messaggio;
    int numlettori;
    int numscrittori;

} Buffer;

void InizioLettura(int, Buffer*);
void FineLettura(int, Buffer*);
void InizioScrittura(int, Buffer*);
void FineScrittura(int,  Buffer*);
void Lettore(int, Buffer*);
void Scrittore(int, Buffer*);

#endif //_HEADER_H_
