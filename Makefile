all: brbtimer.o
	cc brbtimer.o -O2 -o brbtimer -lallegro -lallegro_main -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives

debug: brbtimer.o.debug
	cc brbtimer.o -o brbtimer -lallegro -lallegro_main -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives

install: all
	mkdir -p /usr/local/bin
	cp -f ./brbtimer /usr/local/bin
	chmod 755 /usr/local/bin/brbtimer

brbtimer.o: brbtimer.c
	cc -c -O2 brbtimer.c

brbtimer.o.debug: brbtimer.c
	cc -g -c brbtimer.c

clean:
	rm -f -- *.o brbtimer
