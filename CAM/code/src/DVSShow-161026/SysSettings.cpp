// SysSettings.cpp : implementation file
//

#include "stdafx.h"
#include "DVSShow.h"
#include "SysSettings.h"
#include "DlgVideo1.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDVSShowApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSysSettings dialog


CSysSettings::CSysSettings(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CSysSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSysSettings)
	m_sPicPath = _T("");
	m_sRecordPath = _T("");
	m_nTimeStopRec = 10;
	m_nTimeStopVideo = 999999999;
	m_bStartTempRec = FALSE;
	//}}AFX_DATA_INIT
}


void CSysSettings::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysSettings)
	DDX_Control(pDX, IDC_SLIDER_SATURATION, m_slSaturation);
	DDX_Control(pDX, IDC_SLIDER_HUE, m_slHue);
	DDX_Control(pDX, IDC_SLIDER_CONTRAST, m_slContrast);
	DDX_Control(pDX, IDC_SLIDER_BRIGHTNESS, m_slBrightness);
	DDX_Text(pDX, IDC_EDIT_PICPATH, m_sPicPath);
	DDV_MaxChars(pDX, m_sPicPath, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT_RECORDPATH, m_sRecordPath);
	DDV_MaxChars(pDX, m_sRecordPath, MAX_PATH);
	DDX_Text(pDX, IDC_EDIT_TIME_STOPREC, m_nTimeStopRec);
	DDV_MinMaxInt(pDX, m_nTimeStopRec, 10, 999999999);
	DDX_Text(pDX, IDC_EDIT_TIME_STOPVIDEO, m_nTimeStopVideo);
	DDV_MinMaxInt(pDX, m_nTimeStopVideo, 10, 999999999);
	DDX_Check(pDX, IDC_CHECK_STARTRECORD, m_bStartTempRec);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysSettings, CBCGPDialog)
	//{{AFX_MSG_MAP(CSysSettings)
	ON_BN_CLICKED(IDC_BTN_DEFAULT, OnBtnDefault)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BTN_PICPATHSEL, OnBtnPicpathsel)
	ON_BN_CLICKED(IDC_BTN_RECORDSELPATH, OnBtnRecordselpath)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_SGVIDEO, OnCheckSgvideo)
	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDC_BTN_APP_SGVIDEO, OnBtnAppSgvideo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysSettings message handlers

BOOL CSysSettings::OnInitDialog() 
{
	CBCGPDialog::OnInitDialog();
	

	CBCGPVisualManager2007::SetStyle (CBCGPVisualManager2007::VS2007_LunaBlue);
	CBCGPVisualManager::SetDefaultManager (RUNTIME_CLASS (CBCGPVisualManager2007));
	CBCGPButton::EnableWinXPTheme(TRUE);
	EnableVisualManagerStyle (TRUE, TRUE);	


	m_slBrightness.SetRange(1, 10, TRUE);
	m_slBrightness.SetPos(6);

	m_slHue.SetRange(1, 10, TRUE);
	m_slHue.SetPos(6);

	m_slSaturation.SetRange(1, 10, TRUE);
	m_slSaturation.SetPos(6);

	m_slContrast.SetRange(1, 10, TRUE);
	m_slContrast.SetPos(6);

	//theApp.InitIniConfig();
	m_sPicPath = theApp.m_sPicPath;
	m_sRecordPath = theApp.m_sRecordPath;
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_CAPPIC);
	pBtn->SetCheck(theApp.m_bCapPic);
	pBtn = (CButton*)GetDlgItem(IDC_CHECK_SGVIDEO);
	pBtn->SetCheck(theApp.m_bAlarmVideo);

	m_nTimeStopRec = theApp.m_nStopRec;
	m_nTimeStopVideo = theApp.m_nStopVideo;
	m_bStartTempRec = theApp.m_bnStartTempRec;

	UpdateData(FALSE);
	OnCheckSgvideo();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 默认值
void CSysSettings::OnBtnDefault() 
{
	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();
	if (NULL == pVideo1)
		return;

	int nIndex = pVideo1->m_nSubWndIndex;

	theApp.m_dlgOutput[nIndex].m_dwBrightValue = 6;
	theApp.m_dlgOutput[nIndex].m_dwContrastValue = 6;
	theApp.m_dlgOutput[nIndex].m_dwSaturationValue = 6;
	theApp.m_dlgOutput[nIndex].m_dwHueValue = 6;

	DLL_DVR_ClientSetVideoEffect(&theApp.m_dlgOutput[nIndex].m_TSCamDef, (BYTE)theApp.m_dlgOutput[nIndex].m_dwBrightValue, 
								(BYTE)theApp.m_dlgOutput[nIndex].m_dwContrastValue, (BYTE)theApp.m_dlgOutput[nIndex].m_dwHueValue, 
								(BYTE)theApp.m_dlgOutput[nIndex].m_dwSaturationValue);

	m_slBrightness.SetRange(1, 10, TRUE);
	m_slBrightness.SetPos(6);

	m_slHue.SetRange(1, 10, TRUE);
	m_slHue.SetPos(6);

	m_slSaturation.SetRange(1, 10, TRUE);
	m_slSaturation.SetPos(6);

	m_slContrast.SetRange(1, 10, TRUE);
	m_slContrast.SetPos(6);	
}

