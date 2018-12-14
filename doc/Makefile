TAG     := $(shell git describe | sed 's/-.*//')
DATE    := $(shell git log -1 --format=%ai $(TAG) | sed 's/-/ /g' | awk '{printf "\\\\DTMdisplaydate{%s}{%s}{%s}{-1}\n", $$1, $$2, $$3}')
VERSION := $(shell git describe | sed 's/^[vV]//; s/-.*//')
all:
	echo $(VERSION) > version.tex
	echo $(DATE)    > date.tex
	latex sfs
	bibtex sfs
	latex sfs
	latex sfs
	dvipdf sfs
clean:	
	rm -f epos.pdf *.log *.bbl *.blg *.dvi
