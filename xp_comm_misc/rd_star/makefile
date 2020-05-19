#
# make file just to clean up stuff
#
all: build

build:
	echo Building Module
	base -gen_proc express -exit
	$(MAKE) -f express.mk

clean:
	rm -f AUTOSA*
	rm -f *user*
	rm -f *express*
	rm -f core
	rm -f src/*.obj
	rm -f src/*.o
	rm -f src/*.vo
	rm -f v/*.vo
	rm -rf bin
	rm -rf lib
	rm -rf include
	
archive:
	(cd ..;  \
	rm -f reader.tar ; \
	tar vcf reader.tar \
		star/apps \
		star/avsenv \
		star/README.txt \
		star/src/postconvert_lib.f \
		star/src/readStar.f \
		star/src/low.v star/src/high.v \
		star/src/README \
		star/logo.x \
		star/supersonicSolid \
		star/transExample \
		star/makefile \
		star/v \
	)
