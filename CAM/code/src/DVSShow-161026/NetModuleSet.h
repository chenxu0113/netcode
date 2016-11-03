#if !defined(AFX_NETMODULE_H__5804A914_1225_4D43_ABA3_2D271DDE15E1__INCLUDED_)
#define AFX_NETMODULE_H__5804A914_1225_4D43_ABA3_2D271DDE15E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once

#endif // _MSC_VER > 1000
// CNetModuleSettings.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNetModuleSettings dialog
#include "afxcmn.h"
#include "afxwin.h"



class CNetModuleSettings : public CBCGPDialog
{
	// Construction
public:
	CNetModuleSettings(CWnd* pParent = NULL);   // standard constructor

	bool isNum(string str);
	BOOL IsIpv4str(const char *str);

public:
	// Dialog Data
	//{{AFX_DATA(CSysSettings)
	enum { IDD = IDD_DIG_NETMOD_SET };
	CListCtrl  m_netModListCtrl;
	CIPAddressCtrl m_netIP;
	CEdit		   m_netPort;
	int			   m_iNetPort;
	CComboBox	   m_DoNumComboBox;
	BOOL		   m_bAlarmSetting;
	CButton			m_bAlarmValveCheckBox;
	CButton			m_btnAdd;
	CButton			m_btnModify;
	CButton			m_btnDel;
	CEdit			m_listEdit;
	CString			m_sNetIP;
	CString			m_sNetPort;
	int				m_clickRow;
	int				m_clickColumn;
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
	afx_msg void OnAddModule();
	afx_msg void OnModifyModule();
	afx_msg void OnDelModule();
	afx_msg void OnNMclkQuotalist(NMHDR *pNMHDR, LRESULT *pResult);

	//afx_msg void OnSetNetModPort();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
