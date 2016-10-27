//:Ignore
// XTFlatHeaderCtrl.h interface for the CXTFlatHeaderCtrl class.
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

#if !defined(__XTFLATHEADERCTRL_H__)
#define __XTFLATHEADERCTRL_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTHeaderCtrl is a CHeaderCtrl derived class.  It is used to create
// a CXTHeaderCtrl window similar to CHeaderCtrl, but with additional functionality. 
// 
// A “header control” is a window usually positioned above columns of text
// or numbers. It contains a title for each column, and it can be divided
// into parts. The user can drag the dividers that separate the parts to
// set the width of each column.
//
// Use a header control, represented by class CXTHeaderCtrl, to display
// column headers for a columnar list.  For example, a header control would
// be useful for implementing column controls in a spreadsheet.
// 
// The header control is usually divided into parts, called "header items,"
// each bearing a title for the associated column of text or numbers. Depending
// on the styles you set, you can provide a number of direct ways for users
// to manipulate the header items.
//
class _XT_EXT_CLASS CXTHeaderCtrl : public CHeaderCtrl
{
    DECLARE_DYNAMIC(CXTHeaderCtrl)

public:

    // Constructs a CXTHeaderCtrl object.
    //
	CXTHeaderCtrl();

    // Destroys a CXTHeaderCtrl object, handles cleanup and de-allocation.
    //
    virtual ~CXTHeaderCtrl();

protected:

	// Minimum column size for an auto-size header control.
	int m_iMinSize;
	// Used internally to determine if text is right-to-left or left-to-right
	// (depends on system locale).
	bool m_bRTL;
	// 'true' if the header control columns are auto-sizing.
	bool m_bAutoSize;
	// List of columns that are not sizable.
	CUIntArray m_arFrozenCols;

public:

    // Call this member function to enable auto-sizing for the header control.
	// This will cause the columns in the list control to be sized to fit
	// in the available space when the list control is resized.
    //
	void EnableAutoSize(
		// 'true' to enable an auto-sizing header control.
        bool bEnable=true);

    // Call this member function to freeze a column in the header control.
	// Freezing a column will disable sizing for the column.
    //
	void FreezeColumn(
		// Index of the column to freeze.
        int iCol);

	// Call this member function to thaw a column in the header control.
	// Thawing a column will enable sizing for the column if it was previously
	// frozen.
	//
	void ThawColumn(
		// Index of the column to thaw.
		int iCol);

    // Call this member function to determine if the specified column is
	// frozen.  Returns 'true' if the column is frozen, otherwise returns
	// 'false.'
    //
	bool IsColFrozen(
		// Index of the column to check.
        int iCol);

    // Call this member to set the minimum size for auto-sizing columns.
	// The minimum size represents the smallest size that all columns can
	// be sized to.
    //
	void SetMinSize(
		// Minimum column size.
        int iMinSize);

protected:

    // This member function is used by the header control to determine
	// the text alignment for the system locale.  Returns 'true' if the text
	// alignment is right-to-left, and 'false' if the text alignment is left-to-right.
    //
	virtual bool DetermineRTL();

    // This member function is used by the header control to auto-size the
	// columns for the list control.
    //
	virtual void ApplyFieldWidths(
		// New width to resize all columns to.
        int iNewWidth);

    // This member function is used by the header control to determine
	// the new width for auto-sized columns.
    //
	virtual void FitFieldWidths(
		// New width to resize all columns to.
		int iNewWidth);

    // This member function is used by the header control to determine
	// the total width of all frozen columns in the header control.  Returns
	// the combined size, in pixels, of all frozen columns.
    //
	virtual int GetFrozenColWidth();
    
	//{{AFX_VIRTUAL(CXTHeaderCtrl)
	//}}AFX_VIRTUAL

protected:
	//:Ignore
    //{{AFX_MSG(CXTHeaderCtrl)
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	afx_msg BOOL OnItemchanging(NMHDR* pNMHDR, LRESULT* pResult);
	//:End Ignore

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////
// CXTFlatHeaderCtrl is a CHeaderCtrl derived class.  It is used to create
// list box flat header controls that are similar to list box flat header
// controls seen in Visual Studio&trade and Outlook&trade;.
//
class _XT_EXT_CLASS CXTFlatHeaderCtrl : public CXTHeaderCtrl
{
    DECLARE_DYNAMIC(CXTFlatHeaderCtrl)

public:

