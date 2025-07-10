cc=gcc
source=comp.c lexerf.c
output=comp
cflags=-Wall --static -g

comp.o : comp.c lexerf.h
	$(cc)  -c comp.c

lexerf.o : lexerf.c lexerf.h
	$(cc)  -c lexerf.c

parserf.o : parserf.c parserf.h
	$(cc)  -c parserf.c

cobra : comp.o lexerf.o parserf.o
	$(cc) $(cflags) comp.o lexerf.o parserf.o -o cobra


clean : comp.o lexerf.o
	rm *.o
