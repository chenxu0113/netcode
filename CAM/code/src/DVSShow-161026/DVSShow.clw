; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSysSettings
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DVSShow.h"
LastPage=0

ClassCount=12
Class1=CDVSShowApp
Class2=CDVSShowDoc
Class3=CDVSShowView
Class4=CMainFrame

ResourceCount=10
Resource1=IDD_DLG_PTZPRESET
Resource2=IDR_MENU_RMENU1
Class5=CAboutDlg
Class6=CDlgVideo1
Resource3=IDR_MAINFRAME
Class7=CDlgOutput
Resource4=IDD_DLG_CONTROL
Class8=CSysSettings
Resource5=IDD_DLG_VIDEOSETTINGS
Class9=CControl
Resource6=IDD_DLG_OUTPUT
Resource7=IDD_ABOUTBOX
Class10=CDlgPTZPreset
Class11=CDlgOutputCtrl
Resource8=IDD_DLG_VIDEO1
Class12=CDlgVideoSettings
Resource9=IDR_MAINFRAME1
Resource10=IDD_DLG_SETTINGS

[CLS:CDVSShowApp]
Type=0
HeaderFile=DVSShow.h
ImplementationFile=DVSShow.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CDVSShowDoc]
Type=0
HeaderFile=DVSShowDoc.h
ImplementationFile=DVSShowDoc.cpp
Filter=N

[CLS:CDVSShowView]
Type=0
HeaderFile=DVSShowView.h
ImplementationFile=DVSShowView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_SET_TEST




[CLS:CAboutDlg]
Type=0
HeaderFile=DVSShow.cpp
ImplementationFile=DVSShow.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=6
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308481
Control3=IDC_STATIC,static,1342308353
Control4=1,button,1342373889
Control5=IDC_STATIC,static,1342308481
Control6=IDC_STATIC,button,1342210055

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_APP_EXIT
Command2=ID_SET_SYSSETTINGS
Command3=ID_SET_VIDEOSETTINGS
Command4=ID_VIEW_TOOLBAR
Command5=ID_VIEW_STATUS_BAR
Command6=ID_VIEW_LEFT
Command7=ID_VIEW_VIDEOPLAY
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_SET_VIDEOSETTINGS
Command2=ID_VIDEO_CAPPIC
Command3=ID_VIDEO_CONTROL
Command4=ID_SYS_ABOUT
Command5=ID_SYS_EXIT
CommandCount=5

[DLG:IDD_DLG_VIDEO1]
Type=1
Class=CDlgVideo1
ControlCount=1
Control1=IDC_STATIC_PREVIEWBG,static,1476526348

[CLS:CDlgVideo1]
Type=0
HeaderFile=DlgVideo1.h
ImplementationFile=DlgVideo1.cpp
BaseClass=CFormView
Filter=D
LastObject=CDlgVideo1
VirtualFilter=VWC

[DLG:IDD_DLG_OUTPUT]
Type=1
Class=CDlgOutput
ControlCount=3
Control1=IDC_STATIC_FLAG,static,1342308352
Control2=IDC_STATIC,static,1342177287
Control3=IDC_STATIC,static,1342177287

[CLS:CDlgOutput]
Type=0
HeaderFile=DlgOutput.h
ImplementationFile=DlgOutput.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_RMENU_CONTROL
VirtualFilter=dWC