    // Constructs a CXTFlatHeaderCtrl object.
    //
    CXTFlatHeaderCtrl();

    // Destroys a CXTFlatHeaderCtrl object, handles cleanup and de-allocation.
    //
    virtual ~CXTFlatHeaderCtrl();

protected:
    // Point where right click took place.
	CPoint m_pt;
	// Popup menu resource ID.
	UINT m_popupMenuID;
	// Index of the popup menu contained in the menu.
	int m_nPos;
	// Amount to offset the sort arrow.
	int m_nOffset;
	// Last column pressed during sort.
	int m_nSortedCol;
	// TRUE if left mouse button is pressed.
	BOOL m_bLBtnDown;
	// Used, when column is pressed, to draw a sort arrow.
	BOOL m_bAscending;
	// TRUE to draw a sort arrow.
	BOOL m_bSortArrow;
	// TRUE to disable the popup menu display.
	BOOL m_bEnableMenus;
	// 'true' for initialization.
	bool m_bInitControl;

public:

	// Call this member function to determine if the header control displays
	// a sort arrow. Returns TRUE if the header control displays a sort arrow,
	// otherwise returns FALSE.
	// 
	BOOL HasSortArrow();

    // This member function is called to associate a menu and toolbar 
    // resource with the context menu.
    //
    virtual void SetMenuID(
		// Resource ID for the popup menu used with the header.
        UINT popupMenuID,
		// Position of the submenu to be displayed.
        int nPos=0);

    // This member function must be called after creation to initialize 
    // the font the header will use.
    //
    virtual void InitializeHeader(
		// TRUE if the header's font should be bold.
        BOOL bBoldFont);

    // This member function can be used to toggle the font from bold to normal.
    //
    virtual void SetFontBold(
		// TRUE if the header's font should be bold.
        BOOL bBoldFont = TRUE);

    // This member function will set the sort image for the specified column.
    // Returns a zero-based index of the previously sorted column.
    //
    virtual int SetSortImage(
		// Zero-based index of the column to set the sort image for.
        int nCol,
		// TRUE if ascending, otherwise FALSE.
        BOOL bAsc);

    // Call this member function to enable or disable the column sort arrow.
    //
    virtual void ShowSortArrow(
		// Set to TRUE to draw the column sort arrow.
        BOOL bSortArrow);

    // This member function determines which header item, if any, is at
	// a specified cursor position.  Returns the index of the item at the
	// position specified by 'pt', otherwise returns -1.
    //
    virtual int HitTest(
		// Point to be tested.
        CPoint pt ) const;

    // Call this member function to set the text justification for the
	// header control. Returns TRUE if successful, otherwise returns FALSE.
    //
    virtual BOOL SetAlingment(
		// Text alignment, either LVCFMT_CENTER, LVCFMT_LEFT, or LVCFMT_RIGHT.
        int nFlag);

    // Call this member function to return the index to the currently sorted
	// column. Returns the index of the currently sorted column.
    //
    int GetSortedCol() const;
    
    // Call this member function to return the current sorting order. Returns
	// TRUE if the sorting order is ascending, otherwise returns FALSE.
    //
    BOOL GetAscending() const;

    // Call this member function to set the bitmap image for the specified
    // header item.
    //
    void SetBitmap(
		// Zero-based index of the column to set the sort image for.
        int iCol,
		// Resource ID of the bitmap to use.
        UINT uBitmapID,
		// Style bits to be removed from the HD_ITEM::fmt variable.  For
        // a column that does not display text, pass in HDF_STRING.
        DWORD dwRemove=NULL);

