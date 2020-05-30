all: brbtimer.o
	cc brbtimer.o -o brbtimer -lallegro -lallegro_main -lallegro_image

brbtimer.o: brbtimer.c brbtimer.h
	cc -c brbtimer.c

clean:
	rm -f -- *.o brbtimer
