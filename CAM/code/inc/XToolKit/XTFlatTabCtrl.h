//:Ignore
// XTFlatTabCtrl.h interface for the CXTFlatTabCtrl class.
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

#if !defined(__XTFLATTABCTRL_H__)
#define __XTFLATTABCTRL_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTFlatTabCtrl is a CWnd derived class.  It is used to create an Excel
// style sheet control. This control allows you to define if you want the
// control to have home, end, back, and next buttons.
//
class _XT_EXT_CLASS CXTFlatTabCtrl : public CWnd
{
	DECLARE_DYNAMIC(CXTFlatTabCtrl)

public:

	// Constructs a CXTFlatTabCtrl object.
	//
	CXTFlatTabCtrl();

	// Destroys a CXTFlatTabCtrl object, handles cleanup and de-allocation.
	//
	virtual ~CXTFlatTabCtrl();

protected:

	// RGB value that represents the tab shadow color.
	COLORREF m_clr3DShadow;
	// RGB value that represents the tab outline color.
	COLORREF m_clrBtnText;
	// RGB value that represents the tab highlight color.
	COLORREF m_clr3DHilight;
	// RGB value that represents the selected tab face color.
	COLORREF m_clrWindow;
	// RGB value that represents the normal tab face color.
	COLORREF m_clr3DFace;
	// RGB value that represents the tab text color.
	COLORREF m_clrWindowText;
	// Tooltip for the flat tab control.
	CToolTipCtrl m_ToolTip;
	// Tab control style.
	DWORD m_dwStyle;
	// 'true' if the control is managing views.
	bool m_bManagingViews;
	// 'true' if user defined colors are used.
	bool m_bUserColors;
	// Width for each arrow button.
	int m_cx;
	// Height for each arrow button.
	int m_cy;
	// Area occupied by tabs.
	CRect m_rectTabs;
	// Area occupied by managed views.
	CRect m_rectViews;
	// Index of the currently selected tab.
	int m_nCurSel;
	// Width, in pixels, of the tab control client area.
	int m_nClientWidth;
	// Height, in pixels, of the tab control client area.
	int m_nClientHeight;
	// Amount, in pixels, of the displayed tab offset.
	int m_nOffset;
	// Font that is used for non-selected tabs.
	CFont* m_pNormFont;
	// Font that is used for selected tabs.
	CFont* m_pBoldFont;
	// The horizontal scroll bar (used with FTS_XT_HSCROLL).
	CScrollBar m_ScrollBar_H;
	// Area occupied by the horizontal scroll bar.
	CRect m_rectSB_H;
	// Area occupied by the sizing gripper.
	CRect m_rectGripper;
	// The current gripper position, in pixels, from the left.
	int m_xGripperPos;
	// The current gripper position in percent of the control width.
	int m_iGripperPosPerCent;

	//:Ignore
	// pens used by painting code
	CPen m_penShadow;
	CPen m_penBackSel;
	CPen m_penBackNonSel;
	CPen m_penOutline;
	CPen m_penWindow;
	//:End Ignore

	// Template list containing tab information.
	CArray <XT_TCB_ITEM*, XT_TCB_ITEM*> m_tcbItems;

	//:Ignore
	// tracking related variables
	bool m_bTracking;
	int m_xTrackingDelta;
	CWnd * m_pWndLastFocus;
	//:End Ignore

	//:Ignore
	// Enumerate type that specifies which arrow to display for a particular button.
	//
	enum icon_type
	{
		// Left arrow display.
		arrow_left          =   0x0200,
		// Left home arrow display.
		arrow_left_home     =   0x0201,
		// Right arrow display.
		arrow_right         =   0x0000,
		// Right home arrow display.
		arrow_right_home    =   0x0001
	};

	// internal structures/variables used to control button information
	class CXTFTButtonState
	{
	public:
		CXTFTButtonState();

		void SetInfo(CRect rect, int iCommand, icon_type iconType);

		CRect m_rect;
		bool  m_bPressed;
		bool  m_bEnabled;
		int   m_iCommand;
		icon_type m_IconType;
	};

