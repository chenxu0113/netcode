
// TSSampleDataSvr.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "TSSampleDataSvr.h"
#include "TSSampleDataSvrDlg.h"
#include <dbghelp.h>

#pragma comment( lib, "dbghelp.lib" )
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTSSampleDataSvrApp

BEGIN_MESSAGE_MAP(CTSSampleDataSvrApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTSSampleDataSvrApp ����

CTSSampleDataSvrApp::CTSSampleDataSvrApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	bExitFlag = FALSE;
}


// Ψһ��һ�� CTSSampleDataSvrApp ����

CTSSampleDataSvrApp theApp;

//Gongc ADD 20151021
extern CLog *g_log ;
long __stdcall exception_cb(_EXCEPTION_POINTERS* excp)   
{	
	//AfxMessageBox(_T("��ӭ���������쳣��������.."));
// 	CTSSampleDataSvrApp* pApp = (CTSSampleDataSvrApp*)AfxGetApp();
	if(g_log==NULL)
	{
		g_log = new CLog(_T("\\TSSampleDataSvr"));
	}
	ReguTrace(ERRO,"�����쳣�������쳣��������");
	TCHAR szDirectory[MAX_PATH];
	GetModuleFileName(GetModuleHandle(NULL),szDirectory,MAX_PATH);
	CString strPath = szDirectory;	
	CString strPathName = L"", strModuleName = L"";
	int nIndex = strPath.ReverseFind('\\');
	if (nIndex != 0)
	{
		strPathName = strPath.Left(nIndex);
		strPath.Delete(0,nIndex);
		strModuleName = strPath;
	}
	else
	{
		exit(0);
	}
	CTime ct;
	ct = CTime::GetCurrentTime();  
	CString cname;
	cname.Format(L"%s_%s.dmp",strModuleName,ct.Format(_T("%Y%m%d%H%M%S")));
	CString cpath = strPathName + cname;
	cpath.ReleaseBuffer();
	HANDLE hFile = ::CreateFile(cpath.GetBuffer(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	cpath.ReleaseBuffer();
	if( hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION einfo;
		einfo.ThreadId = ::GetCurrentThreadId();
		einfo.ExceptionPointers = excp;
		einfo.ClientPointers = FALSE;

		::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, MiniDumpNormal, &einfo, NULL, NULL);
		::CloseHandle(hFile);
	}
	/*
	EXCEPTION_EXECUTE_HANDLER equ 1 ��ʾ���Ѿ��������쳣,�������ŵؽ����� 
	EXCEPTION_CONTINUE_SEARCH equ 0 ��ʾ�Ҳ�����,����������,����windows����Ĭ�ϵĴ��������ʾһ�������,������ 
	EXCEPTION_CONTINUE_EXECUTION equ -1 ��ʾ�����Ѿ����޸�,����쳣����������ִ��  
	*/
	ReguTrace(ERRO,"�쳣����������");
	return	 EXCEPTION_EXECUTE_HANDLER;   
}
// CTSSampleDataSvrApp ��ʼ��

BOOL CTSSampleDataSvrApp::InitInstance()
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

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("TSSampleDataSvr.exe"));
	SetErrorMode(1|2);
	SetUnhandledExceptionFilter(exception_cb);   //Gongc ADD 20151021
	CTSSampleDataSvrDlg dlg;
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
