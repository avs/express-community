# Microsoft Developer Studio Project File - Name="Jpglless" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Jpglless - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Jlless.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/osiris/Papyrus3/JpegDir/JpgLless", MYGAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Jpglless - Win32 Release"

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
# ADD BASE CPP /nologo /G3 /Zp1 /W3 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "STRICT" /D "_WIN16_" /D "FILENAME83" /YX /c
# ADD CPP /nologo /Zp1 /W3 /O2 /I "..\Papyrus3\\" /I "..\..\..\Papyrus3\\" /I ".." /D "_WINDOWS" /D "STRICT" /D "NDEBUG" /D "WIN32" /YX /FD /c
# ADD BASE RSC /l 0x40c
# ADD RSC /l 0x40c
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Jpglless - Win32 Debug"

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
# ADD BASE CPP /nologo /Zp1 /W3 /Gm /Zi /Od /Gf /Gy /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "STRICT" /D "_WIN16_" /D "FILENAME83" /YX /Fd"PAPYRUS3.PDB" /c
# ADD CPP /nologo /Zp1 /W3 /Gm /Zi /Od /I "..\..\..\Papyrus3\\" /I ".." /D "_WINDOWS" /D "STRICT" /D "_DEBUG" /D "WIN32" /FR /YX /Fd".\PAPYRUS3.PDB" /FD /c
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

# Name "Jpglless - Win32 Release"
# Name "Jpglless - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;hpj;bat;for;f90"
# Begin Source File

SOURCE=.\decompoptimise.c
# End Source File
# Begin Source File

SOURCE=.\huffcoptimisebug.c
# End Source File
# Begin Source File

SOURCE=.\huffdoptimise.c
# End Source File
# Begin Source File

SOURCE=.\JPEGUTIL.C
# End Source File
# Begin Source File

SOURCE=.\MCU.C
# End Source File
# Begin Source File

SOURCE=.\pmreadoptimise.c
# End Source File
# Begin Source File

SOURCE=.\pnmtoljpoptimisebug.c
# End Source File
# Begin Source File

SOURCE=.\PREDICT.C
# End Source File
# Begin Source File

SOURCE=.\READ.C
# End Source File
# Begin Source File

SOURCE=.\WRITE.C
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\IO.H
# End Source File
# Begin Source File

SOURCE=.\JPEG.H
# End Source File
# Begin Source File

SOURCE=.\JPEGLESS.H
# End Source File
# Begin Source File

SOURCE=.\MCU.H
# End Source File
# Begin Source File

SOURCE=.\PREDICT.H
# End Source File
# Begin Source File

SOURCE=.\PROTO.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
