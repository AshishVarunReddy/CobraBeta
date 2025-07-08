cc=gcc
source=comp.c lexerf.c
output=comp
cflags=-Wall --static -g

comp.o : comp.c lexerf.h
	$(cc)  -c comp.c

lexerf.o : lexerf.c lexerf.h
	$(cc)  -c lexerf.c

cobra : comp.o lexerf.o
	$(cc) $(cflags) comp.o lexerf.o -o cobra


clean : comp.o lexerf.o
	rm *.o
