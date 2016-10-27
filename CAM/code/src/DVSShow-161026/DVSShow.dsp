# Microsoft Developer Studio Project File - Name="DVSShow" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DVSShow - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DVSShow.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DVSShow.mak" CFG="DVSShow - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DVSShow - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DVSShow - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DVSShow - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /Zi /Od /I "..\..\compile\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 GdiPlus.lib HCNetSDK.lib PlayCtrl.lib ws2_32.lib DsSdk.lib winmm.lib GdiPlus.lib ntsdll.lib /nologo /subsystem:windows /incremental:yes /debug /machine:I386 /libpath:"..\..\compile\lib"
# SUBTRACT LINK32 /pdb:none /map
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy  release\dvsshow.exe  ..\..\bin
# End Special Build Tool

!ELSEIF  "$(CFG)" == "DVSShow - Win32 Debug"

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
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "DVSShow - Win32 Release"
# Name "DVSShow - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AW_CMultiViewSplitter\AW_CMultiViewSplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\Control.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOutput.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPTZPreset.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVideo1.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgVideoSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\DVSShow.cpp
# End Source File
# Begin Source File

SOURCE=.\DVSShow.rc
# End Source File
# Begin Source File

SOURCE=.\DVSShowDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DVSShowView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\PTZButton.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SysSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\VideoOutPutView.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AW_CMultiViewSplitter\AW_CMultiViewSplitter.h
# End Source File
# Begin Source File

SOURCE=.\Control.h
# End Source File
# Begin Source File

SOURCE=.\DlgOutput.h
# End Source File
# Begin Source File

SOURCE=.\DlgPTZPreset.h
# End Source File
# Begin Source File

SOURCE=.\DlgVideo1.h
# End Source File
# Begin Source File

SOURCE=.\DlgVideoSettings.h
# End Source File
# Begin Source File

SOURCE=.\DVSShow.h
# End Source File
# Begin Source File

SOURCE=.\DVSShowDoc.h
# End Source File
# Begin Source File

SOURCE=.\DVSShowView.h
# End Source File
# Begin Source File

SOURCE=.\GeneralDef.h
# End Source File
# Begin Source File

SOURCE=.\include\GeneralDef.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\PTZButton.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SysSettings.h
# End Source File
# Begin Source File

SOURCE=.\VideoOutPutView.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\camera.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DVSShow.ico
# End Source File
# Begin Source File

SOURCE=.\res\DVSShow.rc2
# End Source File
# Begin Source File

SOURCE=.\res\DVSShowDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon_exi.ico
# End Source File
# Begin Source File

SOURCE=.\res\il_tree.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=".\res\关于.ico"
# End Source File
# Begin Source File

SOURCE=".\res\设置.ico"
# End Source File
# Begin Source File

SOURCE=".\res\停止.ico"
# End Source File
# Begin Source File

SOURCE=".\res\退出.ico"
# End Source File
# Begin Source File

SOURCE=".\res\显示.ico"
# End Source File
# Begin Source File

SOURCE=".\res\抓拍.ico"
# End Source File
# End Group
# Begin Group "LeftCoolBar"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\LeftCoolBar\CoolTabCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\LeftCoolBar\CoolTabCtrl.h
# End Source File
# Begin Source File

SOURCE=.\LeftCoolBar\scbarg.cpp
# End Source File
# Begin Source File

SOURCE=.\LeftCoolBar\scbarg.h
# End Source File
# Begin Source File

SOURCE=.\LeftCoolBar\sizecbar.cpp
# End Source File
# Begin Source File

SOURCE=.\LeftCoolBar\sizecbar.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
