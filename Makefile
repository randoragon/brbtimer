all: brbtimer.o
	cc brbtimer.o -o brbtimer

brbtimer.o: brbtimer.c
	cc -c brbtimer.c

clean:
	rm -f -- *.o brbtimer
