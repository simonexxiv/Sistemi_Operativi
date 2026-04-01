all: lettscritt

lettscritt: lettscritt.o procedure.o semafori.o
	gcc -o lettore_scrittore lettscritt.o procedure.o semafori.o

lettscritt.o: lettscritt.c header.h
	gcc -c lettscritt.c

procedure.o: procedure.c header.h
	gcc -c procedure.c

semafori.o: semafori.c semafori.h
	gcc -c semafori.c

clean:
	rm -f *.o 
	rm -f lettore_scrittore