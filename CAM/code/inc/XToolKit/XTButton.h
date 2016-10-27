//:Ignore
// XTButton.h : interface for the CXTButton class.
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

#if !defined(__XTBUTTON_H__)
#define __XTBUTTON_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTButton is a CButton derived class.  It is used to create flat style
// and image buttons.  This control has several features including flat,
// semi-flat and three-dimensional borders.
//
class _XT_EXT_CLASS CXTButton : public CButton
{
	DECLARE_DYNAMIC(CXTButton)

public:
	
    // Constructs a CXTButton object.
    //
	CXTButton();

	// Destroys a CXTButton object, handles cleanup and de-allocation.
    //
    virtual ~CXTButton();

protected:

	// Handle to the icon associated with the button.
	HICON m_hIcon;
	// Handle to the icon associated with the button when it is pressed.
	HICON m_hIconPushed;
	// Bitmap associated with the button.
	HBITMAP m_hBitmap;
	// Disabled bitmap associated with the button.
	HBITMAP m_hBitmapMono;
	// Initial size of the button.
	CSize m_sizeImage;
	// XY location of the button image.
	CPoint m_ptImage;
	// XY location of the button text.
	CPoint m_ptText;
	// Gap between the button edge and the image.
	int m_nBorderGap;
	// Gap between the button text and the image.
	int m_nImageGap;
	// The style of the button.  See SetXButtonStyle().
	DWORD m_dwxStyle;
	// TRUE if the button is highlighted.
	BOOL m_bHilite;
	// TRUE if the user has defined the XY coordinates for the image and text.
	BOOL m_bUserPosition;
	// TRUE if the left mouse button is pressed.
	BOOL m_bLBtnDown;
	// TRUE if user defined colors are used.
	BOOL m_bAltColor;
	// Used during paint operations.
	BOOL m_bPainted;
	// TRUE if the button is checked.
	BOOL m_bChecked;
	// An RGB value that represents the button text color.
	COLORREF m_clrBtnText;
	// An RGB value that represents the button face color.
	COLORREF m_clr3DFace;
	// An RGB value that represents the 3D border highlight color.
	COLORREF m_clr3DHilight;
	// An RGB value that represents the 3D border shadow color.
	COLORREF m_clr3DShadow;

	//:Ignore
	//////////////////////////////////////////////////////////////////////
    // XP Flat colors
	//////////////////////////////////////////////////////////////////////
	//:End Ignore

    // An RGB value that represents the background color for highlighted
	// buttons in XP-Flat mode.
	COLORREF m_clrXPFHighlight;
	// An RGB value that represents the background color for pressed buttons
	// in XP-Flat mode.
	COLORREF m_clrXPFPressed;
	// An RGB value that represents the frame color for highlighted/pressed
	// buttons in XP-Flat mode.
	COLORREF m_clrXPFBorder;
	// TRUE if the user set custom XP-Flat colors.
	BOOL m_bXPFUserColors;

	//:Ignore
	DWORD m_dwInitSignature; // used for one-time initialization.
	//:End Ignore

public:

    // This member function sets the highlighting state of a button control.
	// This is the same as its CButton counterpart, except the m_bChecked flag
	// is set.  This is done because MFC does not recognize the button as being
	// checked unless it a radio or check box.
	//
	void SetStateX(
		// Specifies whether the button is to be checked.  TRUE will check the
		// button. FALSE will uncheck it.
        BOOL bChecked);

	// This member function will set the gap between the button's edge and
	// the image. Returns the previous border gap value.
    //
	int SetBorderGap(
		// Amount, in pixels, of the gap between the button edge and the image.
        int nGap);

	// This member function will set the gap between the button's text and
	// the image. Returns the previous border gap value.
    //
	int SetImageGap(
		// Amount, in pixels, of the gap between the button text and the image.
        int nGap);

	// This method will allow the user to define the default colors for the
	// background shadow and highlight colors for the button.
    //
	virtual void SetAlternateColors(
		// An RGB value that represents the user defined face color for 
		// three-dimensional display elements.
        COLORREF clr3DFace,
		// An RGB value that represents the user defined highlight color for
		// three-dimensional display elements (edges facing the light source.)
        COLORREF clr3DHilight,
		// An RGB value that represents the user defined shadow color for 
		// three-dimensional display elements (edges facing away from the
		// light source).
        COLORREF clr3DShadow,
		// An RGB value that represents the user defined text color on push buttons.
        COLORREF clrBtnText);

