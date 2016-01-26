
// TSAlarmServer_WDDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TSAlarmServer_WD.h"
#include "TSAlarmServer_WDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTSAlarmServer_WDDlg 对话框




CTSAlarmServer_WDDlg::CTSAlarmServer_WDDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTSAlarmServer_WDDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTSAlarmServer_WDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTSAlarmServer_WDDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_ABOUT_1, &CTSAlarmServer_WDDlg::OnAbout1)
	ON_COMMAND(ID_EXIT, &CTSAlarmServer_WDDlg::OnExit)
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
	ON_COMMAND(ID_SHOW, &CTSAlarmServer_WDDlg::OnShow)
END_MESSAGE_MAP()


// CTSAlarmServer_WDDlg 消息处理程序

BOOL CTSAlarmServer_WDDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_TrayIcon.Create(this,WM_ICON_NOTIFY,_T("告警服务程序"),m_hIcon,IDR_MENU1); 
	PostMessage(WM_SYSCOMMAND,SC_MINIMIZE,0);   
	ShowWindow(SW_HIDE);

	m_pAlarmTask = NULL;
	m_pReceiveAlarmData = NULL;
	m_pMessageTask = NULL;
	//m_pMessageTask = new CMessageTask;
	m_pReceiveAlarmData = new CReceiveAlarmData;
	m_pAlarmTask = new CAlarmTask;
	

	if (m_pAlarmTask)
	{
		m_pAlarmTask->CreateThread();
	}
	if (m_pReceiveAlarmData)
	{
		m_pReceiveAlarmData->CreateThread();
	}

	if (m_pMessageTask)
	{
		m_pMessageTask->CreateThread();
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTSAlarmServer_WDDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if ((nID == SC_MAXIMIZE))
	{
		ShowWindow(SW_HIDE);
	}
	else if ((nID & 0xFFF0)==SC_CLOSE)
	{
		ShowWindow(SW_HIDE);
	}
	else if(nID == SC_MINIMIZE )
	{
		ShowWindow(SW_HIDE);
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTSAlarmServer_WDDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTSAlarmServer_WDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTSAlarmServer_WDDlg::OnAbout1()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CTSAlarmServer_WDDlg::OnExit()
{
	m_TrayIcon.RemoveIcon();
	CDialog::OnDestroy();
}

LRESULT CTSAlarmServer_WDDlg::OnTrayNotification(WPARAM wParam,LPARAM lParam)
{
	return m_TrayIcon.OnTrayNotification(wParam,lParam);
}

void CTSAlarmServer_WDDlg::OnClose()
{
	ShowWindow(SW_MINIMIZE);
}
void CTSAlarmServer_WDDlg::OnShow()
{
	ShowWindow(SW_SHOWNORMAL);
}