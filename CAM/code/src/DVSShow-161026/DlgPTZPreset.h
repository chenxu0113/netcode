#if !defined(AFX_DLGPTZPRESET_H__3AFF28D5_13FD_4FF0_A536_36D4FACB3AE3__INCLUDED_)
#define AFX_DLGPTZPRESET_H__3AFF28D5_13FD_4FF0_A536_36D4FACB3AE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPTZPreset.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPTZPreset dialog

class CDlgPTZPreset : public CBCGPDialog
{
// Construction
public:
	CDlgPTZPreset(CWnd* pParent = NULL);   // standard constructor
	int m_nCurSel;
	void MoveToPtzPreSet(int nCurSel, int nWndIndex);
	BOOL WriteToINI(int nWndIndex, int nSelected);
	void RedrawComBox(BYTE nCamType);
// Dialog Data
	//{{AFX_DATA(CDlgPTZPreset)
	enum { IDD = IDD_DLG_PTZPRESET };
	CComboBox	m_comboPtzPreset;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPTZPreset)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPTZPreset)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBtnAdd();
	afx_msg void OnBtnDelete();
	afx_msg void OnBtnPoint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPTZPRESET_H__3AFF28D5_13FD_4FF0_A536_36D4FACB3AE3__INCLUDED_)
