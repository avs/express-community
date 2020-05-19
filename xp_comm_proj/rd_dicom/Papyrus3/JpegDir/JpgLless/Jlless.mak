# Microsoft Developer Studio Generated NMAKE File, Based on Jlless.dsp
!IF "$(CFG)" == ""
CFG=Jpglless - Win32 Release
!MESSAGE No configuration specified. Defaulting to Jpglless - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "Jpglless - Win32 Release" && "$(CFG)" != "Jpglless - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Jlless.mak" CFG="Jpglless - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Jpglless - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Jpglless - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "Jpglless - Win32 Release"

OUT1DIR=..\..\PC
OUTDIR=..\..\PC\lib
INTDIR=..\..\PC\lib
# Begin Custom Macros
OutDir=..\..\PC\lib
# End Custom Macros

ALL : "$(OUTDIR)\Jlless.lib"


CLEAN :
	-@erase "$(INTDIR)\decompoptimise.obj"
	-@erase "$(INTDIR)\huffcoptimisebug.obj"
	-@erase "$(INTDIR)\huffdoptimise.obj"
	-@erase "$(INTDIR)\JPEGUTIL.OBJ"
	-@erase "$(INTDIR)\MCU.OBJ"
	-@erase "$(INTDIR)\pmreadoptimise.obj"
	-@erase "$(INTDIR)\pnmtoljpoptimisebug.obj"
	-@erase "$(INTDIR)\PREDICT.OBJ"
	-@erase "$(INTDIR)\READ.OBJ"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\WRITE.OBJ"
	-@erase "$(OUTDIR)\Jlless.lib"

"$(OUT1DIR)" :
    if not exist "$(OUT1DIR)/$(NULL)" mkdir "$(OUT1DIR)"

"$(OUTDIR)" : "$(OUT1DIR)"
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /ML /W3 /O2 /I "..\Papyrus3\\" /I "..\..\..\Papyrus3\\" /I ".." /D "_WINDOWS" /D "STRICT" /D "NDEBUG" /D "WIN32" /Fp"$(INTDIR)\Jlless.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Jlless.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Jlless.lib" 
LIB32_OBJS= \
	"$(INTDIR)\decompoptimise.obj" \
	"$(INTDIR)\huffcoptimisebug.obj" \
	"$(INTDIR)\huffdoptimise.obj" \
	"$(INTDIR)\JPEGUTIL.OBJ" \
	"$(INTDIR)\MCU.OBJ" \
	"$(INTDIR)\pmreadoptimise.obj" \
	"$(INTDIR)\pnmtoljpoptimisebug.obj" \
	"$(INTDIR)\PREDICT.OBJ" \
	"$(INTDIR)\READ.OBJ" \
	"$(INTDIR)\WRITE.OBJ"

"$(OUTDIR)\Jlless.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Jpglless - Win32 Debug"

OUT1DIR=..\..\PC
OUTDIR=..\..\PC\lib
INTDIR=..\..\PC\lib
# Begin Custom Macros
OutDir=..\..\PC\lib
# End Custom Macros

ALL : "$(OUTDIR)\Jlless.lib" "$(OUTDIR)\Jlless.bsc"


CLEAN :
	-@erase "$(INTDIR)\decompoptimise.obj"
	-@erase "$(INTDIR)\decompoptimise.sbr"
	-@erase "$(INTDIR)\huffcoptimisebug.obj"
	-@erase "$(INTDIR)\huffcoptimisebug.sbr"
	-@erase "$(INTDIR)\huffdoptimise.obj"
	-@erase "$(INTDIR)\huffdoptimise.sbr"
	-@erase "$(INTDIR)\JPEGUTIL.OBJ"
	-@erase "$(INTDIR)\JPEGUTIL.SBR"
	-@erase "$(INTDIR)\MCU.OBJ"
	-@erase "$(INTDIR)\MCU.SBR"
	-@erase "$(INTDIR)\pmreadoptimise.obj"
	-@erase "$(INTDIR)\pmreadoptimise.sbr"
	-@erase "$(INTDIR)\pnmtoljpoptimisebug.obj"
	-@erase "$(INTDIR)\pnmtoljpoptimisebug.sbr"
	-@erase "$(INTDIR)\PREDICT.OBJ"
	-@erase "$(INTDIR)\PREDICT.SBR"
	-@erase "$(INTDIR)\READ.OBJ"
	-@erase "$(INTDIR)\READ.SBR"
	-@erase "$(INTDIR)\WRITE.OBJ"
	-@erase "$(INTDIR)\WRITE.SBR"
	-@erase "$(OUTDIR)\Jlless.bsc"
	-@erase "$(OUTDIR)\Jlless.lib"
	-@erase ".\PAPYRUS3.IDB"
	-@erase ".\PAPYRUS3.PDB"

"$(OUT1DIR)" : 
    if not exist "$(OUT1DIR)/$(NULL)" mkdir "$(OUT1DIR)"

