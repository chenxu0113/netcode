//:Ignore
// XTDockBar.h interface for the CXTDockBar class.
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

#if !defined(__XTDOCKBAR_H__)
#define __XTDOCKBAR_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

// class forwards.
class CXTDockWindow;

//////////////////////////////////////////////////////////////////////
// CXTDockBar is a CDockBar derived class.  It is used by CXTFrameWnd and
// CXTMDIFrameWnd to offset CXTToolBars to allow 3D borders.
//
class _XT_EXT_CLASS CXTDockBar : public CDockBar
{
	DECLARE_DYNAMIC(CXTDockBar)

public:

    // Constructs a CXTDockBar object.
	//
	CXTDockBar();

	// Destroys a CXTDockBar object, handles cleanup and de-allocation.
	//
    virtual ~CXTDockBar();

protected:

	// Splitter used to drag.
	int m_nTrackSplitter;
	// TRUE when tracking.
	BOOL m_bTracking;
	// Previous focus window when tracking.
	HWND m_hWndFocus;
	// FLAT styles.
	DWORD m_dwXTStyle;

	// Array of CXTSplitterDock objects.
	CArray <CXTSplitterDock *, CXTSplitterDock *> m_arrSplitters;	

public:

	// This member function is called to determine if the docking window specified
	// by 'nBar' is maximized. Returns a pointer to a valid CXTDockWindow object.
	//
	CXTDockWindow* IsRowMaximized(
		// Index of the bar in the control bar array.
		int nBar);

	// This member function is called to dock the control bar specified by 'pBar'.
	//
	void DockControlBar(
		// A pointer to a valid CControlBar object.
		CControlBar* pBar,
		// Initial size of the docked bar.
		LPCRECT lpRect);

	// This member function is called to maximize the control bar specified
	// by 'pMaxBar'.
	//
	void Maximize(
		// A pointer to a valid CXTDockWindow object.
		CXTDockWindow* pMaxBar,
		// TRUE if the parent frame needs to recalculate the layout.
		BOOL bRecalcLayout = TRUE);

	// This member function is called to normalize the control bar specified
	// by 'pMaxBar'.
	//
	void Normalize(
		// A pointer to a valid CXTDockWindow object.
		CXTDockWindow* pMaxBar,
		// TRUE if the parent frame needs to recalculate the layout.
		BOOL bRecalcLayout = TRUE);

	// Call this member function to determine which CBRS_XT_ (control bar) 
	// settings are currently set for the control bar. It does not handle
	// WS_ (window style) or CBRS_ (control bar style). Returns the current
	// CBRS_XT_ (Control bar) settings for the control bar. See 
	// CXTDockWindow::ModifyXTBarStyle for the complete list of available
	// styles.
	// 	
	DWORD GetXTBarStyle();

	// This member function will set the style for the control bar. It does not 
	// handle WS_ (window style) or CBRS_ (control bar style).
	// 		
	void SetXTBarStyle(
		// New CBRS_XT_ style for the control bar.  See CXTDockWindow::ModifyXTBarStyle
		// for the complete list of available styles.
        DWORD dwNewStyle);

	//:Ignore
    //{{AFX_VIRTUAL(CXTDockBar)
	public:
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual void HitTest(const CPoint& point);
	virtual void DoPaint(CDC* pDC);
	//}}AFX_VIRTUAL
#if _MSC_VER < 1200 // MFC 5.0
	virtual void DrawGripper(CDC* pDC, const CRect& rect);
#endif //_MSC_VER < 1200
    //:End Ignore

protected:

	// This member function is called to remove the row borders for the row
	// of the control bar specified by 'nInitPos'.
	//
	void RemoveRowBorders(
		// Initial position of the control bar whose row is to have its borders
		// removed.
		int nInitPos,
		// Style for the row can be any CBRS_BORDER style.  See 
		// CControlBar::SetBarStyle for more details.
		DWORD dwBarStyle = CBRS_BORDER_ANY,
		// TRUE if the control bar is to be only one sized.
		BOOL bOnlySized = FALSE);

	// This member function is called to set the height for the row of the
	// control bar specified by 'nInitPos'. Returns the amount, in pixels, the
	// row was adjusted.
	//
	int SetRowHeight(
		// Initial position of the bar in the control bar array.
		int nInitPos,
		// Size, in pixels, of the new row height.
		int nNewHeight);

	// This member function is called to stretch the row of control bars to
	// the left. Returns the amount, in pixels, the row was adjusted.
	//
	int StretchRowLeft(
		// Initial position of the bar in the control bar array.
		int nInitPos,
		// Amount, in pixels, to increment the width by.
		int nIncWidth);

	// This member function is called to stretch the row of control bars to
	// the right. Returns the amount, in pixels, the row was adjusted.
	//
	int StretchRowRight(
		// Initial position of the bar in the control bar array.
		int nInitPos,
		// Amount, in pixels, to increment the width by.
		int nIncWidth);

	// This member function gets the number of control bars docked on the
	// row that the control bar, specified by 'nInitPos', resides. Returns the
	// number of control bars docked on the row specified.
	//
	int GetRowSizeBars(
		// Initial position of the bar in the control bar array.
		int nInitPos,
		// A pointer to a valid CControlBar object.
		CControlBar *pNewBar = NULL);

