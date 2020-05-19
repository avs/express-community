# Microsoft Developer Studio Generated NMAKE File, Based on Jlossy.dsp
!IF "$(CFG)" == ""
CFG=Jpglossy - Win32 Release
!MESSAGE No configuration specified. Defaulting to Jpglossy - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "Jpglossy - Win32 Release" && "$(CFG)" != "Jpglossy - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Jlossy.mak" CFG="Jpglossy - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Jpglossy - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Jpglossy - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Jpglossy - Win32 Release"

OUT1DIR=..\..\PC
OUTDIR=..\..\PC\lib
INTDIR=..\..\PC\lib
# Begin Custom Macros
OutDir=..\..\PC\lib
# End Custom Macros

ALL : "$(OUTDIR)\Jlossy.lib"


CLEAN :
	-@erase "$(INTDIR)\jcapimin.obj"
	-@erase "$(INTDIR)\jcapistd.obj"
	-@erase "$(INTDIR)\jccoefct.obj"
	-@erase "$(INTDIR)\jccolor.obj"
	-@erase "$(INTDIR)\jcdctmgr.obj"
	-@erase "$(INTDIR)\jchuff.obj"
	-@erase "$(INTDIR)\jcinit.obj"
	-@erase "$(INTDIR)\jcmainct.obj"
	-@erase "$(INTDIR)\jcmarker.obj"
	-@erase "$(INTDIR)\jcmaster.obj"
	-@erase "$(INTDIR)\jcomapi.obj"
	-@erase "$(INTDIR)\jcparam.obj"
	-@erase "$(INTDIR)\jcphuff.obj"
	-@erase "$(INTDIR)\jcprepct.obj"
	-@erase "$(INTDIR)\jcsample.obj"
	-@erase "$(INTDIR)\jctrans.obj"
	-@erase "$(INTDIR)\jdapimin.obj"
	-@erase "$(INTDIR)\jdapistd.obj"
	-@erase "$(INTDIR)\jdatadst.obj"
	-@erase "$(INTDIR)\jdatasrc.obj"
	-@erase "$(INTDIR)\jdcoefct.obj"
	-@erase "$(INTDIR)\jdcolor.obj"
	-@erase "$(INTDIR)\jddctmgr.obj"
	-@erase "$(INTDIR)\jdhuff.obj"
	-@erase "$(INTDIR)\jdinput.obj"
	-@erase "$(INTDIR)\jdmainct.obj"
	-@erase "$(INTDIR)\jdmarker.obj"
	-@erase "$(INTDIR)\jdmaster.obj"
	-@erase "$(INTDIR)\jdmerge.obj"
	-@erase "$(INTDIR)\jdphuff.obj"
	-@erase "$(INTDIR)\jdpostct.obj"
	-@erase "$(INTDIR)\jdsample.obj"
	-@erase "$(INTDIR)\jdtrans.obj"
	-@erase "$(INTDIR)\jerror.obj"
	-@erase "$(INTDIR)\jfdctflt.obj"
	-@erase "$(INTDIR)\jfdctfst.obj"
	-@erase "$(INTDIR)\jfdctint.obj"
	-@erase "$(INTDIR)\jidctflt.obj"
	-@erase "$(INTDIR)\jidctfst.obj"
	-@erase "$(INTDIR)\jidctint.obj"
	-@erase "$(INTDIR)\jidctred.obj"
	-@erase "$(INTDIR)\jmemmgr.obj"
	-@erase "$(INTDIR)\jmemnobs.obj"
	-@erase "$(INTDIR)\jquant1.obj"
	-@erase "$(INTDIR)\jquant2.obj"
	-@erase "$(INTDIR)\jutils.obj"
	-@erase "$(INTDIR)\rdbmp.obj"
	-@erase "$(INTDIR)\rdcolmap.obj"
	-@erase "$(INTDIR)\rdgif.obj"
	-@erase "$(INTDIR)\rdppm.obj"
	-@erase "$(INTDIR)\rdrle.obj"
	-@erase "$(INTDIR)\rdswitch.obj"
	-@erase "$(INTDIR)\rdtarga.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\wrrle.obj"
	-@erase "$(OUTDIR)\Jlossy.lib"