"$(OUTDIR)" : "$(OUT1DIR)"
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /Zp1 /MLd /W3 /Gm /Zi /Od /I "..\..\..\Papyrus3\\" /I ".." /D "_WINDOWS" /D "STRICT" /D "_DEBUG" /D "WIN32" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Jlless.pch" /YX /Fo"$(INTDIR)\\" /Fd".\PAPYRUS3.PDB" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Jlless.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\decompoptimise.sbr" \
	"$(INTDIR)\huffcoptimisebug.sbr" \
	"$(INTDIR)\huffdoptimise.sbr" \
	"$(INTDIR)\JPEGUTIL.SBR" \
	"$(INTDIR)\MCU.SBR" \
	"$(INTDIR)\pmreadoptimise.sbr" \
	"$(INTDIR)\pnmtoljpoptimisebug.sbr" \
	"$(INTDIR)\PREDICT.SBR" \
	"$(INTDIR)\READ.SBR" \
	"$(INTDIR)\WRITE.SBR"

"$(OUTDIR)\Jlless.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"$(OUTDIR)\Jlless.lib" 
LIB32_OBJS= \
	"$(INTDIR)\decompoptimise.obj" \
	"$(INTDIR)\huffcoptimisebug.obj" \
	"$(INTDIR)\huffdoptimise.obj" \
	"$(INTDIR)\JPEGUTIL.OBJ" \
	"$(INTDIR)\MCU.OBJ" \
	"$(INTDIR)\pmreadoptimise.obj" \
	"$(INTDIR)\pnmtoljpoptimisebug.obj" \
	"$(INTDIR)\PREDICT.OBJ" \
	"$(INTDIR)\READ.OBJ" \
	"$(INTDIR)\WRITE.OBJ"

"$(OUTDIR)\Jlless.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Jlless.dep")
!INCLUDE "Jlless.dep"
!ELSE 
!MESSAGE Warning: cannot find "Jlless.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Jpglless - Win32 Release" || "$(CFG)" == "Jpglless - Win32 Debug"
SOURCE=.\decompoptimise.c

!IF  "$(CFG)" == "Jpglless - Win32 Release"


"$(INTDIR)\decompoptimise.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglless - Win32 Debug"


"$(INTDIR)\decompoptimise.obj"	"$(INTDIR)\decompoptimise.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\huffcoptimisebug.c

!IF  "$(CFG)" == "Jpglless - Win32 Release"


"$(INTDIR)\huffcoptimisebug.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglless - Win32 Debug"


"$(INTDIR)\huffcoptimisebug.obj"	"$(INTDIR)\huffcoptimisebug.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\huffdoptimise.c

!IF  "$(CFG)" == "Jpglless - Win32 Release"


"$(INTDIR)\huffdoptimise.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglless - Win32 Debug"


"$(INTDIR)\huffdoptimise.obj"	"$(INTDIR)\huffdoptimise.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\JPEGUTIL.C

!IF  "$(CFG)" == "Jpglless - Win32 Release"


"$(INTDIR)\JPEGUTIL.OBJ" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglless - Win32 Debug"


"$(INTDIR)\JPEGUTIL.OBJ"	"$(INTDIR)\JPEGUTIL.SBR" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\MCU.C

!IF  "$(CFG)" == "Jpglless - Win32 Release"


"$(INTDIR)\MCU.OBJ" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglless - Win32 Debug"


"$(INTDIR)\MCU.OBJ"	"$(INTDIR)\MCU.SBR" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\pmreadoptimise.c

!IF  "$(CFG)" == "Jpglless - Win32 Release"


"$(INTDIR)\pmreadoptimise.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglless - Win32 Debug"


"$(INTDIR)\pmreadoptimise.obj"	"$(INTDIR)\pmreadoptimise.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\pnmtoljpoptimisebug.c

!IF  "$(CFG)" == "Jpglless - Win32 Release"


"$(INTDIR)\pnmtoljpoptimisebug.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglless - Win32 Debug"


"$(INTDIR)\pnmtoljpoptimisebug.obj"	"$(INTDIR)\pnmtoljpoptimisebug.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\PREDICT.C

!IF  "$(CFG)" == "Jpglless - Win32 Release"


"$(INTDIR)\PREDICT.OBJ" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglless - Win32 Debug"


"$(INTDIR)\PREDICT.OBJ"	"$(INTDIR)\PREDICT.SBR" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\READ.C

!IF  "$(CFG)" == "Jpglless - Win32 Release"


"$(INTDIR)\READ.OBJ" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglless - Win32 Debug"


"$(INTDIR)\READ.OBJ"	"$(INTDIR)\READ.SBR" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\WRITE.C

!IF  "$(CFG)" == "Jpglless - Win32 Release"


"$(INTDIR)\WRITE.OBJ" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "Jpglless - Win32 Debug"


"$(INTDIR)\WRITE.OBJ"	"$(INTDIR)\WRITE.SBR" : $(SOURCE) "$(INTDIR)"


!ENDIF 


!ENDIF 

