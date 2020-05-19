# Microsoft Developer Studio Generated NMAKE File, Based on Papyrus3.dsp
!IF "$(CFG)" == ""
CFG=Papyrus3 - Win32 Release
!MESSAGE No configuration specified. Defaulting to Papyrus3 - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "Papyrus3 - Win32 Release" && "$(CFG)" != "Papyrus3 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Papyrus3.mak" CFG="Papyrus3 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Papyrus3 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Papyrus3 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"

OUT1DIR=.\PC
OUTDIR=.\PC\lib
INTDIR=.\PC\lib
# Begin Custom Macros
OutDir=.\PC\lib
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Papyrus3.lib"

!ELSE 

ALL : "dicomdir - Win32 Release" "Jpglossy - Win32 Release" "Jpglless - Win32 Release" "$(OUTDIR)\Papyrus3.lib"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"Jpglless - Win32 ReleaseCLEAN" "Jpglossy - Win32 ReleaseCLEAN" "dicomdir - Win32 ReleaseCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\PapyConvertFile3.obj"
	-@erase "$(INTDIR)\PapyDataSetRead3.obj"
	-@erase "$(INTDIR)\PapyDataSetWrite3.obj"
	-@erase "$(INTDIR)\PapyEallocPC3.obj"
	-@erase "$(INTDIR)\PapyError3.obj"
	-@erase "$(INTDIR)\PapyFiles3.obj"
	-@erase "$(INTDIR)\PapyFileSystemPC3.obj"
	-@erase "$(INTDIR)\PapyGetGlobalVar3.obj"
	-@erase "$(INTDIR)\PapyInit3.obj"
	-@erase "$(INTDIR)\PapyInitGroups3.obj"
	-@erase "$(INTDIR)\PapyInitModules3.obj"
	-@erase "$(INTDIR)\PapyList3.obj"
	-@erase "$(INTDIR)\PapyRead3.obj"
	-@erase "$(INTDIR)\PapyUtils3.obj"
	-@erase "$(INTDIR)\PapyVars3.obj"
	-@erase "$(INTDIR)\PapyWild3.obj"
	-@erase "$(INTDIR)\PapyWildexit3.obj"
	-@erase "$(INTDIR)\PapyWrite3.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Papyrus3.lib"

"$(OUT1DIR)" :
    if not exist "$(OUT1DIR)/$(NULL)" mkdir "$(OUT1DIR)"

"$(OUTDIR)" : "$(OUT1DIR)"
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /ML /W3 /GX /O2 /I "DicomDir\\" /I "JpegDir\Jpglless\\" /I "JpegDir\Jpglossy\\" /I "..\Papyrus2\\" /D "NDEBUG" /D "_WINDOWS" /D "STRICT" /D "PAPY3_VAR" /D "_MBCS" /D "WIN32" /Fp"$(INTDIR)\Papyrus3.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Papyrus3.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Papyrus3.lib" 
LIB32_OBJS= \
	"$(INTDIR)\PapyConvertFile3.obj" \
	"$(INTDIR)\PapyDataSetRead3.obj" \
	"$(INTDIR)\PapyDataSetWrite3.obj" \
	"$(INTDIR)\PapyEallocPC3.obj" \
	"$(INTDIR)\PapyError3.obj" \
	"$(INTDIR)\PapyFiles3.obj" \
	"$(INTDIR)\PapyFileSystemPC3.obj" \
	"$(INTDIR)\PapyGetGlobalVar3.obj" \
	"$(INTDIR)\PapyInit3.obj" \
	"$(INTDIR)\PapyInitGroups3.obj" \
	"$(INTDIR)\PapyInitModules3.obj" \
	"$(INTDIR)\PapyList3.obj" \
	"$(INTDIR)\PapyRead3.obj" \
	"$(INTDIR)\PapyUtils3.obj" \
	"$(INTDIR)\PapyVars3.obj" \
	"$(INTDIR)\PapyWild3.obj" \
	"$(INTDIR)\PapyWildexit3.obj" \
	"$(INTDIR)\PapyWrite3.obj" \
	"$(INTDIR)\Jlless.lib" \
	"$(INTDIR)\Jlossy.lib" \
	"$(INTDIR)\dicomdir.lib"

"$(OUTDIR)\Papyrus3.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"

OUT1DIR=.\PC
OUTDIR=.\PC\lib
INTDIR=.\PC\lib
# Begin Custom Macros
OutDir=.\PC\lib
# End Custom Macros

!IF "$(RECURSE)" == "0" 

