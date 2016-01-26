
// TSAlarmServer_WDDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TSAlarmServer_WD.h"
#include "TSAlarmServer_WDDlg.h"

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


// CTSAlarmServer_WDDlg �Ի���




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


// CTSAlarmServer_WDDlg ��Ϣ�������

BOOL CTSAlarmServer_WDDlg::OnInitDialog()
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
	m_TrayIcon.Create(this,WM_ICON_NOTIFY,_T("�澯�������"),m_hIcon,IDR_MENU1); 
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTSAlarmServer_WDDlg::OnPaint()
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