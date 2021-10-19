; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CImageMaskDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "imagemask.h"
LastPage=0

ClassCount=3
Class1=CImageMaskApp
Class2=CAboutDlg
Class3=CImageMaskDlg

ResourceCount=3
Resource1=IDD_IMAGEMASK_DIALOG
Resource2=IDD_ABOUTBOX
Resource3=IDR_MENU1

[CLS:CImageMaskApp]
Type=0
BaseClass=CWinApp
HeaderFile=ImageMask.h
ImplementationFile=ImageMask.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=ImageMaskDlg.cpp
ImplementationFile=ImageMaskDlg.cpp

[CLS:CImageMaskDlg]
Type=0
BaseClass=CDialog
HeaderFile=ImageMaskDlg.h
ImplementationFile=ImageMaskDlg.cpp
LastObject=IDC_THRESHOLD_SLIDER
Filter=D
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_IMAGEMASK_DIALOG]
Type=1
Class=CImageMaskDlg
ControlCount=7
Control1=IDC_MSCOMM1,{648A5600-2C6E-101B-82B6-000000000014},1342242816
Control2=IDC_THRESHOLD_SLIDER,msctls_trackbar32,1342242840
Control3=IDC_CAPTURE,button,1342242816
Control4=IDC_SHOWSHRESHOLD,static,1342308352
Control5=IDC_DRAW_CIRCLE,button,1342242816
Control6=IDC_CREATMASK,button,1342242816
Control7=IDC_USE_MASK,button,1342242819

[MNU:IDR_MENU1]
Type=1
Class=CImageMaskDlg
Command1=IDM_FILESAVE
Command2=IDM_FILESAVEAS
Command3=IDM_WRITECONFIG
Command4=IDM_QUICK_LOAD
CommandCount=4

