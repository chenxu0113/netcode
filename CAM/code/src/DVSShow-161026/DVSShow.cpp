// DVSShow.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DVSShow.h"

#include "MainFrm.h"
#include "DVSShowDoc.h"
#include "DVSShowView.h"
#include "DlgVideo1.h"
#include "DlgOutput.h"
#include "Markup.h"

BOOL SplitParams(const TCHAR *szSrcCmd, TCHAR szDstCmd[][64], int *pnCount);
BOOL ParseInputString(const TCHAR strInput[][64], int nCount, InputParams* pParams);

void Unicode2ASCII(const WCHAR *wszUnicode, char *szASCII)
{
	int iASCIILen = 0;
	iASCIILen = WideCharToMultiByte(CP_ACP, 0, wszUnicode, -1, NULL, 0, NULL, NULL);

	WideCharToMultiByte(CP_ACP, 0, wszUnicode, -1, szASCII, iASCIILen, NULL, NULL);
}

#define GET_OPTION_POINTER(PTR)    \
{                                  \
	i++;						   \
	if (strInput[i][0] == _T('-')) \
{                              \
	i--;					   \
	PTR = _T("");				   \
}                              \
	else                       \
{                              \
	PTR = strInput[i];         \
}                              \
}

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDVSShowApp
extern CMainFrame* g_pMainFrame;

BEGIN_MESSAGE_MAP(CDVSShowApp, CWinApp)
	//{{AFX_MSG_MAP(CDVSShowApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDVSShowApp construction

CDVSShowApp::CDVSShowApp()
{
	InitializeCriticalSection(&m_csMapCCamWndMgr);
	InitializeCriticalSection(&m_csNetworkDidoSocket);
	InitializeCriticalSection(&m_csCamLinkSet);
	m_sPicPath = _T("");
	m_sRecordPath = _T("");
	m_nStopRec = 0;
	m_nStopVideo = 0;
	m_nsStopVideo = 0;
	m_bCapPic = FALSE;
	m_bAlarmVideo = FALSE;
	m_bnStartTempRec = FALSE;
	m_pSendThread = NULL;
	m_pSocketThread = NULL;
	m_pDlgAlarm = NULL;
	m_hExit = NULL;
	m_listGangedRule.RemoveAll();
	m_listCamerainfo.RemoveAll();
	m_mapNetworkDidoSocket.RemoveAll();
	m_mapLinkInfoDef.RemoveAll();
	m_mapVideoParam.RemoveAll();
	m_iVideoComboRow = 1;

	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	m_bIsWindowsNT = (osvi.dwPlatformId==VER_PLATFORM_WIN32_NT);

	m_hExit = CreateEvent(NULL, TRUE, FALSE, NULL);
}

CDVSShowApp::~CDVSShowApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDVSShowApp object

CDVSShowApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDVSShowApp initialization

