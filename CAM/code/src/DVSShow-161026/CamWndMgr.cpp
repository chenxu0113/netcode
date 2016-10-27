#include "stdafx.h"
#include "resource.h"
#include "DlgOutput.h"
#include "CamWndMgr.h"


CCamWndMgr::CCamWndMgr(void)
{
	InitializeCriticalSection(&m_csWndMap);
	m_nWndCount = 0;
	m_nCameraNo = 0;
}

CCamWndMgr::~CCamWndMgr(void)
{
	DeleteCriticalSection(&m_csWndMap);
}

void CCamWndMgr::AttachWnd(int nWndIndex, CDlgOutput *pDlg)
{
	EnterCriticalSection(&m_csWndMap);
	m_WndMap[nWndIndex] = pDlg;
	m_nWndCount = m_WndMap.size();
	LeaveCriticalSection(&m_csWndMap);
}

void CCamWndMgr::DetachWnd(int nWndIndex)
{
	EnterCriticalSection(&m_csWndMap);
	m_WndMap.erase(nWndIndex);
	m_nWndCount = m_WndMap.size();
	LeaveCriticalSection(&m_csWndMap);
}

CDlgOutput *CCamWndMgr::GetAttachedWnd(int nWndIndex)
{
	CDlgOutput *pDlg = NULL;
	map<int, CDlgOutput *>::iterator it;
	EnterCriticalSection(&m_csWndMap);
	if (nWndIndex == FIRST_WND)
	{
		if (m_WndMap.size() == 0)
		{
			pDlg = NULL;
		}
		else
		{
			it = m_WndMap.begin();
			pDlg = it->second;
		}
	}
	else
	{
		it = m_WndMap.find(nWndIndex);
		if (it != m_WndMap.end())
		{
			pDlg = it->second;
		}
	}	
	LeaveCriticalSection(&m_csWndMap);

	return pDlg;
}

int CCamWndMgr::GetAttachedWndNum()
{
	return m_nWndCount;
}
