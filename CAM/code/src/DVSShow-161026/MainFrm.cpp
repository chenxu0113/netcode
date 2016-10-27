#include "stdafx.h"
#include "DVSShow.h"

#include "MainFrm.h"
#include "AW_CMultiViewSplitter/AW_CMultiViewSplitter.h"
#include <math.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMainFrame* g_pMainFrame = NULL;
extern CDVSShowApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CBCGPFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_MESSAGE(WM_COPYDATA, OnReceiveInfo)
	ON_COMMAND(ID_VIDEO_START, OnStartVideo)
	ON_COMMAND(ID_SET_SYSSETTINGS, OnSetSysSettings)
	ON_COMMAND(ID_SYS_ABOUT, OnSysAbout)
	ON_COMMAND(ID_SYS_EXIT, OnSysExit)
	ON_COMMAND(ID_VIDEO_CAPPIC, OnCapturePic)
	ON_COMMAND(ID_VIEW_LEFT, OnViewLeft)
	ON_COMMAND(ID_VIEW_ALARM, OnViewAlarm)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LEFT, OnUpdateViewLeft)
	ON_UPDATE_COMMAND_UI(ID_VIDEO_CAPPIC, OnUpdateCapPic)
	ON_UPDATE_COMMAND_UI(ID_VIDEO_CONTROL, OnUpdateControl)
	ON_UPDATE_COMMAND_UI(ID_VIDEO_START, OnUpdateStartVideo)
	ON_UPDATE_COMMAND_UI(ID_VIDEO_STOP, OnUpdateStopVideo)
	ON_COMMAND(ID_SET_VIDEOSETTINGS, OnSetVideosettings)
	ON_COMMAND(ID_VIEW_VIDEOPLAY, OnViewVideoplay)
	ON_COMMAND(ID_VIDEO_STOP, OnStopVideo)
	ON_COMMAND(ID_VIDEO_CONTROL, OnControl)
	ON_COMMAND_EX(ID_VIEW_TOOLBAR, OnBarCheck)
	ON_MESSAGE(WM_TRAYNOTIFICATION,OnTrayNotification)
	ON_COMMAND(ID_SHOWORHIDEWND, &CMainFrame::ShowOrHideWnd)
	ON_UPDATE_COMMAND_UI(ID_SHOWORHIDEWND, &CMainFrame::OnUpdateShowOrHideWnd)
	ON_COMMAND(ID_DESTROY, &CMainFrame::OnDestroy)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_UPDATE_COMMAND_UI(ID_VIEW_VIDEOPLAY, &CMainFrame::OnUpdateViewVideoplay)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_nViewVideo1 = -1;
	m_nCurVideoNum = 4;
	m_bIsCameraNode = FALSE;
	m_bShowContrl = TRUE;
	m_pControl = NULL;
	g_pMainFrame = this;
	
	//// 创建云台控制对话框
// 	m_pControl = new CControl;
//	m_pControl->Create(IDD_DLG_CONTROL, this);
// 	m_pControl->ShowWindow(SW_HIDE);
	
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BMP_CAMERA);
	m_ilTreeImage.Create(16, 16, ILC_COLORDDB|ILC_MASK, 7, 1);
	m_ilTreeImage.Add(&bitmap, RGB(192,192,192));
}

