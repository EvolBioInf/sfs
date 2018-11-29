all:
	make -C srcMs2sfs
	mkdir -p build
	mv srcMs2sfs/ms2sfs  build
	make -C srcBootSfs
	mv srcBootSfs/bootSfs build
test:
	make -C srcMs2sfs test
	make -C srcBootSfs test
clean:
	make -C srcMs2sfs clean
	make -C srcBootSfs clean
