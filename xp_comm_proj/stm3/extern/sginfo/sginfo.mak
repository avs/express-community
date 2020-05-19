# Microsoft Developer Studio Generated NMAKE File, Based on sginfo.dsp
!IF "$(CFG)" == ""
CFG=sginfo - Win32 Debug
!MESSAGE No configuration specified. Defaulting to sginfo - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "sginfo - Win32 Release" && "$(CFG)" != "sginfo - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sginfo.mak" CFG="sginfo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sginfo - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "sginfo - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "sginfo - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\sginfo.lib"


CLEAN :
	-@erase "$(INTDIR)\sgclib.obj"
	-@erase "$(INTDIR)\sgfind.obj"
	-@erase "$(INTDIR)\sghkl.obj"
	-@erase "$(INTDIR)\sgio.obj"
	-@erase "$(INTDIR)\sgsi.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\sginfo.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sginfo.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"sginfo.lib" 
LIB32_OBJS= \
	"$(INTDIR)\sgclib.obj" \
	"$(INTDIR)\sgfind.obj" \
	"$(INTDIR)\sghkl.obj" \
	"$(INTDIR)\sgio.obj" \
	"$(INTDIR)\sgsi.obj"

".\sginfo.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "sginfo - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\sginfo.lib"


CLEAN :
	-@erase "$(INTDIR)\sgclib.obj"
	-@erase "$(INTDIR)\sgfind.obj"
	-@erase "$(INTDIR)\sghkl.obj"
	-@erase "$(INTDIR)\sgio.obj"
	-@erase "$(INTDIR)\sgsi.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\sginfo.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\sginfo.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\sginfo.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\sginfo.lib" 
LIB32_OBJS= \
	"$(INTDIR)\sgclib.obj" \
	"$(INTDIR)\sgfind.obj" \
	"$(INTDIR)\sghkl.obj" \
	"$(INTDIR)\sgio.obj" \
	"$(INTDIR)\sgsi.obj"

"$(OUTDIR)\sginfo.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("sginfo.dep")
!INCLUDE "sginfo.dep"
!ELSE 
!MESSAGE Warning: cannot find "sginfo.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "sginfo - Win32 Release" || "$(CFG)" == "sginfo - Win32 Debug"
SOURCE=.\sgclib.c

"$(INTDIR)\sgclib.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sgfind.c

"$(INTDIR)\sgfind.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sghkl.c

"$(INTDIR)\sghkl.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sgio.c

"$(INTDIR)\sgio.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\sgsi.c

"$(INTDIR)\sgsi.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

