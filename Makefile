all: brbtimer.o
	cc brbtimer.o -O2 -o brbtimer -lallegro -lallegro_main -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives

debug: brbtimer.o.debug
	cc brbtimer.o -o brbtimer -lallegro -lallegro_main -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives

install: all
	mkdir -p -- /usr/local/bin
	ln -sfT -- "$(shell pwd)/brbtimer" /usr/local/bin/brbtimer
	chmod 755 -- /usr/local/bin/brbtimer
	mkdir -p -- /usr/share/man/man1
	cp -- brbtimer.1 /usr/share/man/man1/brbtimer.1
	chmod 644 -- /usr/share/man/man1/brbtimer.1

uninstall:
	rm -f -- /usr/local/bin/brbtimer
	rm -f -- /usr/share/man/man1/brbtimer.1

brbtimer.o: brbtimer.c
	cc -c -O2 brbtimer.c

brbtimer.o.debug: brbtimer.c
	cc -g -c brbtimer.c

clean:
	rm -f -- *.o brbtimer
