//:Ignore
// XTToolBarCtrl.h interface for the CXTToolBarCtrl class.
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

#if !defined(__XTTOOLBARCTRL_H__)
#define __XTTOOLBARCTRL_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTToolBarCtrl is a CToolBarCtrl derived class.  The CXTToolBarCtrl
// class provides the functionality of the Windows toolbar common control.
// This control (and therefore the CXTToolBarCtrl class) is available
// only to programs running under Windows 95 and Windows NT version 3.51
// and later.
//
// A Windows toolbar common control is a rectangular child window that
// contains one or more buttons.  These buttons can display a bitmap image,
// a string, or both.  When the user chooses a button, it sends a command
// message to the toolbar’s owner window.  Typically, the buttons in a
// toolbar correspond to items in the application’s menu.  They provide
// a more direct way for the user to access an application’s commands.
//
// CXTToolBarCtrl objects contain several important internal data structures:
// a list of button image bitmaps or an image list, a list of button label
// stings, and a list of TBBUTTON structures which associate an image and/or
// string with the position, style, state, and command ID of the button.
// Each of the elements of these data structures is referred to by a zero-based
// index.  Before you can use a CXTToolBarCtrl object, you must set up
// these data structures.  The list of strings can only be used for button
// labels.  You cannot retrieve strings from the toolbar.
//
// To use a CXTToolBarCtrl object, you will typically follow these steps: 
// [ol]
// [li]Construct the CXTToolBarCtrl object.[/li]
// [li]Call Create to create the Windows toolbar common control and attach
// it to the CXTToolBarCtrl object.  Indicate the style of toolbar by using
// styles, such as TBSTYLE_TRANSPARENT for a transparent toolbar, or 
// TBSTYLE_DROPDOWN for a toolbar that supports dropdown style buttons.[/li]
// [li]Identify how you want the buttons on the toolbar displayed:
//     [ul]
//     [li]To use bitmap images for buttons, add the button bitmaps to the
//     toolbar by calling AddBitmap.[/li]
//     [li]To use images displayed from an image list for buttons, specify
//     the image list by calling SetImageList, SetHotImageList, or 
//     SetDisabledImageList.[/li]
//     [li]To use string labels for buttons, add the strings to the toolbar
//     by calling AddString and/or AddStrings.[/li]
//     [/ul][/li]
// [li]Add button structures to the toolbar by calling AddButtons.[/li]
// [li]If you want tool tips for a toolbar button, in an owner window,
// that is not a CFrameWnd, you need to handle the TTN_NEEDTEXT messages
// in the toolbar’s owner window as described in CXTToolBarCtrl: Handling
// Tool Tip Notifications.  If the parent window of the toolbar is derived
// from CFrameWnd, tool tips are displayed without any extra effort from
// you because CFrameWnd provides a default handler.[/li] 
// [li]If you want your user to be able to customize the toolbar, make
// a call to CXTToolBar::EnableCustomization(bool bEnable).[/li]
// [/ol]
// You can use SaveState to save the current state of a toolbar control
// in the registry and RestoreState to restore the state based on information
// previously stored in the registry.  In addition to saving the toolbar
// state between uses of the application, applications typically store
// the state before the user begins customizing the toolbar in case the
// user later wants to restore the toolbar to its original state.
//
// [b]Support for Internet Explorer Version 4.0 and Later[/b]
//
// To support functionality introduced in Internet Explorer, version 4.0
// and later, MFC provides image list support and transparent and flat
// styles for toolbar controls. 
//
// A transparent toolbar allows the client under the toolbar to show through.
// To create a transparent toolbar, use both TBSTYLE_FLAT and TBSTYLE_TRANSPARENT
// styles.  Transparent toolbars feature hot tracking; that is, when the
// mouse pointer moves over a hot button on the toolbar, the button's appearance
// changes.  Toolbars created with just the TBSTYLE_FLAT style will contain
// buttons that are not transparent.
//
// Image list support allows a control greater flexibility for default
// behavior, hot images, and disabled images.  Use GetImageList, GetHotImageList,
// and GetDisabledImageList with the transparent toolbar to manipulate
// the image according to its state.
//
class _XT_EXT_CLASS CXTToolBarCtrl : public CToolBarCtrl
{
	DECLARE_DYNAMIC(CXTToolBarCtrl)

public:
	