// 水平滚动
void CSysSettings::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	BOOL bRet = FALSE;
	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();
	if (NULL == pVideo1)
		return;

	int nIndex = pVideo1->m_nSubWndIndex;
	if (!theApp.m_dlgOutput[nIndex].m_bStartVideo)
	{
		if (GetDlgItem(IDC_SLIDER_BRIGHTNESS) == pScrollBar)
		{
			theApp.m_dlgOutput[nIndex].m_dwBrightValue = m_slBrightness.GetPos();

			bRet = TRUE;
		}
		else if (GetDlgItem(IDC_SLIDER_HUE) == pScrollBar)
		{
			theApp.m_dlgOutput[nIndex].m_dwHueValue = m_slHue.GetPos();

			bRet = TRUE;
		}
		else if (GetDlgItem(IDC_SLIDER_CONTRAST) == pScrollBar)
		{
			theApp.m_dlgOutput[nIndex].m_dwContrastValue = m_slContrast.GetPos();

			bRet = TRUE;
		}
		else if (GetDlgItem(IDC_SLIDER_SATURATION) == pScrollBar)
		{
			theApp.m_dlgOutput[nIndex].m_dwSaturationValue = m_slSaturation.GetPos();

			bRet = TRUE;
		}
		if (bRet)
		{
			BOOL bRes = FALSE;
			bRes = DLL_DVR_ClientSetVideoEffect(&theApp.m_dlgOutput[nIndex].m_TSCamDef, (BYTE)theApp.m_dlgOutput[nIndex].m_dwBrightValue, 
				(BYTE)theApp.m_dlgOutput[nIndex].m_dwContrastValue, (BYTE)theApp.m_dlgOutput[nIndex].m_dwHueValue, 
				(BYTE)theApp.m_dlgOutput[nIndex].m_dwSaturationValue);


			if (!bRes)
			{
				::BCGPMessageBox(NULL, _T("视频参数设置错误！"), "提示", MB_OK|MB_ICONWARNING);
				return ;
			}
		}
	}
	CBCGPDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

// 文件夹路径选择
void CSysSettings::OnBtnPicpathsel() 
{
	TCHAR szDir[MAX_PATH];
	BROWSEINFO bi;
	ITEMIDLIST *pidl = NULL;
	
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDir;
	bi.lpszTitle = _T("请选择目录");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	pidl = SHBrowseForFolder(&bi);
	if(pidl == NULL)
		return;
	if (!SHGetPathFromIDList(pidl, szDir))
		return;

	m_sPicPath.Format(_T("%s"), szDir);
	UpdateData(FALSE);
	
}

