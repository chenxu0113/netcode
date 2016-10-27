//:Ignore
// XTControlBar.h : interface for the CXTControlBar class.
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

#if !defined(__XTCONTROLBAR_H__)
#define __XTCONTROLBAR_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Forwards
class CXTControlBar;
class CXTDockWindow;

//////////////////////////////////////////////////////////////////////
// CXTDockContextWndHook is a CXTWndHook derived class.  It listens to
// the relevant messages the parent frame receives.
//
class CXTDockContextWndHook : public CXTWndHook
{
	DECLARE_DYNAMIC(CXTDockContextWndHook)

		CXTControlBar* m_pBar;

public:
	// Constructs a CXTDockContextWndHook object.
	//
	CXTDockContextWndHook(
		CXTControlBar* pBar)
		: m_pBar(pBar)
	{ }

	virtual LRESULT WindowProc(
		UINT message,
		WPARAM wParam,
		LPARAM lParam);
};

//:End Ignore

//////////////////////////////////////////////////////////////////////
// XT_BARPLACEMENT is a stand alone helper structure class.  It is used
// by CXTControlBar to save and restore the control bar's size, location,
// and style.
//
struct XT_BARPLACEMENT
{
    // Vertical size of the control bar.
	CSize sizeVert;
	// Horizontal size of the control bar.
	CSize sizeHorz;
	// Floating size of the control bar.
	CSize sizeFloat;
	// Default size the control bar should be.
	CSize sizeDefault;
	// TRUE if the control bar is the only one docked in a row or column.
	BOOL bUnique;
	// TRUE if the control bar is maximized.
	BOOL bMaximized;
	// Control bar style flags.
	DWORD dwStyle;
};

//////////////////////////////////////////////////////////////////////
// CXTControlBar is a CControlBar derived class.  It is the base class
// for the control bar classes CXTStatusBar, CXTToolBar, CXTDialogBar and
// CXTReBar. A control bar is a window that is usually aligned to the left
// or right of a frame window.  It may contain child items that are either
// HWND-based controls, which are Windows windows that generate and respond
// to Windows messages, or non-HWND-based items, which are not windows
// and are managed by application code or framework code. List boxes and
// edit controls are examples of HWND-based controls; status-bar panes
// and bitmap buttons are examples of non-HWND-based controls.
//
// Control bar windows are usually child windows of a parent frame window
// and are usually siblings to the client view or MDI client of the frame
// window.  A CXTControlBar object uses information about the parent window’s
// client rectangle to position itself.  It then informs the parent window
// as to how much space remains unallocated in the parent window’s client
// area.
//
class _XT_EXT_CLASS CXTControlBar : public CControlBar
{
	friend CXTDockContextWndHook;

    DECLARE_DYNAMIC(CXTControlBar)

public:
	//:Ignore
	// A delegate to notify when visibility of this control bar changes.
	typedef CXTMultiCastDelegate1<bool> NOTEBARVISIBLE;
	NOTEBARVISIBLE NoteBarVisible;
	//:End Ignore
	
protected:

    // Constructs a CXTControlBar object.
    //
    CXTControlBar();

    // This member function is called to set the size of the borders
    // for the control bar.
    //      
    virtual void SetBorders(
        // Pointer to a RECT structure that represents the border size.
        LPCRECT lpRect);

    // This member function is called to set the size of the borders
    // for the control bar.
    //      
    virtual void SetBorders(
        // Specifies the left position.
        int cxLeft = 0,
        // Specifies the top.
        int cyTop = 0,
        // Specifies the right.
        int cxRight = 0,
        // Specifies the bottom.
        int cyBottom = 0);

    // This member function is called to return the size of the control
	// bar's borders. Returns a CRect object.
    //          
    virtual CRect GetBorders() const;

protected:

	// Placement struct to hold size and style information.
	XT_BARPLACEMENT m_bp;

//:Ignore
private:
	CXTDockContextWndHook* m_pHook;
	CXTDockContextWndHook* m_pHookParent;
	CFrameWnd*		       m_pOriginalFrame;
protected:
//:End Ignore

    // Index of the popup menu contained in the menu.
	int m_nPos;
	// Popup menu resource ID.
	UINT m_menuID;
    // 'true' if the parent is a rebar.
	bool m_bInReBar;
	// 'true' to display context menus for control bars.
	bool m_bContextMenus;
	// 8pt control bar font.
	CFont m_font;
	// 8pt vert control bar font.
	CFont m_fontVert;
	// 8pt bold control bar font.
	CFont m_fontBold;

//:Ignore
private:
	bool m_bEnableMultiFrameDocking;
//:End Ignore

public:

