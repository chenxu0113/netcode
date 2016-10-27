// VideoPlayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VideoPlayer.h"
#include "VideoPlayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


UINT DayOfWeek[] = {
		LOCALE_SDAYNAME7,   // Sunday
		LOCALE_SDAYNAME1,   
		LOCALE_SDAYNAME2,
		LOCALE_SDAYNAME3,
		LOCALE_SDAYNAME4, 
		LOCALE_SDAYNAME5, 
		LOCALE_SDAYNAME6   // Saturday
};

BOOL g_bLocalCapPic = FALSE;
CString g_sPathCapPic, g_sPathVideoRec;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
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
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoPlayerDlg dialog
void CALLBACK FileRefDone(DWORD nReserved, DWORD nUser)
{
	CVideoPlayerDlg* pOwner=(CVideoPlayerDlg*)nUser;
//	pOwner->UpdateStepBackUI(TRUE);	
}

void CALLBACK DisplayCBFun(long nPort,char * pBuf,long nSize,long nWidth,long nHeight,long nStamp,long nType,long nReserved)
{
	if (!g_bLocalCapPic)
		return;

	//switch function, of time-consuming; do not call it if you don't wanna save pic
	CString csDir;
	CString csFile;
	CTime time = CTime::GetCurrentTime();
	csFile.Format("%s\\%s_%04d%02d%02d_%02d%02d%02d_%d.bmp", g_sPathCapPic, g_sPathVideoRec, 
		time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond(), GetTickCount());
	PlayM4_ConvertToBmpFile(pBuf, nSize, nWidth, nHeight, nType, csFile.GetBuffer(csFile.GetLength()));
	g_bLocalCapPic = FALSE;
}


CVideoPlayerDlg::CVideoPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoPlayerDlg::IDD, pParent)
{
	m_ctEndTime = 0;
	m_ctStartTime = 0;
	m_iTotalSeconds = 0;
	m_nTimerPlay = -1;
	m_nTimerDateTime = -1;
	m_sVideoPath = _T("");
	m_sCurPlayRec = "";
	m_bAllVideo = FALSE;
	m_bStep = FALSE;
	m_bPlay = FALSE;
	m_bSound = FALSE;
	m_bPause = FALSE;
	m_bFind = FALSE;
	m_bFullScreen = FALSE;
	m_bIsTheSameFile = FALSE;
	m_hFindVideoFiles = NULL;
	m_dwFindVideoFiles = 0;

	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hPlayEnableIcon =			AfxGetApp()->LoadIcon(IDI_ENABLE_PLAY);
	m_hPlayDisableIcon =		AfxGetApp()->LoadIcon(IDI_DISABLE_PLAY);
	m_hPauseEnableIcon =		AfxGetApp()->LoadIcon(IDI_ENABLE_PAUSE);
	m_hPauseDisableIcon =		AfxGetApp()->LoadIcon(IDI_DISABLE_PAUSE);
	m_hStopEnableIcon =			AfxGetApp()->LoadIcon(IDI_ENABLE_STOP);
	m_hStopDisableIcon =		AfxGetApp()->LoadIcon(IDI_DISABLE_STOP);
	m_hSoundStartIcon=			AfxGetApp()->LoadIcon(IDI_ENABLE_SOUND);
	m_hSoundStopIcon=			AfxGetApp()->LoadIcon(IDI_DISABLE_SOUND);
//	m_hGotoHeadDisableIcon =	AfxGetApp()->LoadIcon(IDI_GOTOSTART_DISABLE);
//	m_hGotoTailDisableIcon =	AfxGetApp()->LoadIcon(IDI_GOTOEND_DISABLE);
	m_hFastDisableIcon =		AfxGetApp()->LoadIcon(IDI_DISABLE_FAST);
	m_hFastEnableIcon =			AfxGetApp()->LoadIcon(IDI_ENABLE_FAST);
	m_hSlowEnableIcon =			AfxGetApp()->LoadIcon(IDI_ENABLE_SLOW);
	m_hSlowDisableIcon=			AfxGetApp()->LoadIcon(IDI_DISABLE_SLOW);
//	m_hGotoHeadEnableIcon =		AfxGetApp()->LoadIcon(IDI_GOTOSTART_ENABLE);
//	m_hGotoTailEnableIcon =		AfxGetApp()->LoadIcon(IDI_GOTOEND_ENABLE);
	m_hStepFEnableIcon =		AfxGetApp()->LoadIcon(IDI_ENABLE_STEPFORWARD);
	m_hStepFDisableIcon =		AfxGetApp()->LoadIcon(IDI_DISABLE_STEPFORWARD);
	m_hStepBackEnableIcon =		AfxGetApp()->LoadIcon(IDI_ENABLE_STEPBACK);
	m_hStepBackDisableIcon =	AfxGetApp()->LoadIcon(IDI_DISABLE_STEPBACK);
	m_hCaptureIcon	=			AfxGetApp()->LoadIcon(IDI_CAPPIC);
}

CVideoPlayerDlg::~CVideoPlayerDlg()
{
}

void CVideoPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVideoPlayerDlg)
	DDX_Control(pDX, IDC_SLIDER_VOLUME, m_wndVolume);
	DDX_Control(pDX, IDC_SLIDER_PLAYPROGRESS, m_wndPlayProgress);
	DDX_Control(pDX, IDC_LIST_VIDEOFILE, m_wndVideoList);
	DDX_Control(pDX, IDC_DPT_ENDTIME, m_wndEndTime);
	DDX_Control(pDX, IDC_DPT_STARTTIME, m_wndStartTime);
	DDX_DateTimeCtrl(pDX, IDC_DPT_ENDTIME, m_ctEndTime);
	DDX_DateTimeCtrl(pDX, IDC_DPT_STARTTIME, m_ctStartTime);
	DDX_Text(pDX, IDC_EDIT_VIDEOPATH, m_sVideoPath);
	DDV_MaxChars(pDX, m_sVideoPath, 256);
	DDX_Check(pDX, IDC_CHECK_ALLVIDEO, m_bAllVideo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVideoPlayerDlg, CDialog)
	//{{AFX_MSG_MAP(CVideoPlayerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_FINDVIDEO, OnBtnFindvideo)
	ON_BN_CLICKED(IDC_BTN_VIDEOPATH, OnBtnVideopath)
	ON_BN_CLICKED(IDC_BTN_DELONE, OnBtnDelone)
	ON_BN_CLICKED(IDC_BTN_DELALL, OnBtnDelall)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_VIDEOFILE, OnDblclkListVideofile)
	ON_WM_HSCROLL()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_PLAYPROGRESS, OnReleasedcaptureSliderPlayprogress)
	ON_BN_CLICKED(IDC_BTN_PLAY, OnBtnPlay)
	ON_BN_CLICKED(IDC_BTN_STEPBACK, OnBtnStepback)
	ON_BN_CLICKED(IDC_BTN_STEPFORWARD, OnBtnStepforward)
	ON_BN_CLICKED(IDC_BTN_SLOW, OnBtnSlow)
	ON_BN_CLICKED(IDC_BTN_FAST, OnBtnFast)
	ON_BN_CLICKED(IDC_BTN_CAPPIC, OnBtnCappic)
	ON_BN_CLICKED(IDC_BTN_SOUND, OnBtnSound)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_VOLUME, OnReleasedcaptureSliderVolume)
	ON_WM_CLOSE()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_CHECK_ALLVIDEO, OnCheckAllvideo)
	ON_BN_CLICKED(IDC_BTN_STOPFINDVIDEO, OnBtnStopfindvideo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoPlayerDlg message handlers

BOOL CVideoPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		pSysMenu->DeleteMenu(0, MF_BYPOSITION);
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	// 初始化列表框的列
	m_wndVideoList.InsertColumn(0, "文件名", LVCFMT_LEFT, 400);
	m_wndVideoList.InsertColumn(1, "大小", LVCFMT_CENTER, 80);
	m_wndVideoList.InsertColumn(2, "类型", LVCFMT_CENTER, 60);
	m_wndVideoList.InsertColumn(4, "开始时间", LVCFMT_CENTER, 140);
	m_wndVideoList.InsertColumn(5, "结束时间", LVCFMT_CENTER, 140);
	m_wndVideoList.SetExtendedStyle(m_wndVideoList.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	// 初始化视频文件所在目录
	char szPath[MAX_PATH];
	GetWindowsDirectory(szPath, MAX_PATH);
	m_sIniPath.Format("%s\\CamParam.ini", szPath);
	GetPrivateProfileString("PATH", "RECORDPATH", "", m_sVideoPath.GetBuffer(MAX_PATH), MAX_PATH, m_sIniPath);
	m_sVideoPath.ReleaseBuffer();
	GetPrivateProfileString("PATH", "PICPATH", "", g_sPathCapPic.GetBuffer(MAX_PATH), MAX_PATH, m_sIniPath);
	g_sPathCapPic.ReleaseBuffer();

	UpdateData(FALSE);

	// 初始化开始与结束时间
	CString sFormat;
	CTime tt = CTime::GetCurrentTime();
	CTime ts(1970, 1, 1, 0, 0, 0);
	CTime te(2038, 1, 18, 0, 0, 0);
	m_wndStartTime.SetRange(&ts, &te);
	m_wndEndTime.SetRange(&ts, &te);
	sFormat.Format("yyyy-MM-dd HH:mm:ss");
	m_wndStartTime.SetFormat(sFormat);
	m_wndEndTime.SetFormat(sFormat);
	m_wndStartTime.SetTime(&tt);
	m_wndEndTime.SetTime(&tt);

	// 设置停止播放状态
	SetStopState();
	// 初始化播放窗口句柄
	m_hWndPlay1 = GetDlgItem(IDC_STATIC_WNDPLAY01)->m_hWnd;
	m_hWndPlay2 = GetDlgItem(IDC_STATIC_WNDPLAY02)->m_hWnd;
	// 初始化关于文本框信息
	CFont font;
	VERIFY(font.CreateFont(15,                        // nHeight
							0,                         // nWidth
							0,                         // nEscapement
							0,                         // nOrientation
							FW_NORMAL,                 // nWeight
							FALSE,                     // bItalic
							FALSE,                     // bUnderline
							0,                         // cStrikeOut
							ANSI_CHARSET,              // nCharSet
							OUT_DEFAULT_PRECIS,        // nOutPrecision
							CLIP_DEFAULT_PRECIS,       // nClipPrecision
							DEFAULT_QUALITY,           // nQuality
							DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
							"宋体"));                 // lpszFacename
	GetDlgItem(IDC_STATIC_ABOUT)->SetFont(&font);
	GetDlgItem(IDC_STATIC_TIME)->SetFont(&font);
	font.Detach();
	// 初始化时间
	InitDateAndTime();
	m_nTimerDateTime = SetTimer(TIMER_DATETIME, TIMER_INTERVAL, NULL);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CVideoPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVideoPlayerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVideoPlayerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////
// 初始化日期和时间
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::InitDateAndTime()
{
	TCHAR strWeekday[256];
	CTime tt = CTime::GetCurrentTime();
	::GetLocaleInfo(LOCALE_USER_DEFAULT, DayOfWeek[tt.GetDayOfWeek()-1], strWeekday, sizeof(strWeekday));
	CString str;
	str = tt.Format("%Y年%m月%d日 %H:%M:%S");
	str.Format("%s %s", str, strWeekday);
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(str);
}

/////////////////////////////////////////////
// 查找视频文件
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::OnBtnFindvideo() 
{
	AfxGetApp()->DoWaitCursor(1);
	m_wndVideoList.DeleteAllItems();
	CString sTemp;
	SYSTEMTIME ts, te;
	BOOL bStart = FALSE, bEnd = FALSE;

	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_ALLVIDEO);
	m_bAllVideo = pBtn->GetCheck();

	if (!m_bAllVideo)
	{
		m_wndStartTime.GetWindowText(sTemp);
		GetDateTimeByString(sTemp, ts);
		m_wndEndTime.GetWindowText(sTemp);
		GetDateTimeByString(sTemp, te);
		bStart = CheckDateTime(ts);
		bEnd = CheckDateTime(te);
		if (!bStart || !bEnd)
		{
			MessageBox("开始或结束时间有误！（1970.1.1--2038.1.18之间）", "提示", MB_OK|MB_ICONWARNING);
			return ;
		}

		CTime tts(ts.wYear, ts.wMonth, ts.wDay, ts.wHour, ts.wMinute, ts.wSecond);
		CTime tte(te.wYear, te.wMonth, te.wDay, te.wHour, te.wMinute, te.wSecond);
		CTimeSpan ts1 = tts - tte;
		if (tts >= tte)
		{
			MessageBox("开始时间不能大于等于结束时间！", "提示", MB_OK|MB_ICONWARNING);
			return ;
		}
		m_bFind = TRUE;
//		SearchAllFiles(m_sVideoPath, tts, tte, m_bAllVideo);
		m_params.sPath = m_sVideoPath;
		m_params.ts = tts;
		m_params.te = tte;
		m_params.bAllVideo = m_bAllVideo;
		m_hFindVideoFiles = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFindVideoFiles, this, 0, &m_dwFindVideoFiles);
	}
	else
	{
		CTime tts(1970, 1, 2, 0, 0, 0);
		CTime tte(2038, 1, 18, 0, 0, 0);
		m_bFind = TRUE;
		m_params.sPath = m_sVideoPath;
		m_params.ts = tts;
		m_params.te = tte;
		m_params.bAllVideo = m_bAllVideo;
		m_hFindVideoFiles = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadFindVideoFiles, this, 0, &m_dwFindVideoFiles);
//		SearchAllFiles(m_sVideoPath, tts, tte, m_bAllVideo);
	}
	
	AfxGetApp()->DoWaitCursor(-1);
}

