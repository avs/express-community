# Microsoft Developer Studio Generated NMAKE File, Based on libstm3.dsp
!IF "$(CFG)" == ""
CFG=libstm3 - Win32 Debug
!MESSAGE No configuration specified. Defaulting to libstm3 - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "libstm3 - Win32 Release" && "$(CFG)" != "libstm3 - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libstm3.mak" CFG="libstm3 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libstm3 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libstm3 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "libstm3 - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release

ALL : ".\libstm3.lib"


CLEAN :
	-@erase "$(INTDIR)\atom_properties.obj"
	-@erase "$(INTDIR)\name_to_z.obj"
	-@erase "$(INTDIR)\read_Cgamess.obj"
	-@erase "$(INTDIR)\read_chem_3d.obj"
	-@erase "$(INTDIR)\read_coord.obj"
	-@erase "$(INTDIR)\read_cube.obj"
	-@erase "$(INTDIR)\read_dcar.obj"
	-@erase "$(INTDIR)\read_dcd.obj"
	-@erase "$(INTDIR)\read_dmol_xyz.obj"
	-@erase "$(INTDIR)\read_gamess.obj"
	-@erase "$(INTDIR)\read_kino.obj"
	-@erase "$(INTDIR)\read_mm3.obj"
	-@erase "$(INTDIR)\read_mol.obj"
	-@erase "$(INTDIR)\read_mol2.obj"
	-@erase "$(INTDIR)\read_mopac_esp.obj"
	-@erase "$(INTDIR)\read_mopac_xyz.obj"
	-@erase "$(INTDIR)\read_pdb.obj"
	-@erase "$(INTDIR)\read_plain_coord.obj"
	-@erase "$(INTDIR)\read_shelx.obj"
	-@erase "$(INTDIR)\res_to_color.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase ".\libstm3.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libstm3.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"libstm3.lib" 
LIB32_OBJS= \
	"$(INTDIR)\atom_properties.obj" \
	"$(INTDIR)\name_to_z.obj" \
	"$(INTDIR)\read_Cgamess.obj" \
	"$(INTDIR)\read_chem_3d.obj" \
	"$(INTDIR)\read_coord.obj" \
	"$(INTDIR)\read_cube.obj" \
	"$(INTDIR)\read_dcar.obj" \
	"$(INTDIR)\read_dcd.obj" \
	"$(INTDIR)\read_dmol_xyz.obj" \
	"$(INTDIR)\read_gamess.obj" \
	"$(INTDIR)\read_kino.obj" \
	"$(INTDIR)\read_mm3.obj" \
	"$(INTDIR)\read_mol.obj" \
	"$(INTDIR)\read_mol2.obj" \
	"$(INTDIR)\read_mopac_esp.obj" \
	"$(INTDIR)\read_mopac_xyz.obj" \
	"$(INTDIR)\read_pdb.obj" \
	"$(INTDIR)\read_plain_coord.obj" \
	"$(INTDIR)\read_shelx.obj" \
	"$(INTDIR)\res_to_color.obj"

".\libstm3.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ELSEIF  "$(CFG)" == "libstm3 - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug

ALL : ".\libstm3.lib"


CLEAN :
	-@erase "$(INTDIR)\atom_properties.obj"
	-@erase "$(INTDIR)\name_to_z.obj"
	-@erase "$(INTDIR)\read_Cgamess.obj"
	-@erase "$(INTDIR)\read_chem_3d.obj"
	-@erase "$(INTDIR)\read_coord.obj"
	-@erase "$(INTDIR)\read_cube.obj"
	-@erase "$(INTDIR)\read_dcar.obj"
	-@erase "$(INTDIR)\read_dcd.obj"
	-@erase "$(INTDIR)\read_dmol_xyz.obj"
	-@erase "$(INTDIR)\read_gamess.obj"
	-@erase "$(INTDIR)\read_kino.obj"
	-@erase "$(INTDIR)\read_mm3.obj"
	-@erase "$(INTDIR)\read_mol.obj"
	-@erase "$(INTDIR)\read_mol2.obj"
	-@erase "$(INTDIR)\read_mopac_esp.obj"
	-@erase "$(INTDIR)\read_mopac_xyz.obj"
	-@erase "$(INTDIR)\read_pdb.obj"
	-@erase "$(INTDIR)\read_plain_coord.obj"
	-@erase "$(INTDIR)\read_shelx.obj"
	-@erase "$(INTDIR)\res_to_color.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase ".\libstm3.lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

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
BSC32_FLAGS=/nologo /o"$(OUTDIR)\libstm3.bsc" 
BSC32_SBRS= \
	