    // Constructs a CXTToolBarCtrl object.
    //
	CXTToolBarCtrl();
	
	// Destroys a CXTToolBarCtrl object, handles cleanup and de-allocation.
    //
    virtual ~CXTToolBarCtrl();

    // This member function is used to create a CXTToolBarCtrl.  You construct
	// a CXTToolBarCtrl in two steps.  First, call the constructor.  Then,
	// call Create, which creates the toolbar control and attaches it to the
	// CXTToolBarCtrl object.
    //
    // The toolbar control automatically sets the size and position of
	// the toolbar window.  The height is based on the height of the buttons
	// in the toolbar.  The width is the same as the width of the parent window’s
	// client area.  The CCS_TOP and CCS_BOTTOM styles determine whether the
	// toolbar is positioned along the top or bottom of the client area. 
	// By default, a toolbar has the CCS_TOP style.
    //
    // Apply the following window styles to a toolbar control. 
    //[ul]
    //[li][b]WS_CHILD[/b] - Always[/li]
    //[li][b]WS_VISIBLE[/b] - Usually[/li]
    //[li][b]WS_DISABLED[/b] - Rarely[/li]
    //[/ul]
    // Next, you may want to apply one or more of the common control styles: 
    //[ul]
    //[li][b]CCS_ADJUSTABLE[/b] - Allows toolbars to be customized by the
	//       user.  If this style is used, the toolbar’s owner window must
	//       handle the customization notification messages sent by the toolbar,
	//       as described in CXTToolBarCtrl: Handling Customization Notifications.[/li]
    //[li][b]CCS_BOTTOM[/b] - Causes the control to position itself at
	//       the bottom of the parent window’s client area and sets the width
	//       to be the same as the parent window’s width.[/li]
    //[li][b]CCS_NODIVIDER[/b] - Prevents a two-pixel highlight from being
	//       drawn at the top of the control.[/li]
    //[li][b]CCS_NOHILITE[/b] - Prevents a one-pixel highlight from being
	//       drawn at the top of the control.[/li]
    //[li][b]CCS_NOMOVEY[/b] - Causes the control to resize and move itself
	//       horizontally, but not vertically, in response to a WM_SIZE message.
	//       If the CCS_NORESIZE style is used, this style does not apply.[/li]
    //[li][b]CCS_NOPARENTALIGN[/b] - Prevents the control from automatically
	//       moving to the top or bottom of the parent window.  Instead, the
	//       control keeps its position within the parent window despite
	//       changes to the size of the parent window.  If the CCS_TOP or
	//       CCS_BOTTOM style is also used, the height is adjusted to the
	//       default, but the position and width remain unchanged.[/li]
    //[li][b]CCS_NORESIZE[/b] - Prevents the control from using the default
	//       width and height when setting its initial size or a new size.
	//       Instead, the control uses the width height specified in the request
	//       for creation or sizing.[/li]
    //[li][b]CCS_TOP[/b] - Causes the control to position itself at the
	//       top of the parent window’client area and sets the width to be
	//       the same as the parent window’s width.  Toolbars have this style
	//       by default.[/li]
    //[/ul]
    // Finally, apply a combination of toolbar styles to either the control
	// or the buttons themselves.  The styles are described in the topic "Toolbar
	// Control and Button Styles" in the Platform SDK. Returns TRUE if successful,
	// otherwise returns FALSE.
	//
	virtual BOOL Create(
        // Specifies the toolbar control’s style.  Toolbars must always
		// have the WS_CHILD style. In addition, you can specify any combination
		// of toolbar styles and window styles.
        DWORD dwStyle,
        // Optionally specifies the toolbar control’s size and position.
		// It can be either a CRect object or a RECT structure.
        const RECT& rect,
        // Specifies the toolbar control’s parent window.  It must [b]not[/b] be NULL.
        CWnd* pParentWnd,
        // Specifies the toolbar control’s ID.
        UINT nID);