CMainFrame::~CMainFrame()
{
	if (m_pControl)
	{
		delete m_pControl;
		m_pControl = NULL;
	}

	m_ilTreeImage.DeleteImageList();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_LunaBlue);
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
	CBCGPDockManager::SetDockMode (BCGP_DT_SMART);
	if (CBCGPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;


	if (NULL == theApp.m_pDlgAlarm)
	{
		theApp.m_pDlgAlarm = new CDlgAlarm();
		theApp.m_pDlgAlarm->Create(IDD_DLG_ALARM);
	}

	CBCGPPopupMenu::SetForceMenuFocus (FALSE);

	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);
	
	CClientDC dc (this);
	BOOL bIsHighColor = dc.GetDeviceCaps (BITSPIXEL) > 8;

	UINT uiToolbarHotID = bIsHighColor ? IDB_BITMAP_BAR : 0;
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME1, 0, 0, FALSE, 0, 0, uiToolbarHotID))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndToolBar.SetSizes(CSize (32, 32), CSize (32, 32));
	m_wndToolBar.SetMenuSizes (CSize (22, 22), CSize (16, 16));


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	CBCGPToolBarImages imagesWorkspace;
	imagesWorkspace.SetImageSize (CSize (16, 16));
	imagesWorkspace.SetTransparentColor (RGB (255, 0, 255));
	imagesWorkspace.Load (IDB_BMP_CAMERA);
	imagesWorkspace.SmoothResize(globalData.GetRibbonImageScale());

	if (!m_wndWorkSpace.Create (_T("  "), this, CRect (0, 0, 200, 200),
		TRUE, ID_VIEW_LEFT,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Workspace bar\n");
		return -1;      // fail to create
	}
	m_wndWorkSpace.SetIcon (imagesWorkspace.ExtractIcon (0), FALSE);

	m_wndToolBar.SetWindowText (_T("工具栏"));
	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace.EnableDocking(CBRS_ALIGN_LEFT);
	m_wndToolBar.SetBarStyle (m_wndToolBar.GetBarStyle () & ~CBRS_GRIPPER);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndWorkSpace);

 	CBCGPTabbedControlBar::m_StyleTabWnd = CBCGPTabWnd::STYLE_3D;
 	CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_LunaBlue);
 	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
 	CBCGPDockManager::SetDockMode (BCGP_DT_SMART);


		/////////使浮动与停靠显示相同的头部////
#ifdef _SCB_REPLACE_MINIFRAME
    m_pFloatingFrameClass = RUNTIME_CLASS(CSCBMiniDockFrameWnd);
#endif //_SCB_REPLACE_MINIFRAME

	SetProp(GetSafeHwnd(), theApp.m_pszExeName, MAINWND_PROP_HANDLE);	
	InitialTray();
	AddTray();
	
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style &= ~FWS_ADDTOTITLE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

// 分割视图
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if (!m_wndSplitter.CreateStatic(this, 1, 1))
 		return FALSE;
	
	m_nViewVideo1 = m_wndSplitter.AddView(0, 0, RUNTIME_CLASS(CDlgVideo1), pContext);
	if (!m_nViewVideo1)
		return FALSE;
	
	m_wndSplitter.ShowView(m_nViewVideo1);
	
	return TRUE;
//	return CBCGPFrameWnd::OnCreateClient(lpcs, pContext);
}


// 接收其它进程发送过来的摄像机编号信息
LRESULT CMainFrame::OnReceiveInfo(WPARAM wParam, LPARAM lParam)
{
	

	return 1;
}

// 开始视频
void CMainFrame::OnStartVideo()
{
}

/////////////////////////////////////////////
// 停止视频
// Input: NONE
// Ouput: NONE
// Return: NONE
/////////////////////////////////////////////
void CMainFrame::OnStopVideo()
{
	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();

	int nIndex = pVideo1->m_nSubWndIndex;
	if (!theApp.m_dlgOutput[nIndex].m_bStartVideo)
	{
//		videoParam.hWnd = NULL;
		pVideo1->StopRTVideoEx(nIndex);
		theApp.m_dlgOutput[nIndex].Invalidate(TRUE);
		theApp.m_dlgOutput[nIndex].m_lPlayHandle = -1;
		theApp.m_dlgOutput[nIndex].m_bStartVideo = TRUE;
		theApp.m_dlgOutput[nIndex].m_bCapPic = FALSE;
		theApp.m_dlgOutput[nIndex].m_bStopVideo = FALSE;
		theApp.m_dlgOutput[nIndex].m_lChannelNum = -1;
		theApp.m_dlgOutput[nIndex].m_sCameraIP = "";
		theApp.m_dlgOutput[nIndex].m_nYxIndex = -1;
		theApp.m_dlgOutput[nIndex].m_nCameraPort = -1;
	}
}

/////////////////////////////////////////////
// 系统设置
// Input: NONE
// Ouput: NONE
// Return: NONE
/////////////////////////////////////////////
void CMainFrame::OnSetSysSettings()
{
	//// 创建设置对话框
	CSysSettings dlg;
	dlg.DoModal();
}

