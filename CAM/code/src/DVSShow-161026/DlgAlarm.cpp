// DlgAlarm.cpp : 实现文件
//

#include "stdafx.h"
#include "DVSShow.h"
#include "DlgAlarm.h"


// CDlgAlarm 对话框

IMPLEMENT_DYNAMIC(CDlgAlarm, CDialog)

CDlgAlarm::CDlgAlarm(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAlarm::IDD, pParent)
{
	m_nTimer = -1;
	m_listRtAlarm.RemoveAll();
	InitializeCriticalSection(&m_csRtAlarm);
}

CDlgAlarm::~CDlgAlarm()
{
	m_listRtAlarm.RemoveAll();
	DeleteCriticalSection(&m_csRtAlarm);
}

void CDlgAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RTALARM, m_ListboxRtAlarm);
}


BEGIN_MESSAGE_MAP(CDlgAlarm, CDialog)
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDlgAlarm 消息处理程序

BOOL CDlgAlarm::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_LunaBlue);
 	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
 	CBCGPButton::EnableWinXPTheme(TRUE);
 	EnableVisualManagerStyle (TRUE, TRUE);

	m_nTimer = SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgAlarm::OnClose()
{
	ShowWindow(SW_HIDE);

//	CBCGPDialog::OnClose();
}


void CDlgAlarm::AddToRtAlarm(NetworkDIDOSocket* ndd)
{
	EnterCriticalSection(&m_csRtAlarm);
	m_listRtAlarm.AddTail(ndd);
	LeaveCriticalSection(&m_csRtAlarm);
}

void CDlgAlarm::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_nTimer)
	{
		EnterCriticalSection(&m_csRtAlarm);
		if (m_listRtAlarm.GetCount() <= 0)
		{
			LeaveCriticalSection(&m_csRtAlarm);
			return;
		}
		NetworkDIDOSocket* nds = NULL;
		nds = m_listRtAlarm.GetHead();

		CString sContent = "";
		if (nds->ndc.cNum[nds->cAlarmNum] == nds->ndc.cAlarmValue)	//告警发生
		{
			sContent.Format("[%04d-%02d-%02d %02d:%02d:%02d_%03d] [%s].[%s].[点号=%02d].[值=%d]  发生告警", 
							nds->st.wYear, nds->st.wMonth, nds->st.wDay,
							nds->st.wHour, nds->st.wMinute, nds->st.wSecond, nds->st.wMilliseconds,
							nds->ndc.szIP, nds->ndc.szName, nds->cAlarmNum, nds->ndc.cNum[nds->cAlarmNum]);
		}
		else
		{
			sContent.Format("[%04d-%02d-%02d %02d:%02d:%02d_%03d] [%s].[%s].[点号=%02d].[值=%d]  告警已恢复", 
							nds->st.wYear, nds->st.wMonth, nds->st.wDay,
							nds->st.wHour, nds->st.wMinute, nds->st.wSecond, nds->st.wMilliseconds,
							nds->ndc.szIP, nds->ndc.szName, nds->cAlarmNum, nds->ndc.cNum[nds->cAlarmNum]);
		}
		m_ListboxRtAlarm.InsertString(0, sContent);
	//	m_ListboxRtAlarm.SendMessage(LB_SETHORIZONTALEXTENT, 700, 0);//此行用来设置水平滚动条

		if (nds)
		{
			delete nds;
			nds = NULL;
		}
		m_listRtAlarm.RemoveHead();
		LeaveCriticalSection(&m_csRtAlarm);
	}

	CBCGPDialog::OnTimer(nIDEvent);
}
