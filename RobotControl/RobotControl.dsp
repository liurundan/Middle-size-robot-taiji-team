# Microsoft Developer Studio Project File - Name="RobotControl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=RobotControl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RobotControl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RobotControl.mak" CFG="RobotControl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RobotControl - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "RobotControl - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RobotControl - Win32 Release"

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
# ADD LINK32 pdapi.lib ws2_32.lib strmbasd.lib winmm.lib /nologo /subsystem:windows /machine:I386 /OPT:NOREF
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "RobotControl - Win32 Debug"

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
# ADD LINK32 pdapi.lib ws2_32.lib strmbasd.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "RobotControl - Win32 Release"
# Name "RobotControl - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Behavior"

# PROP Default_Filter ""
# Begin Group "Skills"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ApproachBall.cpp
# End Source File
# Begin Source File

SOURCE=.\Avoid.cpp
# End Source File
# Begin Source File

SOURCE=.\BallGet.cpp
# End Source File
# Begin Source File

SOURCE=.\BasicCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\BasicFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\Defense.cpp
# End Source File
# Begin Source File

SOURCE=.\Dribble.cpp
# End Source File
# Begin Source File

SOURCE=.\FaceBall.cpp
# End Source File
# Begin Source File

SOURCE=.\GotoPos.cpp
# End Source File
# Begin Source File

SOURCE=.\InterceptBall.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveWithBall.cpp
# End Source File
# Begin Source File

SOURCE=.\Offence.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchBall.cpp
# End Source File
# Begin Source File

SOURCE=.\SPhysGotoPos.cpp
# End Source File
# Begin Source File

SOURCE=.\StuckBehavior.cpp
# End Source File
# Begin Source File

SOURCE=.\Tools.cpp
# End Source File
# Begin Source File

SOURCE=.\Touch.cpp
# End Source File
# Begin Source File

SOURCE=.\WSpset.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\AttackPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\Behavior.cpp
# End Source File
# Begin Source File

SOURCE=.\DefendPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\GoalieTaiji.cpp
# End Source File
# Begin Source File

SOURCE=.\SupportPlayer.cpp
# End Source File
# End Group
# Begin Group "Communication"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RobotsUdpCommunication.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotsUdpCommunicationBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotsUdpServer.cpp
# End Source File
# Begin Source File

SOURCE=.\SerialPort.cpp
# End Source File
# Begin Source File

SOURCE=.\udpsocket.cpp
# End Source File
# End Group
# Begin Group "ControlLoop"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ControlLoop.cpp
# End Source File
# End Group
# Begin Group "Dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RobotControl.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotControl.rc
# End Source File
# Begin Source File

SOURCE=.\RobotControlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
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

SOURCE=.\ContourObstacleDetector.cpp
# End Source File
# Begin Source File

SOURCE=.\DistanceCalibration.cpp
# End Source File
# Begin Source File

SOURCE=.\FieldMapper.cpp
# End Source File
# Begin Source File

SOURCE=.\GoalDetector.cpp
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

SOURCE=.\ImageProcessing.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageProcessingDistanceCalibration.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageProcessingDummy.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageProcessingFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageProducer.cpp
# End Source File
# Begin Source File

SOURCE=.\KoordTransformDortmund.cpp
# End Source File
# Begin Source File

SOURCE=.\LineDetector.cpp
# End Source File
# Begin Source File

SOURCE=.\LineScanning.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiImageProducer.cpp
# End Source File
# Begin Source File

SOURCE=.\ObstacleDetector.cpp
# End Source File
# Begin Source File

SOURCE=.\Painter.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotMask.cpp
# End Source File
# Begin Source File

SOURCE=.\ScanLineImageProcessing.cpp
# End Source File
# Begin Source File

SOURCE=.\ScanLines.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpleCluster.cpp
# End Source File
# Begin Source File

SOURCE=.\SimpleObstacleDetector.cpp
# End Source File
# Begin Source File

SOURCE=.\YUVLookupTable.cpp
# End Source File
# End Group
# Begin Group "Player"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AddGotoPosPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\AddJoystickPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\Goalie.cpp
# End Source File
# Begin Source File

