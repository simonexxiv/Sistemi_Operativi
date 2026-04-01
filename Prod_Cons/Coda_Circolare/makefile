all: codacircolare

codacircolare: procedure.o semafori.o codacircolare.o
	gcc -o codacircolare procedure.o semafori.o codacircolare.o

codacircolare.o: codacircolare.c
	gcc -c codacircolare.c

procedure.o: procedure.h procedure.c
	gcc -c procedure.c

semafori.o: semafori.c semafori.h
	gcc -c semafori.c



clean:
	rm -rf *.o
	rm -rf codacircolare

