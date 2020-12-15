CFLAGS=-I/usr/X11R6/include
LDFLAGS=-L/usr/X11R6/lib -lxcb -lxcb-keysyms
CC=cc

all:V: xkl xkd

xkl: xkl.c
	$CC $CFLAGS $LDFLAGS -o xkl parse.c dict.c xkl.c

xkd: xkd.c
	$CC $CFLAGS $LDFLAGS -o xkd dict.c xkd.c

clean:V:
	rm -f xkl
	rm -f xkd
	rm -f xkl.core
	rm -f xkd.core
