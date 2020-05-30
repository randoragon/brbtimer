all: brbtimer.o event.o
	cc brbtimer.o event.o -o brbtimer -lallegro

brbtimer.o: brbtimer.c 
	cc -c brbtimer.c

event.o: event.h event.c
	cc -c event.c

clean:
	rm -f -- *.o brbtimer
