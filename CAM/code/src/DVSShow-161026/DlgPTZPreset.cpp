// DlgPTZPreset.cpp : implementation file
//

#include "stdafx.h"
#include "DVSShow.h"
#include "DlgPTZPreset.h"
#include "DlgVideo1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPTZPreset dialog
extern CDVSShowApp theApp;


CDlgPTZPreset::CDlgPTZPreset(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CDlgPTZPreset::IDD, pParent)
{
	m_nCurSel = -1;
}


void CDlgPTZPreset::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPTZPreset)
	DDX_Control(pDX, IDC_COMBO_PTZPRESET, m_comboPtzPreset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPTZPreset, CBCGPDialog)
	//{{AFX_MSG_MAP(CDlgPTZPreset)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_BN_CLICKED(IDC_BTN_POINT, OnBtnPoint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPTZPreset message handlers

BOOL CDlgPTZPreset::OnInitDialog() 
{
	CBCGPDialog::OnInitDialog();

	CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_LunaBlue);
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
	CBCGPButton::EnableWinXPTheme(TRUE);
	EnableVisualManagerStyle (TRUE, TRUE);	
	
	int i = 0;
	CString str;
	for (i=0; i<255; i++)
	{
		str.Format(_T("%d"), i+1);
		m_comboPtzPreset.InsertString(i, str);
	}
	m_nCurSel = 0;
	m_comboPtzPreset.SetCurSel(m_nCurSel);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 关闭对话框
void CDlgPTZPreset::OnClose() 
{	
	CBCGPDialog::OnClose();
}

// 添加预置点
void CDlgPTZPreset::OnBtnAdd() 
{
	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();
	if (NULL == pVideo1)
		return;

	int nWndIndex = pVideo1->m_nSubWndIndex;
	m_nCurSel = m_comboPtzPreset.GetCurSel();
	int nDevIndex = pVideo1->m_nCurChanNum;
	int nChanIndex = pVideo1->m_nCurChanNum;

	 CString cstrTemp = _T("");
	 m_comboPtzPreset.GetLBText(m_comboPtzPreset.GetCurSel(), cstrTemp);
	 BOOL bRes = DLL_DVR_Preset(&theApp.m_dlgOutput[nWndIndex].m_TSCamDef, PTZ_PRESET_SET,  (BYTE)_ttoi(cstrTemp));	
}

// 删除预置点
void CDlgPTZPreset::OnBtnDelete() 
{
	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();
	if (NULL == pVideo1)
		return;
	int nWndIndex = pVideo1->m_nSubWndIndex;
	m_nCurSel = m_comboPtzPreset.GetCurSel();
	int nDevIndex = pVideo1->m_nCurChanNum;
	int nChanIndex = pVideo1->m_nCurChanNum;

	CString cstrTemp = _T("");
	m_comboPtzPreset.GetLBText(m_comboPtzPreset.GetCurSel(), cstrTemp);
	DLL_DVR_Preset(&theApp.m_dlgOutput[nWndIndex].m_TSCamDef, PTZ_PRESET_DEL,  (BYTE)_ttoi(cstrTemp));		
}

// 写入配置文件
BOOL CDlgPTZPreset::WriteToINI(int nWndIndex, int nSelected)
{
	CString str, str2;
	str.Format(_T("%d"), nWndIndex);
	str2.Format(_T("%d"), nSelected);
	BOOL bRet = FALSE;
	bRet = WritePrivateProfileString(_T("PTZPRESET"), str, str2, theApp.m_sIniPath);

	return bRet;
}

// 转至预置点
void CDlgPTZPreset::OnBtnPoint() 
{
	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();
	if (NULL == pVideo1)
		return;
	MoveToPtzPreSet(m_comboPtzPreset.GetCurSel(), pVideo1->m_nSubWndIndex);
}

void CDlgPTZPreset::MoveToPtzPreSet(int nCurSel, int nWndIndex)
{
	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();
	if(NULL == pVideo1)
		return;

	int iDevIndex = pVideo1->m_nCurChanNum;
	int iChanIndex = pVideo1->m_nCurChanNum;
	int iWndIndex = pVideo1->m_nSubWndIndex;

	m_comboPtzPreset.SetCurSel(nCurSel);
	CString cstrTemp = _T("");
	m_comboPtzPreset.GetLBText(m_comboPtzPreset.GetCurSel(), cstrTemp);
	DLL_DVR_Preset(&theApp.m_dlgOutput[nWndIndex].m_TSCamDef, PTZ_PRESET_MOVE,  (BYTE)_ttoi(cstrTemp));
}

void CDlgPTZPreset::RedrawComBox(BYTE nCamType)
{
	m_comboPtzPreset.ResetContent();

	int nPresetCount = 0;
	int i = 0;
	CString str;

	nPresetCount = DLL_DVR_GetPresetCount(nCamType);

	if (nPresetCount <= 0)
	{
		return;
	}

	for (i=0; i<nPresetCount; i++)
	{
		str.Format(_T("%d"), i+1);		
		m_comboPtzPreset.InsertString(i, str);
	}
	m_nCurSel = 0;		
	m_comboPtzPreset.SetCurSel(m_nCurSel);
}

