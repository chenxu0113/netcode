//:Ignore
// XTTabCtrl.h interface for the CXTTabCtrl class.
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

#if !defined(__XTTABCTRL_H__)
#define __XTTABCTRL_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//:Ignore
#define MESSAGE_MAP_ENTRIES_CXTTabCtrlBase \
	ON_WM_PAINT() \
	ON_WM_ERASEBKGND() \
	ON_WM_SETTINGCHANGE() \
	ON_WM_SYSCOLORCHANGE()

#define MESSAGE_MAP_ENTRIES_CXTTabCtrlBaseEx \
	MESSAGE_MAP_ENTRIES_CXTTabCtrlBase \
	ON_WM_RBUTTONDOWN() \
	ON_WM_CREATE() \
	ON_WM_DESTROY() \
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange) \
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, OnSelchanging) \
	ON_WM_WINDOWPOSCHANGED() \
	ON_MESSAGE(XTWM_INITIAL_UPDATE, OnInitialize) \
	ON_MESSAGE_VOID(WM_INITIALUPDATE, OnInitialUpdate)
//:End Ignore

//:Ignore
class CXTTabCtrlButtons;
//:End Ignore

// XT_NAVBTNFLAGS is an enumeration used by CXTTabCtrl to determine navigation button style.
// 
enum XT_NAVBTNFLAGS
{
	// To show arrow buttons.
	XT_SHOW_ARROWS = 1,
	// To show close button.
	XT_SHOW_CLOSE = 2,
	// To show arrow and close buttons.
	XT_SHOW_ALL = 3
};

//////////////////////////////////////////////////////////////////////
// CXTTabCtrlBase is a stand alone base class.  It is used to draw an XP
// style tab control.

class _XT_EXT_CLASS CXTTabCtrlBase
{
public:

    // Constructs a CXTTabCtrlBase object.
    //
	CXTTabCtrlBase();

	// Destroys a CXTTabCtrlBase object, handles cleanup and de-allocation.
    //
	virtual ~CXTTabCtrlBase();

protected:

    // Black pen (XP only).
	CPen m_penBlack;
    // White pen (XP only).
	CPen m_penWhite;
	// 3D face pen (XP only).
	CPen m_penFace;
	// Non-selected text pen (XP only).
	CPen m_penText;
	// System metrics for SM_CYEDGE (XP only).
	int m_iEdge;
	// Pointer to the tab control associated with this object.
	CTabCtrl *m_pTabCtrl;

	// Call this member function to associate the tab control with this object.
	//
	void ImplAttach(
		// Points to a valid tab control object.
		CTabCtrl *pTabCtrl);

public:

    // 'true' to set the selected tab font to bold.
	bool m_bBoldFont;
	// 'true' to draw an XP border around the tab child window.
	bool m_bXPBorder;
	// 'true' to override system defaults and disable XP look for this control.
	bool m_bDisableXPMode;
	// TRUE for auto-condensing tabs.
	BOOL m_bAutoCondensing;

public:

	// This member function copies the child coordinates of the CTabCtrl client
	// area into the object referenced by 'rcChild'.  The client coordinates
	// specify the upper-left and lower-right corners of the client area. 
    //
	virtual void GetChildRect(
		// A reference to a CRect object to receive the client coordinates.
        CRect& rcChild) const;

	// Call this member function to set visibility of the navigation buttons.  These
	// buttons are used inplace of the default forward and back buttons that are
	// displayed when the tab control is not wide enough to display all tabs.  You can
	// also define a close button to be used to close the active tab.  This will give
	// the tab control a VS.NET style tabbed interface.
	//
	void ShowNavButtons(
        // The value can be one or more of the following:
        //[ul]
        //[li][b]XT_SHOW_ARROWS[/b] - To show arrow buttons.[/li]
        //[li][b]XT_SHOW_CLOSE[/b] - To show close button.[/li]
		//[li][b]XT_SHOW_ALL[/b] - To show arrow and close buttons.[/li]
        //[/ul]
		DWORD dwFlags);

protected:
    // This member function is called by the tab control to draw top aligned
	// XP style tabs.
    //
    virtual void OnDrawTop(
		// Points to the client device context.
        CDC* pDC,
		// Size of the client area to paint.
        CRect rcClient,
		// Size of the selected tab item.
        CRect rcItem);