BOOL CDVSShowApp::InitInstance()
{
	::CreateMutex(NULL, TRUE, m_pszExeName);
	if (GetLastError()==ERROR_ALREADY_EXISTS)
	{
		CWnd *pPrevWnd=CWnd::GetDesktopWindow()->GetWindow(GW_CHILD);
		while(pPrevWnd)
		{
			HANDLE hData = NULL;
			if ((hData = GetProp(pPrevWnd->GetSafeHwnd(), m_pszExeName)) != NULL)	
			{
				if (hData == MAINWND_PROP_HANDLE)
				{
					pPrevWnd->SetForegroundWindow();
					pPrevWnd->ShowWindow(SW_SHOW);
					pPrevWnd->UpdateWindow();
					ParamsCall(m_lpCmdLine, pPrevWnd->GetSafeHwnd());
					return FALSE;
				}
			}
			pPrevWnd = pPrevWnd->GetWindow(GW_HWNDNEXT);
		}
		TRACE("Could not find previous instance main window!\n");
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
//	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("DVSShow"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDVSShowDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CDVSShowView));
	AddDocTemplate(pDocTemplate);

	GetContextMenuManager()->AddMenu(_T("RMenu1"), IDR_MENU_RMENU1);

	if (!AfxSocketInit())
	{
		return FALSE;
	}

	InitIniConfig();
	InitNetworkDidoSocketConfig();
	InitLinkInfoConfig();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	//ParseCommandLine(cmdInfo);	//忽略参数

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_pMainWnd->SetWindowText(_T("视频监控软件"));
	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	
	m_pMainWnd->SetForegroundWindow();
	m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	m_pMainWnd->UpdateWindow();

	InitThread();

	return TRUE;
}

BOOL CDVSShowApp::InitThread()
{
	//初始化定时往服务器发送的线程
	if (m_pSendThread == NULL)
	{
		m_pSendThread = new CSendThread;
		if (m_pSendThread == NULL)
		{
			return FALSE;
		}
		if (!m_pSendThread->CreateThread())
		{			
			delete m_pSendThread;
			m_pSendThread = NULL;
			return FALSE;
		}
		m_pSendThread->m_bAutoDelete = TRUE;
	}

	//初始化数据处理模块，主要是处理网络模块DI/DO返回的数据状态
	if (m_pSocketThread == NULL)
	{
		m_pSocketThread = new CSocketThread;
		if (m_pSocketThread == NULL)
		{
			return FALSE;
		}
		if (!m_pSocketThread->CreateThread())
		{			
			delete m_pSocketThread;
			m_pSocketThread = NULL;
			return FALSE;
		}
		m_pSocketThread->m_bAutoDelete = TRUE;
	}

	return TRUE;
}

CMainFrame* CDVSShowApp::GetMainFrame()
{
	return g_pMainFrame;
}

CWorkspaceBar* CDVSShowApp::GetWorkspaceBar()
{
	return &GetMainFrame()->m_wndWorkSpace;
}

CDlgVideo1* CDVSShowApp::GetDlgVideo1()
{
	CMainFrame* pFrame = GetMainFrame();
	if (pFrame == NULL)
		return NULL;

	CDlgVideo1* pVideo = (CDlgVideo1*)pFrame->m_wndSplitter.GetView(pFrame->m_nViewVideo1);
	
	return pVideo;
}

/////////////////////////////////////////////
// 退出系统
// Input: NONE
// Ouput: NONE
// Return: NONE
/////////////////////////////////////////////
int CDVSShowApp::ExitInstance() 
{
	SetEvent(m_hExit);

	m_listGangedRule.RemoveAll();
	m_listCamerainfo.RemoveAll();
	m_mapLinkInfoDef.RemoveAll();
	m_mapVideoParam.RemoveAll();

	CaptureNetworkDidoSocket();
	m_mapNetworkDidoSocket.RemoveAll();
	ReleaseNetworkDidoSocket();

	map<int, CCamWndMgr *>::iterator it;
	CCamWndMgr *pMgr = NULL;
	for (it=m_MapCCamWndMgr.begin(); it!=m_MapCCamWndMgr.end(); )
	{
		pMgr = it->second;
		if (pMgr)
		{
			delete pMgr;
		}
		it = m_MapCCamWndMgr.erase(it);
	}

	DeleteCriticalSection(&m_csMapCCamWndMgr);
	DeleteCriticalSection(&m_csNetworkDidoSocket);

	CloseHandle(m_hExit);

	return CWinApp::ExitInstance();
}
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CBCGPDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CBCGPDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT


}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CBCGPDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_LunaBlue);
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
	RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
	EnableVisualManagerStyle (TRUE, TRUE);	

	return TRUE;
}
// App command to run the dialog
void CDVSShowApp::OnAppAbout()
{

	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CDVSShowApp message handlers
/////////////////////////////////////////////
// 初始化配置信息
// Input: NONE
// Ouput: NONE
// Return: NONE
/////////////////////////////////////////////
BOOL CDVSShowApp::InitIniConfig()
{
	TCHAR szDirectory[MAX_PATH];
	GetModuleFileName(GetModuleHandle(NULL),szDirectory,MAX_PATH);
	CString str = szDirectory;	
	int nIndex=str.ReverseFind('\\');
	if (nIndex != 0)
	{
		str = str.Left(nIndex);
	}
	m_sIniPath.Format(_T("%s\\CamParam.ini"), str);
	//////////////////////////////
	
	GetPrivateProfileString(SET_PATH, SET_PICPATH, str + _T("\\Pic"), m_sPicPath.GetBuffer(MAX_PATH), MAX_PATH, m_sIniPath);
	m_sPicPath.ReleaseBuffer();
	if (m_sPicPath.IsEmpty())
	{
		m_sPicPath = str + _T("\\Pic");
	}
	if (m_sPicPath.Right(1) == _T("\\") || m_sPicPath.Right(1) == _T("/"))
	{
		m_sPicPath = m_sPicPath.Left(m_sPicPath.GetLength() - 1);
	}
	GetPrivateProfileString(SET_PATH, SET_RECORDPATH, str + _T("\\Record"), m_sRecordPath.GetBuffer(MAX_PATH), MAX_PATH, m_sIniPath);
	m_sRecordPath.ReleaseBuffer();

	if (m_sRecordPath.IsEmpty())
	{
		m_sRecordPath = str + _T("\\Record");
	}
	if (m_sRecordPath.Right(1) == _T("\\") || m_sRecordPath.Right(1) == _T("/"))
	{
		m_sRecordPath = m_sRecordPath.Left(m_sRecordPath.GetLength() - 1);
	}

	m_bCapPic = GetPrivateProfileInt(SET_CAPPIC, SET_CAPPIC, 0, m_sIniPath);
	m_bAlarmVideo = GetPrivateProfileInt(SET_SGVIDEO, SET_ISSGVIDEO, 0, m_sIniPath);
	m_nStopRec = GetPrivateProfileInt(SET_NORMALVIDEO, SET_TIME_STOPREC, 0, m_sIniPath);
	m_nStopVideo = GetPrivateProfileInt(SET_NORMALVIDEO, SET_TIME_STOPVIDEO, 0, m_sIniPath);
	m_bnStartTempRec = GetPrivateProfileInt(SET_NORMALVIDEO, SET_STARTTEMPREC, 0, m_sIniPath);

	CFileFind fileFind;
	if (!fileFind.FindFile(m_sPicPath))
	{
		CreateDirectory(m_sPicPath, NULL);
	}
	if (!fileFind.FindFile(m_sRecordPath))
	{
		CreateDirectory(m_sRecordPath, NULL);
	}

	return TRUE;
}

BOOL CDVSShowApp::InitNetworkDidoSocketConfig()
{

	CaptureNetworkDidoSocket();
	m_mapNetworkDidoSocket.RemoveAll();
	ReleaseNetworkDidoSocket();

	CMarkup xml;

	if(xml.Load(GET_MODULE_FILE_INFO.strPath + "NetModulsInfo.xml"))
	{
		while(xml.FindChildElem("NetModulINFO"))
		{		
			xml.IntoElem();
			int iID=0;
			CString netModuleIp = _T("");
			int iPort = 0;
			int iWorkDoNum = 0;
			BOOL bAlarmValue = FALSE;

			
			xml.FindChildElem("ID");
			iID=atoi(xml.GetChildData());		

			xml.FindChildElem("ip");
			netModuleIp=xml.GetChildData();

			xml.FindChildElem("port");
			iPort=atoi(xml.GetChildData());

			xml.FindChildElem("DoNums");
			iWorkDoNum=atoi(xml.GetChildData());

			xml.FindChildElem("DoAlarmValue");
			bAlarmValue=atoi(xml.GetChildData());
			xml.OutOfElem();
			NetworkDIDOSocket nds;
			SYSTEMTIME st;

			GetLocalTime(&st);

			memset(&nds, 0, sizeof(NetworkDIDOSocket));

			nds.nIndex = iID;
			nds.ndc.nID = iID;	//?????
			nds.ndc.cMaxNum = iWorkDoNum;
			nds.ndc.cAlarmValue = bAlarmValue;
			sprintf(nds.ndc.szName, "网络模块%02d", iID);
			strcpy(nds.ndc.szIP,netModuleIp);

			for (int j=0; j<nds.ndc.cMaxNum; j++)
			{
				nds.ndc.cNum[j] = abs(bAlarmValue-1);	//模块上的第n个DO状态，默认为0
			}
			nds.bInitSuccess = FALSE;
			nds.ndc.nPort = iPort;
			nds.st = st;
			CString sIP(nds.ndc.szIP);
			CaptureNetworkDidoSocket();
			m_mapNetworkDidoSocket.SetAt(sIP, nds);
			ReleaseNetworkDidoSocket();
			}
	}

	return TRUE;
}

BOOL CDVSShowApp::InitLinkInfoConfig()
{
	//int i = 0;
	//LinkInfoDef lid;

	//m_mapLinkInfoDef.RemoveAll();
	//for (i=0; i<1; i++)
	//{
	//	memset(&lid, 0, sizeof(LinkInfoDef));

	//	lid.bChannel = 0;
	//	lid.bNetworkNum = 1;			//?????
	//	strcpy(lid.cCamIP, "10.0.0.203");
	//	strcpy(lid.cNetworkSocketIP, "10.0.0.10");
	//	lid.nCamID = 1;

	//	CString sIP = "";
	//	sIP.Format("%s,%d", lid.cCamIP, lid.bChannel);
	//	m_mapLinkInfoDef.SetAt(sIP, lid);
	//}

	return TRUE;
}

// 响应键盘消息
BOOL CDVSShowApp::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_F8:
			{
				GetMainFrame()->OnBarCheck(ID_VIEW_TOOLBAR);
			}
			return TRUE;
		default:
			break;
		}
	}
	
	return CWinApp::PreTranslateMessage(pMsg);
}


