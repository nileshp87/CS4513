default: rm dv dump
experiments: timelink
timelink: timelink.c
	gcc timelink.c -o timelink
rm: rm.o
	gcc rm.o -o rm
rm.o: rm.c
	gcc -c rm.c
dv: dv.o
	gcc dv.o -o dv
dv.o: dv.c
	gcc -c dv.c
dump: dump.o
	gcc dump.o -o dump
dump.o: dump.c
	gcc -c dump.c
clean:
	/bin/rm -f *.o
	/bin/rm -f rm dump dv