	// This member function is called to enable or disable context menus for
	// the control bar.
	//
	void EnableContextMenus(
		// 'true' to enable context menus.  'false' to disable.
		bool bEnable);

	// Call this member function to determine if the control bar has context
	// menus enabled.  Returns 'true' if context menus are enabled, otherwise
	// returns 'false.'
	//
	bool HasContextMenu();

    // This member function is called to determine if the control bar is nested
    // inside of a CXTReBar object.  Returns 'true' if inside of a rebar.
    //          
    bool InRebar() const;

    // Destroys a CXTControlBar object, handles cleanup and de-allocation.
    //
    virtual ~CXTControlBar();

    // Call this member function to determine which CBRS_XT_ (control bar) 
    // settings are currently set for the control bar.  It does not 
    // handle WS_ (window style) or CBRS_ (control bar style).
    // Returns the current CBRS_XT_ (control bar) settings for the control bar. 
    // See ModifyXTBarStyle for the complete list of available styles.
    //
    virtual DWORD GetXTBarStyle();

    // This member function will set the style for the control bar.  It does not 
    // handle WS_ (window style) or CBRS_ (control bar style).
    //
    virtual void SetXTBarStyle(
		// New CBRS_XT_ style for the control bar, see ModifyXTBarStyle() for available
		// styles.
        DWORD dwStyle);

    // Call this member function to modify a control bar style.  Styles to 
    // be added or removed can be combined by using the bitwise OR (|) 
    // operator. 
    // Returns nonzero if the style was successfully modified, otherwise returns zero.
    //
    virtual BOOL ModifyXTBarStyle(
		// Specifies CBRS_XT_ styles to be removed during style 
        // modification.
        DWORD dwRemove,
		// Specifies CBRS_XT_ styles to be added during style 
        // modification.
        DWORD dwAdd,
		// The desired styles for the control bar can be one or more of the 
        // following:
        //[ul]
        //[li][b]CBRS_XT_BUTTONS[/b] - The control bar has a minimize and
		//       close button when docked.[/li]
        //[li][b]CBRS_XT_GRIPPER[/b] - The control bar has a gripper when
		//       docked.[/li]
        //[li][b]CBRS_XT_GRIPPER_FLAT[/b] - The control bar has a flat
		//       gripper.  Use with CBRS_XT_GRIPPER.[/li]
        //[li][b]CBRS_XT_GRIPPER_GRAD[/b] - The control bar has a gradient
		//       flat gripper.  Use with CBRS_XT_GRIPPER_FLAT.[/li]
        //[li][b]CBRS_XT_GRIPPER_TEXT[/b] - The control bar draws text
		//       in the gripper area.  Use with CBRS_XT_GRIPPER.[/li]
        //[li][b]CBRS_XT_BUTTONS_FLAT[/b] - The control bar min and max
		//       buttons are flat.  Use with CBRS_XT_BUTTONS.[/li]
        //[li][b]CBRS_XT_BORDERS_FLAT[/b] - The control bar borders are flat.[/li] 
        //[li][b]CBRS_XT_CLIENT_OUTLINE[/b] - Draw a single black border
		//       outline around the client area.[/li]
        //[li][b]CBRS_XT_CLIENT_STATIC[/b] - Draw a static rect around
		//       the client area.[/li]
        //[li][b]CBRS_XT_CLIENT_MODAL[/b] - Draw a modal rect around the
		//       client area.[/li]
        //[li][b]CBRS_XT_ALL_FLAT[/b] - The following styles combined by
		//       using the bitwise OR (|) operator;  CBRS_XT_GRIPPER_FLAT, 
		//       CBRS_XT_BUTTONS_FLAT and CBRS_XT_BORDERS_FLAT.[/li]
        //[li][b]CBRS_XT_DEFAULT[/b] - The following styles combined by
		//       using the bitwise OR (|) operator;  CBRS_XT_BUTTONS, 
		//       CBRS_XT_GRIPPER, CBRS_XT_GRIPPER_TEXT.[/li]
        //[/ul]
        BOOL bRedraw=TRUE);

    // This member function is used to set the resource ID for the 
    // popup menu associated with the control bar.
    //
    virtual void SetMenuID(
		// Resource ID of the menu to be used.
        UINT menuID,
		// Zero-based index of the popup menu located in the
        // menu resource.  For example IDR_MAINFRAME's "File"
        // popup menu would be 0.
        int nPos=0);

