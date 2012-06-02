
# Linux default
CC=gcc
SONAME=so
RT=-lrt
SHARED=-shared -Wl,-soname,libtimemachine.so
LDFLAGS= -L. -ltimemachine
CPPFLAGS=-I.
CFLAGS= -O0 -g -Wall -Werror -fPIC -fno-common

# Apple over-rides
#   Trying to avoid using libtool
#
APPLE=$(shell uname -a | grep -c Darwin)
ifeq ($(APPLE),1)
	SONAME=dylib
	RT=
	SHARED=-dynamiclib -flat_namespace
endif


all: libtimemachine.$(SONAME) show_time

test: test_timemachine

timemachine.o: timemachine.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

libtimemachine.$(SONAME): timemachine.o
	$(CC) -lc $(SHARED) -o $@ $<
	${MAKE} show_time check

show_time: show_time.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -ldl $(RT)

test_timemachine: test_timemachine.c timemachine.c timemachine.h minunit.h libtimemachine.$(SONAME)
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -o $@ -L. -ltimemachine -ldl $(RT) -Wl,-rpath,.

check:  test_timemachine
	./test_timemachine

clean:
	rm -f timemachine.o libtimemachine.$(SONAME) show_time test_timemachine
	rm -f *~
	rm -rf *.dSYM

help:
	@echo ""
	@echo "make: builds .so, ./show_time, and builds and runs tests"
	@echo "make test: builds unit tests"
	@echo "make check: builds and runs unit tests"
	@echo ""