    // Call this member function to get the size of a toolbar button. Returns
	// a DWORD value that contains the width and height values in the LOWORD
	// and HIWORD, respectively.
    // 
	DWORD GetButtonSize() const;

    // This member function implements the behavior of the Win32 message
	// TB_GETDISABLEDIMAGELIST, as described in the Platform SDK.
    //
    // The MFC implementation of GetDisabledImageList uses a CImageList
	// object containing the toolbar control's button images, rather than
	// a handle to an image list. Returns a pointer to a CImageList object,
	// or NULL, if no disabled image list is set. 
    // 
	CImageList* GetDisabledImageList() const;

    // This member function implements the behavior of the Win32 message
	// TB_GETHOTIMAGELIST, as described in the Platform SDK.
    //
    // A hot button appears highlighted when the mouse pointer is above
	// it. Returns a pointer to a CImageList object, or NULL, if no disabled
	// image list is set.
    // 
	CImageList* GetHotImageList() const;

    // This member function implements the behavior of the Win32 message
	// TB_GETIMAGELIST, as described in the Platform SDK. Returns a pointer
	// to a CImageList object, or NULL if no disabled image list is set.
    // 
	CImageList* GetImageList() const;

    // Call this member function to get the styles currently applied to
	// a toolbar control. Returns a DWORD containing a combination of toolbar
	// control styles, as described in the Platform SDK.
    // 
	DWORD GetStyle() const;

    // Call this member function to retrieve the maximum number of text
	// rows displayed on a toolbar button. Returns the maximum number of text
	// rows displayed.
    // 
	int GetMaxTextRows() const;

    // Call this member function to check the highlight state of a toolbar
	// button. Returns nonzero if the button is highlighted, otherwise returns zero.
    // 
	BOOL IsButtonHighlighted(
        // The command ID for the toolbar button.
        int nID) const;

    // This member function implements the behavior of the Win32 message
	// TB_SETBUTTONWIDTH, as described in the Platform SDK. Returns nonzero
	// if successful, otherwise returns zero.
    // 
	BOOL SetButtonWidth(
        // Minimum button width, in pixels. Toolbar buttons will never
		// be narrower than this value.
        int cxMin,
        // Maximum button width, in pixels. If button text is too wide,
		// the control displays it with ellipsis points.
        int cxMax);

    // This member function implements the behavior of the Win32 message
	// TB_SETDISABLEDIMAGELIST, as described in the Platform SDK.
    //
    // The MFC implementation of SetDisabledImageList uses a CImageList
	// object containing the toolbar control's disabled button images, rather
	// than a handle to an image list. Returns a pointer to a CImageList object
	// that was previously used by the toolbar control to display disabled
	// button images. 
    // 
	CImageList* SetDisabledImageList(
        // A pointer to a CImageList object containing the images to be
		// used by the toolbar control to display disabled button images.
        CImageList* pImageList);

    // This member function implements the behavior of the Win32 message
	// TB_SETHOTIMAGELIST, as described in the Platform SDK.
    //
    // The MFC implementation of SetHotImageList uses a CImageList object
	// containing the toolbar control's hot button images, rather than a handle
	// to an image list. 
    //
    // A hot button appears highlighted when the pointer is above it. Returns
	// a pointer to a CImageList object that was previously used by the toolbar
	// control to display hot button images. 
    // 
	CImageList* SetHotImageList(
        // A pointer to a CImageList object containing the images to be
		// used by the toolbar control to display hot button images.
        CImageList* pImageList);

