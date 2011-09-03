
##### Configurable options:

## Compiler:
CC=gcc
#CC=cc
CPP=g++

## Compiler flags:

# GCC:  (also -march=pentium etc, for machine-dependent optimizing)
CFLAGS=-Wall -O3 -fomit-frame-pointer -funroll-loops
LDFLAGS=-L/usr/local/lib
LIBS=-lopenbabel
CPPFLAGS=$(CFLAGS) -I/usr/local/include/openbabel-2.0/

# GCC w/ debugging:
#CFLAGS=-Wall -g -DINLINE=

# Compaq C / Digital C:
#CFLAGS=-arch=host -fast

# SunOS:
#CFLAGS=-fast

## Program options:

# Enable long options for cl (eg. "cl --help"), comment out to disable.
# Requires getopt_long(3)  (a GNU extension)
LONGOPTS = -DENABLE_LONG_OPTIONS


##### End of configurable options


#all: cl mcs
all: mcs


mcs: mcs.o cliquer.o graph.o reorder.o
	$(CPP) $(LDFLAGS) -o $@ mcs.o cliquer.o graph.o reorder.o $(LIBS)

mcs.o: mcs.cpp
	$(CPP) $(CPPFLAGS) -o $@ -c $<
	
cliquer.o graph.o reorder.o: cliquer.h set.h graph.h misc.h reorder.h Makefile cliquerconf.h

clean:
	rm -f *.o *~ mcs