    // This member function retrieves the resource ID of the popup menu.
    // Returns a UNIT value that represents the popup menu ID associated
    // with the control bar.
    //
    virtual UINT GetMenuID();

    // This member function is called by the control bar to perform paint
    // operations.
    //          
    virtual void DoPaint(
		// A CDC pointer that represents the current device context.
        CDC* pDC);

    // This member function is called to draw the gripper for the control bar.
    //
    virtual void DrawGripper(
		// A CDC pointer that represents the current device context.
        CDC* pDC,
		// A CRect reference that represents the size of the gripper to draw.
        const CRect& rect);

    // This member function is called to draw the borders for the control bar.
    //          
    virtual void DrawBorders(
		// A CDC pointer that represents the current device context.
        CDC* pDC,
		// A CRect reference that represents the border area to draw.
        CRect& rect);

    // Call this member function to enable a control bar to be docked. 
    // The sides specified must match one of the sides enabled for 
    // docking in the destination frame window, or the control bar 
    // cannot be docked to that frame window.
    //
    void EnableDocking(
		// Specifies whether the control bar supports docking and the sides of
		// its parent window to which the control bar can be docked, if supported.
        // It can be one or more of the following: 
        //[ul]
        //[li][b]CBRS_ALIGN_TOP[/b] - Allows docking at the top of the
		//       client area.[/li]
        //[li][b]CBRS_ALIGN_BOTTOM[/b] - Allows docking at the bottom of
		//       the client area.[/li]
        //[li][b]CBRS_ALIGN_LEFT[/b] - Allows docking on the left side
		//       of the client area.[/li]
        //[li][b]CBRS_ALIGN_RIGHT[/b] - Allows docking on the right side
		//       of the client area.[/li]
        //[li][b]CBRS_ALIGN_ANY[/b] - Allows docking on any side of the
		//       client area.[/li]
        //[li][b]CBRS_FLOAT_MULTI[/b] - Allows multiple control bars to
		//       be floated in a single mini-frame window.[/li]
        //[/ul]
        //  If 0 (that is, indicating no flags), the control 
        //  bar will not dock.
        DWORD dwDockStyle);

    // Call this member function to enable a control bar to be docked. 
    // The sides specified must match one of the sides enabled for 
    // docking in the destination frame window, or the control bar 
    // cannot be docked to that frame window.
    //
    void EnableDockingEx(
		// Specifies whether the control bar supports 
        // docking and the sides of its parent window to 
        // which the control bar can be docked, if supported.
        // It can be one or more of the following: 
        //[ul]
        //[li][b]CBRS_ALIGN_TOP[/b] - Allows docking at the top of the
		//       client area.[/li]
        //[li][b]CBRS_ALIGN_BOTTOM[/b] - Allows docking at the bottom of
		//       the client area.[/li]
        //[li][b]CBRS_ALIGN_LEFT[/b] - Allows docking on the left side
		//       of the client area.[/li]
        //[li][b]CBRS_ALIGN_RIGHT[/b] - Allows docking on the right side
		//       of the client area.[/li]
        //[li][b]CBRS_ALIGN_ANY[/b] - Allows docking on any side of the
		//       client area.[/li]
        //[li][b]CBRS_FLOAT_MULTI[/b] - Allows multiple control bars to
		//       be floated in a single mini-frame window.[/li]
        //[/ul]
        // If 0 (that is, indicating no flags), the control 
        // bar will not dock.
        DWORD dwDockStyle,
		// Specifies the splitter's, inside dockbars, look.
        // It can be one of the following: 
        //[ul]
        //[li][b]CBRS_XT_NONFLAT[/b] - Thick devstudio like non-flat 
        //       splitters.[/li]
        //[li][b]CBRS_XT_SEMIFLAT[/b] - Thin 3D non-flat splitters.[/li]
        //[li][b]CBRS_XT_FLAT[/b] - Flat splitters.[/li]
        //[/ul]
        DWORD dwFlatStyle);

    // This member function is used to determine if the control bar 
    // is docked on the left side.
    // Returns nonzero if it is docked on the left side, otherwise returns zero.
    //
    virtual BOOL IsLeftDocked() const;

    // This member function is used to determine if the control bar 
    // is docked on the right side.
    // Returns nonzero if it is docked on the right side, otherwise returns zero.
    //
    virtual BOOL IsRightDocked() const;

