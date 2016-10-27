//:Ignore
// XTLogoPane.h interface for the CXTLogoPane class.
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

#if !defined(__XTLOGOPANE_H__)
#define __XTLOGOPANE_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTBasePane is a CWnd derived class.  It is the base class used for
// creating CWnd objects to place in status bar panes that do custom paint
// routines.
//
class _XT_EXT_CLASS CXTBasePane : public CWnd
{
	DECLARE_DYNCREATE(CXTBasePane)

public:
	
    // Constructs a CXTBasePane object.
    //
	CXTBasePane();

	// Destroys a CXTBasePane object, handles cleanup and de-allocation.
    //
    virtual ~CXTBasePane();

protected:

	// NULL terminated string that represents the pane text.
	CString m_strWindowText; 
	
    //{{AFX_VIRTUAL(CXTLogoPane)
	//}}AFX_VIRTUAL

public:
	
	// Call this member function to create a status bar pane object that performs
	// custom draw routines.  Returns TRUE if successful, otherwise returns FALSE.
	//
	virtual BOOL Create(
		// Text string to be associated with this pane.
		LPCTSTR lpszWindowName,
		// Pointer to the owner status bar window.
		CWnd* pParentWnd,
		// Window style.
		DWORD dwStyle=WS_CHILD|WS_VISIBLE,
		// Control ID.
		UINT nID=0xffff);

protected:

	// Override this virtual function in your derived class to perform your
	// custom drawing routines.
	//
	virtual void DoPaint(
		// Pointer to the device context.
		CDC* pDC,
		// Size of the client area to draw.
		CRect& rcClient);

	//:Ignore
    //{{AFX_MSG(CXTBasePane)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
    //:End Ignore
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////
// CXTLogoPane is a CXTBasePane derived class.  CXTLogoPane works with
// CXTStatusBar and allows you to create a logo to place in your status
// bar area.
//
class _XT_EXT_CLASS CXTLogoPane : public CXTBasePane
{
public:
	
    // Constructs a CXTLogoPane object.
    //
	CXTLogoPane();

	// Destroys a CXTLogoPane object, handles cleanup and de-allocation.
    //
    virtual ~CXTLogoPane();

protected:

	// Font to be used.
	CFont m_Font;
	// Size of the text to be displayed.
	CSize m_sizeText;

public:

	// This member function will set the text to be displayed in the logo
	// pane.
    //
	virtual void SetLogoText(
        // A NULL terminated string that represents the text to be displayed.
        LPCTSTR lpszLogoText);

	// This member function returns a CString that represents the logo text.
	// Returns a CString object that represents the text that is displayed
	// in the logo pane.
    //
	virtual CString GetLogoText();

	// This member function sets the font to be displayed in the logo pane.
    //
	virtual void SetLogoFont(
        // A NULL terminated string that represents the text to be displayed.
        LPCTSTR lpszFontName,
        // Initial height for the font.
        int nHeight=24,
        // Initial weight for the font.
        int nWeight=FW_BOLD,
        // TRUE if the font is italic.
        BOOL bItalic=TRUE,
        // TRUE if the font is underlined.
        BOOL bUnderline=FALSE);

	// This member function sets the font to be displayed in the logo pane.
    //
	virtual void SetLogoFont(
        // Address of a LOGFONT structure.
        LOGFONT& logFont);

	// This member function will return the size of the text displayed in
	// the logo pane. Returns a CSize object that represents the current size
	// of the logo pane text.
    //
	CSize GetTextSize();

	// This member function will calculate the size of the text that is displayed
	// in the logo pane and initializes 'm_sizeText'.
	//
	void SetTextSize();

    //{{AFX_VIRTUAL(CXTLogoPane)
	//}}AFX_VIRTUAL

protected:

	// Override this virtual function in your derived class to perform your
	// custom drawing routines.
	//
	virtual void DoPaint(
		// Pointer to the device context.
		CDC* pDC,
		// Size of the client area to draw.
		CRect& rcClient);

	//:Ignore
    //{{AFX_MSG(CXTLogoPane)
	//}}AFX_MSG
    //:End Ignore

	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE CSize CXTLogoPane::GetTextSize() {
	SetTextSize(); m_sizeText.cx+=2; return m_sizeText;
}
AFX_INLINE void CXTLogoPane::SetLogoText(LPCTSTR lpszLogoText) {
	m_strWindowText = lpszLogoText;
}
AFX_INLINE CString CXTLogoPane::GetLogoText() {
	return m_strWindowText;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTIconPane is a CXTBasePane derived class.  CXTIconPane works with
// CXTStatusBar and allows you to create a logo to place in your status
// bar area.
//
class _XT_EXT_CLASS CXTIconPane : public CXTBasePane
{
public:
	
    // Constructs a CXTIconPane object.
    //
	CXTIconPane();

	// Destroys a CXTIconPane object, handles cleanup and de-allocation.
    //
    virtual ~CXTIconPane();

protected:

	// Icon that is displayed in the status pane.
	CXTIconHandle m_hIcon;
	// Size of the icon to display.
	CSize m_sizeIcon;

public:

	// Call this member function to set the pane icon for this object.
	//
	void SetPaneIcon(
		// Resource ID of the icon to display.
		int nIconID);

	// Call this member function to set the pane icon for this object.
	//
	void SetPaneIcon(
		// Resource ID of the icon to display.
		LPCTSTR lpszIconID);

	// Override this virtual function in your derived class to perform your
	// custom drawing routines.
	//
	virtual void DoPaint(
		// Pointer to the device context.
		CDC* pDC,
		// Size of the client area to draw.
		CRect& rcClient);

	//:Ignore
    //{{AFX_MSG(CXTIconPane)
	afx_msg void OnEnable(BOOL bEnable);
	//}}AFX_MSG
    //:End Ignore

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTLOGOPANE_H__)

