//:Ignore
// XTCaption.h : interface for the CXTCaption class.
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

#if !defined(__XTCAPTION_H__)
#define __XTCAPTION_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTCaptionButton is a CXTButton derived class.  It is used by
// the CXTCaption class to activate a CXTCaptionPopupWnd window.
//
class _XT_EXT_CLASS CXTCaptionButton : public CXTButton
{
public:

    // This member function will set the style for the caption button.
    // Returns TRUE if successful, otherwise returns FALSE.
    //
    virtual BOOL SetButtonStyle(
        // An RGB value that represents the button background color.
        COLORREF clrFace,
		// An RGB value that represents the font color.
        COLORREF clrText);

protected:

    // This member function will draw the text for the button, if any.
	//
    virtual void DrawButtonText(
		// A CDC pointer that represents the current device context.
        CDC* pDC,
		// A UINT value that represents the current state for the button.
        UINT nState,
		// A CRect reference that represents the current size for the button.
        CRect& rcItem);

	// This member function draws the icon for the button, if any.
    //
	virtual void DrawButtonIcon(
		// A CDC pointer that represents the current device context.
        CDC* pDC,
		// A UINT value that represents the current state for the button.
        UINT nState,
		// A CRect reference that represents the current size for the button.
        CRect& rcItem);
};

class CXTCaptionPopupWnd;

//////////////////////////////////////////////////////////////////////
// CXTCaption is a CStatic derived class.  It is used to create caption
// or info bars similar to those in Microsoft&reg Outlook&trade;.  
//
class _XT_EXT_CLASS CXTCaption : public CStatic
{
	DECLARE_DYNAMIC(CXTCaption)

public:
	
    // Constructs a CXTCaption object.
    //
	CXTCaption();

	// Destroys a CXTCaption object, handles cleanup and de-allocation.
    //
    virtual ~CXTCaption();

protected:

    // Size, in pixels, that the child window should leave for its caption
	// area.
	int m_nOffset;
	// Size, in pixels, for the caption border.
	int m_nBorder;
	// 'true' if the user has specified caption colors other than the default.
	bool m_bUserColors;
	// Border style bits, either CPWS_EX_GROOVE_EDGE | CPWS_EX_RAISED_EDGE.
	DWORD m_dwExStyle;
	// Text style, one of DT_ styles.  See CDC::DrawText(...) for more details.
	DWORD m_dwTextStyle;
	// User defined icon handle. The default value is NULL.
	HICON m_hIcon;
	// An RGB value that represents the user defined border color.
	COLORREF m_clrBorder;
	// An RGB value that represents the user defined background color.
	COLORREF m_clrFace;
	// An RGB value that represents the user defined font color.
	COLORREF m_clrText;
	// A CWnd pointer that represents the child window displayed in the popup
	// window.
	CWnd* m_pChildWnd;
	// A CWnd pointer that represents the child window's parent view.
	CWnd* m_pParentView;
	// A CWnd pointer that represents the splitter window. It is used to track
	// size changes.
	CWnd* m_pSplitterWnd;
	// Width and height of the caption icon area.
	CSize m_sizeIcon;
	// Size of the child window displayed in the popup.
	CRect m_rcChild;
	// Size of the child's parent view.
	CRect m_rcParent;
	// Size of the splitter window.
	CRect m_rcSplitter;
	// Text that will be displayed in the caption.
	CString m_strCaption;
	// Image list used to create the close button icon.
	CImageList m_ilButton;
	// Button used to 'tack' the popup window back in place.
	CXTCaptionButton m_btnCaption;
	// Points to the popup window.
	CXTCaptionPopupWnd* m_pPopupWnd;

public:

	// Call this member function to get a reference to the caption's close
	// / popup button. Returns a reference to a CXTCaptionButton object.
	//
	CXTCaptionButton& GetCaptionButton();

    // This member function will set the caption bar border, background,
	// and font colors.
    //
    virtual void SetCaptionColors(
		// An RGB value that represents the new border color.
        COLORREF clrBorder,
		// An RGB value that represents the new background color.
        COLORREF clrFace,
		// An RGB value that represents the new font color.
        COLORREF clrText);

    // This function will modify the caption style.  You can use this 
    // member function to set the border size that is drawn around the 
    // caption banner, the font that the caption will use, and the caption text 
    // and icon to be displayed.
    //
    virtual void ModifyCaptionStyle(
		// Specifies size, in pixels, of the banner border.
        int nBorderSize,
		// Specifies the new caption font.
        CFont* pFont=NULL,
		// NULL terminated string specifying the new caption text.
        LPCTSTR lpszWindText=NULL,
		// Handle of the icon to be drawn in the caption.
        HICON hIcon=NULL);