	// This member function sets the default face color for the button.
    //
	virtual void SetColorFace(
		// An RGB value that represents the user defined face color.
        COLORREF clrFace);

    // This member function gets the current face color.
	// Returns an RGB value that represents the current face color.
	//
    virtual COLORREF GetColorFace();

	// This member function sets the default highlight color for the button.
    //
	virtual void SetColorHilite(
		// An RGB value that represents the user defined highlight color.
        COLORREF clrHilite);

	// This member function sets the default shadow color for the button.
    //
	virtual void SetColorShadow(
		// An RGB value that represents the user defined shadow color.
        COLORREF clrShadow);

	// This member function sets the default text color for the button.
    //
	virtual void SetColorText(
		// An RGB value that represents the user defined text color.
        COLORREF clrText);

	// This member function sets the colors for the BS_XT_XPFLAT mode.
	//
    virtual void SetXPFlatColors(
		// An RGB value that represents the user defined border color for the
		// flat button.
		COLORREF clrBorder,
		// An RGB value that represents the user defined highlight color for
		// the flat button.
		COLORREF clrHighlight,
        // An RGB value that represents the user defined color for when
		// the flat button is pressed.
		COLORREF clrPressed
        );

	// This member function will set the normal and pushed state icons for
	// the push button. Returns TRUE if successful, otherwise returns FALSE.
    //
	virtual BOOL SetIcon(
		// CSize object that represents the size of the icon.
        CSize size,
		// Handle to the normal icon.
        HICON hIcon,
		// Handle to the pressed icon.
        HICON hIconPushed=NULL,
		// Specifies whether the button is to be redrawn.  A nonzero value redraws
		// the button. A zero value does not redraw the button. The button is redrawn
		// by default.
        BOOL bRedraw=TRUE);

	// This member function will set the normal and pushed state icons for
	// the push button. Returns TRUE if successful, otherwise returns FALSE.
    //
	virtual BOOL SetIcon(
		// CSize object that represents the size of the icon.
        CSize size,
		// Resource ID for the normal icon.
        UINT nID,
		// Resource ID for the pressed icon.
        UINT nPushedID=0,
		// Specifies whether the button is to be redrawn.  A nonzero value redraws
		// the button. A zero value does not redraw the button.  The button is redrawn
		// by default.
        BOOL bRedraw=TRUE);

	// This member function will set the normal and pushed state icons for
	// the push button. Returns TRUE if successful, otherwise returns FALSE.
    //
	virtual BOOL SetIcon(
		// CSize object that represents the size of the icon.
        CSize size,
		// Resource string ID for the normal icon.
        LPCTSTR lpszID,
		// Resource string ID for the pressed icon.
        LPCTSTR lpszPushedID=NULL,
		// Specifies whether the button is to be redrawn.  A nonzero value redraws
		// the button. A zero value does not redraw the button.  The button is redrawn
		// by default.
        BOOL bRedraw=TRUE);
	
	// This member function will set the bitmap associated with the button.
	// Returns TRUE if successful, otherwise returns FALSE.
    //
	virtual BOOL SetBitmap(
		// CSize object that represents the size of the bitmap.
        CSize size,
		// Resource ID for the bitmap.
        UINT nID,
		// Specifies whether the button is to be redrawn.  A nonzero value redraws
		// the button. A zero value does not redraw the button.  The button is redrawn
		// by default.
        BOOL bRedraw=TRUE);

	// This member function will set the XY location of the text and image
	// that is displayed on the push button. Returns TRUE if successful, otherwise
	// returns FALSE.
    //
	virtual BOOL SetTextAndImagePos(
		// XY location of the image displayed on the button.
        CPoint ptImage,
		// XY location of the text displayed on the button.
        CPoint ptText,
		// Specifies whether the button is to be redrawn.  A nonzero value redraws
		// the button. A zero value does not redraw the button.  The button is redrawn
		// by default.
        BOOL bRedraw=TRUE);

