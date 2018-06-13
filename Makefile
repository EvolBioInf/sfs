CC=gcc
CFLAGS= -O3 -Wall -Wshadow -pedantic -std=gnu99 -g
# The source files, object files, libraries and executable name.
SRCFILES= sfs.c interface.c eprintf.c sample.c spectrum.c ran.c sequence_data.c stringUtil.c
OBJFILES= sfs.o interface.o eprintf.o sample.c spectrum.o ran.o sequence_data.o stringUtil.o
LIBS= -lm
EXECFILE=sfs
DIRECTORY=Sfs
# The make rule for the executable
.PHONY : all
all : $(EXECFILE)
$(EXECFILE) : $(OBJFILES)
	$(CC) $(CFLAGS) -o $(EXECFILE) $(OBJFILES) $(LIBS)
$(EXECFILE).o: interface.h eprintf.h
interface.o: interface.h
eprintf.o: eprintf.h

doc:
	cd ../Doc; make clean; make pdf; cd ../$(DIRECTORY)_$(VERSION)

# Other Standard make rules
lint : 
	lint $(SRCFILES) | more
clean:
	rm -f *.o *~
