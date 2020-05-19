#Makefile for vrml2geo library for Windows (part of rd_vrml1)
#Tobias Schiebeck, 26/07/2002

!include ../../../include/xp_defs.mk

SRCDIR = $(XP_ROOT)\xp_comm_proj\rd_vrml\vrml2geo
LLIB=$(XP_LIB_DIR)\libVRML2GEO.a
ODIR=$(XP_LIB_DIR)\xp_comm_proj\rd_vrml

TARGET_SUFFIX=cpp
INCLUDE_DIR = -I$(SRCDIR)/qv/src -I$(SRCDIR)/qv/include -I$(SRCDIR)/include

CXX_OPTS = /nologo /MD /W3 /GR /GX /O2 /D "MSDOS" /D "_VISUALC_" /YX /FD /c

LIBOFILES = \
	$(ODIR)\gobject.obj \
	$(ODIR)\QvLib.obj \
	$(ODIR)\Traverse2.obj \
	$(ODIR)\TravUtil.obj \
	$(ODIR)\TravXForm.obj \
	$(ODIR)\TravTexture.obj \
	$(EMPTY)

default: $(LLIB) 

!include $(XP_MACHRULE_PATH)

clean:
	del $(LLIB) $(ODIR)\*.obj

$(ODIR)\gobject.obj: $(SRCDIR)\gobject.cpp
	$(COMPILE_CXX) $(CXX_OPTS) -c $(SRCDIR)/gobject.cpp $(INCLUDE_DIR) -Fo$*.obj
$(ODIR)\QvLib.obj: $(SRCDIR)\QvLib.cpp
	$(COMPILE_CXX) $(CXX_OPTS) -c $(SRCDIR)/QvLib.cpp $(INCLUDE_DIR) -Fo$*.obj
$(ODIR)\Traverse2.obj: $(SRCDIR)\Traverse2.cpp
	$(COMPILE_CXX) $(CXX_OPTS) -c $(SRCDIR)/Traverse2.cpp $(INCLUDE_DIR) -Fo$*.obj
$(ODIR)\TravTexture.obj: $(SRCDIR)\TravTexture.cpp
	$(COMPILE_CXX) $(CXX_OPTS) -c $(SRCDIR)/TravTexture.cpp $(INCLUDE_DIR) -Fo$*.obj
$(ODIR)\TravUtil.obj: $(SRCDIR)\TravUtil.cpp
	$(COMPILE_CXX) $(CXX_OPTS) -c $(SRCDIR)/TravUtil.cpp $(INCLUDE_DIR) -Fo$*.obj
$(ODIR)\TravXForm.obj: $(SRCDIR)\TravXForm.cpp
	$(COMPILE_CXX) $(CXX_OPTS) -c $(SRCDIR)/TravXForm.cpp $(INCLUDE_DIR) -Fo$*.obj
