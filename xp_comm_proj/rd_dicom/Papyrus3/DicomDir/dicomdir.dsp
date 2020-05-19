# Microsoft Developer Studio Project File - Name="dicomdir" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=dicomdir - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "dicomdir.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "dicomdir - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "dicomdir___Win32_Release"
# PROP BASE Intermediate_Dir "dicomdir___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "dicomdir___Win32_Release"
# PROP Intermediate_Dir "dicomdir___Win32_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I ".." /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x100c /d "NDEBUG"
# ADD RSC /l 0x100c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "dicomdir - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "dicomdir___Win32_Debug"
# PROP BASE Intermediate_Dir "dicomdir___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "dicomdir___Win32_Debug"
# PROP Intermediate_Dir "dicomdir___Win32_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ  /c
# ADD BASE RSC /l 0x100c /d "_DEBUG"
# ADD RSC /l 0x100c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "dicomdir - Win32 Release"
# Name "dicomdir - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DicomDirDataSetRead.c
# End Source File
# Begin Source File

SOURCE=.\DicomDirDataSetWrite.c
# End Source File
# Begin Source File

SOURCE=.\DicomDirFiles.c
# End Source File
# Begin Source File

SOURCE=.\DicomDirInit.c
# End Source File
# Begin Source File

SOURCE=.\DicomDirInitRecords.c
# End Source File
# Begin Source File

SOURCE=.\OGlobalDicomFunc.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DicomDir.h
# End Source File
# Begin Source File

SOURCE=.\DicomDirEnumRecords.h
# End Source File
# Begin Source File

SOURCE=.\DicomdirGlobalVar3.h
# End Source File
# Begin Source File

SOURCE=.\DicomDirInitRecords.h
# End Source File
# Begin Source File

SOURCE=.\DicomdirPrivFunctionDef3.h
# End Source File
# Begin Source File

SOURCE=.\DicomdirPubFunctionDef3.h
# End Source File
# Begin Source File

SOURCE=.\DicomdirTypeDef3.h
# End Source File
# Begin Source File

SOURCE=.\OGlobalDicomFunc.h
# End Source File
# End Group
# End Target
# End Project
