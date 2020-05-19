# Microsoft Developer Studio Project File - Name="ANN" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ANN - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ANN.mak".
!MESSAGE 
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

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ANN - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "include" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"lib\ANN.lib"

!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "include" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"lib\ANN.lib"

!ENDIF 

# Begin Target

# Name "ANN - Win32 Release"
# Name "ANN - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\ANN.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

# ADD CPP /MD /I "ann/include/ANN"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\bd_pr_search.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

# ADD CPP /MD /I "include/ANN"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\bd_search.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

# ADD CPP /MD /I "include/ANN"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\bd_tree.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

# ADD CPP /MD /I "include/ANN"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\brute.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

# ADD CPP /MD /I "include/ANN"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\kd_pr_search.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

# ADD CPP /MD /I "include/ANN"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\kd_search.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

# ADD CPP /MD /I "include/ANN"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\kd_split.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

# ADD CPP /MD /I "include/ANN"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\kd_tree.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

# ADD CPP /MD /I "include/ANN"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\kd_util.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

# ADD CPP /MD /I "include/ANN"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\perf.cxx

!IF  "$(CFG)" == "ANN - Win32 Release"

!ELSEIF  "$(CFG)" == "ANN - Win32 Debug"

# ADD CPP /MD /I "include/ANN"

!ENDIF 

# End Source File
# End Group
# End Target
# End Project