LIB32=link.exe -lib
LIB32_FLAGS=/nologo /out:"libstm3.lib" 
LIB32_OBJS= \
	"$(INTDIR)\atom_properties.obj" \
	"$(INTDIR)\name_to_z.obj" \
	"$(INTDIR)\read_Cgamess.obj" \
	"$(INTDIR)\read_chem_3d.obj" \
	"$(INTDIR)\read_coord.obj" \
	"$(INTDIR)\read_cube.obj" \
	"$(INTDIR)\read_dcar.obj" \
	"$(INTDIR)\read_dcd.obj" \
	"$(INTDIR)\read_dmol_xyz.obj" \
	"$(INTDIR)\read_gamess.obj" \
	"$(INTDIR)\read_kino.obj" \
	"$(INTDIR)\read_mm3.obj" \
	"$(INTDIR)\read_mol.obj" \
	"$(INTDIR)\read_mol2.obj" \
	"$(INTDIR)\read_mopac_esp.obj" \
	"$(INTDIR)\read_mopac_xyz.obj" \
	"$(INTDIR)\read_pdb.obj" \
	"$(INTDIR)\read_plain_coord.obj" \
	"$(INTDIR)\read_shelx.obj" \
	"$(INTDIR)\res_to_color.obj"

".\libstm3.lib" : "$(OUTDIR)" $(DEF_FILE) $(LIB32_OBJS)
    $(LIB32) @<<
  $(LIB32_FLAGS) $(DEF_FLAGS) $(LIB32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("libstm3.dep")
!INCLUDE "libstm3.dep"
!ELSE 
!MESSAGE Warning: cannot find "libstm3.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "libstm3 - Win32 Release" || "$(CFG)" == "libstm3 - Win32 Debug"
SOURCE=.\atom_properties.cxx

"$(INTDIR)\atom_properties.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\name_to_z.cxx

"$(INTDIR)\name_to_z.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\read_Cgamess.cxx

"$(INTDIR)\read_Cgamess.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\read_chem_3d.cxx

"$(INTDIR)\read_chem_3d.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\read_coord.cxx

"$(INTDIR)\read_coord.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\read_cube.cxx

"$(INTDIR)\read_cube.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\read_dcar.cxx

"$(INTDIR)\read_dcar.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\read_dcd.cxx

"$(INTDIR)\read_dcd.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\read_dmol_xyz.cxx

"$(INTDIR)\read_dmol_xyz.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\read_gamess.cxx

"$(INTDIR)\read_gamess.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\read_kino.cxx

"$(INTDIR)\read_kino.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\read_mm3.cxx

"$(INTDIR)\read_mm3.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\read_mol.cxx

"$(INTDIR)\read_mol.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\read_mol2.cxx

"$(INTDIR)\read_mol2.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\read_mopac_esp.cxx

"$(INTDIR)\read_mopac_esp.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\read_mopac_xyz.cxx

"$(INTDIR)\read_mopac_xyz.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\read_pdb.cxx

"$(INTDIR)\read_pdb.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\read_plain_coord.cxx

"$(INTDIR)\read_plain_coord.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\read_shelx.cxx

"$(INTDIR)\read_shelx.obj" : $(SOURCE) "$(INTDIR)"


SOURCE=.\res_to_color.cxx

"$(INTDIR)\res_to_color.obj" : $(SOURCE) "$(INTDIR)"



!ENDIF 

