# Microsoft Developer Studio Generated NMAKE File, Based on dicomdir.dsp
!IF "$(CFG)" == ""
CFG=dicomdir - Win32 Debug
!MESSAGE No configuration specified. Defaulting to dicomdir - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "dicomdir - Win32 Release" && "$(CFG)" != "dicomdir - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dicomdir.mak" CFG="dicomdir - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dicomdir - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "dicomdir - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "dicomdir - Win32 Release"

OUT1DIR=..\PC
OUTDIR=..\PC\lib
INTDIR=..\PC\lib
# Begin Custom Macros
OutDir=..\PC\lib
# End Custom Macros

ALL : "$(OUTDIR)\dicomdir.lib"


CLEAN :
	-@erase "$(INTDIR)\DicomDirDataSetRead.obj"
	-@erase "$(INTDIR)\DicomDirDataSetWrite.obj"
	-@erase "$(INTDIR)\DicomDirFiles.obj"
	-@erase "$(INTDIR)\DicomDirInit.obj"
	-@erase "$(INTDIR)\DicomDirInitRecords.obj"
	-@erase "$(INTDIR)\OGlobalDicomFunc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\dicomdir.lib"

"$(OUT1DIR)" :
    if not exist "$(OUT1DIR)/$(NULL)" mkdir "$(OUT1DIR)"

"$(OUTDIR)" : "$(OUT1DIR)"
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /I ".." /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\dicomdir.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\dicomdir.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\dicomdir.lib" 
LIB32_OBJS= \
	"$(INTDIR)\DicomDirDataSetRead.obj" \
	"$(INTDIR)\DicomDirDataSetWrite.obj" \
	"$(INTDIR)\DicomDirFiles.obj" \
	"$(INTDIR)\DicomDirInit.obj" \
	"$(INTDIR)\DicomDirInitRecords.obj" \
	"$(INTDIR)\OGlobalDicomFunc.obj"

"$(OUTDIR)\dicomdir.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "dicomdir - Win32 Debug"

OUT1DIR=..\PC
OUTDIR=..\PC\lib
INTDIR=..\PC\lib
# Begin Custom Macros
OutDir=..\PC\lib
# End Custom Macros

ALL : "$(OUTDIR)\dicomdir.lib"


CLEAN :
	-@erase "$(INTDIR)\DicomDirDataSetRead.obj"
	-@erase "$(INTDIR)\DicomDirDataSetWrite.obj"
	-@erase "$(INTDIR)\DicomDirFiles.obj"
	-@erase "$(INTDIR)\DicomDirInit.obj"
	-@erase "$(INTDIR)\DicomDirInitRecords.obj"
	-@erase "$(INTDIR)\OGlobalDicomFunc.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\dicomdir.lib"

"$(OUT1DIR)" :
    if not exist "$(OUT1DIR)/$(NULL)" mkdir "$(OUT1DIR)"

"$(OUTDIR)" : "$(OUT1DIR)"
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\dicomdir.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\dicomdir.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\dicomdir.lib" 
LIB32_OBJS= \
	"$(INTDIR)\DicomDirDataSetRead.obj" \
	"$(INTDIR)\DicomDirDataSetWrite.obj" \
	"$(INTDIR)\DicomDirFiles.obj" \
	"$(INTDIR)\DicomDirInit.obj" \
	"$(INTDIR)\DicomDirInitRecords.obj" \
	"$(INTDIR)\OGlobalDicomFunc.obj"

"$(OUTDIR)\dicomdir.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("dicomdir.dep")
!INCLUDE "dicomdir.dep"
!ELSE 
!MESSAGE Warning: cannot find "dicomdir.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "dicomdir - Win32 Release" || "$(CFG)" == "dicomdir - Win32 Debug"
SOURCE=.\DicomDirDataSetRead.c

"$(INTDIR)\DicomDirDataSetRead.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DicomDirDataSetWrite.c

"$(INTDIR)\DicomDirDataSetWrite.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DicomDirFiles.c

"$(INTDIR)\DicomDirFiles.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DicomDirInit.c

"$(INTDIR)\DicomDirInit.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\DicomDirInitRecords.c

"$(INTDIR)\DicomDirInitRecords.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\OGlobalDicomFunc.cpp

"$(INTDIR)\OGlobalDicomFunc.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