/////////////////////////////////////////////
// 线程查找录像文件
// Input: lpvoid-类指针
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void ThreadFindVideoFiles(LPVOID lpvoid)
{
	CVideoPlayerDlg* pDlg = (CVideoPlayerDlg*)lpvoid;
	
	pDlg->GetDlgItem(IDC_BTN_STOPFINDVIDEO)->EnableWindow(TRUE);
	pDlg->GetDlgItem(IDC_DPT_STARTTIME)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_DPT_ENDTIME)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_CHECK_ALLVIDEO)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_BTN_DELONE)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_BTN_DELALL)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_BTN_VIDEOPATH)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_BTN_FINDVIDEO)->EnableWindow(FALSE);
	pDlg->GetDlgItem(IDC_BTN_FINDVIDEO)->SetWindowText("查找中...");
	pDlg->SearchAllFiles(pDlg->m_params.sPath, pDlg->m_params.ts, pDlg->m_params.te, pDlg->m_params.bAllVideo);
	if (pDlg->m_hFindVideoFiles != NULL)
	{
		CloseHandle(pDlg->m_hFindVideoFiles);
		pDlg->m_hFindVideoFiles = NULL;
		pDlg->GetDlgItem(IDC_BTN_FINDVIDEO)->EnableWindow(TRUE);
		pDlg->GetDlgItem(IDC_DPT_STARTTIME)->EnableWindow(TRUE);
		pDlg->GetDlgItem(IDC_DPT_ENDTIME)->EnableWindow(TRUE);
		pDlg->GetDlgItem(IDC_CHECK_ALLVIDEO)->EnableWindow(TRUE);
		pDlg->GetDlgItem(IDC_BTN_DELONE)->EnableWindow(TRUE);
		pDlg->GetDlgItem(IDC_BTN_DELALL)->EnableWindow(TRUE);
		pDlg->GetDlgItem(IDC_BTN_VIDEOPATH)->EnableWindow(TRUE);
		pDlg->GetDlgItem(IDC_BTN_STOPFINDVIDEO)->EnableWindow(FALSE);
		pDlg->GetDlgItem(IDC_BTN_FINDVIDEO)->SetWindowText("查找");
	}
}

/////////////////////////////////////////////
// 停止查找
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////

void CVideoPlayerDlg::OnBtnStopfindvideo() 
{
	m_bFind = FALSE;
	if (m_hFindVideoFiles != NULL)
	{
		CloseHandle(m_hFindVideoFiles);
		m_hFindVideoFiles = NULL;
		GetDlgItem(IDC_BTN_FINDVIDEO)->EnableWindow(TRUE);
		GetDlgItem(IDC_DPT_STARTTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_DPT_ENDTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_ALLVIDEO)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DELONE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DELALL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STOPFINDVIDEO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_FINDVIDEO)->SetWindowText("查找");
	}
}


