//:Ignore
// XTDockWindow.h interface for the CXTDockWindow class.
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

#if !defined(__XTDOCKWINDOW_H__)
#define __XTDOCKWINDOW_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

// forwards
class CXTBarButton;

//////////////////////////////////////////////////////////////////////
// CXTDockWindow is a CXTControlBar derived class.  It is used to create
// sizing / docking windows similar to those seen in Developer Studio.
//
class _XT_EXT_CLASS CXTDockWindow : public CXTControlBar
{
    DECLARE_DYNAMIC(CXTDockWindow)

	friend class CXTBarButton;

public:

    // Constructs a CXTDockWindow object.
    //
    CXTDockWindow();

    // Destroys a CXTDockWindow object, handles cleanup and de-allocation.
    //
    virtual ~CXTDockWindow();

protected:

    // Used to offset the horizontal client area to allow room for the
	// caption when docked.
	int m_cxOffset;
	// Used to determine which button icon to display when the control bar
	// is minimized or maximized depending on which side docked.
	int m_nBtnIndex;
    // CWnd pointer which contains the child window that is displayed in
	// the control bar.  See SetChild(CWnd* pWnd).
	CWnd* m_pChildWnd;
    // Close button used in caption area.
	CXTBarButton* m_pBtnClose;
    // Minimize button used in the caption area.
	CXTBarButton* m_pBtnMinim;
    // Tooltip used for caption buttons.
	CToolTipCtrl m_ToolTip;
    // TRUE if the docking window is currently active.
	BOOL m_bActive;
    // TRUE when the system menu is removed from a floating docking window.
	BOOL m_bMenuRemoved;
    // Points to the parent frame.
	CFrameWnd* m_pParentFrame;
    // Hide button icon.
	CXTIconHandle m_hIconHide;
    // Horizontal max button icon disabled.
	CXTIconHandle m_hIconMaxHDisable;
    // Horizontal max button icon enabled.
	CXTIconHandle m_hIconMaxHEnable;
    // Vert max button icon disabled.
	CXTIconHandle m_hIconMaxVDisable;
    // Vert max button icon enabled.
	CXTIconHandle m_hIconMaxVEnable;
    // Horizontal button icon disabled.
	CXTIconHandle m_hIconHDisable;
    // Horizontal button icon enabled.
	CXTIconHandle m_hIconHEnabled;
    // Vert button icon disabled.
	CXTIconHandle m_hIconVDisable;
    // Vert button icon enabled.
	CXTIconHandle m_hIconVEnabled;

//:Ignore
private:
	// used to validate pending initialization requests
	DWORD m_dwInitSignature;
//:End Ignore
public:

