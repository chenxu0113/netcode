// MyWinApp.cpp : 实现文件
//

#include "stdafx.h"
#include "MyWinApp.h"


// CMyWinApp

IMPLEMENT_DYNCREATE(CMyWinApp, CWinApp)

CMyWinApp::CMyWinApp()
: m_sBaseVer(_T(""))
, m_sModule(_T(""))
, m_sModuleDesc(_T(""))
, m_sModuleVer(_T(""))
{
}

CMyWinApp::~CMyWinApp()
{
}

BOOL CMyWinApp::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CMyWinApp::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinApp::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMyWinApp, CWinApp)
END_MESSAGE_MAP()


// CMyWinApp 消息处理程序


void CMyWinApp::SetVersion(const CString sVer)
{
	m_sModuleVer = sVer;
}

void CMyWinApp::GetVersion(CString &sVer)
{
	sVer = m_sModuleVer;
}

void CMyWinApp::SetModuleDesc(const CString sDesc)
{
	m_sModuleDesc = sDesc;
}

void CMyWinApp::GetModuleDesc(CString &sDesc)
{
	sDesc = m_sModuleDesc;
}