
// TSSampleDataSvrDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TSSampleDataSvr.h"
#include "TSSampleDataSvrDlg.h"

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


// CTSSampleDataSvrDlg 对话框




CTSSampleDataSvrDlg::CTSSampleDataSvrDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTSSampleDataSvrDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTSSampleDataSvrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTSSampleDataSvrDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_ABOUT_1, &CTSSampleDataSvrDlg::OnAbout1)
	ON_COMMAND(ID_EXIT, &CTSSampleDataSvrDlg::OnExit)
	ON_MESSAGE(WM_ICON_NOTIFY, OnTrayNotification)
	ON_COMMAND(ID_SHOW, &CTSSampleDataSvrDlg::OnShow)
	ON_WM_CLOSE(WM_CLOSE,OnClose)
END_MESSAGE_MAP()


// CTSSampleDataSvrDlg 消息处理程序

BOOL CTSSampleDataSvrDlg::OnInitDialog()
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
	m_TrayIcon.Create(this,WM_ICON_NOTIFY,_T("冻结数据入库服务程序"),m_hIcon,IDR_MENU1); 
	PostMessage(WM_SYSCOMMAND,SC_MINIMIZE,0);   
	ShowWindow(SW_HIDE);

	m_pCCSendP2pTask = NULL;
	m_pCCSendP2pTask = new CCSendP2pTask;
	m_pCRedisRecvSample = NULL;
	m_pCRedisRecvSample = new CRedisRecvSample;
	m_pCSaveYmDataThread = NULL;
	m_pCSaveYmDataThread = new CSaveYmDataThread;
	m_pCUpdateRTDBThread = NULL;
	m_pCUpdateRTDBThread = new CUpDateRTDB;
	if (m_pCCSendP2pTask)
	{
		m_pCCSendP2pTask->CreateThread();
	}

	if (m_pCRedisRecvSample)
	{
		m_pCRedisRecvSample->CreateThread();
	}

	if (m_pCUpdateRTDBThread)
	{
		m_pCUpdateRTDBThread->CreateThread();
	}

	for (int i=0;i<THREAD_NUM;i++)
	{
		m_pCProcessThread[i] = NULL;
		m_pCProcessThread[i] = new CProcessThread();
		m_pCProcessThread[i]->m_Index = i;
		if (m_pCProcessThread[i]!=NULL)
		{
			m_pCProcessThread[i]->CreateThread();
		}
	}

	if (m_pCSaveYmDataThread)
	{
		m_pCSaveYmDataThread->CreateThread();
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTSSampleDataSvrDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
	/*else if ((nID & 0xFFF0)==SC_CLOSE)
	{

		ShowWindow(SW_HIDE);
	}*/
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

void CTSSampleDataSvrDlg::OnPaint()
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
HCURSOR CTSSampleDataSvrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTSSampleDataSvrDlg::OnAbout1()
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CTSSampleDataSvrDlg::OnExit()
{
	theApp.bExitFlag = TRUE;
	while(1)
	{
		int Index = 0;
		for (int i=0;i<THREAD_NUM;i++)
		{
			if (m_pCProcessThread[i]->m_RedisDataList.GetSize()<=0)
			{
				Index++;
			}
		}
		if (Index==THREAD_NUM)
		{
			break;
		}
		Sleep(100);
	}
	Sleep(3000);
	m_TrayIcon.RemoveIcon();
	CDialog::OnDestroy();
	exit(0);
}

LRESULT CTSSampleDataSvrDlg::OnTrayNotification(WPARAM wParam,LPARAM lParam)
{
	return m_TrayIcon.OnTrayNotification(wParam,lParam);
}

/*
void CTSSampleDataSvrDlg::OnClose()
{
}
*/
void CTSSampleDataSvrDlg::OnShow()
{
	ShowWindow(SW_SHOWNORMAL);
}

void CTSSampleDataSvrDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//ShowWindow(SW_MINIMIZE);
	theApp.bExitFlag = TRUE;
	while(1)
	{
		int Index = 0;
		for (int i=0;i<THREAD_NUM;i++)
		{
			if (m_pCProcessThread[i]->m_RedisDataList.GetSize()<=0)
			{
				Index++;
			}
		}
		if (Index==THREAD_NUM)
		{
			break;
		}
		Sleep(100);
	}
	Sleep(3000);
 	CDialog::OnClose();
}