    // Call this member function to modify a control bar style. Styles to 
    // be added or removed can be combined by using the bitwise OR (|) 
    // operator. 
	//
    // The desired styles for the control bar can be one or more of the 
    // following:
    //[ul]
    //[li][b]CBRS_XT_BUTTONS[/b] - The control bar has a minimize and close
	//       button when docked.[/li]
    //[li][b]CBRS_XT_GRIPPER[/b] - The control bar has a gripper when docked.[/li]
    //[li][b]CBRS_XT_GRIPPER_FLAT[/b] - The control bar has a flat gripper.
	//       Use with CBRS_XT_GRIPPER.[/li]
    //[li][b]CBRS_XT_GRIPPER_GRAD[/b] - The control bar has a gradient
	//       flat gripper. Use with CBRS_XT_GRIPPER_FLAT.[/li]
    //[li][b]CBRS_XT_GRIPPER_TEXT[/b] - The control bar draws text in the
	//       gripper area. Use with CBRS_XT_GRIPPER.[/li]
    //[li][b]CBRS_XT_BUTTONS_FLAT[/b] - The control bar min and max buttons
	//       are flat. Use with CBRS_XT_BUTTONS.[/li]
    //[li][b]CBRS_XT_BORDERS_FLAT[/b] - The control bar borders are flat.[/li]
    //[li][b]CBRS_XT_CLIENT_OUTLINE[/b] - Draw a single black border outline
	//       around the client area.[/li]
    //[li][b]CBRS_XT_CLIENT_STATIC[/b] - Draw a static rect around the
	//       client area.[/li]
    //[li][b]CBRS_XT_CLIENT_MODAL[/b] - Draw a modal rect around the client
	//       area.[/li]
    //[li][b]CBRS_XT_ALL_FLAT[/b] - The following styles combined by using
	//       the bitwise OR (|) operator CBRS_XT_GRIPPER_FLAT, CBRS_XT_BUTTONS_FLAT,
	//       and CBRS_XT_BORDERS_FLAT.[/li]
    //[li][b]CBRS_XT_DEFAULT[/b] - The following styles combined by using
	//       the bitwise OR (|) operator CBRS_XT_BUTTONS, CBRS_XT_GRIPPER,
	//       and CBRS_XT_GRIPPER_TEXT.[/li]
    //[/ul]
    // Returns nonzero if style was successfully modified, otherwise returns zero.
    //
    virtual BOOL ModifyXTBarStyle(
        // Specifies CBRS_XT_ styles to be removed during style 
        // modification.
        DWORD dwRemove,
        // Specifies CBRS_XT_ styles to be added during style 
        // modification.
        DWORD dwAdd,
		// TRUE to redraw the control bar.
        BOOL bRedraw=TRUE);

    // This member function creates a CXTDockWindow (a docking window).
	// It also sets the control bar default size, style, ID, and caption text.
    // Returns nonzero if successful, otherwise returns zero.
    //
    virtual BOOL Create(
        // Pointer to the window that is the control bar’s parent.
        CWnd* pParentWnd,
        // The control bar’s window ID.
        UINT nID,
        // Points to a null-terminated character string that 
        // represents the control bar name. Used as text for 
        // the caption.
        LPCTSTR lpszCaption=NULL, 
        // Specifies the default size of the control bar.
        CSize size=CSize(200,100),
        // The control bar style. Additional toolbar styles 
        // supported are: 
        //[ul]
        //[li][b]CBRS_TOP[/b] - Control bar is at the top of the frame window.[/li]
        //[li][b]CBRS_BOTTOM[/b] - Control bar is at the bottom of the frame
		//       window.[/li]
        //[li][b]CBRS_NOALIGN[/b] - Control bar is not repositioned when
		//       the parent is resized.[/li]
        //[li][b]CBRS_TOOLTIPS[/b] - Control bar displays tool tips.[/li]
        //[li][b]CBRS_SIZE_DYNAMIC[/b] - Control bar is dynamic.[/li]
        //[li][b]CBRS_SIZE_FIXED[/b] - Control bar is fixed.[/li]
        //[li][b]CBRS_FLOATING[/b] - Control bar is floating.[/li]
        //[li][b]CBRS_FLYBY[/b] - Status bar displays information about
		//       the button.[/li]
        //[li][b]CBRS_HIDE_INPLACE[/b] - Control bar is not displayed to
		//       the user.[/li]
        //[/ul]
        DWORD dwStyle=CBRS_LEFT,
        // Specifies CBRS_XT_ styles to be used during creation.
        // See ModifyXTBarStyle for information about the available 
        // control bar styles.
        DWORD dwBarStyle=CBRS_XT_DEFAULT);

    // This member function is called by the control bar, and can be overloaded
	// in derived classes to return the rect for the child window associated
	// with the control bar.
    //
    virtual void GetInsideRect(
        // Reference to a CRect object which contains the size of 
        // the child window associated with the control bar.
        CRect &rect);

    // This member function is called to determine the minimum extent of
	// the control bar when minimized or maximized. Returns the minimum extent
	// for a minimized control bar.
    //
    virtual int GetMinExt();

