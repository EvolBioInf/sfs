TAG     := $(shell git describe | sed 's/-.*//')
DATE    := $(shell git log -1 --format=%ai $(TAG) | sed 's/-/ /g' | awk '{printf "\\\\DTMdisplaydate{%s}{%s}{%s}{-1}\n", $$1, $$2, $$3}')
VERSION := $(shell git describe | sed 's/^[vV]//; s/-.*//')
all: sfs.pdf
sfs.pdf: sfs.tex ref.bib
	echo $(VERSION) > version.tex
	echo $(DATE)    > date.tex
	latex sfs
	bibtex sfs
	latex sfs
	latex sfs
	dvipdf sfs
publish: sfs.pdf
	if [ -d /mnt/c/Users/haubold/ownCloud\ -\ haubold@evolbio.mpg.de@owncloud.gwdg.de/docs/ ]; then \
		cp sfs.pdf /mnt/c/Users/haubold/ownCloud\ -\ haubold@evolbio.mpg.de@owncloud.gwdg.de/docs/; \
	fi
clean:	
	rm -f sfs.pdf *.log *.bbl *.blg *.dvi