SOURCE=.\JoystickPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\PhysicalMotionModel.cpp
# End Source File
# Begin Source File

SOURCE=.\Player.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerDummy.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayerFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\RCPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\WhiteBoard.cpp
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

SOURCE=.\RobotDummy.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotFactory.cpp
# End Source File
# End Group
# Begin Group "Structures"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BallLocation.cpp
# End Source File
# Begin Source File

SOURCE=.\BallLocationReadWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveVector.cpp
# End Source File
# Begin Source File

SOURCE=.\DriveVectorReadWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\FieldGeometry.cpp
# End Source File
# Begin Source File

SOURCE=.\GameStateReadWriter.cpp
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

SOURCE=.\ObstacleLocationReadWriter.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotData.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotLocationReadWriter.cpp
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
# Begin Source File

SOURCE=.\VisibleObjectReadWriter.cpp
# End Source File
# End Group
# Begin Group "UserInterface"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AddComUserInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\TerminalUserInterface.cpp
# End Source File
# Begin Source File

SOURCE=.\UserInterface.cpp
# End Source File
# End Group
# Begin Group "WorldModel"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AddWriteWorldModel.cpp
# End Source File
# Begin Source File

SOURCE=.\CompassContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\dataClass.cpp
# End Source File
# Begin Source File

SOURCE=.\DynamicSlidingWindowBallFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\EMAObstacleFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorMinimiserSL.cpp
# End Source File
# Begin Source File

SOURCE=.\ErrorMinimiserWorldModel.cpp
# End Source File
# Begin Source File

SOURCE=.\FieldLUT.cpp
# End Source File
# Begin Source File

SOURCE=.\GameStateManager.cpp
# End Source File
# Begin Source File

SOURCE=.\LocationShortTimeMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectInteractionManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ObstacleContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\OdometryContainer.cpp
# End Source File
# Begin Source File

SOURCE=.\RefereeStateMachine.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotPositionKalmanFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\RobotPositionPredictor.cpp
# End Source File
# Begin Source File

SOURCE=.\SLStuckSensor.cpp
# End Source File
# Begin Source File

SOURCE=.\SLVelocitySensor.cpp
# End Source File
# Begin Source File

SOURCE=.\update_robot_location.cpp
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

SOURCE=.\WorldModelDummy.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldModelFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\WorldModelTypeBase.cpp
# End Source File
# End Group
# Begin Group "Simulator"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\odesimcomm.cpp
# End Source File
# Begin Source File

SOURCE=.\SimClient.cpp
# End Source File
# Begin Source File

SOURCE=.\SimulatedRobot.cpp
# End Source File
# Begin Source File

SOURCE=.\SimulatorImageProcessing.cpp
# End Source File
# Begin Source File

SOURCE=.\SimulatorWorldModel.cpp
# End Source File
# Begin Source File

SOURCE=.\udpsocket_ode.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Behavior Headers"

# PROP Default_Filter ""
# Begin Group "Skills Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ApproachBall.h
# End Source File
# Begin Source File

SOURCE=.\Avoid.h
# End Source File
# Begin Source File

SOURCE=.\BallGet.h
# End Source File
# Begin Source File

SOURCE=.\BasicCmd.h
# End Source File
# Begin Source File

SOURCE=.\BasicFunctions.h
# End Source File
# Begin Source File

SOURCE=.\CmdGenerator.h
# End Source File
# Begin Source File

SOURCE=.\Defense.h
# End Source File
# Begin Source File

SOURCE=.\Dribble.h
# End Source File
# Begin Source File

SOURCE=.\FaceBall.h
# End Source File
# Begin Source File

SOURCE=.\GotoPos.h
# End Source File
# Begin Source File

SOURCE=.\InterceptBall.h
# End Source File
# Begin Source File

SOURCE=.\MoveWithBall.h
# End Source File
# Begin Source File

SOURCE=.\Offence.h
# End Source File
# Begin Source File

SOURCE=.\SearchBall.h
# End Source File
# Begin Source File

