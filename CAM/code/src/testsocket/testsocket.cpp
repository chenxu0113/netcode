
// testsocket.cpp : ����Ӧ�ó��������Ϊ��
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


// CtestsocketApp ����

CtestsocketApp::CtestsocketApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
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

// Ψһ��һ�� CtestsocketApp ����

CtestsocketApp theApp;


// CtestsocketApp ��ʼ��

BOOL CtestsocketApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
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

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CtestsocketDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}
