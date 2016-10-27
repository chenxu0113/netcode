
// testsocket.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "testsocket.h"
#include "testsocketDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestsocketApp

BEGIN_MESSAGE_MAP(CtestsocketApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CtestsocketApp 构造

CtestsocketApp::CtestsocketApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	m_pSendThread = NULL;
	m_pWdSocket = NULL;
}

CtestsocketApp::~CtestsocketApp()
{
	if(m_pSendThread && m_pSendThread->m_hThread)
	{		
		TerminateThread(m_pSendThread->m_hThread,0);
		delete m_pSendThread;
		m_pSendThread = NULL;
	}
}

// 唯一的一个 CtestsocketApp 对象

CtestsocketApp theApp;


// CtestsocketApp 初始化

BOOL CtestsocketApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		return FALSE;
	}

	if (m_pSendThread == NULL)
	{
		m_pSendThread = new CSendThread;
		if (m_pSendThread == NULL)
			return FALSE;
		if (!m_pSendThread->CreateThread())
		{			
			delete m_pSendThread;
			m_pSendThread = NULL;
			return FALSE;
		}
	}

	BOOL bRet = FALSE;

// 	m_pWdSocket = new CClientSocket;
// 	bRet = m_pWdSocket->Create();
// 	if (!bRet)
// 	{
// 		return TRUE;
// 	}
// 
// 	bRet = m_pWdSocket->Connect("10.0.0.10", 5022);
// 	if (!bRet)
// 	{
// 		return TRUE;
// 
// 	}
	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CtestsocketDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