SOURCE=.\SPhysGotoPos.h
# End Source File
# Begin Source File

SOURCE=.\StuckBehavior.h
# End Source File
# Begin Source File

SOURCE=.\Tools.h
# End Source File
# Begin Source File

SOURCE=.\Touch.h
# End Source File
# Begin Source File

SOURCE=.\WSpset.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AttackPlayer.h
# End Source File
# Begin Source File

SOURCE=.\Behavior.h
# End Source File
# Begin Source File

SOURCE=.\DefendPlayer.h
# End Source File
# Begin Source File

SOURCE=.\GoalieTaiji.h
# End Source File
# Begin Source File

SOURCE=.\InPlayParameters.h
# End Source File
# Begin Source File

SOURCE=.\OffenceParameter.h
# End Source File
# Begin Source File

SOURCE=.\parameters.h
# End Source File
# Begin Source File

SOURCE=.\position.h
# End Source File
# Begin Source File

SOURCE=.\SupportPlayer.h
# End Source File
# End Group
# Begin Group "Communication Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\RobotsUdpCommunication.h
# End Source File
# Begin Source File

SOURCE=.\RobotsUdpCommunicationBuffer.h
# End Source File
# Begin Source File

SOURCE=.\RobotsUdpServer.h
# End Source File
# Begin Source File

SOURCE=.\SerialPort.h
# End Source File
# Begin Source File

SOURCE=.\udpsocket.h
# End Source File
# End Group
# Begin Group "ControlLoop Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ControlLoop.h
# End Source File
# End Group
# Begin Group "Dialogs Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RobotControl.h
# End Source File
# Begin Source File

SOURCE=.\RobotControlDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
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

SOURCE=.\ContourObstacleDetector.h
# End Source File
# Begin Source File

SOURCE=.\DistanceCalibration.h
# End Source File
# Begin Source File

SOURCE=.\FieldMapper.h
# End Source File
# Begin Source File

SOURCE=.\GoalDetector.h
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

SOURCE=.\ImageProcessing.h
# End Source File
# Begin Source File

SOURCE=.\ImageProcessingDistanceCalibration.h
# End Source File
# Begin Source File

SOURCE=.\ImageProcessingDummy.h
# End Source File
# Begin Source File

SOURCE=.\ImageProcessingFactory.h
# End Source File
# Begin Source File

SOURCE=.\ImageProcessingType.h
# End Source File
# Begin Source File

SOURCE=.\ImageProducer.h
# End Source File
# Begin Source File

SOURCE=.\ImageSource.h
# End Source File
# Begin Source File

SOURCE=.\KoordTransformDortmund.h
# End Source File
# Begin Source File

SOURCE=.\LineDetector.h
# End Source File
# Begin Source File

SOURCE=.\LineScanning.h
# End Source File
# Begin Source File

SOURCE=.\MultiImageProducer.h
# End Source File
# Begin Source File

SOURCE=.\ObstacleDetector.h
# End Source File
# Begin Source File

SOURCE=.\Painter.h
# End Source File
# Begin Source File

SOURCE=.\RobotMask.h
# End Source File
# Begin Source File

SOURCE=.\ScanLineImageProcessing.h
# End Source File
# Begin Source File

SOURCE=.\ScanLines.h
# End Source File
# Begin Source File

SOURCE=.\SimpleCluster.h
# End Source File
# Begin Source File

SOURCE=.\SimpleObstacleDetector.h
# End Source File
# Begin Source File

SOURCE=.\YUVLookupTable.h
# End Source File
# End Group
# Begin Group "Player Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AddGotoPosPlayer.h
# End Source File
# Begin Source File

SOURCE=.\AddJoystickPlayer.h
# End Source File
# Begin Source File

SOURCE=.\Goalie.h
# End Source File
# Begin Source File

SOURCE=.\JoystickPlayer.h
# End Source File
# Begin Source File

SOURCE=.\PhysicalMotionModel.h
# End Source File
# Begin Source File

SOURCE=.\Player.h
# End Source File
# Begin Source File

SOURCE=.\PlayerDummy.h
# End Source File
# Begin Source File

