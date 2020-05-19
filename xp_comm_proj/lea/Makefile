#Makefile for liblea library
#Tobias Schiebeck, 03/03/2003

include ../../include/xp_defs.mk

LIBDIR=../../lib/$(MACHINE)

LLLIB=$(LIBDIR)/liblea.a
ODIR=$(LIBDIR)/xp_comm_proj/lea

default: $(LLLIB)

${ODIR} :
	mkdir ${LIBDIR}/xp_comm_proj
	mkdir ${LIBDIR}/xp_comm_proj/lea

SRCDIR=lea_lib-1.0/src
INCLUDE_DIR = -I$(SRCDIR) -Iinclude

include $(XP_MACHRULE_PATH)

LIBOFILES = \
	 $(ODIR)/advection.o \
	 $(ODIR)/advection_master.o \
	 $(ODIR)/alic.o \
	 $(ODIR)/intensity_mask.o \
	 $(EMPTY)

$(ODIR)/advection.o: $(SRCDIR)/advection.cxx
	$(CXX) $(CXX_OPTS) $(MACH_CXXFLAGS) -c $(SRCDIR)/advection.cxx $(INCLUDE_DIR)  -o$@
$(ODIR)/advection_master.o: $(SRCDIR)/advection_master.cxx
	$(CXX) $(CXX_OPTS) $(MACH_CXXFLAGS) -c $(SRCDIR)/advection_master.cxx $(INCLUDE_DIR) -o$@
$(ODIR)/alic.o: $(SRCDIR)/alic.cxx
	$(CXX) $(CXX_OPTS) $(MACH_CXXFLAGS) -c $(SRCDIR)/alic.cxx $(INCLUDE_DIR) -o$@
$(ODIR)/intensity_mask.o: $(SRCDIR)/intensity_mask.cxx
	$(CXX) $(CXX_OPTS) $(MACH_CXXFLAGS) -c $(SRCDIR)/intensity_mask.cxx $(INCLUDE_DIR) -o$@
$(LLLIB): $(LIBOFILES)
	ar -r $(LLLIB) $(LIBOFILES)
