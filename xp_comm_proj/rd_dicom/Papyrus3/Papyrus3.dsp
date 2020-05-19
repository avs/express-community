# Microsoft Developer Studio Project File - Name="Papyrus3" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Papyrus3 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Papyrus3.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/osiris/Papyrus3", YXGAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Papyrus3 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\Release"
# PROP BASE Intermediate_Dir ".\Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\Release"
# PROP Intermediate_Dir ".\Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G3 /Zp1 /W3 /O2 /I "\osiris\papyrus3" /I "\osiris\papyrus3\jpglless" /I "\osiris\papyrus3\jpglossy" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "STRICT" /D "_WIN16_" /D "FILENAME83" /D "PAPY3_VAR" /YX /c
# ADD CPP /nologo /Zp1 /W3 /GX /O2 /I "DicomDir\\" /I "JpegDir\Jpglless\\" /I "JpegDir\Jpglossy\\" /I "..\Papyrus2\\" /D "NDEBUG" /D "_WINDOWS" /D "STRICT" /D "PAPY3_VAR" /D "_MBCS" /D "WIN32" /YX /FD /c
# ADD BASE RSC /l 0x40c
# ADD RSC /l 0x40c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Papyrus3 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Zp1 /W3 /Gm /Zi /Od /Gf /Gy /I "\osiris\papyrus3" /I "\osiris\papyrus3\jpglless" /I "\osiris\papyrus3\jpglossy" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "STRICT" /D "_WIN16_" /D "FILENAME83" /D "PAPY3_VAR" /YX /c
# ADD CPP /nologo /Zp1 /W3 /Gm /GX /Zi /Od /I "DicomDir\\" /I "JpegDir\Jpglless\\" /I "JpegDir\Jpglossy\\" /I "..\Papyrus2\\" /D "_DEBUG" /D "_WINDOWS" /D "STRICT" /D "PAPY3_VAR" /D "_MBCS" /D "WIN32" /FR /YX /FD /c
# ADD BASE RSC /l 0x40c
# ADD RSC /l 0x40c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "Papyrus3 - Win32 Release"
# Name "Papyrus3 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\PapyConvertFile3.c
# End Source File
# Begin Source File

SOURCE=.\PapyDataSetRead3.c
# End Source File
# Begin Source File

SOURCE=.\PapyDataSetWrite3.c
# End Source File
# Begin Source File

SOURCE=.\PapyEallocPC3.cpp
# End Source File
# Begin Source File

SOURCE=.\PapyError3.c
# End Source File
# Begin Source File

SOURCE=.\PapyFiles3.c
# End Source File
# Begin Source File

SOURCE=.\PapyFileSystemPC3.c
# End Source File
# Begin Source File

SOURCE=.\PapyGetGlobalVar3.c
# End Source File
# Begin Source File

SOURCE=.\PapyInit3.c
# End Source File
# Begin Source File

SOURCE=.\PapyInitGroups3.c
# End Source File
# Begin Source File

SOURCE=.\PapyInitModules3.c
# End Source File
# Begin Source File

SOURCE=.\PapyList3.c
# End Source File
# Begin Source File

SOURCE=.\PapyRead3.c
# End Source File
# Begin Source File

SOURCE=.\PapyUtils3.c
# End Source File
# Begin Source File

SOURCE=.\PapyVars3.c
# End Source File
# Begin Source File

SOURCE=.\PapyWild3.c
# End Source File
# Begin Source File

SOURCE=.\PapyWildexit3.c
# End Source File
# Begin Source File

SOURCE=.\PapyWrite3.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\PapyEalloc3.h
# End Source File
# Begin Source File

SOURCE=.\PapyEnumGroups3.h
# End Source File
# Begin Source File

SOURCE=.\PapyEnumImageGroups3.h
# End Source File
# Begin Source File

SOURCE=.\PapyEnumImagesModules3.h
# End Source File
# Begin Source File

SOURCE=.\PapyEnumModules3.h
# End Source File
# Begin Source File

SOURCE=.\PapyError3.h
# End Source File
# Begin Source File

SOURCE=.\PapyErrorMacros3.h
# End Source File
# Begin Source File

SOURCE=.\PapyFileSystem3.h
# End Source File
# Begin Source File

SOURCE=.\PapyGlobalVar3.h
# End Source File
# Begin Source File

SOURCE=.\PapyInitModules3.h
# End Source File
# Begin Source File

SOURCE=.\PapyPrivFunctionDef3.h
# End Source File
# Begin Source File

SOURCE=.\PapyPubFunctionDef3.h
# End Source File
# Begin Source File

SOURCE=.\PAPYRUS3.h
# End Source File
# Begin Source File

SOURCE=.\PapyTypeDef3.h
# End Source File
# Begin Source File

SOURCE=.\PapyWild3.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
