; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CImageProcessDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "imageprocess.h"
LastPage=0

ClassCount=4
Class1=CImageProcessApp
Class2=CAboutDlg
Class3=CImageProcessDlg
Class4=CImageProcessingMonitor

ResourceCount=2
Resource1=IDD_ABOUTBOX
Resource2=IDD_IMAGEPROCESS_DIALOG

[CLS:CImageProcessApp]
Type=0
BaseClass=CWinApp
HeaderFile=ImageProcess.h
ImplementationFile=ImageProcess.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=ImageProcessDlg.cpp
ImplementationFile=ImageProcessDlg.cpp
LastObject=CAboutDlg

[CLS:CImageProcessDlg]
Type=0
BaseClass=CDialog
HeaderFile=ImageProcessDlg.h
ImplementationFile=ImageProcessDlg.cpp
LastObject=IDC_COLORSELECTOR
Filter=D
VirtualFilter=dWC

[CLS:CImageProcessingMonitor]
Type=0
BaseClass=CWnd
HeaderFile=ImageProcessingMonitor.h
ImplementationFile=ImageProcessingMonitor.cpp
Filter=W
VirtualFilter=WC
LastObject=CImageProcessingMonitor

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_IMAGEPROCESS_DIALOG]
Type=1
Class=CImageProcessDlg
ControlCount=8
Control1=IDC_SHOW_SEGMENTATION,button,1342242819
Control2=IDC_SHOW_WARP_IMAGE,button,1342242819
Control3=IDC_SHOW_SCNALINES,button,1342242819
Control4=IDC_SHOW_RESULTS,button,1342242819
Control5=IDC_COLORSELECTOR,combobox,1344340226
Control6=IDC_LABEL,static,1342308352
Control7=IDC_BALLPOS_IMAGE,static,1342308352
Control8=IDC_BALLPOS_REAL,static,1342308352

