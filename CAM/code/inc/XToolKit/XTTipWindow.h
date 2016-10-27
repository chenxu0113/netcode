//:Ignore
// XTTipWindow.h interface for the CXTTipWindow class.
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

#if !defined(__XTTIPWINDOW_H__)
#define __XTTIPWINDOW_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTTipWindow is a CWnd derived class.  It is used to create a CXTTipWindow
// object. CXTTipWindow objects are tooltip style popup windows that can
// display useful information to the end user.  Possible options include
// shadows, borders, and background color settings.
//
class _XT_EXT_CLASS CXTTipWindow : public CWnd
{
public:

    // Constructs a CXTTipWindow object.
    //
    CXTTipWindow();

    // Destroys a CXTTipWindow object, handles cleanup and de-allocation.
    //
    virtual ~CXTTipWindow();

protected:

	// Stores strings if the tip is multiline.
	CStringArray m_arStrings;
	// Initial size of the tip window.
	CRect m_rcWindow;
	// Initial size of the tip window shadow.
	CRect m_rcShadow;
	// Parent window.
	CWnd* m_pParentWnd;
	// Background color for the tip window.
	COLORREF m_crBackColor;
	// Text color for the tip window.
	COLORREF m_crTextColor;
	// Title text for the tip window.
	CString m_strTitle;
	// Description text for the tip window.
	CString m_strDescrip;
	// Time-out value.
	UINT m_nElapse;
	// Timer event ID.
	UINT m_nEventID;
	// Initial style for the tip window.  See ShowTipWindow(..).
	DWORD m_dwTipStyle;
	// Amount, in pixels, to space the title and description text.
	int m_nLineSpace;
	// Amount, in pixels, of the tip and side margins of where the tip text
	// is displayed.
	CSize m_sizeMargin;

public:

    // This member function will set the initial time-out value for when
	// to close the tip window.
    //
    void SetTimeout(
        //  Specifies the time-out value, in milliseconds, of when the 
        //  tip window should close.
        UINT nElapse);

    // This member function will set the text for the title and description
    // values that are displayed in the tip window.
    //      
    void SetTipText(
        //  A NULL terminated string that is to be displayed as
        //  the title for the tip window.
        LPCTSTR lpszTitle,
        //  A NULL terminated string that is to be displayed as
        //  the description text for the tip window.
        LPCTSTR lpszDescrip,
        //  If TRUE, the tip window will be redrawn.
        BOOL bRedraw=FALSE);

    // This member function will set the text and background colors for
    // the tip window display.
    //
    void SetTipColors(
        //  The RGB color value for the background of the tip window.
        COLORREF crBackColor,
        //  The RGB color value for the text of the tip window.
        COLORREF crTextColor,
        //  If TRUE, the tip window will be redrawn.
        BOOL bRedraw=FALSE);

    // This member function will display a tip window to display a title
	// and description text. Returns TRUE if the tip window was successfully
	// created, otherwise returns FALSE.
    //
    virtual BOOL ShowTipWindow(
        // The initial x and y coordinates of where the tip window is
        // to be displayed.
        const CPoint& point,
        // Points to the parent window of the tip window.
        CWnd* pParentWnd,
        // Initial style for the tip window.  The desired styles 
        // for the tip window can be one or more of the following:
        //[ul]
        //[li][b]TWS_XT_THICKBORDER[/b] - This style will cause the tip
		//       window to display a thick border around its edges.[/li]
        //[li][b]TWS_XT_DROPSHADOW[/b] - This style will cause the tip
		//       window to cast a shadow on the parent window.[/li]
		//[li][b]TWS_XT_ALPHASHADOW[/b] - Used with TWS_XT_DROPSHADOW to display
		//       an alpha style shadow.  By default, mono shadowing is enabled.[/li]
        //[/ul]
        DWORD dwTipStyle=TWS_XT_DROPSHADOW,
        // Specifies the time-out value, in milliseconds, of when
        // the tip window should close.
        UINT nElapse=5000,
		// Set this parameter to 'true' to center the window horizontally on
		// the point passed in.
		bool bCenterHorz = false);

    // This member function will set the text margins for the tip window.
    //
    void SetMargins(
        //  Initial size for the horizontal and vertical text margins.
        CSize size);

    // This member function will set the line spacing for the tip window.
    //
    void SetLineSpace(
        // Initial size for text line spacing.
        int nLineSpace);

protected:

    // This member function will draw a shadow rect to the device context
    // specified by 'pDC'.
    //
    void DrawShadowRect(
        // Device context of the window to cast the shadow on.
        CDC *pDC,
        // Initial size of the shadow.
        const CRect& rect);

    // This member function will set the sizes for the tip window.
    //
    void InitializeSize(
        // The x and y coordinates of where the tip window is to be displayed.
        const CPoint& point);

    //:Ignore
    //{{AFX_VIRTUAL(CXTTipWindow)
    //}}AFX_VIRTUAL
    //:End Ignore

    //:Ignore
    //{{AFX_MSG(CXTTipWindow)
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnPaint();
    afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
    //:End Ignore

    DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTTipWindow::SetTipText(LPCTSTR lpszTitle, LPCTSTR lpszDescrip, BOOL bRedraw) {
    m_strTitle = lpszTitle; m_strDescrip = lpszDescrip; if (bRedraw) { InvalidateRect(NULL); }
}
AFX_INLINE void CXTTipWindow::SetTipColors(COLORREF crBackColor, COLORREF crTextColor, BOOL bRedraw) {
    m_crBackColor = crBackColor; m_crTextColor = crTextColor; if (bRedraw) { InvalidateRect(NULL); }
}
AFX_INLINE void CXTTipWindow::SetMargins(CSize size) {
    m_sizeMargin.cx = __max(3, size.cx); m_sizeMargin.cy = __max(3, size.cy);
}
AFX_INLINE void CXTTipWindow::SetLineSpace(int nLineSpace) {
    m_nLineSpace = __max(2, nLineSpace);
}
AFX_INLINE void CXTTipWindow::SetTimeout(UINT nElapse) {
    m_nElapse = nElapse;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTTIPWINDOW_H__)
