#if !defined(AFX_SYSSETTINGS_H__5804A914_1225_4D43_ABA3_2D271DDE15E1__INCLUDED_)
#define AFX_SYSSETTINGS_H__5804A914_1225_4D43_ABA3_2D271DDE15E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SysSettings.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSysSettings dialog

class CSysSettings : public CBCGPDialog
{
// Construction
public:
	CSysSettings(CWnd* pParent = NULL);   // standard constructor
	void SetCtrlStatus();
	void LoadData();
public:
// Dialog Data
	//{{AFX_DATA(CSysSettings)
	enum { IDD = IDD_DLG_SETTINGS };
	CBCGPSliderCtrl	m_slSaturation;
	CBCGPSliderCtrl	m_slHue;
	CBCGPSliderCtrl	m_slContrast;
	CBCGPSliderCtrl	m_slBrightness;
	CString	m_sPicPath;
	CString	m_sRecordPath;
	int		m_nTimeStopRec;
	int		m_nTimeStopVideo;
	BOOL	m_bStartTempRec;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSysSettings)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnDefault();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBtnPicpathsel();
	afx_msg void OnBtnRecordselpath();
	afx_msg void OnClose();
	afx_msg void OnCheckSgvideo();
	afx_msg void OnBtnOk();
	afx_msg void OnBtnAppSgvideo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSSETTINGS_H__5804A914_1225_4D43_ABA3_2D271DDE15E1__INCLUDED_)