    // This member function is used to determine if the control bar 
    // is docked on the top side.
    // Returns nonzero if it is docked on the top side, otherwise returns zero.
    //
    virtual BOOL IsTopDocked() const;

    // This member function is used to determine if the control bar 
    // is docked on the bottom side.
    // Returns nonzero if it is docked on the bottom side, otherwise returns zero.
    //
    virtual BOOL IsBottomDocked() const;

    // This member function is used to determine if the control bar 
    // is docked on the top or bottom side.
    // Returns nonzero if it is docked on the top or bottom side, otherwise returns zero.
    //
    virtual BOOL IsHorzDocked() const;

    // This member function is used to determine if the control bar 
    // is docked on the left or right side.
    // Returns nonzero if it is docked on the left or right side, otherwise returns zero.
    //
    virtual BOOL IsVertDocked() const;

    // This member function is used to determine if the control bar 
    // is docked or floating.
    // Returns nonzero if it is floating, otherwise returns 0.
    //
    virtual BOOL IsFloating() const;

	// This member function is used to determine the correct background fill
	// color to be used during paint operations. Returns an RGB color value.
	//
	virtual COLORREF GetBackgroundColor() const;

	// Enables or disables redocking of this control bar. You can also override
	// CanDock() for precisely controlling which control bar can be docked
	// to what frame window. By default, this feature is turned off, and should
	// be enabled [b]before[/b] you actually create the control bar.
	//
	void EnableMultiFrameDocking(
		// Tells if this feature is turned on.  Set this parameter to 'true'
		// to allow this control bar to be docked to all CFrameWnd's descendants
		// that have issued EnableDocking().
		bool bEnable = true);

	// Tells if this control bar is allowed to dock to a specific frame.
	// Returns 'true' if it is allowed to dock, otherwise returns 'false'.
	//
	virtual bool CanDock(
		// The frame this control bar will be redocked to.
		CFrameWnd* pFrame);

	// This notification is called whenever this control bar is added to a
	// CXTReBar object. Default implementation sizes the rebar bar such that
	// it fully displays this control bar. Returns 'true' if successful, otherwise
	// returns 'false'.
	//
	virtual bool OnAddedToRebar(
		// Pointer to a rebar bar descriptor to use to fill in the sizing information.
		REBARBANDINFO* pRBBI,
		// 'true' if the control bar is horizontally oriented.
		bool bHorz);

	// This member function is called whenever this control bar is embedded
	// in a CXTReBar control that has just resized the band in which this
	// control resides. Default implementation does nothing.
	//
	virtual void OnRebarBandResized(
		// Descriptor of the band that contains this control bar.
		XT_REBARSIZECHILDINFO* pInfo,
		// Tells if it is horizontally oriented.
		bool bHorz);

	// This member function tells if this control bar shall be visualized
	// when dragging or the system shall render it with a wire frame.  Default
	// implementation uses a wire frame. Returns 'true' to enable full window
	// drag, or 'false' to use the wire frame.
	//
	virtual bool IsFullWindowDrag();

	// This member function calculates the position of the control bar hot
	// spot, i.e. the point that is used to pin the control bar rect to the
	// mouse cursor when dragging it. Returns 'true' if the menu bar defines
	// a hot spot.  In which case, a buffer pointed to by 'pSize' is filled
	// with the hot spot offset from the top-left corner. Returns 'false'
	// to indicate that no hot spot is defined (the default). 
	//
	virtual bool GetHotSpot(
		//[ul]
		//[li][b]IN[/b] - Extent of the rectangle in which the hot spot must be defined.[/li]
		//[li][b]OUT[/b] - Offset of the hot spot from the rect's top-left corner.[/li]
		//[/ul]
		LPSIZE pSize);

	// Call this member function to retrieve the parent dialog window. The 
	// member function searches up the parent chain until a CDialog (or 
	// derived class) object is found.  Returns a pointer to a dialog window 
	// if successful; otherwise NULL.
	//
	CDialog* GetParentDialog() const;

	// Call this member function to retrieve the parent docking window. The 
	// member function searches up the parent chain until a CXTDockWindow (or 
	// derived class) object is found.  Returns a pointer to a docking window 
	// if successful; otherwise NULL.
	//
	CXTDockWindow* GetParentDockWnd() const;

public:

    //:Ignore
#ifdef _DEBUG
	virtual void AssertValidCtrlID(bool bCheckUnique = true);
	static void AssertValidCtrlID(CControlBar *pBar, bool bCheckUnique = true);
#endif //_DEBUG
	virtual void DockTrackingDone();
    virtual void UpdateControlBarFonts();
	virtual bool ShowMoveCursor() const;
	//:End Ignore
	
protected:

    //:Ignore
    //{{AFX_VIRTUAL(CXTControlBar)
    //}}AFX_VIRTUAL
    virtual BOOL OnPopupMenu(CPoint& point, CFrameWnd* pFrameWnd);
    virtual LRESULT WindowProc(UINT nMsg, WPARAM wParam, LPARAM lParam);
    virtual void CalcInsideRect(CRect& rect, BOOL bHorz) const;
    virtual void EraseNonClient();
	virtual BOOL SetStatusText(int nHit);
	virtual void DelayShow(BOOL bShow);
	virtual bool HasCmdHandlers(CWnd* pWnd);
	//:End Ignore
    
    //:Ignore
    //{{AFX_MSG(CXTControlBar)
    afx_msg void OnWindowPosChanging(LPWINDOWPOS lpWndPos);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnNcPaint();
    afx_msg void OnPaint();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnIdleUpdateCmdUI (WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	afx_msg LRESULT OnAddRebar(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnRebarSizeChild(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnQueryVisualize(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnDragTrackingDone(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnQueryHotSpot(WPARAM wp, LPARAM lp);
	//:End Ignore
	
    DECLARE_MESSAGE_MAP()

    friend class CXTReBar;
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE bool CXTControlBar::InRebar() const {
    ASSERT(::IsWindow(m_hWnd)); return m_bInReBar;
}
AFX_INLINE void CXTControlBar::SetBorders(LPCRECT lpRect) {
    /*ASSERT(::IsWindow(m_hWnd));*/ SetBorders(lpRect->left, lpRect->top, lpRect->right, lpRect->bottom);
}
AFX_INLINE CRect CXTControlBar::GetBorders() const {
    /*ASSERT(::IsWindow(m_hWnd));*/ return CRect(m_cxLeftBorder, m_cyTopBorder, m_cxRightBorder, m_cyBottomBorder);
}
AFX_INLINE UINT CXTControlBar::GetMenuID() {
    /*ASSERT(::IsWindow(m_hWnd));*/ return m_menuID;
}
AFX_INLINE void CXTControlBar::SetMenuID(UINT menuID, int nPos) {
    /*ASSERT(::IsWindow(m_hWnd));*/ m_menuID=menuID; m_nPos=nPos;
}
AFX_INLINE BOOL CXTControlBar::IsVertDocked() const {
    ASSERT(::IsWindow(m_hWnd)); return (IsLeftDocked() || IsRightDocked());
}
AFX_INLINE BOOL CXTControlBar::IsHorzDocked() const {
    ASSERT(::IsWindow(m_hWnd)); return (IsTopDocked() || IsBottomDocked());
}
AFX_INLINE BOOL CXTControlBar::IsBottomDocked() const {
    ASSERT(::IsWindow(m_hWnd)); return (GetParent()->GetDlgCtrlID()==AFX_IDW_DOCKBAR_BOTTOM);
}
AFX_INLINE BOOL CXTControlBar::IsTopDocked() const {
    ASSERT(::IsWindow(m_hWnd)); return (GetParent()->GetDlgCtrlID()==AFX_IDW_DOCKBAR_TOP);
}
AFX_INLINE BOOL CXTControlBar::IsRightDocked() const {
    ASSERT(::IsWindow(m_hWnd)); return (GetParent()->GetDlgCtrlID()==AFX_IDW_DOCKBAR_RIGHT);
}
AFX_INLINE BOOL CXTControlBar::IsLeftDocked() const {
    ASSERT(::IsWindow(m_hWnd)); return (GetParent()->GetDlgCtrlID()==AFX_IDW_DOCKBAR_LEFT);
}
AFX_INLINE BOOL CXTControlBar::IsFloating() const {
    ASSERT(::IsWindow(m_hWnd)); return (!IsHorzDocked() && !IsVertDocked());
}
AFX_INLINE void CXTControlBar::EnableContextMenus(bool bEnable) {
	m_bContextMenus = bEnable;
}
AFX_INLINE bool CXTControlBar::HasContextMenu() {
	if (!xtAfxData.bControlBarMenus) return false; return m_bContextMenus;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTCONTROLBAR_H__)