"$(OUT1DIR)" :
    if not exist "$(OUT1DIR)/$(NULL)" mkdir "$(OUT1DIR)"

"$(OUTDIR)" : "$(OUT1DIR)"
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /ML /W3 /O2 /I "..\.." /I ".." /D "NDEBUG" /D "STRICT" /D "WIN32" /D "_WINDOWS" /Fp"$(INTDIR)\Jlossy.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Jlossy.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Jlossy.lib" 
LIB32_OBJS= \
	"$(INTDIR)\jcapimin.obj" \
	"$(INTDIR)\jcapistd.obj" \
	"$(INTDIR)\jccoefct.obj" \
	"$(INTDIR)\jccolor.obj" \
	"$(INTDIR)\jcdctmgr.obj" \
	"$(INTDIR)\jchuff.obj" \
	"$(INTDIR)\jcinit.obj" \
	"$(INTDIR)\jcmainct.obj" \
	"$(INTDIR)\jcmarker.obj" \
	"$(INTDIR)\jcmaster.obj" \
	"$(INTDIR)\jcomapi.obj" \
	"$(INTDIR)\jcparam.obj" \
	"$(INTDIR)\jcphuff.obj" \
	"$(INTDIR)\jcprepct.obj" \
	"$(INTDIR)\jcsample.obj" \
	"$(INTDIR)\jctrans.obj" \
	"$(INTDIR)\jdapimin.obj" \
	"$(INTDIR)\jdapistd.obj" \
	"$(INTDIR)\jdatadst.obj" \
	"$(INTDIR)\jdatasrc.obj" \
	"$(INTDIR)\jdcoefct.obj" \
	"$(INTDIR)\jdcolor.obj" \
	"$(INTDIR)\jddctmgr.obj" \
	"$(INTDIR)\jdhuff.obj" \
	"$(INTDIR)\jdinput.obj" \
	"$(INTDIR)\jdmainct.obj" \
	"$(INTDIR)\jdmarker.obj" \
	"$(INTDIR)\jdmaster.obj" \
	"$(INTDIR)\jdmerge.obj" \
	"$(INTDIR)\jdphuff.obj" \
	"$(INTDIR)\jdpostct.obj" \
	"$(INTDIR)\jdsample.obj" \
	"$(INTDIR)\jdtrans.obj" \
	"$(INTDIR)\jerror.obj" \
	"$(INTDIR)\jfdctflt.obj" \
	"$(INTDIR)\jfdctfst.obj" \
	"$(INTDIR)\jfdctint.obj" \
	"$(INTDIR)\jidctflt.obj" \
	"$(INTDIR)\jidctfst.obj" \
	"$(INTDIR)\jidctint.obj" \
	"$(INTDIR)\jidctred.obj" \
	"$(INTDIR)\jmemmgr.obj" \
	"$(INTDIR)\jmemnobs.obj" \
	"$(INTDIR)\jquant1.obj" \
	"$(INTDIR)\jquant2.obj" \
	"$(INTDIR)\jutils.obj" \
	"$(INTDIR)\rdbmp.obj" \
	"$(INTDIR)\rdcolmap.obj" \
	"$(INTDIR)\rdgif.obj" \
	"$(INTDIR)\rdppm.obj" \
	"$(INTDIR)\rdrle.obj" \
	"$(INTDIR)\rdswitch.obj" \
	"$(INTDIR)\rdtarga.obj" \
	"$(INTDIR)\wrrle.obj"

"$(OUTDIR)\Jlossy.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"

OUT1DIR=..\..\PC
OUTDIR=..\..\PC\lib
INTDIR=..\..\PC\lib
# Begin Custom Macros
OutDir=..\..\PC\lib
# End Custom Macros