/////////////////////////////////////////////
// 检查目录/文件是否存在
// Input: NONE
// Ouput: NONE
// Return: TRUE-SUCCESS,FALSE-FAILED
/////////////////////////////////////////////
UINT CDVSShowApp::CheckFileIsExist(CString sPath)
{
	CFileFind file;
	if (file.IsDirectory())
		return 0;
	if (!file.FindFile(sPath))
		return 1;

	return 0;
}

BOOL CDVSShowApp::ParamsCall(const TCHAR * szCmd, HWND hWnd)
{
	TCHAR szDstCmd[16][64] = {0};
	int nCount = 0;
	if (!SplitParams(szCmd, szDstCmd, &nCount))
	{
		return FALSE;
	}

	InputParams params = {0};
	if (!ParseInputString(szDstCmd, nCount, &params))
	{
		return FALSE;
	}

	CAMINFO caminfo;
	for (int j=0; j<MAX_VIDEO_NUM; j++)
	{
		caminfo.nYXIndex[j] = -1;
		caminfo.nCamNo[j] = 0;
	}

	caminfo.nYXIndex[0] = (int)params.nYxIndex;
	caminfo.nCamNo[0] = params.nCamNo;	
	COPYDATASTRUCT cds;
	cds.dwData = sizeof(COPYDATASTRUCT);
	cds.cbData = sizeof(caminfo);
	cds.lpData = (LPVOID)&caminfo;

	::SendMessage(hWnd, WM_COPYDATA, (WPARAM)STYLE_FULLSCREEN, (LPARAM)&cds);
	
	return FALSE;
}

