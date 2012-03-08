LDFLAGS= -L. -ltimemachine 
CPPFLAGS=
CFLAGS= -O0 -g -Wall -Werror -fPIC
CC=gcc
CXXTESTGEN=cxxtestgen.py

all:	libtimemachine.so show_time test_timemachine

timemachine.o: timemachine.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

libtimemachine.so:	timemachine.o
	$(CC) -lc -shared -Wl,-soname,$@ -o $@ $<

show_time: show_time.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -ldl -lrt

test_timemachine:   test_timemachine.c timemachine.c libtimemachine.so
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -L. -ltimemachine -ldl -lrt -lcheck -Wl,-rpath,.

check:  test_timemachine
	./test_timemachine

clean:
	rm -f timemachine.o libtimemachine.so show_time test_timemachine
