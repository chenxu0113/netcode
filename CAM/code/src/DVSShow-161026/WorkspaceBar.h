// WorkspaceBar.h : interface of the CWorkspaceBar class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WORKSPACEBAR_H__019A981F_F8F4_4BA3_B2FE_F64A84ECF70B__INCLUDED_)
#define AFX_WORKSPACEBAR_H__019A981F_F8F4_4BA3_B2FE_F64A84ECF70B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWorkspaceBar : public CBCGPDockingControlBar
{
public:
	CWorkspaceBar();
	CTreeCtrl	m_wndTree;
// Attributes
protected:
	
	CImageList	m_imgList;
	BOOL	m_bIsRBtnDown;
	CString	m_sCurSelTreeItem;

// Operations
public:
	Devc_Map m_devMap;
	BOOL		InitTree(HTREEITEM hParent=TVI_ROOT);
	HTREEITEM	m_hCamera;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkspaceBar)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWorkspaceBar();

// Generated message map functions
protected:
	//{{AFX_MSG(CWorkspaceBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnRClickTree(NMHDR* pNMHDR, LRESULT* pResult);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKSPACEBAR_H__019A981F_F8F4_4BA3_B2FE_F64A84ECF70B__INCLUDED_)