	// This member function gets the width of the fixed bar including the
	// splitter sizes. Returns the width of the fixed bar including the splitters.
	//
	int GetRowFixedWidth(
		// Initial position of the bar in the control bar array.
		int nInitPos,
		// A pointer to a valid CControlBar object.
		CControlBar *pNewBar = NULL);

	// This member function gets the minimum width of the row for the control
	// bar specified by 'nInitPos'. Returns the minimum width of the row specified.
	//
	int GetRowMinSizedWidth(
		// Initial position of the bar in the control bar array.
		int nInitPos,
		// A pointer to a valid CControlBar object.
		CControlBar *pNewBar = NULL);

	// This member function gets the normal width of the row for the control
	// bar specified by 'nInitPos'. Returns the normal width of the row specified.
	//
	int GetRowSizedWidth(
		// Initial position of the bar in the control bar array.
		int nInitPos,
		// A pointer to a valid CControlBar object.
		CControlBar *pNewBar = NULL);

	// This member function gets the minimum height of the row for the
	// control bar specified by 'nInitPos'. Returns the minimum height of the
	// row specified.
	//
	int GetRowMaxHeight(
		// Initial position of the bar in the control bar array.
		int nInitPos,
		// A pointer to a valid CControlBar object.
		CControlBar *pNewBar = NULL);

	// This member function gets the minimum height of the row for the
	// control bar specified by 'nInitPos'. Returns the minimum height of the
	// row specified.
	//
	int GetRowMinHeight(
		// Initial position of the bar in the control bar array.
		int nInitPos,
		// A pointer to a valid CControlBar object.
		CControlBar *pNewBar = NULL);

	// This member function gets the available height for the dockbar. Returns
	// the dockbar's available height.
	//
	int GetAvailableHeight();

	// This member function gets a pointer to the first control bar in
	// the row. Returns an int value that represents first control bar in
	// the row.
	//
	int GetFirstBarInRow(
		// A pointer to a valid CControlBar object.
		CControlBar *pBar);

	// This member function is called to adjust the row sizes of the row for
	// the control bar specified by 'nInitPos'.
	//
	void AdjustRowSizes(
		// Initial position of the bar in the control bar array.
		int nInitPos);

	// This member function is called to draw the splitter bars that separate
	// the docked control bars.
	//
	void DrawSplitters(
		// A CDC pointer to a valid device context.
		CDC *dc);

	// This member function is called to set the length of the splitters for
	// the docked control bars.
	//
	void SetSplittersLength(
		// Starting position of the bar in the control bar array.
		int nStart,
		// Type of splitter bar; either XT_SPLITTER_VERT or XT_SPLITTER_HORZ.
		int nType,
		// Initial size of the splitter.
		int nLength);

	// This member function is called to free resources allocated for dock
	// splitters.
	//
	void DeleteSplitters();

	// This member function is called to add a splitter to the dockbar.
	//
	void AddSplitter(
		// A CRect object.
		CRect rcSplitter,
		// Type of splitter bar; either XT_SPLITTER_VERT or XT_SPLITTER_HORZ.
		int nType,
		// TRUE if the splitter is an interior row.
		BOOL bInterRow,
		// Initial position of the splitter bar.
		int nPos);

	// This member function is called to return a pointer to the bar specified
	// by 'nInitPos'. Returns a pointer to a valid CControlBar object.
	//
	CControlBar *FindNewBar(
		// Initial position of the bar in the control bar array.
		int nInitPos);

	// This member function is called to return a pointer to the control bar
	// specified by 'nInitPos'. 'nInitPos' is the index of the bar in the control
	// bar array. Returns a pointer to a valid CXTDockWindow object.
	//
	CXTDockWindow *FindUniqueBar(
		// Initial position of the bar in the control bar array.
		int nInitPos);

	// This member function is called during sizing operations to cancel the
	// drag event.
	//
	void OnCancel();

	// This member function is called to get a valid handle for the control
	// bar specified by 'pBar'. Returns a pointer to a valid CXTDockWindow object,
	// otherwise returns NULL.
	//
	CXTDockWindow* GetSafeDockWindow(
		// A pointer to a valid CControlBar object.
		CControlBar* pBar);

    // This member function is called to determine if the control bar specified
	// by 'nPos' is the last control bar in the row.  Returns 'true' if the
	// bar is the last in the row, otherwise returns 'false'.
	//
	bool IsLastBarInRow(
		// Initial position of the bar in the control bar array.
		int nPos);

	// This member function is called during paint operations to draw the
	// non-client areas of the dockbar.
	//
	void DrawNcBorders(
		// A CDC pointer to a valid device context.
		CDC* pDC,
		// A reference to a valid CRect object.
		CRect& rect);

	//:Ignore
    //{{AFX_MSG(CXTDockBar)
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcPaint();
	//}}AFX_MSG
    //:End Ignore
	DECLARE_MESSAGE_MAP()

	friend class CXTDockWindow;
	friend class CXTSplitterWnd;
	friend class CXTSplitterDock;
	friend class CXTSplitterRowDock;
	friend class CXTMiniDockFrameWnd;
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTDockBar::SetXTBarStyle(DWORD dwNewStyle) {
	m_dwXTStyle = dwNewStyle;
}
AFX_INLINE DWORD CXTDockBar::GetXTBarStyle() {
	ASSERT(::IsWindow(m_hWnd));	return m_dwXTStyle;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTDOCKBAR_H__)