    // Call this member function to set the child and notification windows
	// for the caption bar.
    //
    virtual void SetChildWindow(
		// A CWnd pointer that represents the child window to be displayed in
		// the popup window.
        CWnd* pChild,
		// A CWnd pointer that represents the window to receive notification
		// messages.
        CWnd* pNotifyWnd);

    // This member function is called to destroy the popup window and associated
	// children.
    //
    virtual void KillChildWindow();

    // This member function is called to update the text and icon for the
	// caption bar.
    //
    virtual void UpdateCaption(
		// NULL terminated string to display in the caption bar.
        LPCTSTR lpszWindowText,
		// Handle of the icon to display in the caption bar. It can be NULL.
        HICON hIcon);

    // This member function is called to set the formatting style of the
	// text that is displayed in the caption bar.
    //
    virtual void SetTextStyle(
		// Specifies the method of formatting the text.  See CDC::DrawText for
		// a detailed listing of available styles.
        DWORD dwStyle); 

	// This member function is called to retrieve the formatting style of
	// the text that is displayed in the caption bar.  Returns a DWORD value
	// that represents the formatting style of the current caption text.
	//
	virtual DWORD GetTextStyle();

    // This method creates a caption bar window and attaches it to the
	// CXTCaption object. You construct a caption bar in two steps.  First,
	// call the constructor, which constructs the CXTCaption object.  Then
	// call Create, which creates the Window's child window and attaches it
	// to CXTCaption.  Create initializes the window class name and window
	// name and registers values for its style, parent, and ID.  Returns TRUE
	// if successful, otherwise returns FALSE.
    //
    virtual BOOL Create(
		// Specifies the parent window.
        CWnd* pParentWnd,
		// Points to a null-terminated character string that contains the window
		// name. This will be displayed in the caption area.
        LPCTSTR lpszWindowName,
		// Specifies caption bar style.  Can be either CPWS_EX_RAISED_EDGE which
		// will draw a 3D edge around the caption bar, or CPWS_EX_GROOVE_EDGE
		// which will draw a sunken edge.
        DWORD dwExStyle=CPWS_EX_RAISED_EDGE,
		// Specifies the control window style.  Apply any combination of caption
		// bar styles to the control.
        DWORD dwStyle=WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE,
		// Specifies the position and size of the caption bar.  It can be either
		// a RECT structure or a CRect object. 
        const CRect& rect=CRect(0,0,0,0),
		// Specifies the caption bar control ID.
        UINT nID = 0xffff);

	//:Ignore
    //{{AFX_VIRTUAL(CXTCaption)
	//}}AFX_VIRTUAL
    //:End Ignore

protected:

    // This member function is called to draw the caption background.
    //
    virtual void DrawCaptionBack(
		// A CDC pointer that represents the current device context.
        CDC* pDC,
		// A CRect reference that represents the size of the area to paint.
        CRect& rcItem);

    // This member function is called to paint the caption text.
    //
    virtual void DrawCaptionText(
		// A CDC pointer that represents the current device context.
        CDC* pDC,
		// A CRect reference that represents the size of the area to paint.
        CRect& rcItem);

    // This member function is called to paint the caption icon.
    //
    virtual void DrawCaptionIcon(
		// A CDC pointer that represents the current device context.
        CDC* pDC,
		// A CRect reference that represents the size of the area to paint.
        CRect& rcItem);

    // This member function is called to update the caption text.
    //
    virtual void UpdateCaptionText(
		// A CDC pointer that represents the current device context.
        CDC* pDC,
		// A CRect reference that represents the size of the area to paint.
        CRect& rcItem);

	//:Ignore
    //{{AFX_MSG(CXTCaption)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
    afx_msg void OnSysColorChange();
	//}}AFX_MSG
	afx_msg void OnCaptButton();
	afx_msg void OnPushPinButton();
	afx_msg void OnPushPinCancel();
    //:End Ignore

	DECLARE_MESSAGE_MAP()

	friend class CXTCaptionButton;
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTCaption::SetTextStyle(DWORD dwStyle) {
	m_dwTextStyle = dwStyle;
}
AFX_INLINE DWORD CXTCaption::GetTextStyle() {
	return m_dwTextStyle;
}
AFX_INLINE CXTCaptionButton& CXTCaption::GetCaptionButton() {
	ASSERT(::IsWindow(m_btnCaption.m_hWnd)); return m_btnCaption;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTCAPTION_H__)