/////////////////////////////////////////////
// 视频相关设置
// Input: NONE
// Ouput: NONE
// Return: NONE
/////////////////////////////////////////////
void CMainFrame::OnSetVideosettings() 
{
	CDlgVideoSettings dlg;
	dlg.DoModal();
	return;	
}


// 关于信息
void CMainFrame::OnSysAbout()
{
	theApp.OnAppAbout();
}

// 退出系统
void CMainFrame::OnSysExit()
{
	OnClose();
}

// 抓拍图片
void CMainFrame::OnCapturePic()
{
	theApp.m_dlgOutput[theApp.GetDlgVideo1()->m_nSubWndIndex].CapturePic();
}



// 显示/隐藏工具栏
BOOL CMainFrame::OnBarCheck(UINT nID)
{
	ASSERT(ID_VIEW_STATUS_BAR == AFX_IDW_STATUS_BAR);
	ASSERT(ID_VIEW_TOOLBAR == AFX_IDW_TOOLBAR);
	
	//得到工具条
	CBCGPBaseControlBar* pBar = GetControlBar(nID);
	if (pBar != NULL)
	{
		//若控制条可见，则隐藏它；否则，显示它
		ShowControlBar(pBar, (pBar->GetStyle() & WS_VISIBLE) == 0, FALSE,TRUE);
		//处理完毕
		return TRUE;
	}
	//可以让下一个命令目标继续处理
	return FALSE;
}

// 抓拍按钮状态
void CMainFrame::OnUpdateCapPic(CCmdUI* pCmdUI)
{
	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();

	if (pVideo1)
	{
		pCmdUI->Enable(theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_bCapPic);
	}
}

// 显示视频按钮状态
void CMainFrame::OnUpdateStartVideo(CCmdUI* pCmdUI)
{
	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();

	if (pVideo1)
	{
		pCmdUI->Enable(theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_bStartVideo && m_bIsCameraNode);
	}
}

// 关闭视频按钮状态
void CMainFrame::OnUpdateStopVideo(CCmdUI* pCmdUI)
{
	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();

	if (pVideo1)
	{
		pCmdUI->Enable(theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_bStopVideo && m_bIsCameraNode);
	}
}

// 云台控制
void CMainFrame::OnControl()
{
	if (m_bShowContrl)
	{
		m_pControl->ShowWindow(SW_SHOW);
		CWnd *pWnd = NULL;
		pWnd = CWnd::FindWindow(NULL, _T("云台控制"));
		if(pWnd != NULL)pWnd->SetForegroundWindow();
		CRect rc;
		pWnd->GetWindowRect(rc);
		::SetWindowPos(pWnd->m_hWnd, HWND_TOPMOST, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
		m_bShowContrl = FALSE;
	}
	else
	{
		m_pControl->ShowWindow(SW_HIDE);
		m_bShowContrl = TRUE;
	}
}

// 云台控制按钮状态
void CMainFrame::OnUpdateControl(CCmdUI* pCmdUI)
{
	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();
	pCmdUI->Enable(FALSE);//theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_bCapPic);
}

/////////////////////////////////////////////
// 显示/隐藏左侧功能区
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CMainFrame::OnViewLeft() 
{
	ShowControlBar(&m_wndWorkSpace, !m_wndWorkSpace.IsVisible(), FALSE,FALSE);	
}

/////////////////////////////////////////////
// 更新显示/隐藏功能区状态
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CMainFrame::OnUpdateViewLeft(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_wndWorkSpace.IsVisible());
}


/////////////////////////////////////////////
// 启动视频检索与回放
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CMainFrame::OnViewVideoplay() 
{
	TCHAR szPath[MAX_PATH];
	GetModuleFileName(NULL, szPath, MAX_PATH);
	CString sPath;
	sPath.Format(_T("%s"), szPath);
	int nFind = -1;
	nFind = sPath.ReverseFind('\\');
	sPath = sPath.Left(nFind+1);
	sPath += _T("VideoPlayer.exe");
	ShellExecute(NULL, _T("open"), sPath, NULL, NULL, SW_SHOWNORMAL);
}

