// Control.cpp : implementation file
//

#include "stdafx.h"
#include "DVSShow.h"
#include "Control.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControl dialog

extern CDVSShowApp theApp;

CControl::CControl(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CControl::IDD, pParent)
{
	m_niSelSpeed = 0;
}


void CControl::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CControl)
	DDX_Control(pDX, IDC_COMBO_SPEED, m_comboSpeed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CControl, CBCGPDialog)
	//{{AFX_MSG_MAP(CControl)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_COMBO_SPEED, OnSelchangeComboSpeed)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SETSPEED, &CControl::OnBnClickedButtonSetspeed)
	ON_BN_CLICKED(IDC_BTN_PTZ_UP, &CControl::OnBnClickedBtnPtzUp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControl message handlers

BOOL CControl::OnInitDialog() 
{
	CBCGPDialog::OnInitDialog();
	CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_LunaBlue);
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
	CBCGPButton::EnableWinXPTheme(TRUE);

	int i = 0;
	CString str;
	m_comboSpeed.InsertString(0, _T("д╛хо"));
	m_comboSpeed.SetItemData(0, 3);
	for (i=0; i<7; i++)
	{
		str.Format(_T("%d"), i+1);
		m_comboSpeed.InsertString(i+1, str);
		m_comboSpeed.SetItemData(i+1, i+1);
	}
	m_comboSpeed.SetCurSel(0);
	m_niSelSpeed = 3;

	BOOL nret =	m_btnPtzUp.SubclassDlgItem(IDC_BTN_PTZ_UP, this);

	m_btnPtzDown.SubclassDlgItem(IDC_BTN_PTZ_DOWN, this);
	m_btnPtzLeft.SubclassDlgItem(IDC_BTN_PTZ_LEFT, this);
	m_btnPtzRight.SubclassDlgItem(IDC_BTN_PTZ_RIGHT, this);	
	m_btnZoomOut.SubclassDlgItem(IDC_BTN_ZOOM_OUT, this);
	m_btnZoomIn.SubclassDlgItem(IDC_BTN_ZOOM_IN, this);
	m_btnFocusNear.SubclassDlgItem(IDC_BTN_FOCUS_NEAR, this);
	m_btnFocusFar.SubclassDlgItem(IDC_BTN_FOCUS_FAR, this);
	m_btnIrisOpen.SubclassDlgItem(IDC_BTN_IRIS_OPEN, this);
	m_btnIrisClose.SubclassDlgItem(IDC_BTN_IRIS_CLOSE, this);
	EnableVisualManagerStyle (TRUE, TRUE);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CControl::OnClose() 
{
	CMainFrame* pFrame = theApp.GetMainFrame();
	pFrame->m_bShowContrl = TRUE;
	CBCGPDialog::OnClose();
}

void CControl::OnSelchangeComboSpeed() 
{
	int nCurSel = -1;
	nCurSel = m_comboSpeed.GetCurSel();
	m_niSelSpeed = (int)m_comboSpeed.GetItemData(nCurSel);

	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();
	theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].nSpeed = m_niSelSpeed;
}

void CControl::OnBnClickedButtonSetspeed()
{
	int nCurSel = -1;
	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();

	nCurSel = m_comboSpeed.GetCurSel();
	m_niSelSpeed = (int)m_comboSpeed.GetItemData(nCurSel);
	theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].nSpeed = m_niSelSpeed;
}

void CControl::OnBnClickedBtnPtzUp()
{
}
