all: vettoredistato

vettoredistato: procedure.o semafori.o vettoredistato.o
	gcc -o vettoredistato procedure.o semafori.o vettoredistato.o

vettoredistato.o: vettoredistato.c
	gcc -c vettoredistato.c

procedure.o: procedure.h procedure.c
	gcc -c procedure.c

semafori.o: semafori.c semafori.h
	gcc -c semafori.c



clean:
	rm -rf *.o
	rm -rf vettoredistato

