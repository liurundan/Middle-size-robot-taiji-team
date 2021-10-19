# Microsoft Developer Studio Project File - Name="TeamControl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TeamControl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TeamControl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TeamControl.mak" CFG="TeamControl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TeamControl - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TeamControl - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TeamControl - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 pdapi.lib ws2_32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "TeamControl - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 pdapi.lib ws2_32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "TeamControl - Win32 Release"
# Name "TeamControl - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Communication"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RobotsUdpCommunication.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotsUdpCommunicationBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\udpsocket.cpp
# End Source File
# End Group
# Begin Group "Dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CycleInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\EditPopup.cpp
# End Source File
# Begin Source File

SOURCE=.\FieldOfPlay.cpp
# End Source File
# Begin Source File

SOURCE=.\FieldWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\JoystickDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\RealRobotData.cpp
# End Source File
# Begin Source File

SOURCE=.\RemoteRobotWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusColor.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TeamControl.cpp
# End Source File
# Begin Source File

SOURCE=.\TeamControl.rc
# End Source File
# Begin Source File

SOURCE=.\TeamControlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ValPlotWidget.cpp
# End Source File
# End Group
# Begin Group "Fundamental"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Angle.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigReader.cpp
# End Source File
# Begin Source File

SOURCE=.\Frame2D.cpp
# End Source File
# Begin Source File

SOURCE=.\geometry.cpp
# End Source File
# Begin Source File

SOURCE=.\stringconvert.cpp
# End Source File
# Begin Source File

SOURCE=.\Time.cpp
# End Source File
# Begin Source File

SOURCE=.\Vec.cpp
# End Source File
# End Group
# Begin Group "Structures"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BallLocation.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveVector.cpp
# End Source File
# Begin Source File

SOURCE=.\FieldGeometry.cpp
# End Source File
# Begin Source File

SOURCE=.\Journal.cpp
# End Source File
# Begin Source File

SOURCE=.\MessageBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\ObstacleLocation.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotData.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotsException.cpp
# End Source File
# Begin Source File

SOURCE=.\VisibleObject.cpp
# End Source File
# End Group
# Begin Group "Refbox"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RefboxClient.cpp
# End Source File
# Begin Source File

SOURCE=.\RefboxWidget.cpp
# End Source File
# End Group
# Begin Group "Coach"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Coach.cpp
# End Source File
# Begin Source File

SOURCE=.\CoachWidget.cpp
# End Source File
# Begin Source File

SOURCE=.\Policy.cpp
# End Source File
# End Group
# Begin Group "WorldModel"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RefereeStateMachine.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Communication Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RobotsUdpCommunication.h
# End Source File
# Begin Source File

SOURCE=.\RobotsUdpCommunicationBuffer.h
# End Source File
# Begin Source File

SOURCE=.\udpsocket.h
# End Source File
# End Group
# Begin Group "Dialogs Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CycleInfo.h
# End Source File
# Begin Source File

SOURCE=.\EditPopup.h
# End Source File
# Begin Source File

SOURCE=.\FieldOfPlay.h
# End Source File
# Begin Source File

SOURCE=.\FieldWidget.h
# End Source File
# Begin Source File

SOURCE=.\JoystickDialog.h
# End Source File
# Begin Source File

SOURCE=.\MessageDialog.h
# End Source File
# Begin Source File

SOURCE=.\PaintPreferences.h
# End Source File
# Begin Source File

SOURCE=.\Param.h
# End Source File
# Begin Source File

SOURCE=.\RealRobotData.h
# End Source File
# Begin Source File

SOURCE=.\RemoteRobotState.h
# End Source File
# Begin Source File

SOURCE=.\RemoteRobotWidget.h
# End Source File
# Begin Source File

SOURCE=.\RemoteTeamState.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StatusColor.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TeamControl.h
# End Source File
# Begin Source File

SOURCE=.\TeamControlDlg.h
# End Source File
# Begin Source File

SOURCE=.\ValPlotWidget.h
# End Source File
# End Group
# Begin Group "Fundamental Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Angle.h
# End Source File
# Begin Source File

SOURCE=.\ConfigReader.h
# End Source File
# Begin Source File

SOURCE=.\DynamicRingBuffer.h
# End Source File
# Begin Source File

SOURCE=.\Frame2D.h
# End Source File
# Begin Source File

SOURCE=.\geometry.h
# End Source File
# Begin Source File

SOURCE=.\random.h
# End Source File
# Begin Source File

SOURCE=.\RingBuffer.h
# End Source File
# Begin Source File

SOURCE=.\stringconvert.h
# End Source File
# Begin Source File

SOURCE=.\Table.h
# End Source File
# Begin Source File

SOURCE=.\Time.h
# End Source File
# Begin Source File

SOURCE=.\Vec.h
# End Source File
# End Group
# Begin Group "Structures Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BallLocation.h
# End Source File
# Begin Source File

SOURCE=.\DriveVector.h
# End Source File
# Begin Source File

SOURCE=.\FieldGeometry.h
# End Source File
# Begin Source File

SOURCE=.\GameState.h
# End Source File
# Begin Source File

SOURCE=.\Journal.h
# End Source File
# Begin Source File

SOURCE=.\MessageBoard.h
# End Source File
# Begin Source File

SOURCE=.\ObstacleLocation.h
# End Source File
# Begin Source File

SOURCE=.\PlayerRole.h
# End Source File
# Begin Source File

SOURCE=.\RobotData.h
# End Source File
# Begin Source File

SOURCE=.\RobotLocation.h
# End Source File
# Begin Source File

SOURCE=.\RobotProperties.h
# End Source File
# Begin Source File

SOURCE=.\RobotsException.h
# End Source File
# Begin Source File

SOURCE=.\VisibleObject.h
# End Source File
# End Group
# Begin Group "Refbox Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RefboxClient.h
# End Source File
# Begin Source File

SOURCE=.\RefboxWidget.h
# End Source File
# End Group
# Begin Group "Coach Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Coach.h
# End Source File
# Begin Source File

SOURCE=.\CoachWidget.h
# End Source File
# Begin Source File

SOURCE=.\Policy.h
# End Source File
# End Group
# Begin Group "WorldModel Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RefereeStateMachine.h
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Displayer.ico
# End Source File
# Begin Source File

SOURCE=.\res\IDB_TOOLBAR_COLD.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDB_TOOLBAR_DISABLE.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IDB_TOOLBAR_HOT.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TeamControl.ico
# End Source File
# Begin Source File

SOURCE=.\res\TeamControl.rc2
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
