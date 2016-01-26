
// TSSampleDataSvrDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TSSampleDataSvr.h"
#include "TSSampleDataSvrDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
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


// CTSSampleDataSvrDlg �Ի���




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


// CTSSampleDataSvrDlg ��Ϣ�������

BOOL CTSSampleDataSvrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_TrayIcon.Create(this,WM_ICON_NOTIFY,_T("�����������������"),m_hIcon,IDR_MENU1); 
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTSSampleDataSvrDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