	// This member function will set the display style for the button.
	// Returns the previous style that was set.
    //
	DWORD SetXButtonStyle(
		// Specifies the button style. It can be one or more of the following:
	    // [ul]
	    // [li][b]BS_XT_FLAT[/b] - Draws a flat button.[/li]
	    // [li][b]BS_XT_SEMIFLAT[/b] - Draws a semi-flat button.[/li]
	    // [li][b]BS_XT_TWOROWS[/b] - Draws images and text that are centered.[/li]
	    // [li][b]BS_XT_SHOWFOCUS[/b] - Draws a focus rect when the button
		//        has input focus.[/li]
	    // [li][b]BS_XT_HILITEPRESSED[/b] - Highlights the button when pressed.[/li]
	    // [li][b]BS_XT_XPFLAT[/b] - Draws a flat button ala Office XP.[/li]
		//[/ul]
        DWORD dwxStyle,
		// Specifies whether the button is to be redrawn.  A nonzero value redraws
		// the button. A zero value does not redraw the button.  The button is redrawn
		// by default.
        BOOL bRedraw=TRUE);

	// This function returns only the BS_XT_ style values, not any of the
	// other window styles. Returns the button styles for this CXTButton object.
    //
	virtual DWORD GetXButtonStyle();

	// This function removes the icon or bitmap from the button.  All resources
	// taken by the image are freed.
	//
	virtual void ResetImage();

protected:

	// This member function will set the proper XY coordinates for the button
	// text and image.
    //
	void OffsetPoint(
		// XY location of the text and image that are displayed.
        CPoint& point,
		// Initial size of the image associated with the button.
        CSize size);

	// This member function is called to free all associated GDI resources
	// that have been allocated.
    //
	virtual void CleanUpGDI();

	// This member function draws the icon for the button, if any.
    //
	virtual void DrawButtonIcon(
		// A CDC pointer that represents the current device context.
        CDC* pDC,
		// A UINT value that represents the current state for the button.
        UINT nState,
		// A CRect reference that represents the current size for the button.
        CRect& rcItem);

	// This member function will draw the text for the button, if any.
    //
	virtual void DrawButtonText(
		// A CDC pointer that represents the current device context.
        CDC* pDC,
		// A UINT value that represents the current state for the button.
        UINT nState,
		// A CRect reference that represents the current size for the button.
        CRect& rcItem);

	// This member function will draw the bitmap for the button, if any.
	//
	virtual void DrawButtonBitmap(
		// A CDC pointer that represents the current device context.
        CDC* pDC,
		// A UINT value that represents the current state for the button.
        UINT nState,
		// A CRect reference that represents the current size for the button.
        CRect& rcItem);

	// Call this member function to return the buttons text minus the '&'.
	// Returns a CString object that represents the button's text.
    //
	virtual CString GetButtonText(
		// TRUE to remove the '&' used for mnemonics.
        BOOL bRemoveAmpersand);

	// Call this member function to calculate the position of the button image.
    // Returns the top-left position for the button image.
	//
    virtual CPoint CalculateImagePosition(
		// A CDC pointer that represents the current device context.
        CDC* pDC,
		// A UINT value that represents the current state for the button.
        UINT nState,
		// A CRect reference that represents the current size for the button.
        CRect& rcItem,
		// Indicates if the button has a separate image for the pushed state.
        bool bHasPushedImage);


    // Call this helper function to set the pressed state and redraw the button.
	//
    void NoteButtonDepressed(
		// TRUE if the button is pressed, otherwise FALSE.
		BOOL bDepressed);

	//:Ignore

	// defers control initialization
	void DeferInitialUpdate();

    //{{AFX_VIRTUAL(CXTButton)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
    //:End Ignore

	//:Ignore
	//{{AFX_MSG(CXTButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSysColorChange();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnInitControl(WPARAM wParam, LPARAM lParam);
    //:End Ignore

	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTButton::SetImageGap(int nGap) {
	int nOldGap = m_nImageGap; m_nImageGap = nGap; return nOldGap;
}
AFX_INLINE int CXTButton::SetBorderGap(int nGap) {
	int nOldGap = m_nBorderGap; m_nBorderGap = nGap; return nOldGap;
}
AFX_INLINE DWORD CXTButton::GetXButtonStyle() {
	ASSERT(::IsWindow(m_hWnd)); return m_dwxStyle;
}
AFX_INLINE void CXTButton::SetStateX(BOOL bChecked) {
	CButton::SetState(bChecked); m_bChecked = bChecked;
}
AFX_INLINE COLORREF CXTButton::GetColorFace() {
    return m_clr3DFace;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTBUTTON_H__)
