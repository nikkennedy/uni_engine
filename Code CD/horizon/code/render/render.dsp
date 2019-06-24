# Microsoft Developer Studio Project File - Name="render" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 60000
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=render - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "render.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "render.mak" CFG="render - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "render - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "render - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "render - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RENDER_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G6 /MD /W3 /GX /O2 /I "C:\Program Files\NVIDIA Corporation\NVIDIA OpenGL SDK\include\glh" /I "C:\Program Files\NVIDIA Corporation\NVIDIA OpenGL SDK\include\shared" /I "..\global\include" /I "..\horizon\include" /I "include" /I "..\ai\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RENDER_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 nvparse.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib /nologo /dll /machine:I386 /nodefaultlib:"libc" /out:"../Release/render.dll" /libpath:"C:\Program Files\NVIDIA Corporation\NVIDIA OpenGL SDK\lib\Release"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "render - Win32 Debug"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RENDER_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /G6 /MDd /W3 /Gm /GX /ZI /I "C:\Program Files\NVIDIA Corporation\NVIDIA OpenGL SDK\include\glh" /I "C:\Program Files\NVIDIA Corporation\NVIDIA OpenGL SDK\include\shared" /I "..\global\include" /I "..\horizon\include" /I "include" /I "..\ai\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "RENDER_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 nvparse.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib opengl32.lib glu32.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"libcd" /out:"../Debug/render.dll" /pdbtype:sept /libpath:"C:\Program Files\NVIDIA Corporation\NVIDIA OpenGL SDK\lib\Debug"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "render - Win32 Release"
# Name "render - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\collisiondetection.cpp
# End Source File
# Begin Source File

SOURCE=.\src\console.cpp
# End Source File
# Begin Source File

SOURCE=..\global\src\hrz_config.cpp
# End Source File
# Begin Source File

SOURCE=..\global\src\hrz_parse.cpp
# End Source File
# Begin Source File

SOURCE=..\global\src\image.cpp
# End Source File
# Begin Source File

SOURCE=.\src\init.cpp
# End Source File
# Begin Source File

SOURCE=..\global\src\object.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ogl_util.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ogl_vp.cpp
# End Source File
# Begin Source File

SOURCE=..\global\src\p_list.cpp
# End Source File
# Begin Source File

SOURCE=..\global\src\parse.cpp
# End Source File
# Begin Source File

SOURCE=..\global\src\sprite.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\global\include\console.h
# End Source File
# Begin Source File

SOURCE=..\global\include\hrz_config.h
# End Source File
# Begin Source File

SOURCE=..\global\include\image.h
# End Source File
# Begin Source File

SOURCE=.\include\render_api.h
# End Source File
# Begin Source File

SOURCE=..\global\include\sprite.h
# End Source File
# Begin Source File

SOURCE=..\global\include\vert3d.h
# End Source File
# End Group
# Begin Group "nvidia"

# PROP Default_Filter ""
# Begin Source File

SOURCE="C:\Program Files\NVIDIA Corporation\NVIDIA OpenGL SDK\include\glh\GL\GLEXT.H"

!IF  "$(CFG)" == "render - Win32 Release"

!ELSEIF  "$(CFG)" == "render - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE="C:\Program Files\NVIDIA Corporation\NVIDIA OpenGL SDK\include\glh\glh_extensions.h"
# End Source File
# Begin Source File

SOURCE="C:\Program Files\NVIDIA Corporation\NVIDIA OpenGL SDK\include\glh\glh_genext.h"
# End Source File
# End Group
# Begin Group "Global Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\global\src\debug.cpp
# End Source File
# Begin Source File

SOURCE=..\global\src\maths.cpp
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
# Begin Group "Particles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\ogl_particlesystem.cpp
# End Source File
# Begin Source File

SOURCE=.\include\ogl_particlesystem.h
# End Source File
# Begin Source File

SOURCE=..\global\include\particle.h
# End Source File
# Begin Source File

SOURCE=..\global\src\particlesystem.cpp
# End Source File
# Begin Source File

SOURCE=..\global\include\particlesystem.h
# End Source File
# End Group
# Begin Group "Renderer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\ogl.cpp
# End Source File
# Begin Source File

SOURCE=.\include\ogl.h
# End Source File
# Begin Source File

SOURCE=.\src\rend.cpp
# End Source File
# Begin Source File

SOURCE=.\include\rend.h
# End Source File
# End Group
# Begin Group "BSP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\global\src\lbsp.cpp
# End Source File
# Begin Source File

SOURCE=..\global\include\lbsp.h
# End Source File
# End Group
# Begin Group "Textures"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\alphatexture.cpp
# End Source File
# Begin Source File

SOURCE=.\include\alphatexture.h
# End Source File
# Begin Source File

SOURCE=.\src\basictexture.cpp
# End Source File
# Begin Source File

SOURCE=.\include\basictexture.h
# End Source File
# Begin Source File

SOURCE=.\src\invistexture.cpp
# End Source File
# Begin Source File

SOURCE=.\include\invistexture.h
# End Source File
# Begin Source File

SOURCE=.\src\lightmap.cpp
# End Source File
# Begin Source File

SOURCE=.\include\lightmap.h
# End Source File
# Begin Source File

SOURCE=.\src\ogl_texturemanager.cpp
# End Source File
# Begin Source File

SOURCE=.\include\ogl_texturemanager.h
# End Source File
# Begin Source File

SOURCE=.\src\texture.cpp
# End Source File
# Begin Source File

SOURCE=.\include\texture.h
# End Source File
# Begin Source File

SOURCE=.\src\texturemanager.cpp
# End Source File
# Begin Source File

SOURCE=.\include\texturemanager.h
# End Source File
# End Group
# Begin Group "MD2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\global\src\md2.cpp
# End Source File
# Begin Source File

SOURCE=..\global\include\md2.h
# End Source File
# Begin Source File

SOURCE=.\src\ogl_md2_renderer.cpp
# End Source File
# End Group
# End Target
# End Project
