cc=clang
source=comp.c lexerf.c
output=comp
cflags=-Wall --static -g

comp.o : comp.c lexerf.h
	$(cc)  -c comp.c

lexerf.o : lexerf.c lexerf.h
	$(cc)  -c lexerf.c

parserf.o : parserf.c parserf.h
	$(cc)  -c parserf.c

codegeneratorf.o : codegeneratorf.c codegeneratorf.h
	$(cc) -c codegeneratorf.c

stackf.o : stackf.c stackf.h
	$(cc) -c stackf.c

hashtable.o : hashtable.c hashtable.h
	$(cc) -c hashtable.c

cobra : comp.o lexerf.o parserf.o codegeneratorf.o stackf.o hashtable.o
	$(cc) $(cflags) comp.o lexerf.o parserf.o codegeneratorf.o stackf.o hashtable.o -o cobra


clean : comp.o lexerf.o parserf.o codegeneratorf.o stackf.o hashtable.o
	rm *.o
	$(RM) -r cobra assembly/genra output* *dSYM