	// Array of button information.
	CXTFTButtonState m_buttons[4];     
	// Index of "left" button in button array.
	int m_iBtnLeft;
	// Index of "right" button in button array.
	int m_iBtnRight;
	// Index of "home" button in button array.
	int m_iBtnHome;
	// Index of "end" button in button array.
	int m_iBtnEnd;
	//:End Ignore

public:

	// This member function gets the text of a specific tab.  Returns the
	// text of a particular tab, or NULL if an error occurs.
	//
	LPCTSTR GetItemText(
		// The index of the tab whose text is to be retrieved.
		int nIndex
		) const;

	// This member function will set the text of a particular tab.  Returns
	// 'true' when successful.
	//
	bool SetItemText(
		// The index of the tab whose text is to be changed.
		int nIndex,
		// The new title for the tab.
		LPCTSTR pszText
		);

	// This member function gets a CWnd pointer to the window that is associated
	// with a specific tab.  It returns a pointer to the window that is associated
	// with a tab, or it returns NULL if no window is associated with (managed
	// by) the tab.
	//
	CWnd *GetItemWindow(
		// The index of the tab whose managed window is to be retrieved.
		int nIndex
		) const;

	// This member function will set the fonts to be used by the tab control.
	//
	virtual void SetTabFonts(
		// Represents the font used by non-selected tabs.
		CFont* pNormFont,
		// Represents the font used by selected tabs.
		CFont* pBoldFont);

	// This member function will insert a tab into the flat tab control. Returns
	// the index of the tab that has been inserted if successful, otherwise
	// returns -1.
	//
	virtual int InsertItem(
		// Index of the tab to insert.
		int nItem,
		// String resource ID of the tab label.
		UINT nTextID,
		// Optional pointer to the managed control.
		CWnd *pWndControl = NULL
		);

	// This member function will insert a tab into the flat tab control. Returns
	// TRUE if successful, otherwise returns FALSE.
	//
	virtual BOOL InsertItem(
		// Index of the tab to insert.
		int nItem,
		// NULL terminated string that represents the tab label.
		LPCTSTR lpszItem,
		// Optional pointer to the managed control.
		CWnd *pWndControl = NULL
		);

	// This member function will delete the tab specified by 'nItem' from the
	// tab control. Returns TRUE if successful, otherwise returns FALSE.
	//
	virtual BOOL DeleteItem(
		// Index of the tab to delete.
		int nItem);

	// This member function will remove all of the tabs from the tab control.
	// Returns TRUE if successful, otherwise returns FALSE.
	//
	virtual BOOL DeleteAllItems();

	// This member function will retrieve the size of the tab specified by
	// 'nItem'. Returns TRUE if successful, otherwise returns FALSE.
	//
	virtual BOOL GetItemRect(
		// Index of the tab to retrieve the size of.
		int nItem,
		// Points to a RECT structure to receive the size of the tab.
		LPRECT lpRect);

	// Call this function to determine which tab, if any, is at the specified
	// screen position. Returns the zero-based index of the tab, or returns
	// -1 if no tab is at the specified position.
	//
	virtual int HitTest(
		// Pointer to a TCHITTESTINFO structure, as 
		// described in the Platform SDK, which specifies 
		// the screen position to test.
		TCHITTESTINFO *pHitTestInfo) const;

	// Call this function to retrieve the currently selected tab in a flat
	// tab control. Returns a zero-based index of the selected tab if successful,
	// or returns -1 if no tab is selected.
	//
	virtual int GetCurSel() const;

	// This member function selects a tab in a flat tab control. Returns a
	// zero-based index of the previously selected tab if successful, otherwise
	// returns -1.
	//
	virtual int SetCurSel(
		// The zero-based index of the item to be selected.
		int nItem);

	// Call this function to retrieve the number of tabs in the tab control.
	// Returns the number of items in the tab control.
	//
	virtual int GetItemCount() const;

	// This member function will set the tooltip for the tab specified
	// by 'nItem'.
	//
	virtual void SetTipText(
		// The zero-based index of tab to receive the tooltip text.
		int nItem,
		// A pointer to a string containing the tooltip text.
		LPCTSTR lpszTabTip);

