default: dsh server

dsh: dsh.o
	gcc dsh.o -lcrypt -o dsh

dsh.o: dsh.c
	gcc -c dsh.c

server: server.o
	gcc -w server.o -lcrypt -o server

server.o: server.c
	gcc -w -c server.c

clean:
	rm -f *.o server dsh