BOOL SplitParams(const TCHAR *szSrcCmd, TCHAR szDstCmd[][64], int *pnCount)
{
	int i = 0;
	int j = 0;
	int n = 0;

	//按空格拆分
	while (szSrcCmd[n] != _T('\0'))
	{
		while (szSrcCmd[n] == _T(' '))
		{
			n++;
		}

		j = 0;

		if (szSrcCmd[n] == _T('\0'))
		{
			break;
		}

		while (szSrcCmd[n] != _T(' ') && szSrcCmd[n] != _T('\0'))
		{
			szDstCmd[i][j++] = szSrcCmd[n++];
		}
		szDstCmd[i][j] = _T('\0');
		i++;

		if (i >= 32)
		{
			break;
		}

	}

	*pnCount = i;

	return (i > 0);
}

BOOL ParseInputString(const TCHAR strInput[][64], int nCount, InputParams* pParams)
{
	if(pParams == NULL)
	{
		return FALSE;
	}

	memset(pParams, 0, sizeof(InputParams));

	const TCHAR *strArgument = _T("");

	for(int i = 0; i < nCount; i++)
	{
		if(_T('-') != strInput[i][0])
		{
			return FALSE;
		}
		if (0 == _tcscmp(strInput[i], _T("-yxid")))
		{
			GET_OPTION_POINTER(strArgument);
			pParams->nYxIndex = _ttoi(strArgument);
		}
		else if (0 == _tcscmp(strInput[i], _T("-camid")))
		{
			GET_OPTION_POINTER(strArgument);
			pParams->nCamNo = _ttoi(strArgument);
		}
		else
		{
			//未知命令
			return FALSE;
		}

	}

	if(pParams->nYxIndex <= 0)
	{
		return FALSE;
	}

	return TRUE;
}

