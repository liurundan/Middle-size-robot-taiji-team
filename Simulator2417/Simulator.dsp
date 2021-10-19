# Microsoft Developer Studio Project File - Name="Simulator" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Simulator - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Simulator.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Simulator.mak" CFG="Simulator - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Simulator - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Simulator - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Simulator - Win32 Release"

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
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Winmm.lib OpenGL32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /incremental:yes /debug

!ELSEIF  "$(CFG)" == "Simulator - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# SUBTRACT RSC /x
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Winmm.lib OpenGL32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /incremental:no

!ENDIF 

# Begin Target

# Name "Simulator - Win32 Release"
# Name "Simulator - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Agent.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorClasses.cpp
# End Source File
# Begin Source File

SOURCE=.\Command.cpp
# End Source File
# Begin Source File

SOURCE=.\Display.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveVector.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorMinimiserSL.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorMinimiserWorldModel.cpp
# End Source File
# Begin Source File

SOURCE=.\FieldGeometry.cpp
# End Source File
# Begin Source File

SOURCE=.\FieldLUT.cpp
# End Source File
# Begin Source File

SOURCE=.\Geometry.cpp
# End Source File
# Begin Source File

SOURCE=.\LineScanning.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\MyAngle.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTime.cpp
# End Source File
# Begin Source File

SOURCE=.\Objects.cpp
# End Source File
# Begin Source File

SOURCE=.\OdometryContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotPositionKalmanFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\ScanLines.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator.cpp
# End Source File
# Begin Source File

SOURCE=.\Simulator.rc
# End Source File
# Begin Source File

SOURCE=.\SimulatorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SoccerServer.cpp
# End Source File
# Begin Source File

SOURCE=.\SoccerTypes.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\update_robot_location.cpp
# End Source File
# Begin Source File

SOURCE=.\VisibleObject.cpp
# End Source File
# Begin Source File

SOURCE=.\VisualContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\VisualPositionOptimiser.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldModel.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldModelHighLevel.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldModelPredict.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldModelTypeBase.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldModelUpdate.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Agent.h
# End Source File
# Begin Source File

SOURCE=.\ColorClasses.h
# End Source File
# Begin Source File

SOURCE=.\Command.h
# End Source File
# Begin Source File

SOURCE=.\Display.h
# End Source File
# Begin Source File

SOURCE=.\DriveVector.h
# End Source File
# Begin Source File

SOURCE=.\ErrorMinimiserSL.h
# End Source File
# Begin Source File

SOURCE=.\ErrorMinimiserWorldModel.h
# End Source File
# Begin Source File

SOURCE=.\FieldGeometry.h
# End Source File
# Begin Source File

SOURCE=.\FieldLUT.h
# End Source File
# Begin Source File

SOURCE=.\GameState.h
# End Source File
# Begin Source File

SOURCE=.\Geometry.h
# End Source File
# Begin Source File

SOURCE=.\LineScanning.h
# End Source File
# Begin Source File

SOURCE=.\MessageBoard.h
# End Source File
# Begin Source File

SOURCE=.\MyAngle.h
# End Source File
# Begin Source File

SOURCE=.\MyTime.h
# End Source File
# Begin Source File

SOURCE=.\Objects.h
# End Source File
# Begin Source File

SOURCE=.\OdometryContainer.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RingBuffer.h
# End Source File
# Begin Source File

SOURCE=.\RobotLocation.h
# End Source File
# Begin Source File

SOURCE=.\RobotPositionKalmanFilter.h
# End Source File
# Begin Source File

SOURCE=.\ScanLines.h
# End Source File
# Begin Source File

SOURCE=.\Simulator.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorDlg.h
# End Source File
# Begin Source File

SOURCE=.\SoccerServer.h
# End Source File
# Begin Source File

SOURCE=.\SoccerTypes.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\update_robot_location.h
# End Source File
# Begin Source File

SOURCE=.\VisibleObject.h
# End Source File
# Begin Source File

SOURCE=.\VisualContainer.h
# End Source File
# Begin Source File

SOURCE=.\VisualPositionOptimiser.h
# End Source File
# Begin Source File

SOURCE=.\WorldModel.h
# End Source File
# Begin Source File

SOURCE=.\WorldModelType.h
# End Source File
# Begin Source File

SOURCE=.\WorldModelTypeBase.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Simulator.ico
# End Source File
# Begin Source File

SOURCE=.\res\Simulator.rc2
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