void CMainFrame::OnUpdateViewVideoplay(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
}

void CMainFrame::InitialTray()
{
	m_notifyIconData.cbSize=sizeof(NOTIFYICONDATA);
	m_notifyIconData.hWnd=this->m_hWnd;
	m_notifyIconData.uFlags=NIF_ICON|NIF_TIP|NIF_MESSAGE;
	m_notifyIconData.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	lstrcpy(m_notifyIconData.szTip,_T("DVSShow"));
	m_notifyIconData.uCallbackMessage=WM_TRAYNOTIFICATION;
}

void CMainFrame::AddTray()
{
	Shell_NotifyIcon(NIM_ADD,&m_notifyIconData);
}
void CMainFrame::DeleteTray()
{
	Shell_NotifyIcon(NIM_DELETE,&m_notifyIconData);
}
void CMainFrame::ModifyTray()
{
	Shell_NotifyIcon(NIM_MODIFY,&m_notifyIconData);
}

LRESULT CMainFrame::OnTrayNotification(WPARAM wId,LPARAM lEvent)
{
	switch(lEvent)
	{
	case WM_RBUTTONUP:
		{
			CPoint point;
			CMenu menu;
			GetCursorPos(&point);
			if(!menu.LoadMenu(IDR_TRAYMENU))
				return 0;

			CMenu *pSubMenu=menu.GetSubMenu(0);
			if(!pSubMenu)
				return 0;		

			SetForegroundWindow();
			//BCG弹出菜单
			CBCGPContextMenuManager* pcm = theApp.GetContextMenuManager();
			pcm->ShowPopupMenu(pSubMenu->GetSafeHmenu(), point.x, point.y, this);

			PostMessage(WM_NULL,0,0);
			break;
		}
	case WM_LBUTTONDBLCLK:
		ShowOrHideWnd();
		break;
	}
	return 0;
}

void CMainFrame::ShowOrHideWnd()
{
	if(IsWindowVisible())
		ShowWindow(SW_HIDE);
	else
		ShowWindow(SW_SHOWMAXIMIZED);
}

void CMainFrame::OnUpdateShowOrHideWnd(CCmdUI *pCmdUI)
{
	if(IsWindowVisible())
	{
		pCmdUI->SetCheck(1);
	}
	else
		pCmdUI->SetCheck(0);
}

void CMainFrame::OnDestroy()
{
	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();

    for (int i = 0; i < MAX_VIDEO_NUM; i++)
    {
        pVideo1->StopRTVideoEx(i);
    }

	DeleteTray();
	HANDLE hd = RemoveProp(GetSafeHwnd(), theApp.m_pszExeName);
	CFrameWnd::OnClose();
}

void CMainFrame::OnClose()
{
	OnDestroy();
//	ShowWindow(SW_HIDE);
//	CBCGPFrameWnd::OnClose();
}

LRESULT CMainFrame::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{

    return CBCGPFrameWnd::DefWindowProc(message, wParam, lParam);
}

void CMainFrame::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	CBCGPFrameWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	CBCGPFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnViewAlarm()
{
// 	::BCGPMessageBox(NULL, "此功能暂未开放！", "提示", MB_OK|MB_ICONINFORMATION);
// 	return;
	
	if (!theApp.m_pDlgAlarm->IsWindowVisible())
	{
		theApp.m_pDlgAlarm->ShowWindow(SW_SHOWNORMAL);
	}
	else
	{
		theApp.m_pDlgAlarm->ShowWindow(SW_HIDE);
	}
	int nWidth = GetSystemMetrics ( SM_CXSCREEN ); 
	int nHeight= GetSystemMetrics ( SM_CYSCREEN );

	CRect rc;
	theApp.m_pDlgAlarm->GetWindowRect(&rc);
	ClientToScreen(&rc);

	theApp.m_pDlgAlarm->MoveWindow(nWidth-rc.Width()-10, nHeight-rc.Height()-50, rc.Width(), rc.Height());
	theApp.m_pDlgAlarm->SetWindowPos(&wndTopMost, nWidth-rc.Width()-10, nHeight-rc.Height()-50, rc.Width(), rc.Height(), SWP_NOSIZE);
}

