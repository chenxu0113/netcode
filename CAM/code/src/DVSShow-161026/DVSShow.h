// DVSShow.h : main header file for the DVSSHOW application
//

#if !defined(AFX_DVSSHOW_H__987071BF_96C2_491C_9582_4A3AEE81AAD9__INCLUDED_)
#define AFX_DVSSHOW_H__987071BF_96C2_491C_9582_4A3AEE81AAD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "DlgOutput.h"
#include "CamWndMgr.h"
#include "MainFrm.h"
#include "define.h"
#include "SendThread.h"
#include "SocketThread.h"
#include "DlgAlarm.h"

/////////////////////////////////////////////////////////////////////////////
// CDVSShowApp:
// See DVSShow.cpp for the implementation of this class
//

class CSendThread;
class CSocketThread;
class CDlgAlarm;

const ModuleFileInfomations&    GetModuleFileInformations();

#define GET_MODULE_FILE_INFO    (GetModuleFileInformations())

class CDVSShowApp : public CWinApp,
				  public CBCGPWorkspace
{
public:
	CDVSShowApp();
	~CDVSShowApp();
	
// Attributes
public:
	CString			m_sIniPath;
	CString			m_sPicPath;
	CString			m_sRecordPath;
	BOOL			m_bCapPic;
	BOOL			m_bIsWindowsNT;
	BOOL			m_bAlarmVideo;
	BOOL			m_bnStartTempRec;
	int				m_nStopRec;
	int				m_nStopVideo;
	int				m_nsStopVideo;
	HANDLE			m_hExit;
	CDlgOutput		m_dlgOutput[MAX_VIDEO_NUM];

	CDlgAlarm*		m_pDlgAlarm;
	CSendThread*	m_pSendThread;
	CSocketThread*	m_pSocketThread;
	CMap<CString, LPCTSTR, VIDEOPARAM*, VIDEOPARAM*> m_mapVideoParam;
	CMap<CString, LPCTSTR, NetworkDIDOSocket, NetworkDIDOSocket&> m_mapNetworkDidoSocket;
	CMap<CString, LPCTSTR, LinkInfoDef, LinkInfoDef&> m_mapLinkInfoDef;
	CList<CameraLinkage, CameraLinkage&> m_listGangedRule;
	CList<CameraInfoDef, CameraInfoDef&> m_listCamerainfo;
	map<int, CCamWndMgr *>	m_MapCCamWndMgr;
	CRITICAL_SECTION m_csMapCCamWndMgr;
	CRITICAL_SECTION m_csNetworkDidoSocket;
// Methods
public:
	void			CaptureNetworkDidoSocket();
	void			ReleaseNetworkDidoSocket();
	BOOL			InitThread();
	CWorkspaceBar*	GetWorkspaceBar();
	CDlgVideo1*		GetDlgVideo1();
	CMainFrame*		GetMainFrame();
	BOOL			InitIniConfig();
	BOOL			InitNetworkDidoSocketConfig();
	BOOL			InitLinkInfoConfig();
	UINT			CheckFileIsExist(CString sPath);
	BOOL			ParamsCall(const TCHAR *szCmd, HWND hWnd);	//通过参数推送视频
	
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDVSShowApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CDVSShowApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

void Unicode2ASCII(const WCHAR *wszUnicode, char *szASCII);		//宽字符转多字节ASCII字符

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DVSSHOW_H__987071BF_96C2_491C_9582_4A3AEE81AAD9__INCLUDED_)