ALL : "$(OUTDIR)\Papyrus3.lib" "$(OUTDIR)\Papyrus3.bsc"

!ELSE 

ALL : "dicomdir - Win32 Debug" "Jpglossy - Win32 Debug" "Jpglless - Win32 Debug" "$(OUTDIR)\Papyrus3.lib" "$(OUTDIR)\Papyrus3.bsc"

!ENDIF 

!IF "$(RECURSE)" == "1" 
CLEAN :"Jpglless - Win32 DebugCLEAN" "Jpglossy - Win32 DebugCLEAN" "dicomdir - Win32 DebugCLEAN" 
!ELSE 
CLEAN :
!ENDIF 
	-@erase "$(INTDIR)\PapyConvertFile3.obj"
	-@erase "$(INTDIR)\PapyConvertFile3.sbr"
	-@erase "$(INTDIR)\PapyDataSetRead3.obj"
	-@erase "$(INTDIR)\PapyDataSetRead3.sbr"
	-@erase "$(INTDIR)\PapyDataSetWrite3.obj"
	-@erase "$(INTDIR)\PapyDataSetWrite3.sbr"
	-@erase "$(INTDIR)\PapyEallocPC3.obj"
	-@erase "$(INTDIR)\PapyEallocPC3.sbr"
	-@erase "$(INTDIR)\PapyError3.obj"
	-@erase "$(INTDIR)\PapyError3.sbr"
	-@erase "$(INTDIR)\PapyFiles3.obj"
	-@erase "$(INTDIR)\PapyFiles3.sbr"
	-@erase "$(INTDIR)\PapyFileSystemPC3.obj"
	-@erase "$(INTDIR)\PapyFileSystemPC3.sbr"
	-@erase "$(INTDIR)\PapyGetGlobalVar3.obj"
	-@erase "$(INTDIR)\PapyGetGlobalVar3.sbr"
	-@erase "$(INTDIR)\PapyInit3.obj"
	-@erase "$(INTDIR)\PapyInit3.sbr"
	-@erase "$(INTDIR)\PapyInitGroups3.obj"
	-@erase "$(INTDIR)\PapyInitGroups3.sbr"
	-@erase "$(INTDIR)\PapyInitModules3.obj"
	-@erase "$(INTDIR)\PapyInitModules3.sbr"
	-@erase "$(INTDIR)\PapyList3.obj"
	-@erase "$(INTDIR)\PapyList3.sbr"
	-@erase "$(INTDIR)\PapyRead3.obj"
	-@erase "$(INTDIR)\PapyRead3.sbr"
	-@erase "$(INTDIR)\PapyUtils3.obj"
	-@erase "$(INTDIR)\PapyUtils3.sbr"
	-@erase "$(INTDIR)\PapyVars3.obj"
	-@erase "$(INTDIR)\PapyVars3.sbr"
	-@erase "$(INTDIR)\PapyWild3.obj"
	-@erase "$(INTDIR)\PapyWild3.sbr"
	-@erase "$(INTDIR)\PapyWildexit3.obj"
	-@erase "$(INTDIR)\PapyWildexit3.sbr"
	-@erase "$(INTDIR)\PapyWrite3.obj"
	-@erase "$(INTDIR)\PapyWrite3.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Papyrus3.bsc"
	-@erase "$(OUTDIR)\Papyrus3.lib"

"$(OUT1DIR)" :
    if not exist "$(OUT1DIR)/$(NULL)" mkdir "$(OUT1DIR)"

"$(OUTDIR)" : "$(OUT1DIR)"
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MLd /W3 /Gm /GX /Zi /Od /I "DicomDir\\" /I "JpegDir\Jpglless\\" /I "JpegDir\Jpglossy\\" /I "..\Papyrus2\\" /D "_DEBUG" /D "_WINDOWS" /D "STRICT" /D "PAPY3_VAR" /D "_MBCS" /D "WIN32" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Papyrus3.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Papyrus3.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\PapyConvertFile3.sbr" \
	"$(INTDIR)\PapyDataSetRead3.sbr" \
	"$(INTDIR)\PapyDataSetWrite3.sbr" \
	"$(INTDIR)\PapyEallocPC3.sbr" \
	"$(INTDIR)\PapyError3.sbr" \
	"$(INTDIR)\PapyFiles3.sbr" \
	"$(INTDIR)\PapyFileSystemPC3.sbr" \
	"$(INTDIR)\PapyGetGlobalVar3.sbr" \
	"$(INTDIR)\PapyInit3.sbr" \
	"$(INTDIR)\PapyInitGroups3.sbr" \
	"$(INTDIR)\PapyInitModules3.sbr" \
	"$(INTDIR)\PapyList3.sbr" \
	"$(INTDIR)\PapyRead3.sbr" \
	"$(INTDIR)\PapyUtils3.sbr" \
	"$(INTDIR)\PapyVars3.sbr" \
	"$(INTDIR)\PapyWild3.sbr" \
	"$(INTDIR)\PapyWildexit3.sbr" \
	"$(INTDIR)\PapyWrite3.sbr"

