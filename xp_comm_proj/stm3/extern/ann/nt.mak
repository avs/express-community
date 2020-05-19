# Microsoft Developer Studio Generated NMAKE File, Based on ANN.dsp
!IF "$(CFG)" == ""
CFG=ANN - Win32 Debug
!MESSAGE No configuration specified. Defaulting to ANN - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "ANN - Win32 Release" && "$(CFG)" != "ANN - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ANN.mak" CFG="ANN - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ANN - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ANN - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "ANN - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\lib\ANN.lib"


CLEAN :
	-@erase "$(INTDIR)\ANN.obj"
	-@erase "$(INTDIR)\bd_pr_search.obj"
	-@erase "$(INTDIR)\bd_search.obj"
	-@erase "$(INTDIR)\bd_tree.obj"
	-@erase "$(INTDIR)\brute.obj"
	-@erase "$(INTDIR)\kd_pr_search.obj"
	-@erase "$(INTDIR)\kd_search.obj"
	-@erase "$(INTDIR)\kd_split.obj"
	-@erase "$(INTDIR)\kd_tree.obj"
	-@erase "$(INTDIR)\kd_util.obj"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\lib\ANN.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ANN.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"lib\ANN.lib" 
LIB32_OBJS= \
	"$(INTDIR)\ANN.obj" \
	"$(INTDIR)\bd_pr_search.obj" \
	"$(INTDIR)\bd_search.obj" \
	"$(INTDIR)\bd_tree.obj" \
	"$(INTDIR)\brute.obj" \
	"$(INTDIR)\kd_pr_search.obj" \
	"$(INTDIR)\kd_search.obj" \
	"$(INTDIR)\kd_split.obj" \
	"$(INTDIR)\kd_tree.obj" \
	"$(INTDIR)\kd_util.obj" \
	"$(INTDIR)\perf.obj"

".\lib\ANN.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : ".\lib\ANN.lib"


CLEAN :
	-@erase "$(INTDIR)\ANN.obj"
	-@erase "$(INTDIR)\bd_pr_search.obj"
	-@erase "$(INTDIR)\bd_search.obj"
	-@erase "$(INTDIR)\bd_tree.obj"
	-@erase "$(INTDIR)\brute.obj"
	-@erase "$(INTDIR)\kd_pr_search.obj"
	-@erase "$(INTDIR)\kd_search.obj"
	-@erase "$(INTDIR)\kd_split.obj"
	-@erase "$(INTDIR)\kd_tree.obj"
	-@erase "$(INTDIR)\kd_util.obj"
	-@erase "$(INTDIR)\perf.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase ".\lib\ANN.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /I "include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\ANN.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\ANN.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"lib\ANN.lib" 
LIB32_OBJS= \
	"$(INTDIR)\ANN.obj" \
	"$(INTDIR)\bd_pr_search.obj" \
	"$(INTDIR)\bd_search.obj" \
	"$(INTDIR)\bd_tree.obj" \
	"$(INTDIR)\brute.obj" \
	"$(INTDIR)\kd_pr_search.obj" \
	"$(INTDIR)\kd_search.obj" \
	"$(INTDIR)\kd_split.obj" \
	"$(INTDIR)\kd_tree.obj" \
	"$(INTDIR)\kd_util.obj" \
	"$(INTDIR)\perf.obj"

".\lib\ANN.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("ANN.dep")
!INCLUDE "ANN.dep"
!ELSE 
!MESSAGE Warning: cannot find "ANN.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "ANN - Win32 Release" || "$(CFG)" == "ANN - Win32 Debug"
SOURCE=.\src\ANN.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\ANN.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

CPP_SWITCHES=/nologo /MD /W3 /Gm /GX /ZI /Od /I "include" /I "ann/include/ANN" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\ANN.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\ANN.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\src\bd_pr_search.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\bd_pr_search.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

CPP_SWITCHES=/nologo /MD /W3 /Gm /GX /ZI /Od /I "include" /I "include/ANN" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\ANN.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\bd_pr_search.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\src\bd_search.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\bd_search.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

CPP_SWITCHES=/nologo /MD /W3 /Gm /GX /ZI /Od /I "include" /I "include/ANN" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\ANN.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\bd_search.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\src\bd_tree.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\bd_tree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

CPP_SWITCHES=/nologo /MD /W3 /Gm /GX /ZI /Od /I "include" /I "include/ANN" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\ANN.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\bd_tree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\src\brute.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\brute.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

CPP_SWITCHES=/nologo /MD /W3 /Gm /GX /ZI /Od /I "include" /I "include/ANN" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\ANN.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\brute.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\src\kd_pr_search.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\kd_pr_search.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

CPP_SWITCHES=/nologo /MD /W3 /Gm /GX /ZI /Od /I "include" /I "include/ANN" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\ANN.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\kd_pr_search.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\src\kd_search.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\kd_search.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

CPP_SWITCHES=/nologo /MD /W3 /Gm /GX /ZI /Od /I "include" /I "include/ANN" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\ANN.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\kd_search.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\src\kd_split.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\kd_split.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

CPP_SWITCHES=/nologo /MD /W3 /Gm /GX /ZI /Od /I "include" /I "include/ANN" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\ANN.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\kd_split.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\src\kd_tree.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\kd_tree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

CPP_SWITCHES=/nologo /MD /W3 /Gm /GX /ZI /Od /I "include" /I "include/ANN" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\ANN.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\kd_tree.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\src\kd_util.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\kd_util.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

CPP_SWITCHES=/nologo /MD /W3 /Gm /GX /ZI /Od /I "include" /I "include/ANN" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\ANN.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\kd_util.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\src\perf.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\perf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

CPP_SWITCHES=/nologo /MD /W3 /Gm /GX /ZI /Od /I "include" /I "include/ANN" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"$(INTDIR)\ANN.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\perf.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

