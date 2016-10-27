//:Ignore
// XTColorDialog.h : header file
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

#if !defined(__XTCOLORSHEET_H__)
#define __XTCOLORSHEET_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTColorDialog is a multiple inheritance class derived from CXTDialogState
// and CPropertySheet. It is an Office&trade; style color dialog and can be used
// as a drop in replacement for the MFC CColorDialog API.
//
class _XT_EXT_CLASS CXTColorDialog : CXTDialogState, public CPropertySheet
{
	DECLARE_DYNAMIC(CXTColorDialog)

public:

    // Constructs a CXTColorDialog object.
    //
	CXTColorDialog(
		// An RGB value that represents the new color selection.
        COLORREF clrNew,
		// An RGB value that represents the default color selection.
        COLORREF clrCurrent,
		// Style for color dialog. It can be one of the following:
        //[ul]
	    //[li][b]CPS_XT_SHOW3DSELECTION[/b] - Displays the color selection
		//       box with a 3D raised border.[/li]
	    //[li][b]CPS_XT_SHOWHEXVALUE[/b] - Displays the hex equivalent of
		//       the selected color.[/li]
        //[/ul]
        DWORD dwFlags = 0L,
		// A pointer to the dialog box’s parent or owner window.
        CWnd* pWndParent = NULL);

	// Destroys a CXTColorDialog object, handles cleanup and de-allocation.
    //
    virtual ~CXTColorDialog();

protected:

	// Styles for the dialog.
	DWORD m_dwStyle;
	// Edit window to display the color hex value.
	CEdit m_editHex;
	// Size of the rectangle that represents the new color.
	CRect m_rcNew;
	// Size of the rectangle that represents the current color.
	CRect m_rcCurrent;
	// A COLORREF value that contains the RGB information for the new color.
	COLORREF m_clrNew;
	// A COLORREF value that contains the RGB information for the current color.
	COLORREF m_clrCurrent;

public:

	// Call this member function to set the current color selection to the
	// color value specified in 'clr'.  The dialog box will automatically update
	// the user’s selection based on the value of the 'clr' parameter.
    //
	void SetNewColor(
		// An RGB value that represents the color.
        COLORREF clr,
		// TRUE to notify tab pages of a color change.
        BOOL bNotify=TRUE);

	// Call this function to set the new color selection to the color 
	// value specified in 'clr'.
    //
	void SetCurrentColor(
		// An RGB value that represents the color.
        COLORREF clr);

	// Call this member function to retrieve the information about the current
	// color. Returns a COLORREF value that contains the RGB information for
	// the current color specified when the dialog was instantiated.
    //
	COLORREF GetCurrentColor();

	// Call this member function to retrieve the information about the color
	// the user selected. Returns a COLORREF value that contains the RGB information
	// for the new color selected in the color dialog box.
    //
	COLORREF GetColor();

	// Call this member function to copy the string specified by 'strText' to the
	// Windows clipboard. Returns TRUE if successful, otherwise returns FALSE.
    //
	BOOL CopyToClipboard(
		// A NULL terminated string.
        CString strText);

	// This member function returns a CString object that represents the
	// HEX conversion for the specified RGB color. Returns a CString object.
    //
	CString RGBtoHex(
		// An RGB value that represents the color.
        COLORREF clr) const;

	//:Ignore
    //{{AFX_VIRTUAL(CXTColorDialog)
	public:
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
    //:End Ignore

protected:
	//:Ignore
    //{{AFX_MSG(CXTColorDialog)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
    //:End Ignore
	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE COLORREF CXTColorDialog::GetCurrentColor() {
	return m_clrCurrent;
}
AFX_INLINE COLORREF CXTColorDialog::GetColor() {
	return m_clrNew;
}

#define WM_XT_UPDATECOLOR	(WM_USER+1024)

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__XTCOLORSHEET_H__)
