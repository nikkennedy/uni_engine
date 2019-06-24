# Microsoft Developer Studio Project File - Name="horizon" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=horizon - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "horizon.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "horizon.mak" CFG="horizon - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "horizon - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "horizon - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "horizon - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /GX /O2 /I "E:\Stuff\working\horizon\horizon\include" /I "E:\Stuff\working\horizon\global\include" /I "E:\Stuff\working\horizon\render\include" /I "E:\Stuff\working\horizon\ai\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 dxguid.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "horizon - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MTd /W3 /Gm /GX /ZI /Od /I "E:\Stuff\working\horizon\horizon\include" /I "E:\Stuff\working\horizon\global\include" /I "E:\Stuff\working\horizon\render\include" /I "E:\Stuff\working\horizon\ai\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dxguid.lib dxerr9.lib dinput8.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "horizon - Win32 Release"
# Name "horizon - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\horizon\src\ai.cpp
# End Source File
# Begin Source File

SOURCE=.\horizon\src\game.cpp
# End Source File
# Begin Source File

SOURCE=.\horizon\src\main.cpp

!IF  "$(CFG)" == "horizon - Win32 Release"

!ELSEIF  "$(CFG)" == "horizon - Win32 Debug"

# ADD CPP /I "render\include\\"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\horizon\src\player.cpp
# End Source File
# Begin Source File

SOURCE=.\horizon\src\render.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\horizon\include\ai.h
# End Source File
# Begin Source File

SOURCE=.\ai\include\bot.h
# End Source File
# Begin Source File

SOURCE=.\horizon\include\bullet.h
# End Source File
# Begin Source File

SOURCE=.\global\include\collectable.h
# End Source File
# Begin Source File

SOURCE=.\global\include\dxstuff.h
# End Source File
# Begin Source File

SOURCE=.\horizon\include\game.h
# End Source File
# Begin Source File

SOURCE=.\global\include\object.h
# End Source File
# Begin Source File

SOURCE=.\horizon\include\player.h
# End Source File
# Begin Source File

SOURCE=.\render\include\rend.h
# End Source File
# Begin Source File

SOURCE=.\horizon\include\render.h
# End Source File
# Begin Source File

SOURCE=.\render\include\render_api.h
# End Source File
# Begin Source File

SOURCE=.\global\include\sprite.h
# End Source File
# Begin Source File

SOURCE=.\global\include\waypoint.h
# End Source File
# Begin Source File

SOURCE=.\horizon\include\win.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "Global Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\horizon\src\bullet.cpp
# End Source File
# Begin Source File

SOURCE=.\global\src\collectable.cpp
# End Source File
# Begin Source File

SOURCE=.\global\src\directx.cpp
# End Source File
# Begin Source File

SOURCE=.\global\src\hrz_config.cpp
# End Source File
# Begin Source File

SOURCE=.\global\src\hrz_parse.cpp
# End Source File
# Begin Source File

SOURCE=.\global\src\lbsp.cpp
# End Source File
# Begin Source File

SOURCE=.\global\src\maths.cpp
# End Source File
# Begin Source File

SOURCE=.\global\src\md2.cpp
# End Source File
# Begin Source File

SOURCE=.\global\src\object.cpp
# End Source File
# Begin Source File

SOURCE=.\global\src\p_list.cpp
# End Source File
# Begin Source File

SOURCE=.\global\src\parse.cpp
# End Source File
# Begin Source File

SOURCE=.\global\src\particlesystem.cpp
# End Source File
# Begin Source File

SOURCE=.\global\src\sprite.cpp
# End Source File
# Begin Source File

SOURCE=.\global\src\waypoint.cpp
# End Source File
# Begin Source File

SOURCE=.\horizon\src\win.cpp
# End Source File
# End Group
# Begin Group "Global Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\global\include\hrz_config.h
# End Source File
# End Group
# End Target
# End Project
