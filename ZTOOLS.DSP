# Microsoft Developer Studio Project File - Name="ztools" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ztools - Win32 LIB Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ZTOOLS.MAK".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ZTOOLS.MAK" CFG="ztools - Win32 LIB Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ztools - Win32 LIB Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "."
# PROP BASE Intermediate_Dir "."
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "."
# PROP Intermediate_Dir "."
# PROP Target_Dir ""
LINK32=link.exe
MTL=midl.exe
# ADD BASE CPP /nologo /Gz /MD /W3 /GX /Gy /I ".\inc" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_LIB" /Zl /FD /Oxs /GF /c
# ADD CPP /nologo /Gz /MD /W3 /GX /Gy /I ".\inc" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "_LIB" /Zl /FD /Oxs /GF /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /NODEFAULTLIB
# ADD LIB32 /nologo /NODEFAULTLIB
# Begin Target

# Name "ztools - Win32 LIB Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ctime2l.c
# End Source File
# Begin Source File

SOURCE=.\curdir.c
# End Source File
# Begin Source File

SOURCE=.\cvtoem.c
# End Source File
# Begin Source File

SOURCE=.\dodel.c
# End Source File
# Begin Source File

SOURCE=.\error.c
# End Source File
# Begin Source File

SOURCE=.\exetype.c
# End Source File
# Begin Source File

SOURCE=.\fappend.c
# End Source File
# Begin Source File

SOURCE=.\fastcopy.c
# End Source File
# Begin Source File

SOURCE=.\fcopy.c
# End Source File
# Begin Source File

SOURCE=.\fdelete.c
# End Source File
# Begin Source File

SOURCE=.\fexpunge.c
# End Source File
# Begin Source File

SOURCE=.\fgetl.c
# End Source File
# Begin Source File

SOURCE=.\find.c
# End Source File
# Begin Source File

SOURCE=.\fmove.c
# End Source File
# Begin Source File

SOURCE=.\forfile.c
# End Source File
# Begin Source File

SOURCE=.\forsemi.c
# End Source File
# Begin Source File

SOURCE=.\fputl.c
# End Source File
# Begin Source File

SOURCE=.\freespac.c
# End Source File
# Begin Source File

SOURCE=.\init.c
# End Source File
# Begin Source File

SOURCE=.\match.c
# End Source File
# Begin Source File

SOURCE=.\mgetl.c
# End Source File
# Begin Source File

SOURCE=.\ntoi.c
# End Source File
# Begin Source File

SOURCE=.\pathcat.c
# End Source File
# Begin Source File

SOURCE=.\pathchr.c
# End Source File
# Begin Source File

SOURCE=.\pathopen.c
# End Source File
# Begin Source File

SOURCE=.\pname.c
# End Source File
# Begin Source File

SOURCE=.\re_mi.c
# End Source File
# Begin Source File

SOURCE=.\reparse.c
# End Source File
# Begin Source File

SOURCE=.\root.c
# End Source File
# Begin Source File

SOURCE=.\strcmps.c
# End Source File
# Begin Source File

SOURCE=.\string.c
# End Source File
# Begin Source File

SOURCE=.\swchng.c
# End Source File
# Begin Source File

SOURCE=.\update.c
# End Source File
# Begin Source File

SOURCE=.\vector.c
# End Source File
# Begin Source File

SOURCE=.\xtab.c
# End Source File
# Begin Source File

SOURCE=.\ztoolasm.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\inc\config.h
# End Source File
# Begin Source File

SOURCE=.\inc\cvtoem.h
# End Source File
# Begin Source File

SOURCE=.\inc\dir.h
# End Source File
# Begin Source File

SOURCE=.\inc\exe.h
# End Source File
# Begin Source File

SOURCE=.\inc\fcb.h
# End Source File
# Begin Source File

SOURCE=.\inc\parse.h
# End Source File
# Begin Source File

SOURCE=.\inc\re.h
# End Source File
# Begin Source File

SOURCE=.\inc\remi.h
# End Source File
# Begin Source File

SOURCE=.\inc\rm.h
# End Source File
# Begin Source File

SOURCE=.\inc\times.h
# End Source File
# Begin Source File

SOURCE=.\inc\tools.h
# End Source File
# Begin Source File

SOURCE=.\inc\ttypes.h
# End Source File
# End Group
# End Target
# End Project
