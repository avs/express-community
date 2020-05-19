#		"@(#)$RCSfile: nt.mak,v $ $Revision: 1.6 $ $AVS$ $Date: 1998/01/22 15:57:24 $"
#			Copyright (c) 1994 by
#			Advanced Visual Systems Inc.
#			All Rights Reserved
#	
#	This software comprises unpublished confidential information of
#	Advanced Visual Systems Inc. and may not be used, copied or made
#	available to anyone, except in accordance with the license
#	under which it is furnished.
#	
#	

!include ../../../include/xp_defs.mk

SRCDIR = c:\iavsc\xp_comm_proj\metars\libMETARS
LLIB=$(XP_LIB_DIR)\libMETARS.a
ODIR=$(XP_LIB_DIR)\Utility\Modules\Metars\libMETARS
PROJ_INCLUDES=-I$(SRCDIR) -I$(XP_ROOT)\Utility\Modules\GFC

LIBOFILES = $(ODIR)\METARS.obj \
	    $(ODIR)\Dcdmetar.obj \
	    $(ODIR)\Antoi.obj \
	    $(ODIR)\Charcmp.obj \
	    $(ODIR)\Dcdmtrmk.obj \
	    $(ODIR)\Fracpart.obj \
	    $(ODIR)\Prtdmetr.obj \
	    $(ODIR)\Stspack2.obj \
	    $(ODIR)\Stspack3.obj \
	    $(ODIR)\METARSccbridge.obj \
	    $(ODIR)\METARSStation.obj \
	    $(ODIR)\METARSStationChoosen.obj \
            $(EMPTY)

default: $(LLIB)

clean:
	del $(LLIB) $(ODIR)\*.obj

!include $(XP_MACHRULE_PATH)

INCLUDES = $(SRCDIR)\local.h \
	   $(SRCDIR)\metar.h \
	   $(SRCDIR)\METARS.h \
	   $(SRCDIR)\METARSStation.h \
	   $(SRCDIR)\METARSStationChoosen.h \
         $(SRCDIR)\GFC.h \
         $(EMPTY)

$(ODIR)\Antoi.obj: $(SRCDIR)\Antoi.c $(INCLUDES)
$(ODIR)\Charcmp.obj: $(SRCDIR)\Charcmp.c $(INCLUDES)
$(ODIR)\Dcdmetar.obj: $(SRCDIR)\Dcdmetar.c $(INCLUDES)
$(ODIR)\Dcdmtrmk.obj: $(SRCDIR)\Dcdmtrmk.c $(INCLUDES)
$(ODIR)\Fracpart.obj: $(SRCDIR)\Fracpart.c $(INCLUDES)
$(ODIR)\Prtdmetr.obj: $(SRCDIR)\Prtdmetr.c $(INCLUDES)
$(ODIR)\Stspack2.obj: $(SRCDIR)\Stspack2.c $(INCLUDES)
$(ODIR)\Stspack3.obj: $(SRCDIR)\Stspack3.c $(INCLUDES)
$(ODIR)\METARSccbridge.obj: $(SRCDIR)\METARSccbridge.c $(INCLUDES)
$(ODIR)\METARS.obj: $(SRCDIR)\METARS.cxx $(INCLUDES)
$(ODIR)\METARSStation.obj: $(SRCDIR)\METARSStation.cxx $(INCLUDES)
$(ODIR)\METARSStationChoosen.obj: $(SRCDIR)\METARSStationChoosen.cxx $(INCLUDES)
