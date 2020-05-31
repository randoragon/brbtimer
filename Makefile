all: brbtimer.o
	cc brbtimer.o -o brbtimer -lallegro -lallegro_main -lallegro_image -lallegro_font -lallegro_ttf

debug: brbtimer.o.debug
	cc brbtimer.o -o brbtimer -lallegro -lallegro_main -lallegro_image

brbtimer.o: brbtimer.c brbtimer.h
	cc -c brbtimer.c

brbtimer.o.debug: brbtimer.c brbtimer.h
	cc -g -c brbtimer.c

clean:
	rm -f -- *.o brbtimer
