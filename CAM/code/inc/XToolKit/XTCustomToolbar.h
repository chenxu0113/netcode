//:Ignore
// XTCustomToolBar.h interface for the CXTCustomToolBar class.
//
// This file is a part of the Xtreme Toolkit for MFC.
// ©1998-2003 Codejock Software, All Rights Reserved.
//
// This source code can only be used under the terms and conditions 
// outlined in the accompanying license agreement.
//
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

#if !defined(__XT_CUSTOMTOOLBAR__)
#define __XT_CUSTOMTOOLBAR__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


/////////////////////////////////////////////////////////////////////////////
// CXTCustomToolbar is a toolbar that gets dynamically created from
// within toolbar customization page
class _XT_EXT_CLASS CXTCustomToolBar : public CXTToolBar
{
	DECLARE_DYNCREATE(CXTCustomToolBar)

// Construction
public:
	CXTCustomToolBar();

// Attributes
private:
	// Persistently stored title
	CString m_strTitle;

	// images of the added buttons
	CImageList m_images;

	// disabled images of the added buttons, created only when CreateDisabledImageList() gets called
	CImageList m_disabledImages;

// Operations
public:

	// Persistently renames this toolbar
	void RenameCustomBar(const CString& strNewName);

	// Deletes this toolbar
	void DeleteCustomBar();

	// Creates and enables disabled image list
	bool CreateDisabledImageList();

// Overrides

	// Fills in supported customizable features
	virtual void GetCustomBarInfo(CXTCustomControlBarInfo* pInfo);

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXTCustomToolBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXTCustomToolBar();
	virtual void PostNcDestroy();
	virtual void ReportCustomGroups(CXTCustomGroups& groups);

	// Generated message map functions
protected:
	//{{AFX_MSG(CXTCustomToolBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__XT_CUSTOMTOOLBAR__)
//:End Ignore