const ModuleFileInfomations&  GetModuleFileInformations();
const ModuleFileInfomations& GetModuleFileInformations()
{
	static ModuleFileInfomations __s_mi;
	static BOOL bLoad = FALSE;

	if(!bLoad)
	{
		// Get application's full path.

		::GetModuleFileName(NULL, __s_mi.strFullPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
		__s_mi.strFullPath.ReleaseBuffer();

		// Break full path into seperate components.
		_splitpath(
			__s_mi.strFullPath, 
			__s_mi.strDrive.GetBufferSetLength(_MAX_DRIVE + 1),
			__s_mi.strDir.GetBufferSetLength(_MAX_DIR + 1),
			__s_mi.strName.GetBufferSetLength(_MAX_FNAME + 1),
			__s_mi.strExt.GetBufferSetLength(_MAX_EXT + 1));
		__s_mi.strDrive.ReleaseBuffer();
		__s_mi.strDir.ReleaseBuffer();
		__s_mi.strName.ReleaseBuffer();
		__s_mi.strExt.ReleaseBuffer();


		TCHAR   sDrive[_MAX_DRIVE];   
		TCHAR   sDir[_MAX_DIR];   
		TCHAR   sFilename[_MAX_FNAME],Filename[_MAX_FNAME];   
		TCHAR   sExt[_MAX_EXT];   
		GetModuleFileName(AfxGetInstanceHandle(),   Filename,   _MAX_PATH);   
		_tsplitpath(Filename,   sDrive,   sDir,   sFilename,   sExt);   
		CString   homeDir(CString(sDrive)   +   CString(sDir));   
		int   nLen   =   homeDir.GetLength();   
		if(homeDir.GetAt(nLen-1)   !=   _T('\\'))   
			homeDir   +=   _T('\\');   

		__s_mi.strPath = homeDir;

		bLoad = TRUE;
	}

	return __s_mi;
}

void CDVSShowApp::CaptureNetworkDidoSocket()
{return;
	EnterCriticalSection(&m_csNetworkDidoSocket);
}

void CDVSShowApp::ReleaseNetworkDidoSocket()
{return;
	LeaveCriticalSection(&m_csNetworkDidoSocket);
}

