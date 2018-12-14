CC=gcc
export VERSION = $(shell bash ../scripts/getVersion.sh) 
export DATE    = $(shell bash ../scripts/getDate.sh) 
CFLAGS = -DVERSION=\"$(VERSION)\" -DDATE=\"$(DATE)\" -O3 -Wall -Wshadow -pedantic -std=gnu99 -g -I../common/ -L../common/ #-pg
# The source files, object files, libraries and executable name.
SRCFILES= ms2sfs.c interface.c sfs.c
OBJFILES= ms2sfs.o interface.o sfs.o
LIBS= -lm -lcommon
EXECFILE=ms2sfs
DIRECTORY=ms2sfs
# The make rule for the executable
.PHONY : all
all : $(EXECFILE)
$(EXECFILE) : $(OBJFILES)
	$(CC) $(CFLAGS) -o $(EXECFILE) $(OBJFILES) $(LIBS)
$(EXECFILE).o: interface.h eprintf.h
interface.o: interface.h
eprintf.o: eprintf.h

# Other Standard make rules
lint : 
	lint $(SRCFILES) | more
clean:
	rm -f *.o *~
test:
	bash ../scripts/testMs2sfs.sh
