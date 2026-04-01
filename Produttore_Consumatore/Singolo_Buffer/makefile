all: singolobuffer

singolobuffer: procedure.o semafori.o singolobuffer.o
	gcc -o singolobuffer procedure.o semafori.o singolobuffer.o

singolobuffer.o: singolobuffer.c
	gcc -c singolobuffer.c

procedure.o: procedure.h procedure.c
	gcc -c procedure.c

semafori.o: semafori.c semafori.h
	gcc -c semafori.c



clean:
	rm -rf *.o
	rm -rf singolobuffer
