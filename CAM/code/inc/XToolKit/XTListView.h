//:Ignore
// XTListView.h interface for the CXTListView class.
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
//:End Ignore

#if !defined(__XTLISTVIEW_H__)
#define __XTLISTVIEW_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTListView is a multiple inheritance class derived from CListView and
// CXTListCtrlBase.  This class implements flat header and generic sorting
// routines.  See CXTListCtrlBase for additional functionality.
//
class _XT_EXT_CLASS CXTListView : public CListView, public CXTListCtrlBase
{
    DECLARE_DYNCREATE(CXTListView)

public:
    
    // Constructs a CXTListView object.
    //
    CXTListView();

    // Destroys a CXTListView object, handles cleanup and de-allocation.
    //
    virtual ~CXTListView();

    // This member function is called to set extended styles for the list
	// control, ie: LVS_EX_FULLROWSELECT, LVS_EX_GRIDLINES, etc. See MSDN
	// documentation for a complete list of available styles.
	//
    virtual void SetExtendedStyle(
		// DWORD value that specifies the extended list-view control style. This
		// parameter can be a combination of Extended List-View Styles.
        DWORD dwExStyle );

    // This member function is called to return the list control extended
	// style. Returns a DWORD value.
	//
    DWORD GetExtendedStyle();

protected:

public:
    //:Ignore
    //{{AFX_VIRTUAL(CXTListView)
    public:
    virtual void OnInitialUpdate();
    protected:
    virtual void OnDraw(CDC* pDC);      
    virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
    //}}AFX_VIRTUAL
	//:End Ignore

#ifdef _DEBUG
//:Ignore
    
	virtual void AssertValid() const;
    
	virtual void Dump(
		CDumpContext& dc) const;

//:End Ignore
#endif

	//:Ignore
    //{{AFX_MSG(CXTListView)
    afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDestroy();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    //}}AFX_MSG
    //:End Ignore
    DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE DWORD CXTListView::GetExtendedStyle() {
	return CXTListCtrlBase::GetExtendedStyle();
}
AFX_INLINE void CXTListView::SetExtendedStyle(DWORD dwExStyle){
	CXTListCtrlBase::SetExtendedStyle(dwExStyle);
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTLISTVIEW_H__)