"$(OUTDIR)\Papyrus3.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Papyrus3.lib" 
LIB32_OBJS= \
	"$(INTDIR)\PapyConvertFile3.obj" \
	"$(INTDIR)\PapyDataSetRead3.obj" \
	"$(INTDIR)\PapyDataSetWrite3.obj" \
	"$(INTDIR)\PapyEallocPC3.obj" \
	"$(INTDIR)\PapyError3.obj" \
	"$(INTDIR)\PapyFiles3.obj" \
	"$(INTDIR)\PapyFileSystemPC3.obj" \
	"$(INTDIR)\PapyGetGlobalVar3.obj" \
	"$(INTDIR)\PapyInit3.obj" \
	"$(INTDIR)\PapyInitGroups3.obj" \
	"$(INTDIR)\PapyInitModules3.obj" \
	"$(INTDIR)\PapyList3.obj" \
	"$(INTDIR)\PapyRead3.obj" \
	"$(INTDIR)\PapyUtils3.obj" \
	"$(INTDIR)\PapyVars3.obj" \
	"$(INTDIR)\PapyWild3.obj" \
	"$(INTDIR)\PapyWildexit3.obj" \
	"$(INTDIR)\PapyWrite3.obj" \
	"$(INTDIR)\Jlless.lib" \
	"$(INTDIR)\Jlossy.lib" \
	"$(INTDIR)\dicomdir.lib"

"$(OUTDIR)\Papyrus3.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Papyrus3.dep")
!INCLUDE "Papyrus3.dep"
!ELSE 
!MESSAGE Warning: cannot find "Papyrus3.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Papyrus3 - Win32 Release" || "$(CFG)" == "Papyrus3 - Win32 Debug"
SOURCE=.\PapyConvertFile3.c

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"


"$(INTDIR)\PapyConvertFile3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"


"$(INTDIR)\PapyConvertFile3.obj"	"$(INTDIR)\PapyConvertFile3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PapyDataSetRead3.c

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"


"$(INTDIR)\PapyDataSetRead3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"


"$(INTDIR)\PapyDataSetRead3.obj"	"$(INTDIR)\PapyDataSetRead3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PapyDataSetWrite3.c

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"


"$(INTDIR)\PapyDataSetWrite3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"


"$(INTDIR)\PapyDataSetWrite3.obj"	"$(INTDIR)\PapyDataSetWrite3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PapyEallocPC3.cpp

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"


"$(INTDIR)\PapyEallocPC3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"


"$(INTDIR)\PapyEallocPC3.obj"	"$(INTDIR)\PapyEallocPC3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PapyError3.c

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"


"$(INTDIR)\PapyError3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"


"$(INTDIR)\PapyError3.obj"	"$(INTDIR)\PapyError3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PapyFiles3.c

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"


"$(INTDIR)\PapyFiles3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"


"$(INTDIR)\PapyFiles3.obj"	"$(INTDIR)\PapyFiles3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PapyFileSystemPC3.c

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"


"$(INTDIR)\PapyFileSystemPC3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"


"$(INTDIR)\PapyFileSystemPC3.obj"	"$(INTDIR)\PapyFileSystemPC3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PapyGetGlobalVar3.c

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"


"$(INTDIR)\PapyGetGlobalVar3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"


"$(INTDIR)\PapyGetGlobalVar3.obj"	"$(INTDIR)\PapyGetGlobalVar3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PapyInit3.c

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"


"$(INTDIR)\PapyInit3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"


"$(INTDIR)\PapyInit3.obj"	"$(INTDIR)\PapyInit3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PapyInitGroups3.c

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"


"$(INTDIR)\PapyInitGroups3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"


"$(INTDIR)\PapyInitGroups3.obj"	"$(INTDIR)\PapyInitGroups3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PapyInitModules3.c

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"


"$(INTDIR)\PapyInitModules3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"


"$(INTDIR)\PapyInitModules3.obj"	"$(INTDIR)\PapyInitModules3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PapyList3.c

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"


