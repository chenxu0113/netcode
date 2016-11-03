#if !defined(AFX_CAMLINK_H__5804A914_1225_4D43_ABA3_2D271DDE15E1__INCLUDED_)
#define AFX_CAMLINK_H__5804A914_1225_4D43_ABA3_2D271DDE15E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once

#endif // _MSC_VER > 1000
// CCamLinkSettings.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCamLinkSettings dialog
#include "afxcmn.h"
#include "afxwin.h"



class CCamLinkSettings : public CBCGPDialog
{
	// Construction
public:
	CCamLinkSettings(CWnd* pParent = NULL);   // standard constructor

	bool isNum(string str);
	BOOL IsIpv4str(const char *str);
	void SaveCamInfoToXML();

public:
	// Dialog Data
	//{{AFX_DATA(CSysSettings)
	enum { IDD = IDD_CAM_SETTING };
	CListCtrl		m_camLinkCtrlList;
	CIPAddressCtrl	m_camIPCtrl;
	CEdit			m_camPortEdit;
	CEdit			m_totalChaEdit;
	CEdit			m_chaIDEdit;
	int				m_iCamPort;

	CComboBox	   m_netIPComboBox;
	CComboBox	   m_netDoIDComboBox;

	CButton			m_btnAdd;
	CButton			m_btnModify;
	CButton			m_btnDel;

	CString			m_sNetIP;
	CString			m_sNetPort;

	int				m_clickRow;

	int				m_curID;
	int				m_curListRow;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSysSettings)
	virtual BOOL OnInitDialog();

	afx_msg void OnClose();
	afx_msg void OnBtnAdd();
	//afx_msg void OnSetNetModIP();
	afx_msg void OnAddModule();
	afx_msg void OnModifyModule();
	afx_msg void OnDelModule();

	afx_msg void OnNMclkQuotalist(NMHDR *pNMHDR, LRESULT *pResult);

	//afx_msg void OnSetNetModPort();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboNetip();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