    // This member function implements the behavior of the Win32 message
	// TB_SETIMAGELIST, as described in the Platform SDK.
    //
    // The MFC implementation of SetImageList uses a CImageList object
	// containing the toolbar control's button images, rather than a handle
	// to an image list. Returns a pointer to a CImageList object that was
	// previously used by the toolbar control to display button images in
	// their default state. 
    // 
	CImageList* SetImageList(
        // A pointer to a CImageList object containing the images to be
		// used by the toolbar control to display button images in their default
		// state.
        CImageList* pImageList);

    // This member function implements the behavior of the Win32 message
	// TB_GETOBJECT, as described in the Platform SDK. Returns an HRESULT
	// value indicating success or failure of the operation.
    // 
	HRESULT GetDropTarget(
        // A pointer to an IDropTarget interface pointer.  If an error
		// occurs, a NULL pointer is placed in this address.
        IDropTarget** ppDropTarget) const;

    // Call this member function to set the indentation for the first button
	// in a toolbar control. Returns nonzero if successful, otherwise returns
	// zero. 
    // 
	BOOL SetIndent(
        // The value specifying the indentation, in pixels.
        int iIndent);

    // Call this member function to set the maximum number of text rows
	// displayed on a toolbar button.  Returns nonzero if successful, otherwise
	// returns zero. 
    // 
	BOOL SetMaxTextRows(
        // Maximum number of rows to be set. 
        int iMaxRows);

    // Call this member function to set the styles for a toolbar control.
    // 
	void SetStyle(
        // A DWORD containing a combination of toolbar control styles,
        // as described in the Platform SDK.
        DWORD dwStyle);

    // This member function implements the behavior of the Win32 message
	// TB_GETBUTTONINFO, as described in the Platform SDK. Returns nonzero
	// if successful, otherwise returns zero.
    // 
	BOOL GetButtonInfo(
        // The button identifier.
        int nID,
        // A pointer to a TBBUTTONINFO structure that receives the button
		// information.
        TBBUTTONINFO* ptbbi) const;

    // The member function implements the behavior of the Win32 message
	// TB_SETBUTTONINFO, as described in the Platform SDK. Returns nonzero
	// if successful, otherwise returns zero.
    // 
	BOOL SetButtonInfo(
        // The button identifier.
        int nID,
        // A pointer to a TBBUTTONINFO structure that receives the button
		// information.
        TBBUTTONINFO* ptbbi);

    // This member function implements the behavior of the Win32 message
	// TB_SETDRAWTEXTFLAGS, as described in the Platform SDK.
    //
    // This member function sets the flags in the Win32 function DrawText,
	// which draws text in the specified rectangle, formatted according to
	// how the flags are set. Returns a DWORD value containing the previous text
	// drawing flags.
    // 
	DWORD SetDrawTextFlags(
        // A combination of one or more of the DT_ flags, specified in
		// the Win32 function, DrawText, that indicates which bits in 'dwDTFlags'
		// will be used when drawing the text.
        DWORD dwMask,
        // A combination of one or more of the DT_ flags, specified in
		// the Win32 function, DrawText, that indicate how the button text will
		// be drawn.  This value is passed to DrawText when the button text is
		// drawn.
        DWORD dwDTFlags);

    // This member function implements the behavior of the Win32 message
	// TB_GETANCHORHIGHLIGHT, as described in the Platform SDK. Returns nonzero,
	// if anchor highlighting is enabled. If zero, anchor highlighting is disabled.
    // 
	BOOL GetAnchorHighlight() const;

    // This member function implements the behavior of the Win32 message
	// TB_SETANCHORHIGHLIGHT, as described in the Platform SDK. Returns nonzero
	// if successful, otherwise returns zero.
    // 
	BOOL SetAnchorHighlight(
        // Specifies if anchor highlighting is enabled or disabled.  If
		// this value is nonzero, anchor highlighting will be enabled.  If this
		// value is zero, anchor highlighting will be disabled.
        BOOL fAnchor = TRUE);

