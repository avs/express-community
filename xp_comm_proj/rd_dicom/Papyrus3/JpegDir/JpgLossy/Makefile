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


OBJECTS= 	jcapimin.o \
		jcapistd.o \
		jccoefct.o \
		jccolor.o \
		jcdctmgr.o \
		jchuff.o \
		jcinit.o \
		jcmainct.o \
		jcmaster.o \
		jcmarker.o \
		jcomapi.o \
		jcparam.o \
		jcphuff.o \
		jcprepct.o \
		jcsample.o \
		jdapimin.o \
		jdapistd.o \
		jdatadst.o \
		jdatasrc.o \
		jdcoefct.o \
		jdcolor.o \
		jddctmgr.o \
		jdhuff.o \
		jdinput.o \
		jdmainct.o \
		jdmarker.o \
		jdmaster.o \
		jdmerge.o \
		jdphuff.o \
		jdpostct.o \
		jdsample.o \
		jerror.o \
		jfdctint.o \
		jfdctflt.o \
		jfdctfst.o \
		jidctflt.o \
		jidctfst.o \
		jidctint.o \
		jidctred.o \
		jmemmgr.o \
		jmemnobs.o \
		jquant1.o \
		jquant2.o \
		jutils.o \
		rdcolmap.o


LIBRARY=	libJpglossy.a

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
.INIT:   $(LIBDIR) $(ARCH)
$(LIBDIR) $(ARCH) :
		test -d $@ || mkdir -p $@

#$(ARCH)/%.o: %.c
#		$(COMPILE.c) $< -o $@



