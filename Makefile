LDFLAGS= -L. -ltimemachine 
CPPFLAGS=-I.
CFLAGS= -O0 -g -Wall -Werror -fPIC
CC=gcc
CXXTESTGEN=cxxtestgen.py

all: libtimemachine.so show_time

test: test_timemachine

timemachine.o: timemachine.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

libtimemachine.so: timemachine.o
	$(CC) -lc -shared -Wl,-soname,$@ -o $@ $<
	${MAKE} show_time check

show_time: show_time.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -ldl -lrt

test_timemachine: test_timemachine.c timemachine.c timemachine.h minunit.h libtimemachine.so
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -L. -ltimemachine -ldl -lrt -Wl,-rpath,.

check:  test_timemachine
	./test_timemachine

clean:
	rm -f timemachine.o libtimemachine.so show_time test_timemachine
	rm -f *~

help:
	@echo ""
	@echo "make: builds .so, ./show_time, and builds and runs tests"
	@echo "make test: builds unit tests"
	@echo "make check: builds and runs unit tests"
	@echo ""