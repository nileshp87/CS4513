all: player nutella

nutella: nutella.o msock.o playerlib.o
	gcc -g nutella.o msock.o playerlib.o -o nutella

player: playerlib.o player.o
	gcc playerlib.o player.o -o player

player.o: player.c
	gcc -g -c player.c

nutella.o: nutella.c
	gcc  -g -c nutella.c

playerlib.o: playerlib.c
	gcc -c playerlib.c

msock.o: msock.c
	gcc -c msock.c

clean:
	rm -f *.o nutella player
