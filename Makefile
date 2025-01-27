CC ?= gcc
LDFLAGS := -lallegro -lallegro_main -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives
TARGET := brbtimer

.PHONY: all debug brbtimer.o.debug install uninstall clean

all: brbtimer.o
	$(CC) -O2 -o $(TARGET) $(LDFLAGS) brbtimer.o

debug: brbtimer.o.debug
	$(CC) -g -Og -o $(TARGET) $(LDFLAGS) brbtimer.o

brbtimer.o: brbtimer.c
	cc -c -O2 brbtimer.c

brbtimer.o.debug: brbtimer.c
	cc -g -c brbtimer.c

install: all
	mkdir -p -- /usr/local/bin
	ln -sfT -- "$(shell pwd)/$(TARGET)" /usr/local/bin/$(TARGET)
	chmod 755 -- /usr/local/bin/$(TARGET)
	mkdir -p -- /usr/share/man/man1
	cp -- brbtimer.1 /usr/share/man/man1/brbtimer.1
	chmod 644 -- /usr/share/man/man1/brbtimer.1

uninstall:
	rm -f -- /usr/local/bin/$(TARGET)
	rm -f -- /usr/share/man/man1/brbtimer.1

clean:
	rm -f -- *.o $(TARGET)