/////////////////////////////////////////////
// 检测日期是否正确
// Input: tt-日期
// Ouput: NONE
// Return: TRUE-SUCCESS,FALSE-FAILED
//////////////////////////////////////////////
BOOL CVideoPlayerDlg::CheckDateTime(SYSTEMTIME tt)
{
	CTime ts(1970, 1, 1, 0, 0, 0);
	CTime te(2038, 1, 18, 0, 0, 0);
	CTime tt2(tt.wYear, tt.wMonth, tt.wDay, tt.wHour, tt.wMinute, tt.wSecond);

	if (tt2 <= ts || tt2 >= te)
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////
// 通过日期字符串，获取日期
// Input: sDate-日期字符串，tt-返回日期
// Ouput: NONE
// Return: TRUE-SUCCESS,FALSE-FAILED
//////////////////////////////////////////////
BOOL CVideoPlayerDlg::GetDateTimeByString(CString sDate, SYSTEMTIME& tt)
{
	CString sTemp;
	int nFind = -1;

	tt.wYear = 2010;
	tt.wMonth = 1;
	tt.wDay = 1;
	tt.wHour = 0;
	tt.wMinute = 0;
	tt.wSecond = 0;

	nFind = sDate.Find('-');
	tt.wYear = atoi(sDate.Left(nFind));
	sDate = sDate.Right(sDate.GetLength()-nFind-1);
	nFind = sDate.Find('-');
	tt.wMonth = atoi(sDate.Left(nFind));
	sDate = sDate.Right(sDate.GetLength()-nFind-1);
	nFind = sDate.Find(" ");
	tt.wDay = atoi(sDate.Left(nFind));
	sDate = sDate.Right(sDate.GetLength()-nFind-1);
	nFind = sDate.Find(":");
	tt.wHour = atoi(sDate.Left(nFind));
	sDate = sDate.Right(sDate.GetLength()-nFind-1);
	nFind = sDate.Find(":");
	tt.wMinute = atoi(sDate.Left(nFind));
	sDate = sDate.Right(sDate.GetLength()-nFind-1);
	tt.wSecond = atoi(sDate);

	return TRUE;
}

/////////////////////////////////////////////
// 查找视频文件-全部文件
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::SearchAllFiles(CString sPath, CTime tFrom, CTime tTo, BOOL bAllVideo/* =FALSE */)
{
	BOOL bFineDir = FALSE, bFindFile = FALSE;
	CFileFind fdDir, fdRecFile;
	CString sPathTemp, sFileName, sTemp;
	int nIndex = 0;
	CTime tt;

//	UpdateData();	
	bFineDir = fdDir.FindFile(sPath + "\\*.*");
	while (bFineDir)
	{
		bFineDir = fdDir.FindNextFile();
		if (fdDir.IsDots())
			continue;
		if (fdDir.IsDirectory() && m_bFind)
		{
			sPathTemp.Format("%s\\%s", sPath, fdDir.GetFileName());
			SearchAllFiles(sPathTemp, tFrom, tTo, bAllVideo);
		}
		else
		{
//			bFindFile = fdRecFile.FindFile(sPath + "\\*.mp4");
//			while (bFindFile)
			{
//				bFindFile = fdRecFile.FindNextFile();
//				if (fdRecFile.IsDots())
//					continue;
				fdDir.GetCreationTime(tt);
				if (!(CheckDate(tFrom, tt) && CheckDate(tt, tTo)) && !bAllVideo)
					continue ;
				{
//					sFileName = fdRecFile.GetFileName();
					sFileName = fdDir.GetFilePath();
					if (sFileName.Right(4).Compare(".mp4"))
						continue ;
					int nCount = m_wndVideoList.GetItemCount();
					m_wndVideoList.InsertItem(LVIF_TEXT|LVIF_STATE, nCount, sFileName, LVIS_SELECTED, LVIS_SELECTED, 0, 0);
					sTemp.Format("%ld KB", fdDir.GetLength()/1024);
					m_wndVideoList.SetItemText(nCount, 1, sTemp);
					m_wndVideoList.SetItemText(nCount, 2, "mp4");
					fdDir.GetCreationTime(tt);
					sTemp.Format("%d-%02d-%02d %02d:%02d:%02d", tt.GetYear(), tt.GetMonth(), tt.GetDay(), tt.GetHour(), tt.GetMinute(), tt.GetSecond());
					m_wndVideoList.SetItemText(nCount, 3, sTemp);
					fdDir.GetLastWriteTime(tt);
					sTemp.Format("%d-%02d-%02d %02d:%02d:%02d", tt.GetYear(), tt.GetMonth(), tt.GetDay(), tt.GetMonth(), tt.GetMinute(), tt.GetSecond());
					m_wndVideoList.SetItemText(nCount, 4, sTemp);
				}
			}
		}
	}
	if (m_wndVideoList.GetItemCount() > 0)
	{
		m_wndVideoList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
		m_wndVideoList.SetItemState(0, ~LVIS_SELECTED, LVIS_SELECTED);
	}
}

/////////////////////////////////////////////
// 检测日期是否在正确的范围内
// Input: tFrom-开始时间，tTo-结束时间
// Ouput: NONE
// Return: TRUE-在正确的范围内，FALSE-不在正确的范围内
//////////////////////////////////////////////
BOOL CVideoPlayerDlg::CheckDate(CTime tFrom, CTime tTo)
{
	if (tFrom.GetYear() > tTo.GetYear())
	{
		return FALSE;
	}
	else
	{
		if (tFrom.GetYear() == tTo.GetYear())
		{
			if (tFrom.GetMonth() > tTo.GetMonth())
			{
				return FALSE;
			}
			else
			{
				if (tFrom.GetMonth() == tTo.GetMonth())
				{
					if (tFrom.GetDay() > tTo.GetDay())
					{
						return FALSE;
					}
					else
					{
						if (tFrom.GetDay() == tTo.GetDay())
						{
							if (tFrom.GetHour() > tTo.GetHour())
							{
								return FALSE;
							}
							else
							{
								if (tFrom.GetHour() == tTo.GetHour())
								{
									if (tFrom.GetMinute() > tTo.GetMinute())
									{
										return FALSE;
									}
									else
									{
										if (tFrom.GetMinute() == tTo.GetMinute())
										{
											if (tFrom.GetSecond() > tTo.GetSecond())
											{
												return FALSE;
											}
										}//end sec
									}//end min
								}
							}//end hour
						}
					}//end day
				}
			}//end month
		}
	}//end year
	return TRUE;
}


/////////////////////////////////////////////
// 视频文件路径选择
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::OnBtnVideopath() 
{
	AfxGetApp()->DoWaitCursor(1);
	char szDir[MAX_PATH];
	BROWSEINFO bi;
	ITEMIDLIST *pidl = NULL;
	
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDir;
	bi.lpszTitle = "请选择目录";
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	pidl = SHBrowseForFolder(&bi);
	if(pidl == NULL)
		return;
	if (!SHGetPathFromIDList(pidl, szDir))
		return;

	m_sVideoPath.Format("%s", szDir);
	GetDlgItem(IDC_EDIT_VIDEOPATH)->SetWindowText(m_sVideoPath);

	AfxGetApp()->DoWaitCursor(-1);
}

/////////////////////////////////////////////
// 系统消息翻译
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
BOOL CVideoPlayerDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_SYSKEYDOWN)
	{
//		if(pMsg->wParam == VK_F4)
//			return TRUE;
	}
	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE)
			return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////