    // This member function is used to determine if the docked control
	// bar is maximized. Returns nonzero if the docked control bar is maximized,
	// otherwise returns zero.
    //
    virtual BOOL IsMaximized() const;

    // This member function is used to determine if the docked control
	// bar is unique. Returns nonzero if the docked control bar is unique, otherwise
	// returns zero.
    //
    virtual BOOL IsUnique() const;

    // This member function is called to set the normal size the for 
    // the control bar.
    //
    virtual void SetNormalSize(
        // A reference to a CSize object that contains the normal
        // size that the control bar should be.
        const CSize &size);

    // This member function is called to initialize the default size 
    // to the current float size.
    //
    virtual void SetFloatSize();

    // This member function is called to return the control bar to 
    // its normal size.
    //
    virtual void Normalize();

    // This member function is called to minimize the control bar. Returns
    // the docking window's minimized size in pixels.
    //
    virtual int Minimize();
    
    // This member function is called to maximize the control bar.
    //
    virtual void Maximize(
		// Indicates the width or height of the bar.
		int size);

    // Call this function to enable a control bar to be docked. The sides 
    // specified must match one of the sides enabled for docking in the 
    // destination frame window, or the control bar cannot be docked to 
    // that frame window.
    //
    virtual void EnableDocking(
        // Specifies whether the control bar supports docking 
        // and the sides of its parent window to which the 
        // control bar can be docked, if supported. It can be one 
        // or more of the following: 
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
		//       be floated in a single mini-frame window. If 0 (that is, indicating
		//       no flags), the control bar will not dock.[/li]
        //[/ul]
        DWORD dwDockStyle);

    // This member function is used to determine the width of the control
	// bar when docked vertically. Returns an int value that represents the
	// current width of the vertically docked control bar.
    //  
    virtual int GetVertSize();

    // This member function is used to determine the width of the control
	// bar when docked horizontally. Returns an int value that represents
	// the current height of the horizontally docked control bar.
    //
    virtual int GetHorzSize();

    // Call this member function to restore the settings of each control
	// bar owned by the frame window. This information is written to the 
	// initialization file using SaveBarSize. Information restored includes
	// visibility, horizontal/vertical orientation, docking state, and control bar
	// position. This function call is handled by CXTFrameWnd::LoadBarState,
	// and should [b]not[/b] be called directly.
    //
    void LoadBarSize(
        // Name of a section in the initialization file or a key 
        // in the Windows registry where state information is stored.
        CString strSection);

    // Call this function to store information about each control bar owned
	// by the frame window. This information can be read from the initialization
	// file using LoadBarSize. Information stored includes visibility, 
    // horizontal/vertical orientation, docking state, and control bar
	// position. This function call is handled by CXTFrameWnd::SaveBarState,
	// and should [b]not[/b] be called directly.
    //
    void SaveBarSize(
        // Name of a section in the initialization file or a key 
        // in the Windows registry where state information is stored.
        CString strSection);

    // This member function overrides CWnd::SetWindowText(...), and sets
	// the control bar’s title to the specified text.
    //
    void SetWindowTextX(
        // Points to a CString object or null-terminated string to 
        // be used as the new title or control text.
        LPCTSTR lpszString);

	// Call this member function to create a view.  You can then call SetChild
    // to add the view to this docking window. Returns a CWnd pointer to
	// the newly created view if successful, otherwise returns NULL.
    //	
    virtual CWnd* CreateView(
        // CView runtime class associated with the tab.
        CRuntimeClass *pViewClass,
        // CDocument associated with the view.
        CDocument *pDocument,
        // Create context for the view.
        CCreateContext *pContext);

    // This member function is used to associate a child window that is to be 
    // displayed in the control bar.
    //
    virtual void SetChild(
        // Points to the child CWnd object that is associated with the control
        // bar. This pointer is used to resize the child window accordingly.
        CWnd* pWnd);

