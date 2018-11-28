all:
	(cd ms2sfs;  make all; mkdir -p ../build; mv ms2sfs  ../build)
	(cd bootSfs; make all; mkdir -p ../build; mv bootSfs ../build)
test:
	(cd ms2sfs;  make test)
	(cd bootSfs; make test)
clean:
	(cd ms2sfs;  make clean)
	(cd bootSfs; make clean)
