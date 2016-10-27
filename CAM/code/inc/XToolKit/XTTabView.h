//:Ignore
// XTTabView.h interface for the CXTTabView class.
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

#if !defined(__XTTABVIEW_H__)
#define __XTTABVIEW_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTTabView is a multiple inheritance class derived from CCtrlView and
// CXTTabCtrlBaseEx. CXTTabView can be used to create a view that contains
// nested views displayed in a tab control.  See CXTTabCtrlBaseEx for additional
// functionality.
//
class _XT_EXT_CLASS CXTTabView : public CCtrlView, public CXTTabCtrlBaseEx
{
	DECLARE_DYNCREATE(CXTTabView)

	friend class CXTTabCtrlBase;
	friend class CXTTabCtrlBaseEx;

public:

    // Constructs a CXTTabView object.
    //
	CXTTabView();

	// Destroys a CXTTabView object, handles cleanup and de-allocation.
    //
    virtual ~CXTTabView();

protected:

public:

	// Call this member function to retrieve a reference pointer to the CTabCtrl
	// object associated with this view. Returns a CTabCtrl reference to the
	// object associated with this view.
    //
	virtual CTabCtrl& GetTabCtrl () const;

	// Call this function to assign an image list to the tab control associated
	// with this view. Returns a pointer to the previous image list, or NULL,
	// if there is no previous image list.
    //
	virtual CImageList* SetTabImageList(
        // Pointer to the image list to be assigned to the tab control.
        CImageList *pImageList);

	// This member function retrieves the handle of the tooltip control associated
	// with the tab control.  The tab control creates a tooltip control if
	// it has the TCS_TOOLTIPS style.  You can also assign a tooltip control
	// to a tab control by using the SetToolTips member function. Returns
	// the handle of the tooltip control if successful, otherwise returns NULL.
    //
	virtual CToolTipCtrl* GetToolTips();
	
	// Call this function to assign a tooltip control to the tab control.
	// You can associate the tooltip control with a tab control by making
	// a call to GetToolTips.
    //
	virtual void SetToolTips(
        // Pointer to a tooltip control.
        CToolTipCtrl* pWndTip);

	// Call this member function to enable or disable tooltip usage. Returns
	// TRUE if the tooltip control was found and updated, otherwise returns
	// FALSE.
    //
	virtual BOOL EnableToolTips(
        // TRUE to enable tooltip usage.
        BOOL bEnable);

	// Call this member function to update the document name with the tab
	// label.
    //
	void UpdateDocTitle();

protected:

    //:Ignore
    //{{AFX_VIRTUAL(CXTTabView)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
    //:End Ignore

protected:
	//:Ignore
    //{{AFX_MSG(CXTTabView)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LRESULT OnInitialize(WPARAM wp, LPARAM lp) { return OnInitializeImpl(wp, lp); };
    //:End Ignore
	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE CTabCtrl& CXTTabView::GetTabCtrl() const {
	ASSERT_VALID(this); return (CTabCtrl&)*this;
}
AFX_INLINE CImageList* CXTTabView::SetTabImageList(CImageList *pImageList) {
	ASSERT_VALID(this); return GetTabCtrl().SetImageList(pImageList);
}
AFX_INLINE CToolTipCtrl* CXTTabView::GetToolTips() {
	return GetTips();
}
AFX_INLINE void CXTTabView::SetToolTips(CToolTipCtrl* pWndTip) {
	SetTips(pWndTip);
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__XTTABVIEW_H__)