    // This member function is called to initialize the XT_BARPLACEMENT struct
    // which contains size, location, and style information for the control bar.
    //
    virtual void SetBarPlacement(
        // Points to an XT_BARPLACEMENT struct which is used to
        // initialize the placement for the control bar.
        XT_BARPLACEMENT* pBP);

    // This member function is called to calculate the size of the gripper
	// displayed in the caption area of a docked control bar. Returns a CRect
	// value that represents the size and location of the first grip line
	// in the caption area when docked.
    //
    virtual CRect GetGripperRect();

    // This member function is called to calculate the size of the frame
	// buttons used in the caption area of a docked control bar. Returns a
	// CRect value that represents the size and location of the close frame
	// button when docked.
    //
    virtual CRect GetButtonRect();

    // This member function checks to see if the object passed in as 'pObject'
	// is a CXTDockWindow object. Returns TRUE if the object is a CXTDockWindow
	// object, otherwise returns FALSE.
    //
    static BOOL IsDockWindow(
        // Represents a valid CObject pointer.
        CObject* pObject);

	// This member function will set the caption text for the docking window.
	//
	virtual void SetCaption(
		// A NULL terminated string that represents the new caption title.
		LPCTSTR lpszCaption);

protected:

    // This member function is called by the control bar to draw the gripper
	// in the caption area of a docked control bar.
    //  
    virtual void OnDrawGripper(
        // Points to a device context. The function draws the gripper into 
        // this device context. 
        CDC* pDC);

    // This member function is called by the control bar to draw its borders.
    //
    virtual void DrawBorders(
        // Points to a device context. The function draws the borders into 
        // this device context. 
        CDC *pDC,
        // A reference to a CRect object that represents the size of the
        // border to be drawn.
        CRect &rect);

    // This member function is called by the control bar to perform 
    // paint operations.
    //
    virtual void EraseNonClient();

    // This member function is called by the docking window to draw the
	// caption area for the docked bars.
    //          
    virtual void DrawCaption(
        // Pointer to the current device context.
        CDC* pDC,
        // Size of the area to paint.
        const CRect& rect);

    // This member function is called by the docking window to set the 
    // caption bar to an "active" or "inactive" state.
    //          
    virtual void SetActiveState(
        // Set to TRUE to cause the caption bar to appear active.
        BOOL bActive);

	// This member function is called to refresh the caption button display
	// for the docking window.
	//
	virtual void UpdateButtonXStyles();

    //:Ignore
    //{{AFX_VIRTUAL(CXTDockWindow)
    public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
    virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);
    virtual void DoPaint(CDC* pDC);
    //}}AFX_VIRTUAL
    virtual void DelayShow(BOOL bShow);
	virtual void HideShowButtons(int nCmdShow);
	virtual void SetDockWindowStyle(DWORD dwStyle);
	//:End Ignore

