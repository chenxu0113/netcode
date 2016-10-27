#if !defined(AFX_CONTROL_H__9689645B_801B_46DB_96F0_49A343B3AAB6__INCLUDED_)
#define AFX_CONTROL_H__9689645B_801B_46DB_96F0_49A343B3AAB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Control.h : header file
//
#include "PTZButton.h"

/////////////////////////////////////////////////////////////////////////////
// CControl dialog

class CControl : public CBCGPDialog
{
// Construction
public:
	CControl(CWnd* pParent = NULL);   // standard constructor
	int m_niSelSpeed;
// Dialog Data
	//{{AFX_DATA(CControl)
	enum { IDD = IDD_DLG_CONTROL };
	CBCGPComboBox	m_comboSpeed;
	CPTZButton m_btnPtzUp;
	CPTZButton m_btnPtzDown;
	CPTZButton m_btnPtzLeft;
	CPTZButton m_btnPtzRight;
	CPTZButton m_btnZoomIn;//zoom in
	CPTZButton m_btnZoomOut;//zoom out
	CPTZButton m_btnFocusNear;//focus near
	CPTZButton m_btnFocusFar;//focus far
	CPTZButton m_btnIrisOpen;//iris open
	CPTZButton m_btnIrisClose;//iris close
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CControl)

	afx_msg void OnClose();
	afx_msg void OnSelchangeComboSpeed();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSetspeed();
	afx_msg void OnBnClickedBtnPtzUp();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROL_H__9689645B_801B_46DB_96F0_49A343B3AAB6__INCLUDED_)
