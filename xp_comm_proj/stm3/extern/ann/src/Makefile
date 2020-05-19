#-----------------------------------------------------------------------------
# Makefile for core library
#
# ANN: Approximate Nearest Neighbors
# Version: 0.1 (Beta release)
#-----------------------------------------------------------------------------
# Revision 0.1  03/04/98
#	Initial release
#-----------------------------------------------------------------------------

#-----------------------------------------------------------------------------
# Some basic definitions:
#		BASEDIR		where include, src, lib, ... are
#		INCLIB		include directory
#		LIBLIB		library directory
#-----------------------------------------------------------------------------
BASEDIR	= ..
INCDIR	= $(BASEDIR)/include
LIBDIR	= $(BASEDIR)/lib

SOURCES = ANN.cxx brute.cxx kd_tree.cxx kd_util.cxx kd_split.cxx \
	kd_search.cxx kd_pr_search.cxx bd_tree.cxx bd_search.cxx \
	bd_pr_search.cxx perf.cxx

HEADERS = kd_tree.h kd_split.h kd_util.h kd_search.h \
	kd_pr_search.h perf.h pr_queue.h pr_queue_k.h

OBJECTS = $(SOURCES:.cxx=.o)

#-----------------------------------------------------------------------------
# Rules
#-----------------------------------------------------------------------------

.cxx.o:
	$(C++) -c -I$(INCDIR) $(CFLAGS) $(CFLAGS2) $<

#-----------------------------------------------------------------------------
# Make the library
#-----------------------------------------------------------------------------

default:
	@echo "Specify a target configuration"

targets: $(LIBDIR)/$(ANNLIB)

$(LIBDIR)/$(ANNLIB): $(OBJECTS)
	$(MAKELIB) $(ANNLIB) $(OBJECTS)
	$(RANLIB) $(ANNLIB)
	mv $(ANNLIB) $(LIBDIR)

#-----------------------------------------------------------------------------
# Make object files
#-----------------------------------------------------------------------------

ANN.o: ANN.cxx
	$(C++) -c -I$(INCDIR) $(CFLAGS) $(CFLAGS2) ANN.cxx

brute.o: brute.cxx
	$(C++) -c -I$(INCDIR) $(CFLAGS) $(CFLAGS2) brute.cxx

kd_tree.o: kd_tree.cxx
	$(C++) -c -I$(INCDIR) $(CFLAGS) $(CFLAGS2) kd_tree.cxx

kd_util.o: kd_util.cxx
	$(C++) -c -I$(INCDIR) $(CFLAGS) $(CFLAGS2) kd_util.cxx

kd_split.o: kd_split.cxx
	$(C++) -c -I$(INCDIR) $(CFLAGS) $(CFLAGS2) kd_split.cxx

kd_search.o: kd_search.cxx
	$(C++) -c -I$(INCDIR) $(CFLAGS) $(CFLAGS2) kd_search.cxx

kd_pr_search.o: kd_pr_search.cxx
	$(C++) -c -I$(INCDIR) $(CFLAGS) $(CFLAGS2) kd_pr_search.cxx

bd_tree.o: bd_tree.cxx
	$(C++) -c -I$(INCDIR) $(CFLAGS) $(CFLAGS2) bd_tree.cxx

bd_search.o: bd_search.cxx
	$(C++) -c -I$(INCDIR) $(CFLAGS) $(CFLAGS2) bd_search.cxx

bd_pr_search.o: bd_pr_search.cxx
	$(C++) -c -I$(INCDIR) $(CFLAGS) $(CFLAGS2) bd_pr_search.cxx

perf.o: perf.cxx
	$(C++) -c -I$(INCDIR) $(CFLAGS) $(CFLAGS2) perf.cxx

#-----------------------------------------------------------------------------
# Configuration definitions
#-----------------------------------------------------------------------------

include ../Make-config

#-----------------------------------------------------------------------------
# Cleaning
#-----------------------------------------------------------------------------

clean:
	-rm -f *.o core

realclean: clean

