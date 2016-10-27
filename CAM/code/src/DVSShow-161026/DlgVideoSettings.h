#if !defined(AFX_DLGVIDEOSETTINGS_H__3B857228_2965_463D_BE38_6383284E82DD__INCLUDED_)
#define AFX_DLGVIDEOSETTINGS_H__3B857228_2965_463D_BE38_6383284E82DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgVideoSettings.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgVideoSettings dialog

class CDlgVideoSettings : public CBCGPDialog
{
// Construction
public:
	CDlgVideoSettings(CWnd* pParent = NULL);   // standard constructor
	void		SetCurSelNum(int nWndNum);
// Dialog Data
	//{{AFX_DATA(CDlgVideoSettings)
	enum { IDD = IDD_DLG_VIDEOSETTINGS };
	CComboBox	m_wndVideoWndNum;
	//}}AFX_DATA
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgVideoSettings)
	afx_msg void OnSelchangeComboVideowndnum();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgVideoSettings)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGVIDEOSETTINGS_H__3B857228_2965_463D_BE38_6383284E82DD__INCLUDED_)