    // This member function implements the behavior of the Win32 message
	// TB_GETHOTITEM, as described in the Platform SDK. Returns the zero-based
	// index of the hot item in a toolbar.
    // 
	int GetHotItem() const;

    // This member function implements the behavior of the Win32 message
	// TB_SETHOTITEM, as described in the Platform SDK. Returns the index
	// of the previous hot item, or -1, if there was no hot item. 
    // 
	int SetHotItem(
        // The zero-based index number of the item that will be made hot.
		// If this value is -1, none of the items will be hot. 
        int nHot);

    // This member function implements the behavior of the Win32 message
	// TB_GETINSERTMARK, as described in the Platform SDK.
    // 
	void GetInsertMark(
        // A pointer to a TBINSERTMARK structure that receives the insertion mark.
        TBINSERTMARK* ptbim) const;

    // This member function implements the behavior of the Win32 message
	// TB_SETINSERTMARK, as described in the Platform SDK.
    // 
	void SetInsertMark(
        // A pointer to the TBINSERTMARK structure that contains the insertion
		// mark. 
        TBINSERTMARK* ptbim);

    // This member function implements the behavior of the Win32 message
	// TB_GETMAXSIZE, as described in the Platform SDK. Returns nonzero if
	// successful, otherwise returns zero.
    // 
	BOOL GetMaxSize(
        // A pointer to a SIZE structure that receives the size of the items. 
        LPSIZE pSize) const;

    // This member function implements the behavior of the Win32 message
	// TB_INSERTMARKHITTEST, as described in the Platform SDK. Returns nonzero
	// if successful, otherwise returns zero.
    // 
	BOOL InsertMarkHitTest(
        // A pointer to a POINT structure that contains the hit test coordinates,
		// relative to the client area of the toolbar.
        LPPOINT ppt,
        // A pointer to a TBINSERTMARK structure that receives the insertion
		// mark information. 
        LPTBINSERTMARK ptbim) const;

    // This member function implements the behavior of the Win32 message
	// TB_GETEXTENDEDSTYLE, as described in the Platform SDK. Returns a DWORD
	// that represents the extended styles currently in use for the toolbar
	// control.  For a list of styles, see "Toolbar Extended Styles," in the
	// Platform SDK.
    // 
	DWORD GetExtendedStyle() const;

    // This member function implements the behavior of the Win32 message
	// TB_SETEXTENDEDSTYLE, as described in the Platform SDK. Returns a DWORD
	// value that represents the previous extended styles. For a list of styles,
	// see "Toolbar Extended Styles," in the Platform SDK.
    // 
	DWORD SetExtendedStyle(
        // A value specifying the new extended styles.  This parameter
		// can be a combination of the toolbar extended styles.
        DWORD dwExStyle);

    // This member function implements the behavior of the Win32 message
	// TB_GETINSERTMARKCOLOR, as described in the Platform SDK. Returns a
	// COLORREF value that contains the current insertion mark color. 
    // 
	COLORREF GetInsertMarkColor() const;

    // This member function implements the behavior of the Win32 message
	// TB_SETINSERTMARKCOLOR, as described in the Platform SDK. Returns a
	// COLORREF value that contains the previous insertion mark color.
    // 
	COLORREF SetInsertMarkColor(
        // A COLORREF value that contains the new insertion mark color.
        COLORREF clrNew);

    // This member function implements the behavior of the Win32 message
	// TB_MAPACCELERATOR, as described in the Platform SDK. Returns nonzero
	// if successful, otherwise returns zero.
    // 
	BOOL MapAccelerator(
        // Accelerator character to be mapped.  This character is the same
		// character that is underlined in the button's text.
        TCHAR chAccel,
        // A pointer to a UINT that receives the command identifier of
		// the button that corresponds to the accelerator specified in 'chAccel'.
        UINT* pIDBtn);