// 删除一条视频记录
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::OnBtnDelone() 
{
	AfxGetApp()->DoWaitCursor(1);
	UpdateData(TRUE);
	
	int iFilePosSel = 0;
	CString csFileName;
	
	POSITION  iPos = m_wndVideoList.GetFirstSelectedItemPosition();
	if (iPos == NULL)
	{
		MessageBox("没有选择录像文件！", "提示", MB_OK|MB_ICONWARNING);
		return;
	}
	iFilePosSel = m_wndVideoList.GetNextSelectedItem(iPos);
	csFileName = m_wndVideoList.GetItemText(iFilePosSel, 0);
	if (csFileName.GetLength()==0)
	{
		MessageBox("没有选择录像文件！", "提示", MB_OK|MB_ICONWARNING);
		return;
	}

	if (IDOK != MessageBox("此操作将直接从硬盘上删除此文件，确定删除吗？", "提示", MB_OKCANCEL|MB_ICONWARNING))
		return ;
	if (DeleteFile(csFileName))
		m_wndVideoList.DeleteItem(iFilePosSel);
	else
		MessageBox("不能删除文件！", "提示", MB_OK|MB_ICONWARNING);
	AfxGetApp()->DoWaitCursor(-1);
}

/////////////////////////////////////////////
// 删除列表中所有视频记录
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::OnBtnDelall() 
{
	AfxGetApp()->DoWaitCursor(1);
	int iCount=0, i=0;
	CString csFileName;
	char szLan[1024] = {0};
	
	iCount = m_wndVideoList.GetItemCount();
	if (iCount < 1)
	{
		MessageBox("没有录像文件！", "提示", MB_OK|MB_ICONWARNING);
		return ;
	}
	if (IDOK != MessageBox("此操作将直接从硬盘上删除视频文件，确定删除吗？", "提示", MB_OKCANCEL|MB_ICONWARNING))
		return ;
	if (IDOK != MessageBox("您确认删除所有文件吗？", "提示", MB_OKCANCEL|MB_ICONWARNING))
		return ;
	for (i = iCount; i > 0; i--)
	{
		csFileName = m_wndVideoList.GetItemText(i-1, 0);
		if (csFileName.GetLength() == 0)
			continue;
		if (DeleteFile(csFileName))
			m_wndVideoList.DeleteItem(i-1);
		else
			MessageBox("不能删除文件！", "提示", MB_OK|MB_ICONWARNING);
	}
	AfxGetApp()->DoWaitCursor(-1);
}

/////////////////////////////////////////////
// 灰化播放按钮状态
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::SetStopState()
{
	CButton *pBtn = NULL;
	
//	m_rcScreen=CRect(0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));
	m_wndPlayProgress.SetRange(0, 100);
	m_wndPlayProgress.SetPos(0);
	m_wndVolume.SetRange(0, 0xffff);
	m_wndVolume.SetPos(0x7fff);
	m_wndPlayProgress.EnableWindow(FALSE);
	m_wndVolume.EnableWindow(FALSE);
	
	pBtn = (CButton *)GetDlgItem(IDC_BTN_PLAY); 
	pBtn->SetIcon(m_hPlayDisableIcon);
	pBtn->EnableWindow(FALSE);

	pBtn = (CButton *)GetDlgItem(IDC_BTN_PLAY);
	pBtn->SetIcon(m_hPlayDisableIcon);
	pBtn->EnableWindow(FALSE);
	
	pBtn = (CButton *)GetDlgItem(IDC_BTN_STOP);
	pBtn->SetIcon(m_hStopDisableIcon);
	pBtn->EnableWindow(FALSE);
	
	pBtn = (CButton *)GetDlgItem(IDC_BTN_FAST);
	pBtn->SetIcon(m_hFastDisableIcon);
	pBtn->EnableWindow(FALSE);
	
	pBtn = (CButton *)GetDlgItem(IDC_BTN_SLOW);
	pBtn->SetIcon(m_hSlowDisableIcon);
	pBtn->EnableWindow(FALSE);
	
	pBtn = (CButton *)GetDlgItem(IDC_BTN_CAPPIC);
	pBtn->SetIcon(m_hCaptureIcon);
	pBtn->EnableWindow(FALSE);
	
	pBtn = (CButton *)GetDlgItem(IDC_BTN_STEPBACK);
	pBtn->SetIcon(m_hStepBackDisableIcon);
	pBtn->EnableWindow(FALSE);
	
	pBtn = (CButton *)GetDlgItem(IDC_BTN_STEPFORWARD);
	pBtn->SetIcon(m_hStepFDisableIcon);
	pBtn->EnableWindow(FALSE);

	pBtn = (CButton *)GetDlgItem(IDC_BTN_SOUND);
	pBtn->SetIcon(m_hSoundStopIcon);
	pBtn->EnableWindow(FALSE);
}