// 确定，且关闭对话框
void CSysSettings::OnBtnOk() 
{
	UpdateData(TRUE);
	OnBtnAppSgvideo();
	BOOL bPicPath = FALSE, bRecPath = FALSE, bCapPic = FALSE, bSGVideo = FALSE;
	BOOL bStopRec = FALSE, bStopVideo = FALSE, bStartTempRec = FALSE;
	theApp.m_sPicPath = m_sPicPath;
	bPicPath = WritePrivateProfileString(SET_PATH, SET_PICPATH, m_sPicPath, theApp.m_sIniPath);
	theApp.m_sRecordPath = m_sRecordPath;
	bRecPath = WritePrivateProfileString(SET_PATH, SET_RECORDPATH, m_sRecordPath, theApp.m_sIniPath);
	
	CString str;
	str.Format("%d", m_bStartTempRec);
	theApp.m_bnStartTempRec = m_bStartTempRec;
	WritePrivateProfileString(SET_STARTTEMPREC, SET_STARTTEMPREC, str, theApp.m_sIniPath);

	// 抓拍图片是否给出提示
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_CAPPIC);
	theApp.m_bCapPic = pBtn->GetCheck(); 
	str.Format(_T("%d"), theApp.m_bCapPic);
	bCapPic = WritePrivateProfileString(SET_CAPPIC, SET_CAPPIC, str, theApp.m_sIniPath);
	// 是否告警推视频
	pBtn = (CButton*)GetDlgItem(IDC_CHECK_SGVIDEO);
	theApp.m_bAlarmVideo = pBtn->GetCheck(); 
	str.Format(_T("%d"), theApp.m_bAlarmVideo);
	bSGVideo = WritePrivateProfileString(SET_SGVIDEO, SET_ISSGVIDEO, str, theApp.m_sIniPath);
	// 普通异常视频
	str.Format(_T("%d"), theApp.m_nStopRec);
	bStopRec = WritePrivateProfileString(SET_NORMALVIDEO, SET_TIME_STOPREC, str, theApp.m_sIniPath);
	str.Format(_T("%d"), theApp.m_nStopVideo);
	bStopVideo = WritePrivateProfileString(SET_NORMALVIDEO, SET_TIME_STOPVIDEO, str, theApp.m_sIniPath);
	str.Format(_T("%d"), theApp.m_bnStartTempRec);
	bStartTempRec = WritePrivateProfileString(SET_NORMALVIDEO, SET_STARTTEMPREC, str, theApp.m_sIniPath);

	theApp.m_nStopRec = m_nTimeStopRec;
	theApp.m_nStopVideo = m_nTimeStopVideo;
	theApp.m_bnStartTempRec = m_bStartTempRec;

	if (!bPicPath || !bRecPath || !bCapPic || !bSGVideo || !bStopRec || !bStopVideo || !bStartTempRec)
	{
		::BCGPMessageBox(NULL, "保存失败！", "提示", MB_OK|MB_ICONWARNING);
		return ;
	}
	else
	{
		::BCGPMessageBox(NULL, "保存成功！", "提示", MB_OK|MB_ICONINFORMATION);	
	}
	
	CBCGPDialog::OnOK();
}

// 设置控件状态
void CSysSettings::SetCtrlStatus()
{
	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();
	if (NULL == pVideo1)
		return;

	if (!theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_bStartVideo)
	{
		m_slBrightness.EnableWindow(TRUE);
		m_slContrast.EnableWindow(TRUE);
		m_slHue.EnableWindow(TRUE);
		m_slSaturation.EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DEFAULT)->EnableWindow(TRUE);
	}
	else
	{
		m_slBrightness.EnableWindow(FALSE);
		m_slContrast.EnableWindow(FALSE);
		m_slHue.EnableWindow(FALSE);
		m_slSaturation.EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DEFAULT)->EnableWindow(FALSE);
	}
}

// 录像路径选择按钮
void CSysSettings::OnBtnRecordselpath() 
{
	TCHAR szDir[MAX_PATH];
	BROWSEINFO bi;
	ITEMIDLIST *pidl = NULL;
	
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDir;
	bi.lpszTitle = _T("请选择目录");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	pidl = SHBrowseForFolder(&bi);
	if(pidl == NULL)
		return;
	if (!SHGetPathFromIDList(pidl, szDir))
		return;

	m_sRecordPath.Format(_T("%s"), szDir);
	UpdateData(FALSE);
}

// 关闭对话框
void CSysSettings::OnClose() 
{
	CBCGPDialog::OnClose();
}

/////////////////////////////////////////////
// 是否告警推视频
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CSysSettings::OnCheckSgvideo() 
{
	CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK_SGVIDEO);
	BOOL bCheck = 0;
	bCheck = pBtn->GetCheck();
	if (bCheck)
	{
		GetDlgItem(IDC_STATIC_TIME_STOPREC)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_TIME_STOPVIDEO)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TIME_STOPREC)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TIME_STOPVIDEO)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_APP_SGVIDEO)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_INSTRUCTION)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_STARTRECORD)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_TIME_STOPREC)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_TIME_STOPVIDEO)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TIME_STOPREC)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TIME_STOPVIDEO)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_APP_SGVIDEO)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_INSTRUCTION)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_STARTRECORD)->EnableWindow(FALSE);
	}
}


/////////////////////////////////////////////
// 应用
// Input: NONE
// Ouput: NONE
// Return: NONE
//////////////////////////////////////////////
void CSysSettings::OnBtnAppSgvideo() 
{
	UpdateData(TRUE);
	theApp.m_nStopRec = m_nTimeStopRec;
	theApp.m_nStopVideo = m_nTimeStopVideo;
	theApp.m_bnStartTempRec = m_bStartTempRec;
}

void CSysSettings::LoadData()
{
	m_sPicPath = theApp.m_sPicPath;
	m_sRecordPath = theApp.m_sRecordPath;

	m_nTimeStopRec = theApp.m_nStopRec;
	m_nTimeStopVideo = theApp.m_nStopVideo;
	m_bStartTempRec = theApp.m_bnStartTempRec;

	UpdateData(FALSE);
}