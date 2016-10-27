//:Ignore
// XTSplitterWnd.h : header file
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

#if !defined(__XTSPLITTERWND_H__)
#define __XTSPLITTERWND_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTSplitterWnd is a CSplitterWnd derived class.  It adds the ability
// to hide and show splitter panes, based upon its index.
//
class _XT_EXT_CLASS CXTSplitterWnd : public CSplitterWnd
{
    DECLARE_DYNAMIC(CXTSplitterWnd)

public:

    // Constructs a CXTSplitterWnd object.
    //
    CXTSplitterWnd();

    // Destroys a CXTSplitterWnd object, handles cleanup and de-allocation.
    //
    virtual ~CXTSplitterWnd();

protected:

    // Index of the hidden column.
	int m_nHiddenCol;
    // Index of the hidden row.
	int m_nHiddenRow;
	// The style of the splitter window.  See SetSplitterStyle(...).
	DWORD m_dwxStyle;
	// TRUE if full window dragging is enabled.
	BOOL m_bFullDrag;
	// Previous cursor position.
	CPoint m_point;
	// TRUE if the flat splitter style is used.
	BOOL m_bFlatSplit;

public:

    // This member function is called to show the column that was previously
	// hidden.
    //
    virtual void ShowColumn();

    // This member function will hide a column based upon its index.
    //
    virtual void HideColumn(
		// Index of the column to hide.
        int nColHide);

    // This member function is called to show the row that was previously
	// hidden.
    //
    virtual void ShowRow();

    // This member function will hide a row based upon its index.
    //
    virtual void HideRow(
		// Index of the row to hide.
        int nRowHide);

    // This member function is called to switch, or swap, a splitter view
	// with another. Returns TRUE if successful, otherwise returns FALSE.
    //
    virtual BOOL SwitchView(
		// Specifies a row.
        int nRow,
		// Specifies a column.
        int nCol,
		// Specifies the view to switch the specified pane with.
        CView *pNewView);

    // This member function is called to replace an existing splitter view
	// with another. Returns a CView pointer to the view that was replaced,
	// otherwise returns NULL.
    //
    virtual CView* ReplaceView(
		// Specifies a row.
        int nRow,
		// Specifies a column.
        int nCol,
		// Specifies the runtime class to replace the specified pane with.
        CRuntimeClass *pViewClass);

    // This member function is called to replace an existing splitter view
	// with another. Returns a CView pointer to the view that was replaced,
	// otherwise returns NULL.
    //
    virtual CView* ReplaceView(
		// Specifies a row.
        int nRow,
		// Specifies a column.
        int nCol,
		// Points to an already existing view.
        CView* pNewView);

    // Call this member function to modify a splitter window's style. Styles
	// to be added or removed can be combined by using the bitwise OR (|)
	// operator.
    //
    virtual void SetSplitterStyle(
		// Specifies XT_SPLIT_ styles to be added during style modification.
		// The desired styles for the control bar can be one or more of the 
        // following:
        //[ul]
    	//[li][b]XT_SPLIT_DOTTRACKER[/b] - The splitter window will use a
		//       dotted tracker rather than the splitter default.[/li]
    	//[li][b]XT_SPLIT_NOFULLDRAG[/b] - Disable the "Show window contents while
		//       dragging" option, even if it is set in Windows.[/li]
    	//[li][b]XT_SPLIT_NOBORDER[/b] - The splitter window will not draw a
		//       border around the pane.[/li]
    	//[li][b]XT_SPLIT_NOSIZE[/b] - Do not allow splitter window panes to
		//       be resized.[/li]
        //[/ul]
        DWORD dwxStyle);

    // Call this member function to return the current style for the splitter
	// window. Returns the current style of the splitter window.  See 
	// SetSplitterStyle(...) for more details.
    //
    virtual DWORD GetSplitterStyle();

    // Call this member function to enable or disable flat splitters.
    //
    virtual void EnableFlatLook(
		// TRUE to enable flat splitters.
        BOOL bFlatSplitter);

    //:Ignore
    //{{AFX_VIRTUAL(CXTSplitterWnd)
    protected:
    virtual void SetSplitCursor(int ht);
    virtual void OnInvertTracker(const CRect& rect);
    virtual void StartTracking(int ht);
    virtual void StopTracking(BOOL bAccept);
    virtual void DrawTracker(const CRect& rect, CBrush* pBrush);
    virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg);
    //}}AFX_VIRTUAL
    //:End Ignore

protected:
    //:Ignore
    //{{AFX_MSG(CXTSplitterWnd)
    afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
    //:End Ignore
    DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE DWORD CXTSplitterWnd::GetSplitterStyle() {
    return m_dwxStyle;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTSplitterWndEx is a CXTSplitterWnd derived class.  It can be used
// with the CXTOutBarCtrl to display a gap between the top of the splitter
// and the toolbar area.
//
class _XT_EXT_CLASS CXTSplitterWndEx : public CXTSplitterWnd  
{
    DECLARE_DYNAMIC(CXTSplitterWndEx)

public:
    
    // Constructs a CXTSplitterWndEx object.
    //
    CXTSplitterWndEx();

    // Destroys a CXTSplitterWndEx object, handles cleanup and de-allocation.
    //
    virtual ~CXTSplitterWndEx();
    
protected:

    // Size, in pixels, of the top border.
	int m_cyTopBorderGap;
	// 'true' to draw a top border line.
	bool m_bShowTopBorder;

public:

    // Call this member function to show a top border for the splitter
	// window, similar to Outlook.  Enabled by default.
    //
    virtual void ShowTopBorder(
		// If TRUE, a white edge will be drawn along the top.
        bool bShowTopBorder=true,
		// Amount, in pixels, to offset the splitter edge.
        int cyTopBorderGap=7);

    //:Ignore
    //{{AFX_VIRTUAL(CXTSplitterWndEx)
    //}}AFX_VIRTUAL
	protected:
    virtual void GetInsideRect(CRect& rect) const;
	public:
 	virtual void RecalcLayout();
    virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg);
    //:End Ignore
    
    //:Ignore
    //{{AFX_MSG(CXTSplitterWndEx)
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    //}}AFX_MSG
    //:End Ignore
    
    DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTSplitterWndEx::ShowTopBorder(bool bShowTopBorder, int cyTopBorderGap) {
    m_bShowTopBorder = bShowTopBorder; m_cyTopBorderGap = cyTopBorderGap;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__XTSPLITTERWND_H__)