ALL : "$(OUTDIR)\Jlossy.lib" "$(OUTDIR)\Jlossy.bsc"


CLEAN :
	-@erase "$(INTDIR)\jcapimin.obj"
	-@erase "$(INTDIR)\jcapimin.sbr"
	-@erase "$(INTDIR)\jcapistd.obj"
	-@erase "$(INTDIR)\jcapistd.sbr"
	-@erase "$(INTDIR)\jccoefct.obj"
	-@erase "$(INTDIR)\jccoefct.sbr"
	-@erase "$(INTDIR)\jccolor.obj"
	-@erase "$(INTDIR)\jccolor.sbr"
	-@erase "$(INTDIR)\jcdctmgr.obj"
	-@erase "$(INTDIR)\jcdctmgr.sbr"
	-@erase "$(INTDIR)\jchuff.obj"
	-@erase "$(INTDIR)\jchuff.sbr"
	-@erase "$(INTDIR)\jcinit.obj"
	-@erase "$(INTDIR)\jcinit.sbr"
	-@erase "$(INTDIR)\jcmainct.obj"
	-@erase "$(INTDIR)\jcmainct.sbr"
	-@erase "$(INTDIR)\jcmarker.obj"
	-@erase "$(INTDIR)\jcmarker.sbr"
	-@erase "$(INTDIR)\jcmaster.obj"
	-@erase "$(INTDIR)\jcmaster.sbr"
	-@erase "$(INTDIR)\jcomapi.obj"
	-@erase "$(INTDIR)\jcomapi.sbr"
	-@erase "$(INTDIR)\jcparam.obj"
	-@erase "$(INTDIR)\jcparam.sbr"
	-@erase "$(INTDIR)\jcphuff.obj"
	-@erase "$(INTDIR)\jcphuff.sbr"
	-@erase "$(INTDIR)\jcprepct.obj"
	-@erase "$(INTDIR)\jcprepct.sbr"
	-@erase "$(INTDIR)\jcsample.obj"
	-@erase "$(INTDIR)\jcsample.sbr"
	-@erase "$(INTDIR)\jctrans.obj"
	-@erase "$(INTDIR)\jctrans.sbr"
	-@erase "$(INTDIR)\jdapimin.obj"
	-@erase "$(INTDIR)\jdapimin.sbr"
	-@erase "$(INTDIR)\jdapistd.obj"
	-@erase "$(INTDIR)\jdapistd.sbr"
	-@erase "$(INTDIR)\jdatadst.obj"
	-@erase "$(INTDIR)\jdatadst.sbr"
	-@erase "$(INTDIR)\jdatasrc.obj"
	-@erase "$(INTDIR)\jdatasrc.sbr"
	-@erase "$(INTDIR)\jdcoefct.obj"
	-@erase "$(INTDIR)\jdcoefct.sbr"
	-@erase "$(INTDIR)\jdcolor.obj"
	-@erase "$(INTDIR)\jdcolor.sbr"
	-@erase "$(INTDIR)\jddctmgr.obj"
	-@erase "$(INTDIR)\jddctmgr.sbr"
	-@erase "$(INTDIR)\jdhuff.obj"
	-@erase "$(INTDIR)\jdhuff.sbr"
	-@erase "$(INTDIR)\jdinput.obj"
	-@erase "$(INTDIR)\jdinput.sbr"
	-@erase "$(INTDIR)\jdmainct.obj"
	-@erase "$(INTDIR)\jdmainct.sbr"
	-@erase "$(INTDIR)\jdmarker.obj"
	-@erase "$(INTDIR)\jdmarker.sbr"
	-@erase "$(INTDIR)\jdmaster.obj"
	-@erase "$(INTDIR)\jdmaster.sbr"
	-@erase "$(INTDIR)\jdmerge.obj"
	-@erase "$(INTDIR)\jdmerge.sbr"
	-@erase "$(INTDIR)\jdphuff.obj"
	-@erase "$(INTDIR)\jdphuff.sbr"
	-@erase "$(INTDIR)\jdpostct.obj"
	-@erase "$(INTDIR)\jdpostct.sbr"
	-@erase "$(INTDIR)\jdsample.obj"
	-@erase "$(INTDIR)\jdsample.sbr"
	-@erase "$(INTDIR)\jdtrans.obj"
	-@erase "$(INTDIR)\jdtrans.sbr"
	-@erase "$(INTDIR)\jerror.obj"
	-@erase "$(INTDIR)\jerror.sbr"
	-@erase "$(INTDIR)\jfdctflt.obj"
	-@erase "$(INTDIR)\jfdctflt.sbr"
	-@erase "$(INTDIR)\jfdctfst.obj"
	-@erase "$(INTDIR)\jfdctfst.sbr"
	-@erase "$(INTDIR)\jfdctint.obj"
	-@erase "$(INTDIR)\jfdctint.sbr"
	-@erase "$(INTDIR)\jidctflt.obj"
	-@erase "$(INTDIR)\jidctflt.sbr"
	-@erase "$(INTDIR)\jidctfst.obj"
	-@erase "$(INTDIR)\jidctfst.sbr"
	-@erase "$(INTDIR)\jidctint.obj"
	-@erase "$(INTDIR)\jidctint.sbr"
	-@erase "$(INTDIR)\jidctred.obj"
	-@erase "$(INTDIR)\jidctred.sbr"
	-@erase "$(INTDIR)\jmemmgr.obj"
	-@erase "$(INTDIR)\jmemmgr.sbr"
	-@erase "$(INTDIR)\jmemnobs.obj"
	-@erase "$(INTDIR)\jmemnobs.sbr"
	-@erase "$(INTDIR)\jquant1.obj"
	-@erase "$(INTDIR)\jquant1.sbr"
	-@erase "$(INTDIR)\jquant2.obj"
	-@erase "$(INTDIR)\jquant2.sbr"
	-@erase "$(INTDIR)\jutils.obj"
	-@erase "$(INTDIR)\jutils.sbr"
	-@erase "$(INTDIR)\rdbmp.obj"
	-@erase "$(INTDIR)\rdbmp.sbr"
	-@erase "$(INTDIR)\rdcolmap.obj"
	-@erase "$(INTDIR)\rdcolmap.sbr"
	-@erase "$(INTDIR)\rdgif.obj"
	-@erase "$(INTDIR)\rdgif.sbr"
	-@erase "$(INTDIR)\rdppm.obj"
	-@erase "$(INTDIR)\rdppm.sbr"
	-@erase "$(INTDIR)\rdrle.obj"
	-@erase "$(INTDIR)\rdrle.sbr"
	-@erase "$(INTDIR)\rdswitch.obj"
	-@erase "$(INTDIR)\rdswitch.sbr"
	-@erase "$(INTDIR)\rdtarga.obj"
	-@erase "$(INTDIR)\rdtarga.sbr"
	-@erase "$(INTDIR)\wrrle.obj"
	-@erase "$(INTDIR)\wrrle.sbr"
	-@erase "$(OUTDIR)\Jlossy.bsc"
	-@erase "$(OUTDIR)\Jlossy.lib"
	-@erase ".\PAPYRUS3.IDB"
	-@erase ".\PAPYRUS3.PDB"