// Generated message map functions
protected:

    //:Ignore
    bool HasGripper();
    bool HasButtons();
    bool HasFlatButtons();
    bool HasCaption();
    bool HasGradientCaption();
    bool HasText();
    bool HasTwoGrips();
    bool HasSemiFlatBorder();
    bool HasClientBorderOutline();
    bool HasClientBorderStatic();
    bool HasClientBorderModal();
    //:End Ignore

    //:Ignore
    //{{AFX_MSG(CXTDockWindow)
    afx_msg UINT OnNcHitTest(CPoint point);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
    afx_msg void OnButtonClose();
    afx_msg void OnUpdateButtonClose(CCmdUI* pCmdUI);
    afx_msg void OnButtonMinimize();
    afx_msg void OnUpdateButtonMinimize(CCmdUI* pCmdUI);
    afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	//}}AFX_MSG
    afx_msg LRESULT OnXTInitialUpdate(WPARAM, LPARAM);
	afx_msg void OnInitialUpdate();
    //:End Ignore

    DECLARE_MESSAGE_MAP()

    friend class CXTDockBar;
    friend class CXTFrameWnd;
    friend class CXTSplitterDock;
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTDockWindow::GetVertSize() {
    ASSERT(::IsWindow(m_hWnd)); return m_bp.sizeDefault.cx;
}
AFX_INLINE int CXTDockWindow::GetHorzSize() {
    ASSERT(::IsWindow(m_hWnd)); return m_bp.sizeDefault.cy;
}
AFX_INLINE void CXTDockWindow::SetChild(CWnd *pWnd) {
    ASSERT(::IsWindow(m_hWnd)); m_pChildWnd=pWnd;
}
AFX_INLINE BOOL CXTDockWindow::IsMaximized() const {
    ASSERT(::IsWindow(m_hWnd)); return (m_bp.bMaximized);
}
AFX_INLINE BOOL CXTDockWindow::IsUnique() const {
    ASSERT(::IsWindow(m_hWnd)); return (m_bp.bUnique);
}
AFX_INLINE void CXTDockWindow::SetFloatSize() {
    ASSERT(::IsWindow(m_hWnd)); m_bp.sizeDefault = m_bp.sizeFloat;
}
AFX_INLINE bool CXTDockWindow::HasGripper() {
    return ((m_bp.dwStyle & CBRS_XT_GRIPPER) == CBRS_XT_GRIPPER);
}
AFX_INLINE bool CXTDockWindow::HasButtons() {
    return (HasGripper() && ((m_bp.dwStyle & CBRS_XT_BUTTONS) == CBRS_XT_BUTTONS));
}
AFX_INLINE bool CXTDockWindow::HasFlatButtons() {
    return (HasButtons() && (((m_bp.dwStyle & CBRS_XT_BUTTONS_FLAT) == CBRS_XT_BUTTONS_FLAT) || xtAfxData.bXPMode));
}
AFX_INLINE bool CXTDockWindow::HasCaption() {
    return (HasGripper() && xtAfxData.bXPMode || ((m_bp.dwStyle & CBRS_XT_GRIPPER_FLAT) == CBRS_XT_GRIPPER_FLAT));
}
AFX_INLINE bool CXTDockWindow::HasGradientCaption() {
    return (HasCaption() && ((m_bp.dwStyle & CBRS_XT_GRIPPER_GRAD) == CBRS_XT_GRIPPER_GRAD));
}
AFX_INLINE bool CXTDockWindow::HasText() {
    return (HasGripper() && ((m_bp.dwStyle & CBRS_XT_GRIPPER_TEXT) == CBRS_XT_GRIPPER_TEXT));
}
AFX_INLINE bool CXTDockWindow::HasTwoGrips() {
    return (HasGripper() && ((m_bp.dwStyle & CBRS_XT_TWOGRIP) == CBRS_XT_TWOGRIP));
}
AFX_INLINE bool CXTDockWindow::HasSemiFlatBorder() {
    return ((m_bp.dwStyle & CBRS_XT_SEMIFLAT) == CBRS_XT_SEMIFLAT && !xtAfxData.bXPMode);
}
AFX_INLINE bool CXTDockWindow::HasClientBorderOutline() {
    return ((m_bp.dwStyle & CBRS_XT_CLIENT_OUTLINE) == CBRS_XT_CLIENT_OUTLINE);
}
AFX_INLINE bool CXTDockWindow::HasClientBorderStatic() {
    return ((m_bp.dwStyle & CBRS_XT_CLIENT_STATIC) == CBRS_XT_CLIENT_STATIC);
}
AFX_INLINE bool CXTDockWindow::HasClientBorderModal() {
    return ((m_bp.dwStyle & CBRS_XT_CLIENT_MODAL) == CBRS_XT_CLIENT_MODAL);
}
AFX_INLINE BOOL CXTDockWindow::IsDockWindow(CObject* pObject) {
    return pObject->IsKindOf(RUNTIME_CLASS(CXTDockWindow));
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTDOCKWINDOW_H__)
