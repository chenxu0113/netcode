
// TSSampleDataSvr.cpp : 定义应用程序的类行为。
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


// CTSSampleDataSvrApp 构造

CTSSampleDataSvrApp::CTSSampleDataSvrApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
	bExitFlag = FALSE;
}


// 唯一的一个 CTSSampleDataSvrApp 对象

CTSSampleDataSvrApp theApp;

//Gongc ADD 20151021
extern CLog *g_log ;
long __stdcall exception_cb(_EXCEPTION_POINTERS* excp)   
{	
	//AfxMessageBox(_T("欢迎来到天溯异常捕获中心.."));
// 	CTSSampleDataSvrApp* pApp = (CTSSampleDataSvrApp*)AfxGetApp();
	if(g_log==NULL)
	{
		g_log = new CLog(_T("\\TSSampleDataSvr"));
	}
	ReguTrace(ERRO,"程序异常！进入异常处理函数。");
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
	EXCEPTION_EXECUTE_HANDLER equ 1 表示我已经处理了异常,可以优雅地结束了 
	EXCEPTION_CONTINUE_SEARCH equ 0 表示我不处理,其他人来吧,于是windows调用默认的处理程序显示一个错误框,并结束 
	EXCEPTION_CONTINUE_EXECUTION equ -1 表示错误已经被修复,请从异常发生处继续执行  
	*/
	ReguTrace(ERRO,"异常处理函数结束");
	return	 EXCEPTION_EXECUTE_HANDLER;   
}
// CTSSampleDataSvrApp 初始化

BOOL CTSSampleDataSvrApp::InitInstance()
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

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("TSSampleDataSvr.exe"));
	SetErrorMode(1|2);
	SetUnhandledExceptionFilter(exception_cb);   //Gongc ADD 20151021
	CTSSampleDataSvrDlg dlg;
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
