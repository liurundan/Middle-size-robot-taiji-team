; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CColorToolDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "colortool.h"
LastPage=0

ClassCount=7
Class1=CColorToolApp
Class2=CAboutDlg
Class3=CColorToolDlg
Class4=CEditPopupDlg
Class5=CEditPopup
Class6=CLogSlider
Class7=CParameterControls

ResourceCount=11
Resource1=IDR_ACCELERATOR4
Resource2=IDR_ACCELERATOR6
Resource3=IDR_ACCELERATOR7
Resource4=IDD_ABOUTBOX
Resource5=IDD_PARAMETER_CONTROLS
Resource6=IDR_ACCELERATOR5
Resource7=IDR_ACCELERATOR1
Resource8=IDR_ACCELERATOR3
Resource9=IDR_ACCELERATOR2
Resource10=IDD_COLORTOOL_DIALOG
Resource11=IDR_MENU1

[CLS:CColorToolApp]
Type=0
BaseClass=CWinApp
HeaderFile=ColorTool.h
ImplementationFile=ColorTool.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=ColorToolDlg.cpp
ImplementationFile=ColorToolDlg.cpp
LastObject=CAboutDlg

[CLS:CColorToolDlg]
Type=0
BaseClass=CDialog
HeaderFile=ColorToolDlg.h
ImplementationFile=ColorToolDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDM_CAMERA

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

[CLS:CLogSlider]
Type=0
BaseClass=CSliderCtrl
HeaderFile=LogSlider.h
ImplementationFile=LogSlider.cpp

[CLS:CParameterControls]
Type=0
BaseClass=CDialog
HeaderFile=ParameterControls.h
ImplementationFile=ParameterControls.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_REDGAIN_SLIDER

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_COLORTOOL_DIALOG]
Type=1
Class=CColorToolDlg
ControlCount=34
Control1=IDC_PREVIOUSCOLOR,button,1342242816
Control2=IDC_NEXTCOLOR,button,1342242816
Control3=IDC_COLORSELECTOR,combobox,1344340226
Control4=IDC_HMIN_SLIDER,msctls_trackbar32,1342242840
Control5=IDC_HMAX_SLIDER,msctls_trackbar32,1342242840
Control6=IDC_SMIN_SLIDER,msctls_trackbar32,1342242840
Control7=IDC_SMAX_SLIDER,msctls_trackbar32,1342242840
Control8=IDC_IMIN_SLIDER,msctls_trackbar32,1342242840
Control9=IDC_IMAX_SLIDER,msctls_trackbar32,1342242840
Control10=IDC_HMIN_EDIT,edit,1350631552
Control11=IDC_HMAX_EDIT,edit,1350631552
Control12=IDC_SMIN_EDIT,edit,1350631552
Control13=IDC_SMAX_EDIT,edit,1350631552
Control14=IDC_IMIN_EDIT,edit,1350631552
Control15=IDC_IMAX_EDIT,edit,1350631552
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC,button,1342177287
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308352
Control20=IDC_STATIC,button,1342177287
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,button,1342177287
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_SETCOLOR,button,1342242816
Control27=IDC_ADDCOLOR,button,1342242816
Control28=IDC_STATIC,static,1342308352
Control29=IDC_STATIC,static,1342308352
Control30=IDC_STATIC,static,1342308352
Control31=IDC_STATIC,static,1342308352
Control32=IDC_STATIC,static,1342308352
Control33=IDC_STATIC,static,1342308352
Control34=IDC_LUTLable,static,1342308352

[DLG:IDD_PARAMETER_CONTROLS]
Type=1
Class=CParameterControls
ControlCount=17
Control1=IDC_EXPOSURE_SLIDER,msctls_trackbar32,1342242840
Control2=IDC_GAIN_SLIDER,msctls_trackbar32,1342242840
Control3=IDC_EXPOSURE_EDIT,edit,1350631552
Control4=IDC_GAIN_EDIT,edit,1350631552
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_REDGAIN_SLIDER,msctls_trackbar32,1342242840
Control12=IDC_BLUEGAIN_SLIDER,msctls_trackbar32,1342242840
Control13=IDC_REDGAIN_EDIT,edit,1350631552
Control14=IDC_BLUEGAIN_EDIT,edit,1350631552
Control15=IDC_STATIC,button,1342177287
Control16=IDC_STATIC,button,1342177287
Control17=IDC_STATIC,button,1342177287

[MNU:IDR_MENU1]
Type=1
Class=CColorToolDlg
Command1=IDM_MENUITEM_CREATLUT
Command2=IDM_MENUITEM_FILEOPEN
Command3=IDM_MENUITEM_OPENLUT
Command4=IDM_MENUITEM_FILESAVE
Command5=IDM_MENUITEM_FILESAVE_AS
Command6=IDM_MENUITEM_SAVELUT
Command7=IDM_MENUITEM_SAVEASLUT
Command8=IDM_MENUITEM_USELUT
Command9=IDM_MENUITEM_SHOW_ORIGIMAGE
Command10=IDM_PREV_COLOR
Command11=IDM_NEXT_COLOR
Command12=IDM_QUICKLOAD
Command13=IDM_QUICKSAVE
Command14=IDM_CAMERA
CommandCount=14

[ACL:IDR_ACCELERATOR1]
Type=1
Class=?
Command1=IDM_MENUITEM_CREATLUT
CommandCount=1

[ACL:IDR_ACCELERATOR2]
Type=1
Class=?
Command1=IDM_MENUITEM_USELUT
CommandCount=1

[ACL:IDR_ACCELERATOR3]
Type=1
Class=?
Command1=IDM_MENUITEM_SHOW_ORIGIMAGE
CommandCount=1

[ACL:IDR_ACCELERATOR4]
Type=1
Class=?
Command1=IDM_MENUITEM_FILESAVE
CommandCount=1

[ACL:IDR_ACCELERATOR5]
Type=1
Class=?
Command1=IDM_MENUITEM_SAVELUT
CommandCount=1

[ACL:IDR_ACCELERATOR6]
Type=1
Class=?
Command1=IDM_PREV_COLOR
CommandCount=1

[ACL:IDR_ACCELERATOR7]
Type=1
Class=?
Command1=IDM_NEXT_COLOR
CommandCount=1