"$(OUT1DIR)" :
    if not exist "$(OUT1DIR)/$(NULL)" mkdir "$(OUT1DIR)"

"$(OUTDIR)" : "$(OUT1DIR)"
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MLd /W3 /Gm /Zi /Od /I "..\.." /I ".." /D "_DEBUG" /D "STRICT" /D "WIN32" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Jlossy.pch" /YX /Fo"$(INTDIR)\\" /Fd".\PAPYRUS3.PDB" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Jlossy.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\jcapimin.sbr" \
	"$(INTDIR)\jcapistd.sbr" \
	"$(INTDIR)\jccoefct.sbr" \
	"$(INTDIR)\jccolor.sbr" \
	"$(INTDIR)\jcdctmgr.sbr" \
	"$(INTDIR)\jchuff.sbr" \
	"$(INTDIR)\jcinit.sbr" \
	"$(INTDIR)\jcmainct.sbr" \
	"$(INTDIR)\jcmarker.sbr" \
	"$(INTDIR)\jcmaster.sbr" \
	"$(INTDIR)\jcomapi.sbr" \
	"$(INTDIR)\jcparam.sbr" \
	"$(INTDIR)\jcphuff.sbr" \
	"$(INTDIR)\jcprepct.sbr" \
	"$(INTDIR)\jcsample.sbr" \
	"$(INTDIR)\jctrans.sbr" \
	"$(INTDIR)\jdapimin.sbr" \
	"$(INTDIR)\jdapistd.sbr" \
	"$(INTDIR)\jdatadst.sbr" \
	"$(INTDIR)\jdatasrc.sbr" \
	"$(INTDIR)\jdcoefct.sbr" \
	"$(INTDIR)\jdcolor.sbr" \
	"$(INTDIR)\jddctmgr.sbr" \
	"$(INTDIR)\jdhuff.sbr" \
	"$(INTDIR)\jdinput.sbr" \
	"$(INTDIR)\jdmainct.sbr" \
	"$(INTDIR)\jdmarker.sbr" \
	"$(INTDIR)\jdmaster.sbr" \
	"$(INTDIR)\jdmerge.sbr" \
	"$(INTDIR)\jdphuff.sbr" \
	"$(INTDIR)\jdpostct.sbr" \
	"$(INTDIR)\jdsample.sbr" \
	"$(INTDIR)\jdtrans.sbr" \
	"$(INTDIR)\jerror.sbr" \
	"$(INTDIR)\jfdctflt.sbr" \
	"$(INTDIR)\jfdctfst.sbr" \
	"$(INTDIR)\jfdctint.sbr" \
	"$(INTDIR)\jidctflt.sbr" \
	"$(INTDIR)\jidctfst.sbr" \
	"$(INTDIR)\jidctint.sbr" \
	"$(INTDIR)\jidctred.sbr" \
	"$(INTDIR)\jmemmgr.sbr" \
	"$(INTDIR)\jmemnobs.sbr" \
	"$(INTDIR)\jquant1.sbr" \
	"$(INTDIR)\jquant2.sbr" \
	"$(INTDIR)\jutils.sbr" \
	"$(INTDIR)\rdbmp.sbr" \
	"$(INTDIR)\rdcolmap.sbr" \
	"$(INTDIR)\rdgif.sbr" \
	"$(INTDIR)\rdppm.sbr" \
	"$(INTDIR)\rdrle.sbr" \
	"$(INTDIR)\rdswitch.sbr" \
	"$(INTDIR)\rdtarga.sbr" \
	"$(INTDIR)\wrrle.sbr"

