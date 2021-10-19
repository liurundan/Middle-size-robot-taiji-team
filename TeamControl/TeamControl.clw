; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=RemoteRobotWidget
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "teamcontrol.h"
LastPage=0

ClassCount=15
Class1=CoachWidget
Class2=CEditPopupDlg
Class3=CEditPopup
Class4=FieldOfPlay
Class5=FieldWidget
Class6=JoystickDialog
Class7=CRealRobotData
Class8=RefboxWidget
Class9=RemoteRobotWidget
Class10=StatusColor
Class11=CTeamControlApp
Class12=CAboutDlg
Class13=CTeamControlDlg
Class14=CValPlotWidget

ResourceCount=11
Resource1=IDR_TOOLBAR1
Resource2=IDD_REALROBOTDATA
Resource3=IDD_TEAMCONTROL_DIALOG
Resource4=IDD_COACHWIDGET
Resource5=IDD_REMOTEROBOTWIDGET
Resource6=IDD_MESSAGE_BOARD_DIALOG
Resource7=IDD_ABOUTBOX
Resource8=IDD_REFBOXWIDGET
Resource9=IDD_JOYSTICKDIALOG
Resource10=IDD_FIELDWIDGET
Class15=MessageDialog
Resource11=IDR_MENU1

[CLS:CoachWidget]
Type=0
BaseClass=CDialog
HeaderFile=CoachWidget.h
ImplementationFile=CoachWidget.cpp

[CLS:CEditPopupDlg]
Type=0
BaseClass=CDialog
HeaderFile=EditPopup.cpp
ImplementationFile=EditPopup.cpp

[CLS:CEditPopup]
Type=0
BaseClass=CEdit
HeaderFile=EditPopup.h
ImplementationFile=EditPopup.cpp

[CLS:FieldOfPlay]
Type=0
BaseClass=CDialog
HeaderFile=FieldOfPlay.h
ImplementationFile=FieldOfPlay.cpp

[CLS:FieldWidget]
Type=0
BaseClass=CWnd
HeaderFile=FieldWidget.h
ImplementationFile=FieldWidget.cpp

[CLS:JoystickDialog]
Type=0
BaseClass=CDialog
HeaderFile=JoystickDialog.h
ImplementationFile=JoystickDialog.cpp

[CLS:CRealRobotData]
Type=0
BaseClass=CDialog
HeaderFile=RealRobotData.h
ImplementationFile=RealRobotData.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_MOTORVEL_3

[CLS:RefboxWidget]
Type=0
BaseClass=CDialog
HeaderFile=RefboxWidget.h
ImplementationFile=RefboxWidget.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_OWNSCORE

[CLS:RemoteRobotWidget]
Type=0
BaseClass=CDialog
HeaderFile=RemoteRobotWidget.h
ImplementationFile=RemoteRobotWidget.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_ACTIVATE_CHECK

[CLS:StatusColor]
Type=0
BaseClass=CWnd
HeaderFile=StatusColor.h
ImplementationFile=StatusColor.cpp

[CLS:CTeamControlApp]
Type=0
BaseClass=CWinApp
HeaderFile=TeamControl.h
ImplementationFile=TeamControl.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=TeamControlDlg.cpp
ImplementationFile=TeamControlDlg.cpp
LastObject=CAboutDlg

[CLS:CTeamControlDlg]
Type=0
BaseClass=CDialog
HeaderFile=TeamControlDlg.h
ImplementationFile=TeamControlDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_COMM_RATE_SPIN

[CLS:CValPlotWidget]
Type=0
BaseClass=CWnd
HeaderFile=ValPlotWidget.h
ImplementationFile=ValPlotWidget.cpp

[DLG:IDD_COACHWIDGET]
Type=1
Class=CoachWidget
ControlCount=8
Control1=IDC_OWNS_BALL_CHECK,button,1342242819
Control2=IDC_COMM_BALL_CHECK,button,1342242819
Control3=IDC_BROADCAST_CHECK,button,1342242819
Control4=IDC_POLICY_COMBO,combobox,1344340226
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,button,1342177287
Control8=IDC_MESSAGE_EDIT,static,1350701068

[DLG:IDD_FIELDWIDGET]
Type=1
Class=FieldOfPlay
ControlCount=0

[DLG:IDD_JOYSTICKDIALOG]
Type=1
Class=JoystickDialog
ControlCount=0

[DLG:IDD_REALROBOTDATA]
Type=1
Class=CRealRobotData
ControlCount=3
Control1=IDC_VCC,static,1342308352
Control2=IDC_MOTORVEL_0,static,1342308352
Control3=IDC_ROBOT_VELOCITY,static,1342308352

