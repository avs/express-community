#	@(#)Makefile	1.3	5/3/91

include ../../../../../include/xp_defs.mk

DSTDIR=		..
MANDIR=		$(DSTDIR)/man
INCDIR=		$(DSTDIR)/include
BINDIR=		$(DSTDIR)/test
LIBDIR=		../$(DSTDIR)/$(ARCH)/lib
LINTLIBDIR=	$(LIBDIR)/lint

CPPFLAGS=	-I$(INCDIR) -I../.. -D_NO_LONGLONG
PROJ_CFLAGS=	-O2 $(CPPFLAGS)
LINTFLAGS=	-hb

OBJECTS= 	decompoptimise.o \
		huffcoptimisebug.o \
		huffdoptimise.o \
		jpegutil.o \
		mcu.o \
		pmreadoptimise.o \
		pnmtoljpoptimisebug.o \
		predict.o \
		read.o \
		write.o

LIBRARY=	libJpglless.a

#OBJECTS=	$(SOURCES:%.c=$(ARCH)/%.o)

$(LIBDIR)/$(LIBRARY): $(OBJECTS)
		ar rv $@ $(OBJECTS)

#lint:		$(SOURCES)
#		$(LINT.c) $(SOURCES)


#createdir:	
#		test -d $(ARCH) || mkdir -p $(ARCH)
#		test -d $(DSTDIR)/$(ARCH)/lib || mkdir -p $(DSTDIR)/$(ARCH)/lib

clean:
		-rm -Rf $(ARCH) *.o

.KEEP_STATE:
.INIT:	$(LIBDIR) $(ARCH)
$(LIBDIR) $(ARCH) :
		test -d $@ || mkdir -p $@

#$(ARCH)/%.o: %.c
#		$(COMPILE.c) $< -o $@