"$(INTDIR)\PapyList3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"


"$(INTDIR)\PapyList3.obj"	"$(INTDIR)\PapyList3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PapyRead3.c

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"


"$(INTDIR)\PapyRead3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"


"$(INTDIR)\PapyRead3.obj"	"$(INTDIR)\PapyRead3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PapyUtils3.c

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"


"$(INTDIR)\PapyUtils3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"


"$(INTDIR)\PapyUtils3.obj"	"$(INTDIR)\PapyUtils3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PapyVars3.c

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"


"$(INTDIR)\PapyVars3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"


"$(INTDIR)\PapyVars3.obj"	"$(INTDIR)\PapyVars3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PapyWild3.c

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"


"$(INTDIR)\PapyWild3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"


"$(INTDIR)\PapyWild3.obj"	"$(INTDIR)\PapyWild3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PapyWildexit3.c

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"


"$(INTDIR)\PapyWildexit3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"


"$(INTDIR)\PapyWildexit3.obj"	"$(INTDIR)\PapyWildexit3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PapyWrite3.c

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"


"$(INTDIR)\PapyWrite3.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"


"$(INTDIR)\PapyWrite3.obj"	"$(INTDIR)\PapyWrite3.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"

"Jpglless - Win32 Release" : 
   cd ".\JpegDir\JpgLless"
   $(MAKE) /$(MAKEFLAGS) /F .\Jlless.mak CFG="Jpglless - Win32 Release" 
   cd "..\.."

"Jpglless - Win32 ReleaseCLEAN" : 
   cd ".\JpegDir\JpgLless"
   $(MAKE) /$(MAKEFLAGS) /F .\Jlless.mak CFG="Jpglless - Win32 Release" RECURSE=1 CLEAN 
   cd "..\.."

!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"

"Jpglless - Win32 Debug" : 
   cd ".\JpegDir\JpgLless"
   $(MAKE) /$(MAKEFLAGS) /F .\Jlless.mak CFG="Jpglless - Win32 Debug" 
   cd "..\.."

"Jpglless - Win32 DebugCLEAN" : 
   cd ".\JpegDir\JpgLless"
   $(MAKE) /$(MAKEFLAGS) /F .\Jlless.mak CFG="Jpglless - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\.."

!ENDIF 

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"

"Jpglossy - Win32 Release" : 
   cd ".\JpegDir\JpgLossy"
   $(MAKE) /$(MAKEFLAGS) /F .\Jlossy.mak CFG="Jpglossy - Win32 Release" 
   cd "..\.."

"Jpglossy - Win32 ReleaseCLEAN" : 
   cd ".\JpegDir\JpgLossy"
   $(MAKE) /$(MAKEFLAGS) /F .\Jlossy.mak CFG="Jpglossy - Win32 Release" RECURSE=1 CLEAN 
   cd "..\.."

!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"

"Jpglossy - Win32 Debug" : 
   cd ".\JpegDir\JpgLossy"
   $(MAKE) /$(MAKEFLAGS) /F .\Jlossy.mak CFG="Jpglossy - Win32 Debug" 
   cd "..\.."

"Jpglossy - Win32 DebugCLEAN" : 
   cd ".\JpegDir\JpgLossy"
   $(MAKE) /$(MAKEFLAGS) /F .\Jlossy.mak CFG="Jpglossy - Win32 Debug" RECURSE=1 CLEAN 
   cd "..\.."

!ENDIF 

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"

"dicomdir - Win32 Release" : 
   cd ".\dicomdir"
   $(MAKE) /$(MAKEFLAGS) /F .\dicomdir.mak CFG="dicomdir - Win32 Release" 
   cd ".."

"dicomdir - Win32 ReleaseCLEAN" : 
   cd ".\dicomdir"
   $(MAKE) /$(MAKEFLAGS) /F .\dicomdir.mak CFG="dicomdir - Win32 Release" RECURSE=1 CLEAN 
   cd ".."

!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"

"dicomdir - Win32 Debug" : 
   cd ".\dicomdir"
   $(MAKE) /$(MAKEFLAGS) /F .\dicomdir.mak CFG="dicomdir - Win32 Debug" 
   cd ".."

"dicomdir - Win32 DebugCLEAN" : 
   cd ".\dicomdir"
   $(MAKE) /$(MAKEFLAGS) /F .\dicomdir.mak CFG="dicomdir - Win32 Debug" RECURSE=1 CLEAN 
   cd ".."

!ENDIF 


!ENDIF 

