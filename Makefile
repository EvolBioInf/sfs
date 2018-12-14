all:
	make -C common
	make -C srcMs2sfs
	mkdir -p build
	cp srcMs2sfs/ms2sfs  build
	make -C srcBootSfs
	cp srcBootSfs/bootSfs build
test:
	make -C srcMs2sfs test
	make -C srcBootSfs test
clean:
	make -C srcMs2sfs clean
	make -C srcBootSfs clean
	make -C common clean
.PHONY:	doc
doc:	
	make -C doc