/////////////////////////////////////////////
// 双击视频文件
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::OnDblclkListVideofile(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLISTVIEW* pNMListView = (NMLISTVIEW*)pNMHDR;
	CString str; 
	str = m_wndVideoList.GetItemText(pNMListView->iItem, pNMListView->iSubItem);
	str.TrimLeft(" ");
	str.TrimRight(" ");
	if (!str.Compare(m_sCurPlayRec))
		m_bIsTheSameFile = TRUE;
	else
		m_bIsTheSameFile = FALSE;
	if (m_bPlay)
	{
		OnBtnStop();
	}
	OnBtnPlay();

	*pResult = 0;
}

/////////////////////////////////////////////
// 停止播放视频
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::StopPlayVideo()
{
	m_bPause = FALSE;
	if (m_bPlay)
	{
		m_bPlay = FALSE;
		PlayM4_Stop(USED_PORT);
		PlayM4_CloseFile(USED_PORT);
		if (m_bSound)
			StartPlaySound(FALSE);
		SetStopState();
		g_sPathVideoRec = "";
		m_sCurPlayRec = "";
	}
}

/////////////////////////////////////////////
// 播放视频
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::StartPlayVideo()
{
	int iFilePosSel = 0, nPos = 0;
	CString csFileName;

	UpdateData(TRUE);
	POSITION  iPos = m_wndVideoList.GetFirstSelectedItemPosition();
	if (iPos == NULL)
	{
		MessageBox("请选择要播放的文件！", "提示", MB_OK|MB_ICONWARNING);
		return;
	}
	iFilePosSel = m_wndVideoList.GetNextSelectedItem(iPos);
	
	PlayM4_SetFileRefCallBack(USED_PORT, FileRefDone, (DWORD)this);
	csFileName = m_wndVideoList.GetItemText(iFilePosSel, 0);
	if (csFileName.GetLength()==0)
		return;
	if (!PlayM4_OpenFile(USED_PORT, csFileName.GetBuffer(csFileName.GetLength())))
		return;

	// 获取录像文件的IP地址及窗口号
	CString sTemp = csFileName;
	int nFind = -1;
	nFind = sTemp.ReverseFind('\\');
	sTemp = sTemp.Right(sTemp.GetLength()-nFind-1);
	nFind = sTemp.Find("_");
	g_sPathVideoRec = sTemp.Left(nFind+1);
	sTemp = sTemp.Right(sTemp.GetLength()-nFind-1);
	nFind = sTemp.Find("_");
	g_sPathVideoRec += sTemp.Left(nFind);

	m_sCurPlayRec = csFileName;
	m_iTotalSeconds = PlayM4_GetFileTime(USED_PORT);
 	if (m_iTotalSeconds <= 0)
 	{
 		PlayM4_CloseFile(USED_PORT);
		MessageBox("文件大小为零！", "提示", MB_OK|MB_ICONWARNING);
		return;
 	}
 	
	PlayM4_SetDisplayCallBack(USED_PORT, DisplayCBFun);	
	if ((m_bPlay = PlayM4_Play(USED_PORT, m_hWndPlay1)) == FALSE)
	{
		PlayM4_CloseFile(USED_PORT);
		return;
	}
}

/////////////////////////////////////////////
// 滚动条-横向
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar->m_hWnd == m_wndPlayProgress.m_hWnd && m_bPlay)
	{
		nPos = m_wndPlayProgress.GetPos();
		m_wndPlayProgress.SetPos(nPos);
	}
	else if (pScrollBar->m_hWnd == m_wndVolume.m_hWnd && m_bSound)
	{
		nPos = m_wndVolume.GetPos();
		m_wndVolume.SetPos(nPos);
	}
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

/////////////////////////////////////////////
// 定时器
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TIMER_PLAY)
	{
		SetPlaying();
	}
	else if (nIDEvent == TIMER_DATETIME)
		InitDateAndTime();
	
	CDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////
// 播放时状态
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::SetPlaying()
{
	if (m_iTotalSeconds == 0)
		return;
	
	DWORD nCurSecond = PlayM4_GetPlayedTime(USED_PORT);
	m_wndPlayProgress.SetPos(nCurSecond*100/m_iTotalSeconds);
	CString csStatus;
	csStatus.Format("%02d:%02d:%02d / %02d:%02d:%02d ",\
						nCurSecond/3600,(nCurSecond%3600)/60,nCurSecond%60,\
						m_iTotalSeconds/3600,(m_iTotalSeconds%3600)/60,m_iTotalSeconds%60);
	float fCurPos = PlayM4_GetPlayPos(USED_PORT);
	GetDlgItem(IDC_STATIC_STATUS01)->SetWindowText(csStatus);
	if (fCurPos <= 1.00+0.000001 && fCurPos >= 1.00-0.000001)
	{
		if (m_bFullScreen)
		{
			SetFullScreen(!m_bFullScreen);
			m_bFullScreen = !m_bFullScreen;
		}
		OnBtnStop();
	}
}