[DLG:IDD_REFBOXWIDGET]
Type=1
Class=RefboxWidget
ControlCount=28
Control1=IDC_OWNKICKOFF,button,1342242816
Control2=IDC_OWNTHROWIN,button,1342242816
Control3=IDC_OWNGOALKICK,button,1342242816
Control4=IDC_OWNCORNERKICK,button,1342242816
Control5=IDC_OWNFREEKICK,button,1342242816
Control6=IDC_OWNPENALTYKICK,button,1342242816
Control7=IDC_STOP,button,1342242817
Control8=IDC_START,button,1342242816
Control9=IDC_READY,button,1342242816
Control10=IDC_DROPBALL,button,1342242816
Control11=IDC_OPPKICKOFF,button,1342242816
Control12=IDC_OPPTHROWIN,button,1342242816
Control13=IDC_OPPGOALKICK,button,1342242816
Control14=IDC_OPPCORNERKICK,button,1342242816
Control15=IDC_OPPFREEKICK,button,1342242816
Control16=IDC_OPPPENALTYKICK,button,1342242816
Control17=IDC_CHANGEHALF,button,1342246656
Control18=IDC_CHANGELABEL,button,1342242817
Control19=IDC_STATIC,button,1342177287
Control20=IDC_STATIC,button,1342177287
Control21=IDC_STATIC,button,1342177287
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,button,1342177287
Control24=IDC_STATIC,button,1342177287
Control25=IDC_STATIC,static,1342308353
Control26=IDC_STATIC,static,1342308352
Control27=IDC_REFBOX_IP,static,1342308352
Control28=IDC_REFBOXCONNECT_CHECK,button,1342242819

[DLG:IDD_REMOTEROBOTWIDGET]
Type=1
Class=RemoteRobotWidget
ControlCount=15
Control1=IDC_CONNECTROBOT_CHECK,button,1342242819
Control2=IDC_MESSAGE_CHECK,button,1342242819
Control3=IDC_JOYSTICK_CHECK,button,1342242819
Control4=IDC_ACTIVATE_CHECK,button,1342242819
Control5=IDC_PLAYER_SELECT,combobox,1344340226
Control6=IDC_ROLE_SELECT,combobox,1344340226
Control7=IDC_REFSTATE_SELECT,combobox,1344340226
Control8=IDC_STATIC,button,1342177287
Control9=IDC_CHANGETEAM_BUTTON,button,1342242816
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_IP_EDIT,static,1342308352
Control15=IDC_STATIC,button,1342177287

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_TEAMCONTROL_DIALOG]
Type=1
Class=CTeamControlDlg
ControlCount=6
Control1=IDC_COMM_RATE_SPIN,msctls_updown32,1342177318
Control2=IDC_COMMRATE_EDIT,edit,1350633600
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,button,1342177287
Control5=IDC_REFERENCE_COMBO,combobox,1344340226
Control6=IDC_STATIC,button,1342177287

[DLG:IDD_MESSAGE_BOARD_DIALOG]
Type=1
Class=MessageDialog
ControlCount=1
Control1=IDC_MSG,static,1342308352

[TB:IDR_TOOLBAR1]
Type=1
Class=?
Command1=ID_BTN_ZOOMIN
Command2=ID_BTN_ZOOMOUT
Command3=ID_BTN_ZOOMALL
Command4=ID_BTN_ZOOMUNDO
Command5=ID_BTN_ZOOMREDO
Command6=ID_BTN_FILPSIDES
Command7=ID_BTN_FILPGOALS
Command8=ID_BTN_NEXTREFROBOT
Command9=ID_BTN_SHOWWMROBOT
Command10=ID_BTN_SHOWWMBALL
Command11=ID_BTN_SHOWWMOBSTACLES
Command12=ID_BTN_SHOWVISLINES
Command13=ID_BTN_SHOWVISBALL
Command14=ID_BTN_SHOWVISOBSTACLES
Command15=ID_BTN_SHOWVISGOALS
Command16=ID_BIN_SHOWSLPOS
Command17=ID_BTN_SHOWAUXLINES
Command18=ID_BTN_SHOWROBOTIDS
Command19=ID_BTN_SHOWROBOTBALLLINKS
Command20=ID_BTN_ROBOTTRACE
Command21=ID_BTN_BALLTRACE
Command22=ID_BTN_CLEARLINE
Command23=ID_BTN_CLOCK
CommandCount=23

[MNU:IDR_MENU1]
Type=1
Class=?
Command1=IDM_PLAYING_FIELD
Command2=IDM_TRAINER
Command3=IDM_REFEREE_STATES
Command4=IDM_ROBOT_DATA
Command5=IDM_ROBOT_REFERCE
CommandCount=5

[CLS:MessageDialog]
Type=0
HeaderFile=MessageDialog.h
ImplementationFile=MessageDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=MessageDialog

