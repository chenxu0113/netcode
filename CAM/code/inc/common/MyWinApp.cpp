// MyWinApp.cpp : ʵ���ļ�
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
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	return TRUE;
}

int CMyWinApp::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinApp::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMyWinApp, CWinApp)
END_MESSAGE_MAP()


// CMyWinApp ��Ϣ�������


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