    // This member function is called by the tab control to draw bottom aligned
	// XP style tabs.
    //
	virtual void OnDrawBottom(
		// Points to the client device context.
        CDC* pDC,
		// Size of the client area to paint.
        CRect rcClient,
		// Size of the selected tab item.
        CRect rcItem);

    // This member function is called by the tab control to draw left aligned
	// XP style tabs.
    //
    virtual void OnDrawLeft(
        // Points to the client device context.
        CDC* pDC,
		// Size of the client area to paint.
        CRect rcClient,
		// Size of the selected tab item.
        CRect rcItem);

    // This member function is called by the tab control to draw right aligned
	// XP style tabs.
    //
    virtual void OnDrawRight(
		// Points to the client device context.
        CDC* pDC,
		// Size of the client area to paint.
        CRect rcClient,
		// Size of the selected tab item.
        CRect rcItem);

    // This member function is called by the tab control to add padding to a
	// tab label for use with XP style tabs.
    //
    virtual void OnAddPadding(
		// Tab label to add padding to.
        CXTString& strLabelText);

    //:Ignore
	BOOL OnEraseBkgndImpl(CDC* pDC);
	void OnPaintImpl();
	void OnSettingChangeImpl_Post(UINT uFlags, LPCTSTR lpszSection);
	void OnSysColorChangeImpl_Post();
	void PaintButtons();
	CXTTabCtrlButtons* m_pNavBtns;
    //:End Ignore
};

//////////////////////////////////////////////////////////////////////
// CXTTabCtrlBaseEx is a CXTTabCtrlBase derived class.  It is used to create
// a CXTTabCtrlBaseEx class object.
//
class _XT_EXT_CLASS CXTTabCtrlBaseEx : public CXTTabCtrlBase
{
protected:
	
	// Points to the last active view that belongs to the main frame window.
	CView* m_pLastActiveView;
	// Points to the parent and will equal 'm_pParentFrame' in non-dialog applications.
	CWnd* m_pParentWnd;
	// Points to the parent frame.
	CFrameWnd* m_pParentFrame;
	// Index of the popup menu contained in the menu.
	int m_nPos;
	// Popup menu resource ID.
	UINT m_popupMenuID;
	// TRUE to send initial update to views when created.
	BOOL m_bInitialUpdate;

//:Ignore
private:
	DWORD      m_dwInitSignature;
	BOOL       m_bInitialUpdatePending;
	int        m_nOldIndex;
//:End Ignore

public:

    // Template list containing tab information.
    CList <XT_TCB_ITEM*, XT_TCB_ITEM*> m_tcbItems;

    // Constructs a CXTTabCtrlBase object.
    //
	CXTTabCtrlBaseEx();

	// Destroys a CXTTabCtrlBase object, handles cleanup and de-allocation.
    //
	virtual ~CXTTabCtrlBaseEx();

	// This member function is called to initialize the font for the tab control
	// associated with this view.
    //
    virtual void InitializeFont();

	// This member function retrieves the handle of the tooltip control associated
	// with the tab control.  The tab control creates a tooltip control if
	// it has the TCS_TOOLTIPS style.  You can also assign a tooltip control
	// to a tab control by using the SetToolTips member function. Returns
	// the handle of the tooltip control if successful, otherwise returns NULL.
    //
	virtual CToolTipCtrl* GetTips();

	// Call this function to assign a tooltip control to the tab control.
	// You can associate the tooltip control with a tab control by making
	// a call to GetToolTips.
    //
	virtual void SetTips(
		// Pointer to a tooltip control.
        CToolTipCtrl* pWndTip);

