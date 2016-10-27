//:Ignore
// XTOutlookBar.h interface for the CXTContentItems class.
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

#if !defined(__XTOUTLOOKBAR_H__)
#define __XTOUTLOOKBAR_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// XT_CONTENT_ITEM is a stand alone helper structure class.  It is used
// by CXTOutlookBar.
//
struct XT_CONTENT_ITEM
{
	// Icon handle for the menu item.
	CXTIconHandle m_hIcon;
	// Zero-based index for the menu item.
	int m_nIndex;
	// Text label for the menu item.
	CString m_strText;
};

//////////////////////////////////////////////////////////////////////
// CCXTOutlookBar is a CListBox derived class.  It is used to implement
// an Outlook bar style control.  It can only be used with the LBS_OWNERDRAWVARIABLE
// style bit set. This is a simpler version of CXTOutBarCtrl and does not
// allow for shortcut folders.
//
class _XT_EXT_CLASS CXTOutlookBar : public CListBox
{
    DECLARE_DYNAMIC(CXTOutlookBar)

public:

    // Constructs a CXTOutlookBar object.
    //
    CXTOutlookBar();

    // Destroys a CXTOutlookBar object, handles cleanup and de-allocation.
    //
    virtual ~CXTOutlookBar();

protected:

	// Width of the menu icon.
	int m_cxIcon;
	// Height of the menu icon.
	int m_cyIcon;
	// Currently selected menu index.
	int m_nIndex;
	// 'true' when the menu item is selected.
	bool m_bHilight;
	// 'true' when the user has defined custom colors for the Outlook bar.
	bool m_bUserColors;
	// Holds the cursor position.
	CPoint m_point;
	// RGB value representing the background color.
	COLORREF m_clrBack;
	// RGB value representing the text color.
	COLORREF m_clrText;
	// Array of XT_CONTENT_ITEM structs that represent each item in the 
	// Outlook bar.
	CList<XT_CONTENT_ITEM*,XT_CONTENT_ITEM*> m_arContentItems;

public:

	// This member function creates an Outlook Bar control. Returns nonzero
	// if successful, otherwise returns zero.
	//
	virtual BOOL Create(
        // Window style.
        DWORD dwStyle,
        // The size and position of the window, in client coordinates of 
	    // 'pParentWnd'.
        const RECT& rect,
        // The parent window.
        CWnd* pParentWnd,
        // The ID of the child window.
        UINT nID);

    // Call this member function to set the text and background colors for
    // the Outlook bar.
    //
    void SetColors(
        // RGB value representing the text color.
        COLORREF clrText,
        // RGB value representing the background color.
        COLORREF clrBack);

	// This member function gets an RGB value that represents the control's
	// background color. Returns an RGB value that represents the background
	// color.
	//
	COLORREF GetBackColor();

	// This member function gets an RGB value that represents the control's
	// text color. Returns an RGB value that represents the text color.
	//
	COLORREF GetTextColor();

	// This member function inserts a menu item into the Outlook bar.  Unlike
	// the AddMenuItem member function, InsertMenuItem does not cause an Outlook
	// bar with the LBS_SORT style to be sorted. Returns the zero-based index
	// of the position at which the menu item was inserted. The return value
	// is LB_ERR if an error occurs.  The return value is LB_ERRSPACE if insufficient
	// space is available to store the new menu item.
    //
	int InsertMenuItem(
        // Specifies the zero-based index of the position to insert the
		// menu item. If this parameter is -1, the menu item is added to the
		// end of the list.
        int iIndex,
        // Resource ID of the icon associated with this menu item.
        UINT nIconID,
        // Points to the null-terminated string for the menu item.
        LPCTSTR lpszText);

	// Call this member function to add a menu item to an Outlook bar.  If
	// the Outlook bar was not created with the LBS_SORT style, the menu item
	// is added to the end of the Outlook bar.  Otherwise, the menu item is
	// inserted into the Outlook bar, and the Outlook bar is sorted.  If the
	// Outlook bar was created with the LBS_SORT style but not the LBS_HASSTRINGS
	// style, the framework sorts the Outlook bar by one or more calls to
	// the CompareItem member function.  Use InsertMenuItem to insert a menu
	// item into a specific location within the Outlook bar. Returns the zero-based
	// index to the menu item in the Outlook bar.  The return value is LB_ERR
	// if an error occurs.  The return value is LB_ERRSPACE if insufficient
	// space is available to store the new menu item.
    //
	int AddMenuItem(
        // Resource ID of the icon associated with this menu item.
        UINT nIconID,
        // Points to the null-terminated string for the menu item.
        LPCTSTR lpszText);

	// Call this member function to return an XT_CONTENT_ITEM object that
	// represents the menu item specified by 'iItem'. Returns an XT_CONTENT_ITEM
	// pointer.
    //
	XT_CONTENT_ITEM* GetMenuItem(
        // Specifies the zero-based index of the menu item to retrieve.
        int iItem);

    //:Ignore
    //{{AFX_VIRTUAL(CXTOutlookBar)
    public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    //}}AFX_VIRTUAL
    //:End Ignore

protected:

	// This member function is called by the control for flicker free drawing.
    //
	void OnNoFlickerPaint(
        // Points to the current device context.
        CDC* pDC);

    //:Ignore
    //{{AFX_MSG(CXTOutlookBar)
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
    //:End Ignore
    DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTOutlookBar::SetColors(COLORREF clrText, COLORREF clrBack) {
    m_clrBack = clrBack; m_clrText = clrText; m_bUserColors = true;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTOUTLOOKBAR_H__)