SOURCE=.\PlayerFactory.h
# End Source File
# Begin Source File

SOURCE=.\PlayerType.h
# End Source File
# Begin Source File

SOURCE=.\RCPlayer.h
# End Source File
# Begin Source File

SOURCE=.\WhiteBoard.h
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

SOURCE=.\RobotDummy.h
# End Source File
# Begin Source File

SOURCE=.\RobotFactory.h
# End Source File
# Begin Source File

SOURCE=.\RobotType.h
# End Source File
# End Group
# Begin Group "Structures Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BallLocation.h
# End Source File
# Begin Source File

SOURCE=.\BallLocationReadWriter.h
# End Source File
# Begin Source File

SOURCE=.\DriveVector.h
# End Source File
# Begin Source File

SOURCE=.\DriveVectorReadWriter.h
# End Source File
# Begin Source File

SOURCE=.\FieldGeometry.h
# End Source File
# Begin Source File

SOURCE=.\GameState.h
# End Source File
# Begin Source File

SOURCE=.\GameStateReadWriter.h
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

SOURCE=.\ObstacleLocationReadWriter.h
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

SOURCE=.\RobotLocationReadWriter.h
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
# Begin Source File

SOURCE=.\VisibleObjectReadWriter.h
# End Source File
# End Group
# Begin Group "UserInterface Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AddComUserInterface.h
# End Source File
# Begin Source File

SOURCE=.\TerminalUserInterface.h
# End Source File
# Begin Source File

SOURCE=.\UserInterface.h
# End Source File
# Begin Source File

SOURCE=.\UserInterfaceType.h
# End Source File
# End Group
# Begin Group "WorldModel Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AddWriteWorldModel.h
# End Source File
# Begin Source File

SOURCE=.\CompassContainer.h
# End Source File
# Begin Source File

SOURCE=.\dataClass.h
# End Source File
# Begin Source File

SOURCE=.\DynamicSlidingWindowBallFilter.h
# End Source File
# Begin Source File

SOURCE=.\EMAObstacleFilter.h
# End Source File
# Begin Source File

SOURCE=.\ErrorMinimiserSL.h
# End Source File
# Begin Source File

SOURCE=.\ErrorMinimiserWorldModel.h
# End Source File
# Begin Source File

SOURCE=.\FieldLUT.h
# End Source File
# Begin Source File

SOURCE=.\GameStateManager.h
# End Source File
# Begin Source File

SOURCE=.\LocationShortTimeMemory.h
# End Source File
# Begin Source File

SOURCE=.\ObjectInteractionManager.h
# End Source File
# Begin Source File

SOURCE=.\ObstacleContainer.h
# End Source File
# Begin Source File

SOURCE=.\OdometryContainer.h
# End Source File
# Begin Source File

SOURCE=.\RefereeStateMachine.h
# End Source File
# Begin Source File

SOURCE=.\RobotPositionKalmanFilter.h
# End Source File
# Begin Source File

SOURCE=.\RobotPositionPredictor.h
# End Source File
# Begin Source File

SOURCE=.\SLStuckSensor.h
# End Source File
# Begin Source File

SOURCE=.\SLVelocitySensor.h
# End Source File
# Begin Source File

SOURCE=.\update_robot_location.h
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

SOURCE=.\WorldModelDummy.h
# End Source File
# Begin Source File

SOURCE=.\WorldModelFactory.h
# End Source File
# Begin Source File

SOURCE=.\WorldModelType.h
# End Source File
# Begin Source File

SOURCE=.\WorldModelTypeBase.h
# End Source File
# End Group
# Begin Group "Simulator Headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\odesimcomm.h
# End Source File
# Begin Source File

SOURCE=.\SimClient.h
# End Source File
# Begin Source File

SOURCE=.\SimulatedRobot.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorImageProcessing.h
# End Source File
# Begin Source File

SOURCE=.\SimulatorWorldModel.h
# End Source File
# Begin Source File

SOURCE=.\udpsocket_ode.h
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\RobotControl.ico
# End Source File
# Begin Source File

SOURCE=.\res\RobotControl.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
