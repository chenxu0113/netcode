// DlgVideoSettings.cpp : implementation file
//

#include "stdafx.h"
#include "DVSShow.h"
#include "DlgVideoSettings.h"
#include "MainFrm.h"
#include "DlgVideo1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDVSShowApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CDlgVideoSettings dialog


CDlgVideoSettings::CDlgVideoSettings(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgVideoSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgVideoSettings)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgVideoSettings::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgVideoSettings)
	DDX_Control(pDX, IDC_COMBO_VIDEOWNDNUM, m_wndVideoWndNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgVideoSettings, CBCGPDialog)
	//{{AFX_MSG_MAP(CDlgVideoSettings)
	ON_CBN_SELCHANGE(IDC_COMBO_VIDEOWNDNUM, OnSelchangeComboVideowndnum)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgVideoSettings message handlers

BOOL CDlgVideoSettings::OnInitDialog() 
{
	CBCGPDialog::OnInitDialog();
	CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_LunaBlue);
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
	CBCGPButton::EnableWinXPTheme(TRUE);
	EnableVisualManagerStyle (TRUE, TRUE);
	
	int i = 0;
	CString str;
	for (i=1; i<=MAX_ROWCOL; i++)
	{
		str.Format(_T("%d"), i*i);
		m_wndVideoWndNum.InsertString(i-1, str);
		m_wndVideoWndNum.SetItemData(i-1, i*i);
	}
	m_wndVideoWndNum.SetCurSel(theApp.m_iVideoComboRow);
	//OnSelchangeComboVideowndnum();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////
// 设置当前视频窗口数
// Input: nWndNum-当前视频窗口数
// Ouput: NONE
// Return: NONE
/////////////////////////////////////////////
void CDlgVideoSettings::SetCurSelNum(int nWndNum)
{
	if (m_wndVideoWndNum.GetCount() <= 0)
		return ;
	int i = 0;
	for (i=0; i<MAX_ROWCOL; i++)
	{
		int nTemp = (int)m_wndVideoWndNum.GetItemData(i);
		if (nTemp == nWndNum)
		{
			CMainFrame* pFrame = theApp.GetMainFrame();
			m_wndVideoWndNum.SetCurSel(i);
			pFrame->m_nCurVideoNum = nWndNum;
			break ;
		}
	}
}

/////////////////////////////////////////////
// 当击当前下拉框
// Input: NONE
// Ouput: NONE
// Return: NONE
/////////////////////////////////////////////
void CDlgVideoSettings::OnSelchangeComboVideowndnum() 
{
	int nNum = 0, nCurSel = -1;
	CMainFrame* pFrame = theApp.GetMainFrame();

	nCurSel = m_wndVideoWndNum.GetCurSel();
	nNum = (int)m_wndVideoWndNum.GetItemData(nCurSel);
	pFrame->m_nCurVideoNum = nNum;

	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();
	if (NULL == pVideo1)
		return;
	pVideo1->ArrageOutputWnds(nNum);
	theApp.m_iVideoComboRow = nCurSel;
}