    // This member function implements the behavior of the Win32 message
	// TB_MARKBUTTON, as described in the Platform SDK. Returns nonzero if
	// successful, otherwise returns zero.
    // 
	BOOL MarkButton(
        // The button identifier.
        int nID,
        // Specifies the highlight state to be set.  By default, TRUE.
		// If set to FALSE, the button is set to its default state. 
        BOOL bHighlight = TRUE);

    // This member function implements the behavior of the Win32 message
	// TB_MOVEBUTTON, as described in the Platform SDK. Returns nonzero if
	// successful, otherwise returns zero.
    // 
	BOOL MoveButton(
        // The zero-based index of the button to be moved.
        UINT nOldPos,
        // The zero-based index of the button's destination.
        UINT nNewPos);

    // This member function implements the behavior of the Win32 message
	// TB_HITTEST, as described in the Platform SDK. Returns an integer value
	// indicating the location of a point on a toolbar.  If the value is zero
	// or a positive value, this return value is the zero-based index of the
	// non-separator item in which the point lies.  If the return value is
	// negative, the point does not lie within a button.  The absolute value
	// of the return value is the index of a separator item or the nearest
	// non-separator item.
    // 
	int HitTest(
        // A pointer to a POINT structure that contains the x-coordinate
		// of the hit test in the x member and the y-coordinate of the hit test
		// in the y member.  The coordinates are relative to the toolbar's client
		// area.
        LPPOINT ppt) const;