/////////////////////////////////////////////
// 设置播放时各控件状态
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::SetPlayState_Ctrls()
{
	CButton *pButton;
	
	m_wndPlayProgress.EnableWindow(TRUE);
	m_wndVolume.EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_PLAY);
	pButton->SetIcon(m_hPauseEnableIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_STOP);
	pButton->SetIcon(m_hStopEnableIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_FAST);
	pButton->SetIcon(m_hFastEnableIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_SLOW);
	pButton->SetIcon(m_hSlowEnableIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_CAPPIC);
	pButton->SetIcon(m_hCaptureIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_STEPBACK);
	pButton->SetIcon(m_hStepBackEnableIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_STEPFORWARD);
	pButton->SetIcon(m_hStepFEnableIcon);
	pButton->EnableWindow(TRUE);
	
	pButton = (CButton *)GetDlgItem(IDC_BTN_SOUND);
	if (m_bSound)
		pButton->SetIcon(m_hSoundStartIcon);
	else
		pButton->SetIcon(m_hSoundStopIcon);
	pButton->EnableWindow(TRUE);
}

/////////////////////////////////////////////
// 按钮-停止播放
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::OnBtnStop() 
{
	AfxGetApp()->DoWaitCursor(1);
	if (!m_bPlay)
		return ;
	m_wndPlayProgress.SetPos(0); 
	GetDlgItem(IDC_STATIC_STATUS01)->SetWindowText("");
	GetDlgItem(IDC_STATIC_WNDPLAY01)->Invalidate();
	if (m_nTimerPlay != -1)
	{
		KillTimer(m_nTimerPlay);
		m_nTimerPlay = -1;
	}
	StopPlayVideo();
	AfxGetApp()->DoWaitCursor(-1);
}

/////////////////////////////////////////////
// 播放进度条-单击
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::OnReleasedcaptureSliderPlayprogress(NMHDR* pNMHDR, LRESULT* pResult) 
{
	AfxGetApp()->DoWaitCursor(1);
	int nPos = 0;
	nPos = m_wndPlayProgress.GetPos();
	m_wndPlayProgress.SetPos(nPos);
	PlayM4_SetPlayedTimeEx(USED_PORT, m_iTotalSeconds*nPos*10);
	AfxGetApp()->DoWaitCursor(-1);
	
	*pResult = 0;
}

/////////////////////////////////////////////
// 按钮-播放/暂停
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::OnBtnPlay() 
{
	AfxGetApp()->DoWaitCursor(1);
	if (m_bPlay)// && m_bIsTheSameFile)
	{
		if (m_bStep)
		{
			PlayM4_Play(USED_PORT, m_hWndPlay1);
			SetPlayState_Ctrls();
			m_bStep = FALSE;
		}
		else
			PausePlayVideo();
		return ;
	}

	StartPlayVideo();
	
	if (m_bPlay)
	{
		SetPlayState_Ctrls();
		StartPlaySound(TRUE);
		m_nTimerPlay = SetTimer(TIMER_PLAY, TIMER_INTERVAL, NULL);
	}
	AfxGetApp()->DoWaitCursor(-1);
}

/////////////////////////////////////////////
// 按钮-步退
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::OnBtnStepback() 
{
	AfxGetApp()->DoWaitCursor(1);
	if (m_bPause)
		PausePlayVideo();
	PlayM4_OneByOneBack(USED_PORT);
	m_bStep = TRUE;
	SetPauseState_Ctrls();
	AfxGetApp()->DoWaitCursor(-1);
}

/////////////////////////////////////////////
// 按钮-步进
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::OnBtnStepforward() 
{
	AfxGetApp()->DoWaitCursor(1);
	if (m_bPause)
		PausePlayVideo();
	PlayM4_OneByOne(USED_PORT);
	m_bStep = TRUE;
	SetPauseState_Ctrls();
	AfxGetApp()->DoWaitCursor(-1);
}

/////////////////////////////////////////////
// 暂停播放视频
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::PausePlayVideo()
{
	if (PlayM4_Pause(USED_PORT, !m_bPause))
		m_bPause =! m_bPause;
	
	if (m_bPause)
		SetPauseState_Ctrls();
	else
		SetPlayState_Ctrls();
}

/////////////////////////////////////////////
// 设置播放暂停
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::SetPauseState_Ctrls()
{
	CButton *pButton = NULL;
	pButton = (CButton *)GetDlgItem(IDC_BTN_PLAY);
	pButton->SetIcon(m_hPlayEnableIcon);
	pButton->EnableWindow(TRUE);
}

/////////////////////////////////////////////
// 按钮-减缓播放
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::OnBtnSlow() 
{
	AfxGetApp()->DoWaitCursor(1);
	PlayM4_Slow(USED_PORT);
	AfxGetApp()->DoWaitCursor(-1);
}

/////////////////////////////////////////////
// 按钮-加速播放
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::OnBtnFast() 
{
	AfxGetApp()->DoWaitCursor(1);
	PlayM4_Fast(USED_PORT);
	AfxGetApp()->DoWaitCursor(-1);
}

/////////////////////////////////////////////
// 按钮-抓拍图片
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::OnBtnCappic() 
{
	g_bLocalCapPic = TRUE;
}

/////////////////////////////////////////////
// 按钮-是否开启声音
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::OnBtnSound() 
{
	AfxGetApp()->DoWaitCursor(1);
	StartPlaySound(!m_bSound);
	AfxGetApp()->DoWaitCursor(-1);
}

/////////////////////////////////////////////
// 是否开启声音
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::StartPlaySound(BOOL bSound/* =FALSE */)
{
	if (!m_bPlay)
		return;
	
	if (bSound)
	{
		m_bSound = PlayM4_PlaySound(USED_PORT);
		PlayM4_SetVolume(USED_PORT, (WORD)m_wndVolume.GetPos());
		SetPlaySoundState();
	}
	else
	{
		PlayM4_StopSound();
		m_bSound = FALSE;
		SetStopSoundState();
	}
}