"$(OUTDIR)\Jlossy.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Jlossy.lib" 
LIB32_OBJS= \
	"$(INTDIR)\jcapimin.obj" \
	"$(INTDIR)\jcapistd.obj" \
	"$(INTDIR)\jccoefct.obj" \
	"$(INTDIR)\jccolor.obj" \
	"$(INTDIR)\jcdctmgr.obj" \
	"$(INTDIR)\jchuff.obj" \
	"$(INTDIR)\jcinit.obj" \
	"$(INTDIR)\jcmainct.obj" \
	"$(INTDIR)\jcmarker.obj" \
	"$(INTDIR)\jcmaster.obj" \
	"$(INTDIR)\jcomapi.obj" \
	"$(INTDIR)\jcparam.obj" \
	"$(INTDIR)\jcphuff.obj" \
	"$(INTDIR)\jcprepct.obj" \
	"$(INTDIR)\jcsample.obj" \
	"$(INTDIR)\jctrans.obj" \
	"$(INTDIR)\jdapimin.obj" \
	"$(INTDIR)\jdapistd.obj" \
	"$(INTDIR)\jdatadst.obj" \
	"$(INTDIR)\jdatasrc.obj" \
	"$(INTDIR)\jdcoefct.obj" \
	"$(INTDIR)\jdcolor.obj" \
	"$(INTDIR)\jddctmgr.obj" \
	"$(INTDIR)\jdhuff.obj" \
	"$(INTDIR)\jdinput.obj" \
	"$(INTDIR)\jdmainct.obj" \
	"$(INTDIR)\jdmarker.obj" \
	"$(INTDIR)\jdmaster.obj" \
	"$(INTDIR)\jdmerge.obj" \
	"$(INTDIR)\jdphuff.obj" \
	"$(INTDIR)\jdpostct.obj" \
	"$(INTDIR)\jdsample.obj" \
	"$(INTDIR)\jdtrans.obj" \
	"$(INTDIR)\jerror.obj" \
	"$(INTDIR)\jfdctflt.obj" \
	"$(INTDIR)\jfdctfst.obj" \
	"$(INTDIR)\jfdctint.obj" \
	"$(INTDIR)\jidctflt.obj" \
	"$(INTDIR)\jidctfst.obj" \
	"$(INTDIR)\jidctint.obj" \
	"$(INTDIR)\jidctred.obj" \
	"$(INTDIR)\jmemmgr.obj" \
	"$(INTDIR)\jmemnobs.obj" \
	"$(INTDIR)\jquant1.obj" \
	"$(INTDIR)\jquant2.obj" \
	"$(INTDIR)\jutils.obj" \
	"$(INTDIR)\rdbmp.obj" \
	"$(INTDIR)\rdcolmap.obj" \
	"$(INTDIR)\rdgif.obj" \
	"$(INTDIR)\rdppm.obj" \
	"$(INTDIR)\rdrle.obj" \
	"$(INTDIR)\rdswitch.obj" \
	"$(INTDIR)\rdtarga.obj" \
	"$(INTDIR)\wrrle.obj"

