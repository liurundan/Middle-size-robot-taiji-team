# Microsoft Developer Studio Project File - Name="ImageProcess" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ImageProcess - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ImageProcess.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ImageProcess.mak" CFG="ImageProcess - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ImageProcess - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ImageProcess - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ImageProcess - Win32 Release"

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
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 pdapi.lib imagelib.lib OpenGL32.lib strmbasd.lib winmm.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "ImageProcess - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 pdapi.lib imagelib.lib OpenGL32.lib strmbasd.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ImageProcess - Win32 Release"
# Name "ImageProcess - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
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

SOURCE=.\Time.cpp
# End Source File
# Begin Source File

SOURCE=.\Vec.cpp
# End Source File
# End Group
# Begin Group "ImageProcessing"

# PROP Default_Filter ""
# Begin Group "ImageMonitors"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FileMonitor.cpp
# End Source File
# End Group
# Begin Group "Images"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ImageConversion.cpp
# End Source File
# Begin Source File

SOURCE=.\RGBImage.cpp
# End Source File
# Begin Source File

SOURCE=.\UYVYImage.cpp
# End Source File
# Begin Source File

SOURCE=.\YUVImage.cpp
# End Source File
# End Group
# Begin Group "ImageSources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Camera.cpp
# End Source File
# Begin Source File

SOURCE=.\FileSource.cpp
# End Source File
# Begin Source File

SOURCE=.\UsbCamera.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\BallDetector.cpp
# End Source File
# Begin Source File

SOURCE=.\ChainCoding.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorClasses.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorClassifier.cpp
# End Source File
# Begin Source File

SOURCE=.\Image.cpp
# End Source File
# Begin Source File

SOURCE=.\Image2WorldMapping.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageIO.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageProcessingMonitor.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageProducer.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageVarianceFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\KoordTransformDortmund.cpp
# End Source File
# Begin Source File

SOURCE=.\LineScanning.cpp
# End Source File
# Begin Source File

SOURCE=.\MorphologicOperators.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiImageProducer.cpp
# End Source File
# Begin Source File

SOURCE=.\Painter.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotMask.cpp
# End Source File
# Begin Source File

SOURCE=.\ScanLines.cpp
# End Source File
# Begin Source File

SOURCE=.\YUVLookupTable.cpp
# End Source File
# End Group
# Begin Group "Dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ImageProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageProcess.rc
# End Source File
# Begin Source File

SOURCE=.\ImageProcessDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\mscomm.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Structures"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DriveVector.cpp
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
# Begin Group "Robot"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\OmniRobot.cpp
# End Source File
# Begin Source File

SOURCE=.\Robot.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotFactory.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Fundamental Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Angle.h
# End Source File
# Begin Source File

SOURCE=.\ConfigReader.h
# End Source File
# Begin Source File

SOURCE=.\Frame2D.h
# End Source File
# Begin Source File

SOURCE=.\geometry.h
# End Source File
# Begin Source File

SOURCE=.\Time.h
# End Source File
# Begin Source File

SOURCE=.\Vec.h
# End Source File
# End Group
# Begin Group "ImageProcessing Headers"

# PROP Default_Filter ""
# Begin Group "ImageMonitors Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\FileMonitor.h
# End Source File
# End Group
# Begin Group "Images Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ImageConversion.h
# End Source File
# Begin Source File

SOURCE=.\RGBImage.h
# End Source File
# Begin Source File

SOURCE=.\UYVYImage.h
# End Source File
# Begin Source File

SOURCE=.\YUVImage.h
# End Source File
# End Group
# Begin Group "ImageSources Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Camera.h
# End Source File
# Begin Source File

SOURCE=.\FileSource.h
# End Source File
# Begin Source File

SOURCE=.\ImageSource.h
# End Source File
# Begin Source File

SOURCE=.\UsbCamera.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\BallDetector.h
# End Source File
# Begin Source File

SOURCE=.\ChainCoding.h
# End Source File
# Begin Source File

SOURCE=.\ColorClasses.h
# End Source File
# Begin Source File

SOURCE=.\ColorClassifier.h
# End Source File
# Begin Source File

SOURCE=.\Image.h
# End Source File
# Begin Source File

SOURCE=.\Image2WorldMapping.h
# End Source File
# Begin Source File

SOURCE=.\ImageBuffer.h
# End Source File
# Begin Source File

SOURCE=.\ImageIO.h
# End Source File
# Begin Source File

SOURCE=.\ImageMonitor.h
# End Source File
# Begin Source File

SOURCE=.\ImageProcessingMonitor.h
# End Source File
# Begin Source File

SOURCE=.\ImageProducer.h
# End Source File
# Begin Source File

SOURCE=.\ImageVarianceFilter.h
# End Source File
# Begin Source File

SOURCE=.\KoordTransformDortmund.h
# End Source File
# Begin Source File

SOURCE=.\LineScanning.h
# End Source File
# Begin Source File

SOURCE=.\MorphologicOperators.h
# End Source File
# Begin Source File

SOURCE=.\MultiImageProducer.h
# End Source File
# Begin Source File

SOURCE=.\Painter.h
# End Source File
# Begin Source File

SOURCE=.\RobotMask.h
# End Source File
# Begin Source File

SOURCE=.\ScanLines.h
# End Source File
# Begin Source File

SOURCE=.\YUVLookupTable.h
# End Source File
# End Group
# Begin Group "Dialogs Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ImageProcess.h
# End Source File
# Begin Source File

SOURCE=.\ImageProcessDlg.h
# End Source File
# Begin Source File

SOURCE=.\mscomm.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Structures Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\DriveVector.h
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
# Begin Group "Robot Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\OmniRobot.h
# End Source File
# Begin Source File

SOURCE=.\Robot.h
# End Source File
# Begin Source File

SOURCE=.\RobotFactory.h
# End Source File
# Begin Source File

SOURCE=.\RobotType.h
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ImageProcess.ico
# End Source File
# Begin Source File

SOURCE=.\res\ImageProcess.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section ImageProcess : {648A5600-2C6E-101B-82B6-000000000014}
# 	2:21:DefaultSinkHeaderFile:mscomm.h
# 	2:16:DefaultSinkClass:CMSComm
# End Section
# Section ImageProcess : {E6E17E90-DF38-11CF-8E74-00A0C90F26F8}
# 	2:5:Class:CMSComm
# 	2:10:HeaderFile:mscomm.h
# 	2:8:ImplFile:mscomm.cpp
# End Section