	// This member function will get the tooltip text associated with the
	// tab specified by 'nItem'. Returns a CString object containing the text
	// to be used in the tooltip.
	//
	virtual CString GetTipText(
		// The zero-based index of the tab to retrieve the tooltip 
		// text for.
		int nItem);

	// This member function will cause the tab control to reposition
	// the tabs to the home position.
	//
	virtual void Home();

	// This member function changes the location of the sizing gripper.
	// The function has no effect if the FTS_XT_HSCROLL is not used.
	//
	void SetGripperPosition(
		// The position for the gripper, relative to the left-hand-side of
		// the control.
		int x,
		// Indicates that the position is a percent of the control width,
		// as opposed to an absolute location in pixels.
		bool bPercent);

	// This member function gets the location of the sizing gripper, in pixels,
	// relative to the left hand side of the control. Returns an integer value
	// representing the location of the sizing gripper, in pixels, relative
	// to the left hand side of the control.
	//
	int GetGripperPosition() const;

	// Call this function to synchronize the tab control's horizontal scroll
	// bar with the horizontal scroll bar of the current view.
	// 
	// You should call this function if anything happens in the view that
	// affects the horizontal scroll bar (e.g. a user typing text into an
	// edit control could make the text wider, thus requiring a call to this
	// function).
	//
	void SyncScrollBar();
	
	// This member function is called to set the shadow color for all tabs.
	//
	void SetTabShadowColor(
		// RGB value that represents the tab shadow color.
		COLORREF crShadow);
	
	// This member function gets an RGB value that represents the shadow color
	// of the tab. Returns an RGB value that represents the tab shadow color.
	//
	COLORREF GetTabShadowColor() const;
	
	// This member function is called to set the highlight color for all tabs.
	//
	void SetTabHilightColor(
		// RGB value that represents the tab highlight color.
		COLORREF crHilight);
	
	// This member function gets an RGB value that represents the highlight
	// color of the tab. Returns an RGB value that represents the tab highlight color.
	//
	COLORREF GetTabHilightColor() const;
	
	// This member function is called to set the background color for normal tabs.
	//
	void SetTabBackColor(
		// RGB value that represents the tab background color.
		COLORREF crBack);
	
	// This member function gets an RGB value that represents the background
	// color of the tab. Returns an RGB value that represents the tab background color.
	//
	COLORREF GetTabBackColor() const;
	
	// This member function is called to set the text color for normal tabs.
	//
	void SetTabTextColor(
		// RGB value that represents the tab text color.
		COLORREF crText);
	
	// This member function gets an RGB value that represents the text color
	// of the tab. Returns an RGB value that represents the tab text color.
	//
	COLORREF GetTabTextColor() const;
	
	// This member function is called to set the background color for selected tabs.
	//
	void SetSelTabBackColor(
		// RGB value that represents the selected tab background color.
		COLORREF crBack);
	
	// This member function gets an RGB value that represents the background
	// color for selected tabs. Returns an RGB value that represents the
	// selected tabs background color.
	//
	COLORREF GetSelTabBackColor() const;
	
	// This member function is called to set the text color for selected tabs.
	//
	void SetSelTabTextColor(
		// RGB value that represents the selected tab text color.
		COLORREF crText);
	
	// This member function gets an RGB value that represents the text color
	// for selected tabs. Returns an RGB value that represents the selected
	// tab text color.
	//
	COLORREF GetSelTabTextColor() const;

	// Call this member function to reset the tab control to use default system colors.
	//
	void UpdateDefaultColors();

