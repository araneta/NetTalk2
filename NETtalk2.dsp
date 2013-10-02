# Microsoft Developer Studio Project File - Name="NETtalk2" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=NETtalk2 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "NETtalk2.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "NETtalk2.mak" CFG="NETtalk2 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "NETtalk2 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "NETtalk2 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "NETtalk2 - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x421 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x421 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Msimg32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "NETtalk2 - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x421 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x421 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "NETtalk2 - Win32 Release"
# Name "NETtalk2 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\1_5SPAW.cpp
# End Source File
# Begin Source File

SOURCE=.\library\ado2.cpp
# End Source File
# Begin Source File

SOURCE=.\BackPro.cpp
# End Source File
# Begin Source File

SOURCE=.\library\ButtonStyle.cpp
# End Source File
# Begin Source File

SOURCE=.\KataDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LearningDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\library\ListCtrlSort.cpp
# End Source File
# Begin Source File

SOURCE=.\MainDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NaturalLanguage.cpp
# End Source File
# Begin Source File

SOURCE=.\NETtalk2.cpp
# End Source File
# Begin Source File

SOURCE=.\NETtalk2.rc
# End Source File
# Begin Source File

SOURCE=.\NETtalk2Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PengecualianDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SingkatanDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\library\StaticStyle.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TDPSOLA.cpp
# End Source File
# Begin Source File

SOURCE=.\library\TitleStyle.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\1_5SPAW.h
# End Source File
# Begin Source File

SOURCE=.\library\ado2.h
# End Source File
# Begin Source File

SOURCE=.\BackPro.h
# End Source File
# Begin Source File

SOURCE=.\library\ButtonStyle.h
# End Source File
# Begin Source File

SOURCE=.\KataDlg.h
# End Source File
# Begin Source File

SOURCE=.\LearningDlg.h
# End Source File
# Begin Source File

SOURCE=.\library\ListCtrlSort.h
# End Source File
# Begin Source File

SOURCE=.\MainDlg.h
# End Source File
# Begin Source File

SOURCE=.\NaturalLanguage.h
# End Source File
# Begin Source File

SOURCE=.\NETtalk2.h
# End Source File
# Begin Source File

SOURCE=.\NETtalk2Dlg.h
# End Source File
# Begin Source File

SOURCE=.\PengecualianDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SingkatanDlg.h
# End Source File
# Begin Source File

SOURCE=.\library\StaticStyle.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TDPSOLA.h
# End Source File
# Begin Source File

SOURCE=.\library\TitleStyle.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Styles\01_cdwriter_unmount.ico
# End Source File
# Begin Source File

SOURCE=.\Styles\02_desktop.ico
# End Source File
# Begin Source File

SOURCE=.\Styles\03_lock.ico
# End Source File
# Begin Source File

SOURCE=.\Styles\04_softwareD.ico
# End Source File
# Begin Source File

SOURCE=.\Styles\05_ksirtet.ico
# End Source File
# Begin Source File

SOURCE=.\res\BACK.BMP
# End Source File
# Begin Source File

SOURCE=.\res\botok.ico
# End Source File
# Begin Source File

SOURCE=.\res\BUTTON.BMP
# End Source File
# Begin Source File

SOURCE=.\res\close.bmp
# End Source File
# Begin Source File

SOURCE=.\res\icon.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon7.ico
# End Source File
# Begin Source File

SOURCE=.\Styles\main_back.BMP
# End Source File
# Begin Source File

SOURCE=.\res\min.bmp
# End Source File
# Begin Source File

SOURCE=.\res\NETtalk2.ico
# End Source File
# Begin Source File

SOURCE=.\res\NETtalk2.rc2
# End Source File
# Begin Source File

SOURCE=.\res\play.bmp
# End Source File
# Begin Source File

SOURCE=.\res\skin.BMP
# End Source File
# Begin Source File

SOURCE=.\res\tombol.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
