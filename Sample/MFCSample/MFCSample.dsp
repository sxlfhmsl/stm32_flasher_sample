# Microsoft Developer Studio Project File - Name="MFCSample" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MFCSample - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MFCSample.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MFCSample.mak" CFG="MFCSample - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MFCSample - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "MFCSample - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "MFCSample"
# PROP Scc_LocalPath "..\.."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MFCSample - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\..\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "MFC_APPLI" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 nafxcw.lib /nologo /subsystem:windows /incremental:yes /machine:I386
# SUBTRACT LINK32 /verbose

!ELSEIF  "$(CFG)" == "MFCSample - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\..\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /map /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /incremental:no

!ENDIF 

# Begin Target

# Name "MFCSample - Win32 Release"
# Name "MFCSample - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\EditWnd.cpp

!IF  "$(CFG)" == "MFCSample - Win32 Release"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "MFCSample - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\IOMessage.cpp

!IF  "$(CFG)" == "MFCSample - Win32 Release"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "MFCSample - Win32 Debug"

# ADD CPP /Yu"stdafx.h"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MFCSample.cpp

!IF  "$(CFG)" == "MFCSample - Win32 Release"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "MFCSample - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\MFCSample.rc
# End Source File
# Begin Source File

SOURCE=.\MFCSampleDlg.cpp

!IF  "$(CFG)" == "MFCSample - Win32 Release"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "MFCSample - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\PtkAPI.cpp

!IF  "$(CFG)" == "MFCSample - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "MFCSample - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\src\PtkToolBox.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\EditWnd.h
# End Source File
# Begin Source File

SOURCE=.\IOMessage.h
# End Source File
# Begin Source File

SOURCE=.\MFCSample.h
# End Source File
# Begin Source File

SOURCE=.\MFCSampleDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\src\PtkAPI.h
# End Source File
# Begin Source File

SOURCE=..\..\src\PtkToolBox.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\MFCSample.ico
# End Source File
# Begin Source File

SOURCE=.\res\MFCSample.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