	//:Ignore
	//{{AFX_VIRTUAL(CXTFlatTabCtrl)
	virtual BOOL Create(DWORD dwStyle, const CRect& rect, CWnd* pParentWnd, UINT nID);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnNotify(WPARAM, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL
	//:End Ignore

protected:

	// Call this function to determine which button, if any, is at the specified
	// screen position. Returns the zero-based index of the button within
	// the button's array or -1 if no button is at the specified position.
	//
	virtual int ButtonHitTest(
		// A CPoint reference representing the specified screen position.
		CPoint& pt) const;

	// This member function will get the width, in pixels, of the tab specified
	// by 'nItem'. Returns the width, in pixels, of the tab.
	//
	int GetTabWidth(
		// The zero-based index of the tab to retrieve the width for.
		int nItem) const;

	// This member function will return the total width of all of the tabs
	// in the flat tab control. Returns the total combined width, in pixels,
	// of all the tabs in the control.
	//
	int GetTotalTabWidth() const;

	// This member function will return the total width of all of the arrow
	// buttons that are visible in the flat tab control. Returns the total
	// width, in pixels, of all the visible arrow buttons.
	//
	int GetTotalArrowWidth() const;

	// This member function will return the total area width of all of the
	// tabs in the flat tab control. Returns the total area width, in pixels,
	// of all the tabs in the flat tab control.
	//
	int GetTotalTabAreaWidth() const;

	// This member function will draw a tab to the device context specified
	// by 'pDC'. Returns the x position of the next tab to be drawn.
	//
	int DrawTab(
		// Points to the device context to draw the tab to.
		CDC* pDC,
		// XY location of the top left corner of the tab to draw.
		const CPoint& pt,
		// 'true' if the tab is currently selected.
		bool bSelected,
		// A NULL terminated string that represents the tab label.
		LPCTSTR lpszTabLabel);

	// This member function is used by the flat tab control to draw an arrow 
	// button to the device context specified by 'pDC'.
	//
	void DrawButton(
		// Points to the device context to draw the tab to.
		CDC* pDC,
		// XY location of the top left corner of the tab to draw.
		CXTFTButtonState& button_state
		) const;

	// This member function will force all of the tabs to be repainted.
	//
	void InvalidateTabs();

	// This member function will enable or disable the arrow buttons
	// depending on the current tab display state.
	//
	void EnableButtons();

	// This member function will free the resources allocated for the
	// icons used by the arrow buttons.
	//
	void FreeButtonIcons();

	// This member function will create the icon resources that are
	// used by the arrow buttons.
	//
	void CreateButtonIcons();

	// This member function will draw the horizontal sizing gripper at a 
	// specified location.
	//
	void DrawGripper(
		// Points to the device context to draw the gripper to.
		CDC* pDC,
		// Location of the gripper.
		CRect rect
		) const;

	// The member function is used internally to toggle the state of the
	// sizing-grip tracking mode.
	//
	void SetTracking(
		// 'true' to enable tracking, or 'false' to disable tracking.
		bool bTracking);

	// This member function frees all memory occupied by the tab items.
	// 
	void ClearAllItems();

	// This member function is called when the tab control is resized.  It 
	// is responsible for updating internal structures which are dependant
	// on the control's size.
	//
	void RecalcLayout();

	// This internal function deletes an item from the tab item list. Returns
	// TRUE if successful, otherwise returns FALSE.
	// 
	virtual BOOL _DeleteItem(
		// Index of the tab to delete.
		int nItem);

	// This internal function calculates the overlap between two tabs. Returns
	// an integer value that represents the overlap between the tabs.
	//
	virtual int GetOverlap() const;

	//:Ignore
	//{{AFX_MSG(CXTFlatTabCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLeftArrow();
	afx_msg void OnRightArrow();
	afx_msg void OnHomeArrow();
	afx_msg void OnEndArrow();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	//:End Ignore
	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTFlatTabCtrl::InsertItem(int nItem, UINT nTextID, CWnd *pWndControl) {
	ASSERT(IsWindow(m_hWnd)); CString strItem; strItem.LoadString(nTextID); return InsertItem(nItem, strItem, pWndControl);
}
AFX_INLINE int CXTFlatTabCtrl::GetCurSel() const {
	ASSERT(IsWindow(m_hWnd)); return m_nCurSel;
}
AFX_INLINE int CXTFlatTabCtrl::GetItemCount() const {
	ASSERT(IsWindow(m_hWnd)); int iItemCount = (int)m_tcbItems.GetSize(); return iItemCount;
}
AFX_INLINE void CXTFlatTabCtrl::SetTabFonts(CFont* pNormFont, CFont* pBoldFont) {
	if (pNormFont){ m_pNormFont = pNormFont; } if (pBoldFont){ m_pBoldFont = pBoldFont; }
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTFLATTABCTRL_H__)