	// Call this function to register a tab with the tooltip control, so
	// that the information stored in the tooltip is displayed when the cursor
	// is on the tab.
    //
	virtual void AddToolTip(
		// Index of the tab.
        UINT nIDTab,
		// Pointer to the text for the tool.
        LPCTSTR lpszText);

	// Call this function to update the tooltip text for the specified tab.
    //
	virtual void UpdateToolTip(
		// Index of the tab.
        int nIDTab,
		// Pointer to the text for the tool.        
        LPCTSTR lpszText);

	// Call this function to update the tooltip text for the specified tab.
    //
	virtual void UpdateToolTip(
		// CRuntimeClass associated with the tab.
        CRuntimeClass *pViewClass,
		// Pointer to the text for the tool.
        LPCTSTR lpszText);

	// This member function is called to reset the values for the tooltip
	// control based upon the information stored for each tab.
    //
	virtual void ResetToolTips();

	//:Ignore
	// Call this member function to enable or disable tooltip usage. Returns
	// TRUE if the tooltip control was found and updated, otherwise returns
	// FALSE.
    //
	virtual BOOL EnableToolTipsImpl(
        // TRUE to enable tooltip usage.
        BOOL bEnable);
	//:End Ignore
    
	// Call this member function to add a view to the tab control associated
	// with this view. Returns TRUE if successful, otherwise returns FALSE.
    //	
    virtual BOOL AddView(
		// Pointer to the text for the tab associated with the view.
        LPCTSTR lpszLabel,
		// CView runtime class associated with the tab.
        CRuntimeClass *pViewClass,
		// CDocument associated with the view.
        CDocument* pDoc=NULL,
		// Create context for the view.
        CCreateContext *pContext=NULL,
		// -1 to add to the end.
        int iIndex=-1,
		// Icon index for the tab.  If -1, 'iIndex' is used to determine the index.
        int iIconIndex=-1);

	// Call this member function to add a view to the tab control associated
	// with this view. Returns TRUE if successful, otherwise returns FALSE.
    //	
    virtual BOOL AddView(
		// Pointer to the text for the tab associated with the view.
        LPCTSTR lpszLabel,
		// An existing view to be added to the tab control.
        CView* pView,
		// -1 to add to the end.
        int iIndex=-1,
		// Icon index for the tab.  If -1, nIndex is used to determine the index.
        int iIconIndex=-1);

	// This member function is called to add a control to the tab control
	// associated with this view. Returns TRUE if successful, otherwise returns
	// FALSE.
	//
    virtual BOOL AddControl(
		//	Pointer to the text for the tab associated with the view.
        LPCTSTR lpszLabel,
		// CWnd object associated with the tab.
        CWnd* pWnd,
		// Tab index of where to insert the new view.  Default is -1 to add to
		// the end.
        int iIndex=-1,
		// Icon index for the tab.  If -1, 'iIndex' is used to determine the index.
        int iIconIndex=-1);

	// This member function returns a pointer to a view from the specified
	// runtime class. Returns a pointer to a CView object, otherwise returns
	// NULL.
    //
	virtual CWnd* GetView(
		// CView runtime class associated with the tab.
        CRuntimeClass *pViewClass);

	// This member function returns a pointer to a view from the specified
	// runtime class. Returns a pointer to a CView object, otherwise returns
	// NULL.
    //
	virtual CWnd* GetView(
		// Tab index.
        int nView);

	// This member function returns a pointer to the active view associated
	// with the selected tab. Returns a pointer to the active view, otherwise
	// returns NULL.
    //
	virtual CWnd* GetActiveView();

	// This member function is called to activate the specified view and deactivate
	// all remaining views.
    //
	virtual void ActivateView(
		// CWnd object to make active.
        CWnd* pTabView);

