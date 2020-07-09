# Microsoft Developer Studio Project File - Name="oflash" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=oflash - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "oflash.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "oflash.mak" CFG="oflash - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "oflash - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "oflash - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "oflash - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /Od /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D BUILD_FT2232_FTD2XX=1 /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib lib\FTD2XX.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "oflash - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib lib/FTD2XX.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "oflash - Win32 Release"
# Name "oflash - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\common\binarybuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\nor\cfi_flash.cpp
# End Source File
# Begin Source File

SOURCE=.\config.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\jtag\dongle\dongle_jtag.cpp
# End Source File
# Begin Source File

SOURCE=.\common\flash.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\ft2232\ft2232.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\jtag\interface.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\nor\jedec_flash.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\jtag\jtag.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\jtag\mem_rdwr.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\nand\nand.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\nand\nand_base.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\nand\nand_ecc.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\nand\nand_ids.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\nand\nand_menu.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\nor\nor_menu.cpp
# End Source File
# Begin Source File

SOURCE=.\oflash.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\jtag\openjtag\openjtag_jtag.cpp
# End Source File
# Begin Source File

SOURCE=.\cpu\arm\s3c24x0\pin2410.cpp
# End Source File
# Begin Source File

SOURCE=.\cpu\arm\s3c24x0\pin2440.cpp
# End Source File
# Begin Source File

SOURCE=.\cpu\arm\s3c2450\pin2450.cpp
# End Source File
# Begin Source File

SOURCE=.\cpu\arm\s3c6410\pin6410.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\jtag\dongle\ppt.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\nand\s3c2410.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\jtag\s3c2410_mem_rdwr.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\nand\s3c2440.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\jtag\s3c2440_mem_rdwr.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\jtag\s3c2450_mem_rdwr.cpp
# End Source File
# Begin Source File

SOURCE=.\cpu\arm\s3c24x0\s3c24x0.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\nand\s3c6410.cpp
# End Source File
# Begin Source File

SOURCE=.\drivers\jtag\s3c6410_mem_rdwr.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc
# End Source File
# Begin Source File

SOURCE=.\drivers\jtag\wiggler\wiggler_jtag.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
