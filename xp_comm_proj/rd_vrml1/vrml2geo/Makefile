#Makefile for vrml2geo library (part of rd_vrml1)
#Tobias Schiebeck, 26/07/2002

include ../../../include/xp_defs.mk

LLIB=$(XP_LIB_DIR)/libVRML2GEO.a
ODIR=$(XP_LIB_DIR)/xp_comm_proj/rd_vrml1

default: $(LLIB)

${XP_LIB_DIR}/xp_comm_proj/rd_vrml1 :
	mkdir ${XP_LIB_DIR}/xp_comm_proj
	mkdir ${XP_LIB_DIR}/xp_comm_proj/rd_vrml1

SRCDIR=$(XP_ROOT)/xp_comm_proj/rd_vrml1/vrml2geo
INCLUDE_DIR = -I$(SRCDIR)/qv/src -I$(SRCDIR)/qv/include -I$(SRCDIR)/include

LIBOFILES = \
	 $(ODIR)/gobject.o \
	 $(ODIR)/QvLib.o \
	 $(ODIR)/Traverse2.o \
	 $(ODIR)/TravUtil.o \
	 $(ODIR)/TravXForm.o \
	 $(ODIR)/TravTexture.o \
	 $(EMPTY)

include $(XP_MACHRULE_PATH)
	 
$(ODIR)/gobject.o: $(SRCDIR)/gobject.cpp
	$(COMPILE_CXX) $(CXX_OPTS) -c $(SRCDIR)/gobject.cpp $(INCLUDE_DIR) -o $*.o
$(ODIR)/QvLib.o: $(SRCDIR)/QvLib.cpp
	$(COMPILE_CXX) $(CXX_OPTS) -c $(SRCDIR)/QvLib.cpp $(INCLUDE_DIR) -o $*.o
$(ODIR)/Traverse2.o: $(SRCDIR)/Traverse2.cpp
	$(COMPILE_CXX) $(CXX_OPTS) -c $(SRCDIR)/Traverse2.cpp $(INCLUDE_DIR) -o $*.o
$(ODIR)/TravTexture.o: $(SRCDIR)/TravTexture.cpp
	$(COMPILE_CXX) $(CXX_OPTS) -c $(SRCDIR)/TravTexture.cpp $(INCLUDE_DIR) -o $*.o
$(ODIR)/TravUtil.o: $(SRCDIR)/TravUtil.cpp
	$(COMPILE_CXX) $(CXX_OPTS) -c $(SRCDIR)/TravUtil.cpp $(INCLUDE_DIR) -o $*.o
$(ODIR)/TravXForm.o: $(SRCDIR)/TravXForm.cpp
	$(COMPILE_CXX) $(CXX_OPTS) -c $(SRCDIR)/TravXForm.cpp $(INCLUDE_DIR) -o $*.o

$(LLIB): $(LIBOFILES)
	ar -r $(LLIB) $(LIBOFILES)