	//:Ignore
    //{{AFX_VIRTUAL(CXTToolBarCtrl)
	//}}AFX_VIRTUAL
    //:End Ignore

protected:
	//:Ignore
    //{{AFX_MSG(CXTToolBarCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
    //:End Ignore

	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE DWORD CXTToolBarCtrl::GetButtonSize() const {
	ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, TB_GETBUTTONSIZE, 0, 0L);
}
AFX_INLINE CImageList* CXTToolBarCtrl::GetDisabledImageList() const {
	ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, TB_GETDISABLEDIMAGELIST, 0, 0));
}
AFX_INLINE CImageList* CXTToolBarCtrl::GetHotImageList() const {
	ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, TB_GETHOTIMAGELIST, 0, 0));
}
AFX_INLINE CImageList* CXTToolBarCtrl::GetImageList() const {
	ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, TB_GETIMAGELIST, 0, 0));
}
AFX_INLINE DWORD CXTToolBarCtrl::GetStyle() const {
	ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, TB_GETSTYLE, 0, 0L);
}
AFX_INLINE int CXTToolBarCtrl::GetMaxTextRows() const {
	ASSERT(::IsWindow(m_hWnd)); return (INT) ::SendMessage(m_hWnd, TB_GETTEXTROWS, 0, 0L);
}
AFX_INLINE BOOL CXTToolBarCtrl::IsButtonHighlighted(int nID) const {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_ISBUTTONHIGHLIGHTED, nID, 0);
}
AFX_INLINE BOOL CXTToolBarCtrl::SetButtonWidth(int cxMin, int cxMax) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_SETBUTTONWIDTH, 0, MAKELPARAM(cxMin, cxMax));
}
AFX_INLINE CImageList* CXTToolBarCtrl::SetHotImageList(CImageList* pImageList) {
	ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, TB_SETHOTIMAGELIST, 0, (LPARAM)pImageList->GetSafeHandle()));
}
AFX_INLINE CImageList* CXTToolBarCtrl::SetImageList(CImageList* pImageList) {
	ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, TB_SETIMAGELIST, 0, (LPARAM)pImageList->GetSafeHandle()));
}
AFX_INLINE BOOL CXTToolBarCtrl::SetIndent(int iIndent) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_SETINDENT, iIndent, 0L);
}
AFX_INLINE BOOL CXTToolBarCtrl::SetMaxTextRows(int iMaxRows) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_SETMAXTEXTROWS, iMaxRows, 0L);
}
AFX_INLINE void CXTToolBarCtrl::SetStyle(DWORD dwStyle) {
	ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, TB_SETSTYLE, 0, dwStyle);
}
AFX_INLINE BOOL CXTToolBarCtrl::GetButtonInfo(int nID, TBBUTTONINFO* ptbbi) const {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_GETBUTTONINFO, nID, (LPARAM)ptbbi);
}
AFX_INLINE BOOL CXTToolBarCtrl::SetButtonInfo(int nID, TBBUTTONINFO* ptbbi) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_SETBUTTONINFO, nID, (LPARAM)ptbbi);
}
AFX_INLINE DWORD CXTToolBarCtrl::SetDrawTextFlags(DWORD dwMask, DWORD dwDTFlags) {
	ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, TB_SETDRAWTEXTFLAGS, dwMask, dwDTFlags);
}
AFX_INLINE BOOL CXTToolBarCtrl::GetAnchorHighlight() const {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_GETANCHORHIGHLIGHT, 0, 0);
}
AFX_INLINE BOOL CXTToolBarCtrl::SetAnchorHighlight(BOOL fAnchor) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_SETANCHORHIGHLIGHT, fAnchor, 0);
}
AFX_INLINE int CXTToolBarCtrl::GetHotItem() const {
	ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, TB_GETHOTITEM, 0, 0);
}
AFX_INLINE int CXTToolBarCtrl::SetHotItem(int nHot) {
	ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, TB_SETHOTITEM, nHot, 0);
}
AFX_INLINE void CXTToolBarCtrl::GetInsertMark(TBINSERTMARK* ptbim) const {
	ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, TB_GETINSERTMARK, 0, (LPARAM)ptbim);
}
AFX_INLINE void CXTToolBarCtrl::SetInsertMark(TBINSERTMARK* ptbim) {
	ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, TB_SETINSERTMARK, 0, (LPARAM)ptbim);
}
AFX_INLINE BOOL CXTToolBarCtrl::GetMaxSize(LPSIZE pSize) const {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_GETMAXSIZE, 0, (LPARAM)pSize);
}
AFX_INLINE BOOL CXTToolBarCtrl::InsertMarkHitTest(LPPOINT ppt, LPTBINSERTMARK ptbim) const {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_INSERTMARKHITTEST, (WPARAM)ppt, (LPARAM)ptbim);
}
AFX_INLINE DWORD CXTToolBarCtrl::GetExtendedStyle() const {
	ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, TB_GETEXTENDEDSTYLE, 0, 0L);
}
AFX_INLINE DWORD CXTToolBarCtrl::SetExtendedStyle(DWORD dwExStyle) {
	ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, TB_SETEXTENDEDSTYLE, 0, dwExStyle);
}
AFX_INLINE COLORREF CXTToolBarCtrl::GetInsertMarkColor() const {
	ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, TB_GETINSERTMARKCOLOR, 0, 0);
}
AFX_INLINE COLORREF CXTToolBarCtrl::SetInsertMarkColor(COLORREF clrNew) {
	ASSERT(::IsWindow(m_hWnd)); return (COLORREF) ::SendMessage(m_hWnd, TB_SETINSERTMARKCOLOR, 0, (LPARAM) clrNew);
}
AFX_INLINE BOOL CXTToolBarCtrl::MapAccelerator(TCHAR chAccel, UINT* pIDBtn) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_MAPACCELERATOR, (WPARAM)chAccel, (LPARAM)pIDBtn);
}
AFX_INLINE BOOL CXTToolBarCtrl::MarkButton(int nID, BOOL bHighlight) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_MARKBUTTON, nID, MAKELPARAM(bHighlight, 0));
}
AFX_INLINE BOOL CXTToolBarCtrl::MoveButton(UINT nOldPos, UINT nNewPos) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_MOVEBUTTON, nOldPos, nNewPos);
}
AFX_INLINE int CXTToolBarCtrl::HitTest(LPPOINT ppt) const {
	ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, TB_HITTEST, 0, (LPARAM)ppt);
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTTOOLBARCTRL_H__)
