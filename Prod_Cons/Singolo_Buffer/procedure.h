#define SPAZIO_DISPONIBILE 0
#define MESSAGGIO_DISPONIBILE 1

struct condivisa{
    int valore;
};

void produttore(struct condivisa *s, int sem_id);
void consumatore(struct condivisa *s, int sem_id);