/////////////////////////////////////////////
// 设置声音开启状态
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::SetPlaySoundState()
{
	CButton *pButton = NULL;
	pButton = (CButton *)GetDlgItem(IDC_BTN_SOUND);
	pButton->SetIcon(m_hSoundStartIcon);
	pButton->EnableWindow(TRUE);	
}


/////////////////////////////////////////////
// 设置声音关闭状态
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::SetStopSoundState()
{
	CButton *pButton = NULL;
	pButton = (CButton *)GetDlgItem(IDC_BTN_SOUND);
	pButton->SetIcon(m_hSoundStopIcon);
	pButton->EnableWindow(TRUE);
}

/////////////////////////////////////////////
// 状态条-设置声音大小
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::OnReleasedcaptureSliderVolume(NMHDR* pNMHDR, LRESULT* pResult) 
{
	AfxGetApp()->DoWaitCursor(1);
	int nPos = 0;
	nPos = m_wndVolume.GetPos();
	m_wndVolume.SetPos(nPos);
	PlayM4_SetVolume(USED_PORT, (WORD)m_wndVolume.GetPos());

	AfxGetApp()->DoWaitCursor(-1);
	
	*pResult = 0;
}

/////////////////////////////////////////////
// 退出软件
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::OnClose() 
{
	AfxGetApp()->DoWaitCursor(1);
	OnBtnStop();
	if (m_nTimerDateTime != -1)
	{
		KillTimer(m_nTimerDateTime);
		m_nTimerDateTime = -1;
	}
// 	NET_DVR_ReleaseDevice_Card();
// 	NET_DVR_ReleaseDDraw_Card();
// 	NET_DVR_ClientAudioStop();

	AfxGetApp()->DoWaitCursor(-1);
	CDialog::OnClose();
}

/////////////////////////////////////////////
// 左键双击视频窗口
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (IsInWnd(point) && m_bPlay)
	{
		m_bFullScreen = !m_bFullScreen;
		SetFullScreen(m_bFullScreen);
		if (m_bFullScreen)
		{
//			PlayBackFullScreen();
		}
		else
		{
//			PlayBackShowNormal();
		}
		PlayM4_RefreshPlay(USED_PORT);
	}
	
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CVideoPlayerDlg::SetFullScreen(BOOL bFullScreen)
{
	if (bFullScreen)
	{
		SetFullScreenState(TRUE);
		GetWindowPlacement(&m_struOldWndpl);
		GetDlgItem(IDC_STATIC_WNDPLAY01)->GetWindowRect(m_rcOldWinRect);
		
		CRect rectWholeDlg;//entire client(including title bar)
		CRect rectClient;//client area(not including title bar)
		CRect rectFullScreen;
		int cxFullScreen, cyFullScreen;
		GetWindowRect(&rectWholeDlg);
		RepositionBars(0, 0xffff, AFX_IDW_PANE_FIRST, reposQuery, &rectClient);
		ClientToScreen(&rectClient);

		cxFullScreen = ::GetSystemMetrics(SM_CXSCREEN);
		cyFullScreen = ::GetSystemMetrics(SM_CYSCREEN);
		
		rectFullScreen.left = rectWholeDlg.left - rectClient.left;
		rectFullScreen.top = rectWholeDlg.top - rectClient.top;
		rectFullScreen.right = rectWholeDlg.right + cxFullScreen - rectClient.right;
		rectFullScreen.bottom = rectWholeDlg.bottom + cyFullScreen - rectClient.bottom + 5;
		WINDOWPLACEMENT struWndpl;
		struWndpl.length = sizeof(WINDOWPLACEMENT);
		struWndpl.flags = 0;
		struWndpl.showCmd = SW_SHOWNORMAL;
		struWndpl.rcNormalPosition = rectFullScreen;
		SetWindowPlacement(&struWndpl);
		CRect rc;
		rc.left = -2;
		rc.top = -2;
		rc.right = cxFullScreen + 4;
		rc.bottom = cyFullScreen + 5;
		GetDlgItem(IDC_STATIC_WNDPLAY01)->MoveWindow(rc);
	}
	else
	{
		SetFullScreenState(FALSE);
		SetWindowPlacement(&m_struOldWndpl);
		ScreenToClient(m_rcOldWinRect);
		GetDlgItem(IDC_STATIC_WNDPLAY01)->MoveWindow(m_rcOldWinRect);
	}
}

/////////////////////////////////////////////
// 设置窗口最大化时各控件按钮状态
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CVideoPlayerDlg::SetFullScreenState(BOOL bFlag/* =FALSE */)
{
	BOOL bStatus = bFlag ? SW_HIDE : SW_SHOW;
	GetDlgItem(IDC_LIST_VIDEOFILE)->ShowWindow(bStatus);
	GetDlgItem(IDC_STATIC_TIME)->ShowWindow(bStatus);
	GetDlgItem(IDC_STATIC_STATUS01)->ShowWindow(bStatus);
	GetDlgItem(IDC_SLIDER_PLAYPROGRESS)->ShowWindow(bStatus);
}

/////////////////////////////////////////////
// 是否在视频窗口内
// Input: point-当前鼠标双击点
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
BOOL CVideoPlayerDlg::IsInWnd(CPoint point)
{
	CRect rc;
	GetDlgItem(IDC_STATIC_WNDPLAY01)->GetWindowRect(&rc);
	ScreenToClient(&rc);
	if (PtInRect(&rc, point))
		return TRUE;

	return FALSE;
}

void CVideoPlayerDlg::OnCheckAllvideo() 
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_ALLVIDEO);
	if (pBtn->GetCheck())
	{
		GetDlgItem(IDC_DPT_STARTTIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_DPT_ENDTIME)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_DPT_STARTTIME)->EnableWindow(TRUE);
		GetDlgItem(IDC_DPT_ENDTIME)->EnableWindow(TRUE);
	}
}