	// This member function will set a view active based on the specified
	// runtime class.
    //
	virtual void SetActiveView(
		// CView runtime class associated with the tab.
        CRuntimeClass *pViewClass);

	// This member function will set a view active based on the specified
	// runtime class.
    //
    virtual void SetActiveView(
		// CWnd object to make active.
        CWnd* pTabView);

	// This member function will set a view active based on the specified
	// tab index.
    //
    virtual void SetActiveView(
		// Tab index.
        int nActiveTab);

	// This member function will remove a view based on the specified
	// tab index.
    //
	virtual void DeleteView(
		// Tab index of the view.
        int nView,
		// TRUE to destroy the list item.
        BOOL bDestroyWnd=TRUE);

	// This member function will remove a view based on the specified CWnd
	// pointer associated with the tab.
    //
	virtual void DeleteView(
		// Points to the CWnd object associated with the tab.
        CWnd* pView,
		// TRUE to destroy the list item.
        BOOL bDestroyWnd=TRUE);

	// This member function will remove a view based on the specified CView
	// runtime class associated with the tab.
    //
    virtual void DeleteView(
		// CView runtime class associated with the tab.
        CRuntimeClass *pViewClass,
		// TRUE to destroy the list item.
        BOOL bDestroyWnd=TRUE);

	// This member function will return the name for a view based on the tab
	// index. Returns a NULL terminated string that represents the tab item
	// text.
    //
    virtual LPCTSTR GetViewName(
		// Tab index of the view.
        int nView);

	// This member function will return the name for a view based on the tab
	// index. Returns a NULL terminated string that represents the tab item 
	// text.
    //
    virtual LPCTSTR GetViewName(
		// CView runtime class associated with the tab.
        CRuntimeClass *pViewClass);

	// Call this member function to resize the tab view specified by 'pView'.
    //
	virtual void ResizeTabView(
		// A pointer to a CWnd object to be resized.
        CWnd* pView);

	// This member function is used by the tab control bar to remove an item
	// from the tab view list.
    //
	virtual void RemoveListItem(
		// The POSITION value of the item to be removed.
        POSITION pos,
		// TRUE to destroy the list item.
        BOOL bDestroyWnd=TRUE);

	// Call this member function to remove all the tabs, including all associated
	// views. Returns TRUE if successful, otherwise returns FALSE.
	//
	virtual BOOL RemoveAllTabs(
        // TRUE to destroy the window associated with the tab item.
        BOOL bDestroyWnd=TRUE);

	// Call this member function to retrieve the tab index from the current cursor
	// position. Returns an integer based index of the tab, or –1, if no tab
	// is at the specified 'point'.
    //
	virtual int GetTabFromPoint(
		// Pointer to a CPoint object that contains the cursor screen coordinates.
		// Use default for current cursor position.
        CPoint point);

	// Call this member function to see if the specified CWnd object is a
	// child of the tab control. Returns TRUE if the specified CWnd object
	// is a child of the tab control, otherwise returns FALSE.
    //
	virtual BOOL IsChildView(
		// A pointer to a CWnd object.
        CWnd* pView);

	// Call this member function to set the text for the specified tab. Returns
	// TRUE if successful, otherwise returns FALSE.
    //
	BOOL SetTabText(
		// Index of the tab.
        int nTab,
		// New text for the tab label.
        LPCTSTR lpszLabel);

	// Call this member function to set the text for the specified tab. Returns
	// TRUE if successful, otherwise returns FALSE.
    //
    BOOL SetTabText(
        // CWnd object associated with the tab.
        CWnd* pView,
		// New text for the tab label.
        LPCTSTR lpszLabel);

	// Call this member function to set the text for the specified tab. Returns
	// TRUE if successful, otherwise returns FALSE.
    //
    BOOL SetTabText(
		// CRuntimeClass of the CWnd associated with the tab.
        CRuntimeClass *pViewClass,
		// New text for the tab label.
        LPCTSTR lpszLabel);

