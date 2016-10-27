; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CVideoPlayerDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "VideoPlayer.h"

ClassCount=3
Class1=CVideoPlayerApp
Class2=CVideoPlayerDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_VIDEOPLAYER_DIALOG

[CLS:CVideoPlayerApp]
Type=0
HeaderFile=VideoPlayer.h
ImplementationFile=VideoPlayer.cpp
Filter=N

[CLS:CVideoPlayerDlg]
Type=0
HeaderFile=VideoPlayerDlg.h
ImplementationFile=VideoPlayerDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LIST_VIDEOFILE

[CLS:CAboutDlg]
Type=0
HeaderFile=VideoPlayerDlg.h
ImplementationFile=VideoPlayerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=6
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308481
Control3=IDC_STATIC,static,1342308353
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342177287
Control6=IDC_STATIC,static,1342308481

[DLG:IDD_VIDEOPLAYER_DIALOG]
Type=1
Class=CVideoPlayerDlg
ControlCount=32
Control1=IDC_STATIC,button,1342177287
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_DPT_STARTTIME,SysDateTimePick32,1342242848
Control5=IDC_DPT_ENDTIME,SysDateTimePick32,1342242848
Control6=IDC_CHECK_ALLVIDEO,button,1342242819
Control7=IDC_LIST_VIDEOFILE,SysListView32,1350631437
Control8=IDC_BTN_FINDVIDEO,button,1342242816
Control9=IDC_BTN_DELONE,button,1342242816
Control10=IDC_BTN_DELALL,button,1342242816
Control11=IDC_STATIC_WNDPLAY01,static,1342308352
Control12=IDC_STATIC_WNDPLAY02,static,1073872896
Control13=IDC_EDIT_VIDEOPATH,edit,1350633600
Control14=IDC_BTN_VIDEOPATH,button,1342242816
Control15=IDC_STATIC,static,1342308352
Control16=IDC_SLIDER_PLAYPROGRESS,msctls_trackbar32,1476460568
Control17=IDC_BTN_PLAY,button,1342242880
Control18=IDC_BTN_STOP,button,1342242880
Control19=IDC_BTN_SLOW,button,1342242880
Control20=IDC_BTN_FAST,button,1342242880
Control21=IDC_BTN_CAPPIC,button,1342242880
Control22=IDC_BTN_STEPBACK,button,1073807424
Control23=IDC_BTN_STEPFORWARD,button,1073807424
Control24=IDC_BTN_SOUND,button,1342242880
Control25=IDC_SLIDER_VOLUME,msctls_trackbar32,1476460568
Control26=IDC_STATIC_STATUS01,static,1342308354
Control27=IDC_STATIC,button,1342177287
Control28=IDC_STATIC_ABOUT,static,1342308352
Control29=IDC_STATIC_TIME,static,1342308352
Control30=IDC_STATIC,button,1342177287
Control31=IDC_STATIC,button,1342177287
Control32=IDC_BTN_STOPFINDVIDEO,button,1476460544