"$(OUTDIR)\Jlossy.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Jlossy.dep")
!INCLUDE "Jlossy.dep"
!ELSE 
!MESSAGE Warning: cannot find "Jlossy.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Jpglossy - Win32 Release" || "$(CFG)" == "Jpglossy - Win32 Debug"
SOURCE=.\jcapimin.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jcapimin.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jcapimin.obj"	"$(INTDIR)\jcapimin.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jcapistd.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jcapistd.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jcapistd.obj"	"$(INTDIR)\jcapistd.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jccoefct.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jccoefct.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jccoefct.obj"	"$(INTDIR)\jccoefct.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jccolor.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jccolor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jccolor.obj"	"$(INTDIR)\jccolor.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jcdctmgr.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jcdctmgr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jcdctmgr.obj"	"$(INTDIR)\jcdctmgr.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jchuff.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jchuff.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jchuff.obj"	"$(INTDIR)\jchuff.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jcinit.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jcinit.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jcinit.obj"	"$(INTDIR)\jcinit.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jcmainct.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jcmainct.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jcmainct.obj"	"$(INTDIR)\jcmainct.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jcmarker.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jcmarker.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jcmarker.obj"	"$(INTDIR)\jcmarker.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jcmaster.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jcmaster.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jcmaster.obj"	"$(INTDIR)\jcmaster.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jcomapi.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jcomapi.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jcomapi.obj"	"$(INTDIR)\jcomapi.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jcparam.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jcparam.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jcparam.obj"	"$(INTDIR)\jcparam.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jcphuff.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jcphuff.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jcphuff.obj"	"$(INTDIR)\jcphuff.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jcprepct.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jcprepct.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jcprepct.obj"	"$(INTDIR)\jcprepct.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jcsample.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jcsample.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jcsample.obj"	"$(INTDIR)\jcsample.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jctrans.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jctrans.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jctrans.obj"	"$(INTDIR)\jctrans.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jdapimin.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jdapimin.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jdapimin.obj"	"$(INTDIR)\jdapimin.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jdapistd.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jdapistd.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jdapistd.obj"	"$(INTDIR)\jdapistd.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jdatadst.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jdatadst.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jdatadst.obj"	"$(INTDIR)\jdatadst.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jdatasrc.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jdatasrc.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jdatasrc.obj"	"$(INTDIR)\jdatasrc.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jdcoefct.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jdcoefct.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jdcoefct.obj"	"$(INTDIR)\jdcoefct.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jdcolor.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jdcolor.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jdcolor.obj"	"$(INTDIR)\jdcolor.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jddctmgr.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jddctmgr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jddctmgr.obj"	"$(INTDIR)\jddctmgr.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jdhuff.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jdhuff.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jdhuff.obj"	"$(INTDIR)\jdhuff.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jdinput.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jdinput.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jdinput.obj"	"$(INTDIR)\jdinput.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jdmainct.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jdmainct.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jdmainct.obj"	"$(INTDIR)\jdmainct.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jdmarker.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jdmarker.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jdmarker.obj"	"$(INTDIR)\jdmarker.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jdmaster.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jdmaster.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jdmaster.obj"	"$(INTDIR)\jdmaster.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jdmerge.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jdmerge.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jdmerge.obj"	"$(INTDIR)\jdmerge.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jdphuff.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jdphuff.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jdphuff.obj"	"$(INTDIR)\jdphuff.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jdpostct.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jdpostct.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jdpostct.obj"	"$(INTDIR)\jdpostct.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jdsample.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jdsample.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jdsample.obj"	"$(INTDIR)\jdsample.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jdtrans.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jdtrans.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jdtrans.obj"	"$(INTDIR)\jdtrans.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jerror.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jerror.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jerror.obj"	"$(INTDIR)\jerror.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jfdctflt.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jfdctflt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jfdctflt.obj"	"$(INTDIR)\jfdctflt.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jfdctfst.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jfdctfst.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jfdctfst.obj"	"$(INTDIR)\jfdctfst.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jfdctint.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jfdctint.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jfdctint.obj"	"$(INTDIR)\jfdctint.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jidctflt.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jidctflt.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jidctflt.obj"	"$(INTDIR)\jidctflt.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jidctfst.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jidctfst.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jidctfst.obj"	"$(INTDIR)\jidctfst.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jidctint.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jidctint.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jidctint.obj"	"$(INTDIR)\jidctint.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jidctred.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jidctred.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jidctred.obj"	"$(INTDIR)\jidctred.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jmemmgr.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jmemmgr.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jmemmgr.obj"	"$(INTDIR)\jmemmgr.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jmemnobs.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jmemnobs.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jmemnobs.obj"	"$(INTDIR)\jmemnobs.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jquant1.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jquant1.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jquant1.obj"	"$(INTDIR)\jquant1.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jquant2.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jquant2.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jquant2.obj"	"$(INTDIR)\jquant2.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\jutils.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\jutils.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\jutils.obj"	"$(INTDIR)\jutils.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rdbmp.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\rdbmp.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\rdbmp.obj"	"$(INTDIR)\rdbmp.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rdcolmap.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\rdcolmap.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\rdcolmap.obj"	"$(INTDIR)\rdcolmap.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rdgif.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\rdgif.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\rdgif.obj"	"$(INTDIR)\rdgif.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rdppm.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\rdppm.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\rdppm.obj"	"$(INTDIR)\rdppm.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rdrle.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\rdrle.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\rdrle.obj"	"$(INTDIR)\rdrle.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rdswitch.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\rdswitch.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\rdswitch.obj"	"$(INTDIR)\rdswitch.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\rdtarga.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\rdtarga.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\rdtarga.obj"	"$(INTDIR)\rdtarga.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\wrrle.c

!IF  "$(CFG)" == "Jpglossy - Win32 Release"


"$(INTDIR)\wrrle.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglossy - Win32 Debug"


"$(INTDIR)\wrrle.obj"	"$(INTDIR)\wrrle.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