	// Call this member function to enable or disable the popup menu display whenever
	// a user right clicks on the header control.
	//
	void EnablePopupMenus(
		// TRUE to enable popup menus, and FALSE to disable.
		BOOL bEnableMenus);

protected:

    // This member function is called by the header during paint operations.
    //
    virtual void DrawBorders(
		// Points to the current device context.
        CDC* pDC,
		// Area to be drawn.
        CRect& rect);

#if _MSC_VER < 1200 // MFC 5.0

    // This member function retrieves a count of the items in a header
	// control. Returns the number of header control items, if successful,
	// otherwise returns -1.
    //
    virtual int GetItemCount() const;

    // This member function retrieves the index values of the items in
	// the header control, in the order in which they appear from left to right.
    // If you use the default value of 'iCount', GetOrderArray fills the
	// parameter using GetItemCount. Returns nonzero if successful, otherwise,
	// it returns zero.
    //
    virtual BOOL GetOrderArray(
		// A pointer to the address of a buffer that receives the index 
        // values of the items in the header control, in the order in which 
        // they appear from left to right. 
        LPINT piArray,
		// The number of header control items.
        int iCount=-1);

    // This method implements the behavior of the Win32 message HDM_GETITEMRECT, 
    // as described in the Platform SDK. Returns nonzero if successful,
	// otherwise it returns zero.
    //
    virtual BOOL GetItemRect(
		// The zero-based index of the header control item.
        int nIndex,
		// A pointer to the address of a RECT structure that receives the 
        // bounding rectangle information.
        LPRECT lpRect) const;
#endif

    // Called to send WM_NOTIFY to tell parent's owner that the column needs
	// to be sorted.
    //
    void SendNotify(
		// Index of the column to be sorted.
        int iIndex);

    //:Ignore
    //{{AFX_VIRTUAL(CXTFlatHeaderCtrl)
    public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    protected:
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL
    //:End Ignore

    //:Ignore
    //{{AFX_MSG(CXTFlatHeaderCtrl)
    afx_msg void OnPaint();
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
    //}}AFX_MSG
    afx_msg void OnSortAsc();
    afx_msg void OnSortDsc();
    afx_msg void OnAlignLeft();
    afx_msg void OnAlignCenter();
    afx_msg void OnAlignRight();
	afx_msg LRESULT OnInitControl(WPARAM wParam, LPARAM lParam);
    //:End Ignore

    DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTFlatHeaderCtrl::HasSortArrow() {
	return m_bSortArrow;
}
AFX_INLINE void CXTFlatHeaderCtrl::SetFontBold(BOOL bBoldFont) {
    ASSERT(::IsWindow(m_hWnd)); SetFont(bBoldFont?&xtAfxData.fontBold:&xtAfxData.font);
}
AFX_INLINE void CXTFlatHeaderCtrl::ShowSortArrow(BOOL bSortArrow) {
    ASSERT(::IsWindow(m_hWnd)); m_bSortArrow = bSortArrow; Invalidate();
}
AFX_INLINE int CXTFlatHeaderCtrl::GetSortedCol() const {
    ASSERT(::IsWindow(m_hWnd)); return m_nSortedCol;
}
AFX_INLINE BOOL CXTFlatHeaderCtrl::GetAscending() const {
    ASSERT(::IsWindow(m_hWnd)); return m_bAscending;
}
AFX_INLINE void CXTFlatHeaderCtrl::EnablePopupMenus(BOOL bEnableMenus) {
	m_bEnableMenus = bEnableMenus;
}
#if _MSC_VER < 1200 // MFC 5.0
AFX_INLINE int CXTFlatHeaderCtrl::GetItemCount() const {
    ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, HDM_GETITEMCOUNT, 0, 0L);
}
AFX_INLINE BOOL CXTFlatHeaderCtrl::GetItemRect(int nIndex, LPRECT lpRect) const {
    ASSERT(::IsWindow(m_hWnd)); ASSERT(lpRect != NULL); return (BOOL)::SendMessage(m_hWnd, HDM_GETITEMRECT, nIndex, (LPARAM)lpRect);
}
#endif

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTFLATHEADERCTRL_H__)
