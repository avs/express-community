#Makefile for liblea library for Windows
#Tobias Schiebeck, 03/03/2003

!include ../../include/xp_defs.mk

LIBDIR=../../lib/pc

LLIB=$(LIBDIR)/liblea.a
ODIR=$(LIBDIR)/xp_comm_proj/lea

CXX_OPTS = /nologo /TP /MD /W3 /GR /GX /O2 /D "MSDOS" /D "_VISUALC_" /YX /FD /c

LIBOFILES = \
	 $(ODIR)/advection.obj \
	 $(ODIR)/advection_master.obj \
	 $(ODIR)/alic.obj \
	 $(ODIR)/intensity_mask.obj \
	 $(EMPTY)

default: $(LLIB)
		
#$(ODIR) :
#	mkdir $(LIBDIR)/xp_comm_proj;mkdir $(LIBDIR)/xp_comm_proj/lea
	
SRCDIR=lea_lib-1.0/src
INCLUDE_DIR = -I$(SRCDIR) -Iinclude

!include $(XP_MACHRULE_PATH)

clean:
	del $(LLIB) $(ODIR)\*.obj
	 
$(ODIR)/advection.obj: $(SRCDIR)/advection.cxx
	$(COMPILE_CXX) $(CXX_OPTS) -c $(SRCDIR)/advection.cxx $(INCLUDE_DIR) -Fo$*.obj
$(ODIR)/advection_master.obj: $(SRCDIR)/advection_master.cxx
	$(COMPILE_CXX) $(CXX_OPTS) -c $(SRCDIR)/advection_master.cxx $(INCLUDE_DIR) -Fo$*.obj
$(ODIR)/alic.obj: $(SRCDIR)/alic.cxx
	$(COMPILE_CXX) $(CXX_OPTS) -c $(SRCDIR)/alic.cxx $(INCLUDE_DIR) -Fo$*.obj
$(ODIR)/intensity_mask.obj: $(SRCDIR)/intensity_mask.cxx
	$(COMPILE_CXX) $(CXX_OPTS) -c $(SRCDIR)/intensity_mask.cxx $(INCLUDE_DIR) -Fo$*.obj
