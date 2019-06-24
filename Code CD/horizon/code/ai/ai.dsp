# Microsoft Developer Studio Project File - Name="ai" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ai - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ai.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ai.mak" CFG="ai - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ai - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ai - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ai - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AI_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /I "..\horizon\include" /I "..\global\include" /I "..\ai\include" /I "..\render\include" /I "c:\program files\dxsdk\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AI_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 dxguid.lib dxerr9.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../Release/gameai.dll"

!ELSEIF  "$(CFG)" == "ai - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AI_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /Od /I "E:\Stuff\working\horizon\ai\include" /I "E:\Stuff\working\horizon\global\include" /I "E:\Stuff\working\horizon\render\include" /I "E:\Stuff\working\horizon\horizon\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "AI_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dxguid.lib dxerr9.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib /nologo /dll /debug /machine:I386 /out:"../Debug/gameai.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ai - Win32 Release"
# Name "ai - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\aibase.cpp
# End Source File
# Begin Source File

SOURCE=.\src\aibasic.cpp
# End Source File
# Begin Source File

SOURCE=.\src\bot.cpp
# End Source File
# Begin Source File

SOURCE=..\horizon\src\bullet.cpp
# End Source File
# Begin Source File

SOURCE=..\global\src\hrz_config.cpp
# End Source File
# Begin Source File

SOURCE=..\global\src\hrz_parse.cpp
# End Source File
# Begin Source File

SOURCE=.\src\init.cpp
# End Source File
# Begin Source File

SOURCE=..\global\src\md2.cpp
# End Source File
# Begin Source File

SOURCE=..\global\src\object.cpp
# End Source File
# Begin Source File

SOURCE=..\global\src\parse.cpp
# End Source File
# Begin Source File

SOURCE=..\global\src\particlesystem.cpp
# End Source File
# Begin Source File

SOURCE=..\global\src\waypoint.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\global\include\aibase.h
# End Source File
# Begin Source File

SOURCE=.\include\aibasic.h
# End Source File
# Begin Source File

SOURCE=.\include\bot.h
# End Source File
# Begin Source File

SOURCE=..\horizon\include\bullet.h
# End Source File
# Begin Source File

SOURCE=..\global\include\object.h
# End Source File
# Begin Source File

SOURCE=..\global\include\waypoint.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Global Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\global\src\debug.cpp
# End Source File
# Begin Source File

SOURCE=..\global\src\lbsp.cpp
# End Source File
# Begin Source File

SOURCE=..\global\src\maths.cpp
# End Source File
# Begin Source File

SOURCE=..\global\src\p_list.cpp
# End Source File
# End Group
# Begin Group "Global Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\global\include\debug.h
# End Source File
# Begin Source File

SOURCE=..\global\include\maths.h
# End Source File
# Begin Source File

SOURCE=..\global\include\types.h
# End Source File
# End Group
# End Target
# End Project
