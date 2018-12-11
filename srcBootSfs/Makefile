CC=gcc
CFLAGS= -O3 -Wall -Wshadow -pedantic -std=gnu99 -g
# The source files, object files, libraries and executable name.
SRCFILES= bootSfs.c interface.c eprintf.c gsl_rng.c tab.c sfs.c
OBJFILES= bootSfs.o interface.o eprintf.o gsl_rng.o tab.o sfs.o
LIBS= -lm -lgsl -lblas
EXECFILE=bootSfs
DIRECTORY=bootSfs
VERSION=0.1
# The make rule for the executable
.PHONY : all
all : $(EXECFILE)
$(EXECFILE) : $(OBJFILES)

	$(CC) $(CFLAGS) -o $(EXECFILE) $(OBJFILES) $(LIBS)
interface.o: interface.h
eprintf.o: eprintf.h

doc:
	cd ../Doc; make clean; make pdf; cd ../$(DIRECTORY)_$(VERSION)

# Other Standard make rules
lint : 
	lint $(SRCFILES) | more
clean:
	rm -f *.o *~
tarfile:
	cd ../Doc; make clean; make pdf; cd ../$(DIRECTORY)_$(VERSION)
	mkdir $(DIRECTORY)_$(VERSION)
	cp -rf $(SRCFILES) *.h  README COPYING Makefile ../Doc/sfs2sfsDoc.pdf $(DIRECTORY)_$(VERSION)
	tar cvzfh $(EXECFILE)_$(VERSION).tgz $(DIRECTORY)_$(VERSION)
	mv $(EXECFILE)_$(VERSION).tgz ../
	/bin/rm -r $(DIRECTORY)_$(VERSION)

test:
	bash ../scripts/testBootSfs.sh