	// This member function is called to activate the next view in the tab
	// control. Returns a CWnd pointer to the newly activated view.
    //
	CWnd* NextView();

	// This member function is called to activate the previous view in the
	// tab control. Returns a CWnd pointer to the newly activated view.
    //
	CWnd* PrevView();

	// Call this member function to enable or disable the tab auto-condensing
	// mode.  Auto-condensing mode affects the tab control's behavior when
	// there is not enough room to fit all tabs.  Without auto-condensation,
	// the CXTTabCtrl control behaves like a standard tab control (i.e. it
	// will display a slider control that allows the user to pan between tabs).
	// With the auto-condensing mode enabled, CXTTabCtrl attempts to fit all
	// tabs in the available space by trimming the tab label text.  This behavior
	// is similar to the behavior displayed by Visual C++'s Workspace View.
	// For instance, you can see the FileView tab shrink if you shrink the
	// Workspace View.
    //
    void SetAutoCondense(
		// TRUE to enable auto-condense mode.
        BOOL bEnable);

	// This member function returns the state of the tab control's auto-condense
	// mode. See SetAutoCondense() for a full explanation of this mode. Returns
	// TRUE if auto-condense is enabled, or FALSE if it is disabled.
	//
    BOOL GetAutoCondense();

	// This member function will modify the style for the tab control associated
	// with this view and set the appropriate font depending on the tab's
	// orientation. Returns nonzero if the style was successfully modified,
	// otherwise returns zero. 
	//
	virtual BOOL ModifyTabStyle(
		// Specifies window styles to be removed during style modification.
        DWORD dwRemove,
		// Specifies window styles to be added during style modification.
        DWORD dwAdd,
		// Flags to be passed to SetWindowPos, or zero if SetWindowPos should
		// not be called. The default is zero.  See CWnd::ModifyStyle for more
		// details.
        UINT nFlags=0);

	// This member function is used to get the last known view that belongs
	// to the frame. Returns a CView pointer to the last known view.
	//
	CView* GetLastKnownChildView();

	// This member function is used to set the resource ID for the popup menu
	// used by the tab control.
    //
	virtual void SetMenuID(
        // ID for the tab control popup menu.
        UINT popupMenuID,
		// Index position in the menu resource.
        int nPos=0);

	// This member function returns the menu resource associated with the
	// tab control. Returns the resource ID of the menu associated with the
	// tab control.
    //
	virtual UINT GetMenuID();

	// Call this member function to allow WM_INITIALUPATE message to be sent
	// to views after creation.
    //
	virtual void SendInitialUpdate(
		// TRUE to send initial update message.
        BOOL bInitialUpdate);

protected:

	// This member function is a virtual method that is called to handle a
	// TCN_SELCHANGING event. Override in your derived class to add additional
	// functionality.
    //
	virtual void OnSelChanging();

	// This member function is a virtual method that is called to handle a
	// TCN_SELCHANGE event. Override in your derived class to add additional
	// functionality.
    //
	virtual void OnSelChange();

	// This member function is called to set the tooltip and tab text for
	// the specified tab. Returns TRUE if successful, otherwise returns FALSE.
    //
	BOOL UpdateTabLabel(
		// Index of the tab.
        int nTab,
		// Address of an XT_TCB_ITEM struct associated with the tab.
        XT_TCB_ITEM* pMember,
		// NULL terminated string that represents the new tab label.
        LPCTSTR lpszLabel);

    // This member function creates the CWnd object that is associated
	// with a tab control item. Returns a pointer to the newly created CWnd
	// object, otherwise returns NULL.
    //
	virtual CWnd* CreateTabView(
		// CView runtime class to be created.
        CRuntimeClass *pViewClass,
		// CDocument associated with view.
        CDocument *pDocument,
		// Create context for the view.
        CCreateContext *pContext);

