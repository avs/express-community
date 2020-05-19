# Microsoft Developer Studio Project File - Name="libstm3" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libstm3 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libstm3.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libstm3 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /D "_AFXDLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x410 /d "NDEBUG"
# ADD RSC /l 0x410 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"libstm3.lib"

!ELSEIF  "$(CFG)" == "libstm3 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MD /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x410 /d "_DEBUG"
# ADD RSC /l 0x410 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"libstm3.lib"

!ENDIF 

# Begin Target

# Name "libstm3 - Win32 Release"
# Name "libstm3 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\atom_properties.cxx
# End Source File
# Begin Source File

SOURCE=.\name_to_z.cxx
# End Source File
# Begin Source File

SOURCE=.\read_Cgamess.cxx
# End Source File
# Begin Source File

SOURCE=.\read_chem_3d.cxx
# End Source File
# Begin Source File

SOURCE=.\read_coord.cxx
# End Source File
# Begin Source File

SOURCE=.\read_cube.cxx
# End Source File
# Begin Source File

SOURCE=.\read_dcar.cxx
# End Source File
# Begin Source File

SOURCE=.\read_dcd.cxx
# End Source File
# Begin Source File

SOURCE=.\read_dmol_xyz.cxx
# End Source File
# Begin Source File

SOURCE=.\read_gamess.cxx
# End Source File
# Begin Source File

SOURCE=.\read_kino.cxx
# End Source File
# Begin Source File

SOURCE=.\read_mm3.cxx
# End Source File
# Begin Source File

SOURCE=.\read_mol.cxx
# End Source File
# Begin Source File

SOURCE=.\read_mol2.cxx
# End Source File
# Begin Source File

SOURCE=.\read_mopac_esp.cxx
# End Source File
# Begin Source File

SOURCE=.\read_mopac_xyz.cxx
# End Source File
# Begin Source File

SOURCE=.\read_pdb.cxx
# End Source File
# Begin Source File

SOURCE=.\read_plain_coord.cxx
# End Source File
# Begin Source File

SOURCE=.\read_shelx.cxx
# End Source File
# Begin Source File

SOURCE=.\res_to_color.cxx
# End Source File
# End Group
# End Target
# End Project
