#define MUTEXL 0
#define SYNCH 1

#define NUM_LETTORI 6
#define NUM_SCRITTORI 6

typedef long msg;

typedef struct {

    int numlettori;
    msg messaggio;

} Buffer;

void Lettore (int, Buffer*);
void Scrittore (int, Buffer*);