[DLG:IDD_DLG_SETTINGS]
Type=1
Class=CSysSettings
ControlCount=32
Control1=IDC_EDIT_PICPATH,edit,1350633600
Control2=IDC_BTN_PICPATHSEL,button,1342242816
Control3=IDC_SLIDER_BRIGHTNESS,msctls_trackbar32,1476460568
Control4=IDC_SLIDER_HUE,msctls_trackbar32,1476460568
Control5=IDC_SLIDER_CONTRAST,msctls_trackbar32,1476460568
Control6=IDC_SLIDER_SATURATION,msctls_trackbar32,1476460568
Control7=IDC_BTN_DEFAULT,button,1476460544
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,button,1342177287
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDIT_RECORDPATH,edit,1350633600
Control16=IDC_BTN_RECORDSELPATH,button,1342242816
Control17=IDC_STATIC,static,1342308352
Control18=IDC_CHECK_CAPPIC,button,1342242819
Control19=IDC_CHECK_SGVIDEO,button,1342242819
Control20=IDC_STATIC,button,1342177287
Control21=IDC_RADIO_NORMALVIDEO,button,1476526089
Control22=IDC_RADIO_SPECIALVIDEO,button,1476395017
Control23=IDC_STATIC_TIME_STOPREC,static,1476526080
Control24=IDC_EDIT_TIME_STOPREC,edit,1484857472
Control25=IDC_STATIC_TIME_STOPVIDEO,static,1476526080
Control26=IDC_EDIT_TIME_STOPVIDEO,edit,1484857472
Control27=IDC_BTN_OK,button,1342242816
Control28=IDC_BTN_APP_SGVIDEO,button,1476460544
Control29=IDC_STATIC,button,1342177287
Control30=IDC_STATIC_INSTRUCTION,static,1342308352
Control31=IDC_STATIC,button,1342177287
Control32=IDC_CHECK_STARTRECORD,button,1476460547

[CLS:CSysSettings]
Type=0
HeaderFile=SysSettings.h
ImplementationFile=SysSettings.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT_TIME_STOPREC

[DLG:IDD_DLG_CONTROL]
Type=1
Class=CControl
ControlCount=18
Control1=IDC_BTN_PTZ_UP,button,1342275584
Control2=IDC_BTN_PTZ_DOWN,button,1342275584
Control3=IDC_BTN_PTZ_LEFT,button,1342275584
Control4=IDC_BTN_PTZ_RIGHT,button,1342275584
Control5=IDC_BTN_ZOOM_IN,button,1342275584
Control6=IDC_BTN_ZOOM_OUT,button,1342275584
Control7=IDC_BTN_FOCUS_NEAR,button,1342275584
Control8=IDC_BTN_FOCUS_FAR,button,1342275584
Control9=IDC_BTN_IRIS_OPEN,button,1342275584
Control10=IDC_BTN_IRIS_CLOSE,button,1342275584
Control11=IDC_COMBO_SPEED,combobox,1344339971
Control12=IDC_STATIC,button,1342177287
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342177288
Control18=IDC_BUTTON_SETSPEED,button,1342242816

[CLS:CControl]
Type=0
HeaderFile=Control.h
ImplementationFile=Control.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_COMBO_SPEED

[MNU:IDR_MENU_RMENU1]
Type=1
Class=CDlgVideo1
Command1=ID_VIDEO_CAPPIC
Command2=ID_RMENU_CONTROL
Command3=ID_RMENU_PRESET
Command4=ID_RMENU_START_TEMPRECORD
Command5=ID_RMENU_STOP_TEMPRECORD
Command6=ID_RMENU_STOPVIDEO
CommandCount=6

[DLG:IDD_DLG_PTZPRESET]
Type=1
Class=CDlgPTZPreset
ControlCount=6
Control1=IDC_STATIC,button,1342177287
Control2=IDC_BTN_ADD,button,1342242816
Control3=IDC_BTN_DELETE,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_COMBO_PTZPRESET,combobox,1344340227
Control6=IDC_BTN_POINT,button,1342242816

[CLS:CDlgPTZPreset]
Type=0
HeaderFile=DlgPTZPreset.h
ImplementationFile=DlgPTZPreset.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_COMBO_PTZPRESET

[CLS:CDlgOutputCtrl]
Type=0
HeaderFile=DlgOutputCtrl.h
ImplementationFile=DlgOutputCtrl.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgOutputCtrl

[DLG:IDD_DLG_VIDEOSETTINGS]
Type=1
Class=CDlgVideoSettings
ControlCount=3
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308352
Control3=IDC_COMBO_VIDEOWNDNUM,combobox,1344340227

[CLS:CDlgVideoSettings]
Type=0
HeaderFile=DlgVideoSettings.h
ImplementationFile=DlgVideoSettings.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_COMBO_VIDEOWNDNUM

[TB:IDR_MAINFRAME1]
Type=1
Class=?
Command1=ID_SET_SYSSETTINGS
Command2=ID_VIDEO_CAPPIC
Command3=ID_VIDEO_CONTROL
Command4=ID_SYS_ABOUT
Command5=ID_SYS_EXIT
CommandCount=5