    // This member function is used internally by the tab control to calculate
	// the width of a tab based on its label text. Returns an integer value that
	// represents the width of a tab.
    //
    int CalculateTabWidth(
		// Points to the current device context.
		CDC *pDC,
		// Represents the tab label text.
		CString& sLabel,
		// Set to 'true' if the tab item has an icon.
		bool bHasIcon);

    // This member function is used internally by the tab control to shrink,
	// or unshrink, tabs based on the control's width and the state of the
	// auto-condensation mode. See SetAutoCondense() for more information.
    //
    void Condense();

    //:Ignore
	void OnRButtonDownImpl(UINT nFlags, CPoint point);
	int OnCreateImpl_Post(LPCREATESTRUCT lpCreateStruct);
	void OnDestroyImpl_Pre();
	void OnSelchangeImpl(NMHDR* pNMHDR, LRESULT* pResult);
	void OnSelchangingImpl(NMHDR* pNMHDR, LRESULT* pResult);
	void OnWindowPosChangedImpl_Pre(WINDOWPOS FAR* lpwndpos);
	void OnWindowPosChangedImpl_Post(WINDOWPOS FAR* lpwndpos);
	BOOL PreTranslateMessageImpl(MSG* pMsg);
	void PreSubclassWindowImpl_Post();
	BOOL OnCmdMsgImpl_Pre(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	LRESULT OnInitializeImpl(WPARAM, LPARAM);
	void OnInitialUpdateImpl();
    //:End Ignore
};

//////////////////////////////////////////////////////////////////////
// CXTTabCtrl is a multiple inheritance class derived from CTabCtrl and
// CXTTabCtrlBaseEx. It is used to create a CXTTabCtrl class object.  See
// CXTTabCtrlBaseEx for additional functionality.
//
class _XT_EXT_CLASS CXTTabCtrl : public CTabCtrl, public CXTTabCtrlBaseEx
{
	DECLARE_DYNAMIC(CXTTabCtrl)

	friend class CXTTabCtrlBase;
	friend class CXTTabCtrlBaseEx;

public:
	
    // Constructs a CXTTabCtrl object.
    //
	CXTTabCtrl();

	// Destroys a CXTTabCtrl object, handles cleanup and de-allocation.
    //
    virtual ~CXTTabCtrl();

protected:

public:
	// Call this member function to enable or disable tooltip usage. Returns
	// TRUE if the tooltip control was found and updated, otherwise returns
	// FALSE.
    //
	virtual BOOL EnableToolTips(
		// TRUE to enable tooltip usage.
        BOOL bEnable);

	//:Ignore
    //{{AFX_VIRTUAL(CXTTabCtrl)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
    //:End Ignore

	//:Ignore
    //{{AFX_MSG(CXTTabCtrl)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnPaint();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	afx_msg LRESULT OnInitialize(WPARAM wp, LPARAM lp) { return OnInitializeImpl(wp, lp); };
	afx_msg void OnInitialUpdate() {OnInitialUpdateImpl(); }

    //:End Ignore
	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE UINT CXTTabCtrlBaseEx::GetMenuID() {
	ASSERT(::IsWindow(m_pTabCtrl->GetSafeHwnd())); return m_popupMenuID;
}
AFX_INLINE CWnd* CXTTabCtrlBaseEx::GetActiveView() {
	return GetView(m_pTabCtrl->GetCurSel());
}
AFX_INLINE void CXTTabCtrlBaseEx::SetMenuID(UINT popupMenuID, int nPos) {
	m_popupMenuID = popupMenuID; m_nPos = nPos;
}
AFX_INLINE void CXTTabCtrlBaseEx::SendInitialUpdate(BOOL bInitialUpdate) {
	m_bInitialUpdate = bInitialUpdate;
}
AFX_INLINE CView* CXTTabCtrlBaseEx::GetLastKnownChildView() {
 	return m_pLastActiveView;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTTABCTRL_H__)

