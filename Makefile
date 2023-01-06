CC = gcc
CFLAGS  = -g -Wall

nxs:
	cc src/*.c $(CFLAGS) -o nxs

clean:
	-rm nxs

install:
	cp nxs /usr/local/bin/

uninstall:
	-rm /usr/local/bin/nxs
