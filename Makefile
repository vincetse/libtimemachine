LDFLAGS=
CPPFLAGS= -fPIC -Wall
CFLAGS= -O0 -g
CC=gcc

all:	libtimemachine.so show_time

timemachine.o: timemachine.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

libtimemachine.so:	timemachine.o
	$(CC) -lc -shared -Wl,-soname,$@ -o $@ $<

show_time: show_time.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -ldl -lrt

clean:
	rm -f timemachine.o libtimemachine.so show_time
