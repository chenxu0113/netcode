//:Ignore
// XTResize.h: interface for the CXTResize class.
//
// This file is a part of the Xtreme Toolkit for MFC.
// ©1998-2003 Codejock Software, All Rights Reserved.
//
// This source code can only be used under the terms and conditions 
// outlined in the accompanying license agreement.
//
// support@codejock.com
// http://www.codejock.com
//--------------------------------------------------------------------
// Based on the resizable classes created by Torben B. Haagh. Used by permission.
// http://www.codeguru.com/dialog/torbenResizeDialog.shtml
//--------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////
//:End Ignore

#if !defined(__XTRESIZE_H__)
#define __XTRESIZE_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// The XT_SIZING structure is a stand alone helper structure class.  It
// is used by the CXTResize class to maintain size and ID for a particular
// window being sized.
//
struct XT_SIZING
{
	// Control identifier of the window sized.
	UINT id;
	// Size of the window sized.
	XT_RESIZERECT rrc; 
};

//////////////////////////////////////////////////////////////////////
// The CXTSizeIcon class is a CStatic derived helper class. It is used
// by CXTResize to display the sizing grip in the lower right corner of
// a sizing window.
//
class _XT_EXT_CLASS CXTSizeIcon : public CStatic
{
public:
	
	// Constructs a CXTSizeIcon object.
	//
	CXTSizeIcon();

protected:

	// Handle to the cursor displayed for the size icon.
	HCURSOR m_hCursor;
	// Size icon bitmap
	CBitmap	m_bmSizeIcon;

	//:Ignore
	//{{AFX_MSG(CXTSizeIcon)
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	//:End Ignore
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////
// CXTItem is a stand alone helper class.  It is used by CXTResize to
// maintain information about each item to be sized or moved.
//
class _XT_EXT_CLASS CXTItem
{
public:
	
	// Constructs a CXTItem object.
	//
	CXTItem(
		// Pointer to the window to be sized or moved.
		CWnd* pwnd,
		// Reference to a CXTResizeRect object.
		const CXTResizeRect& rrcSizing,
		// Reference to a CRect object.
		CRect& rcWindow,
		// TRUE if the window is to be deleted.
		BOOL bDeleteWnd);

	// Destroys a CXTItem object, handles cleanup and de-allocation.
    //
	virtual ~CXTItem();
	
	// A pointer to the window to be sized or moved.
	CWnd* m_pwnd;
	// Sizing option.
	CXTResizeRect m_rrcSizing;
	// Last control size.
	CXTResizeRect m_rrcWindow;
	// Initial control size.
	CXTResizeRect m_rrcInitWindow;
	// TRUE if the window is to be deleted
	BOOL m_bDeleteWnd;
	// Initial size/move has been completed.
	BOOL m_bInitialSize;
};

//////////////////////////////////////////////////////////////////////
// CXTResize is a base class.  It is used by resizing dialogs, property
// sheets, and form views.  It acts as a manager to maintain size and location
// of the dialog and dialog items.
//
class _XT_EXT_CLASS CXTResize
{
public:
    // Constructs a CXTResize object.
    //
	CXTResize(
		// Points to the parent or owner window object, of type CWnd, to which the
		// resizing object belongs.
		CWnd* pwnd,
		// Flags that are to be passed to CXTResize that specify the attributes
		// of the resizing property page.  They can be one or more of the following,
		// and can be combined using the or (|) operator:
		//[ul]
        //[li][b]SZ_NOSIZEICON[/b] - Do not add size icon.[/li]
        //[li][b]SZ_NOHORISONTAL[/b] - No horizontal resizing.[/li]
        //[li][b]SZ_NOVERTICAL[/b] - No vertical resizing.[/li]
        //[li][b]SZ_NOMINSIZE[/b] - Do not require a minimum size.[/li]
        //[li][b]SZ_NOCLIPCHILDREN[/b] - Do not set clip children style.[/li]
        //[li][b]SZ_NOTRANSPARENTGROUP[/b] - Do not set transparent style
		//       for group boxes.[/li]
		//[/ul]
		const UINT nFlags = 0);

	// Destroys a CXTResize object, handles cleanup and de-allocation.
    //
	virtual ~CXTResize();

	// The SetResize function specifies how much each side of a control will
	// move when the dialog is resized.  If a control should be repositioned
	// (e.g. an OK button) then all four sides should move by the same amount
	// of pixels, as the dialog is resized.  If a control should be resized
	// just as much as the dialog (e.g. the list control in the file dialog),
	// then the left and top sides shouldn't move, and the right and bottom
	// sides should move by the same amount of pixels as the dialog.
	//
	void SetResize(
		// Specifies the control's ID.
		const UINT nID,
		// How much the left side will move when the dialog is resized.
		XT_RESIZE left,
		// How much the top side will move when the dialog is resized.
		XT_RESIZE top,
		// How much the right side will move when the dialog is resized.
		XT_RESIZE right,
		// How much the bottom side will move when the dialog is resized.
		XT_RESIZE bottom);

	// The SetResize function specifies how much each side of a control will
	// move when the dialog is resized.  If a control should be repositioned
	// (e.g. an OK button) then all four sides should move by the same amount
	// of pixels, as the dialog is resized.  If a control should be resized
	// just as much as the dialog (e.g. the list control in the file dialog),
	// then the left and top sides shouldn't move, and the right and bottom
	// sides should move by the same amount of pixels as the dialog.
	//
	void SetResize(
		// Specifies the control's ID.
		const UINT nID,
		// How much the left, top, right and bottom sides will move when 
		// the dialog is resized.
		const XT_RESIZERECT& rrcSizing);

	// The SetResize function specifies how much each side of a control will
	// move when the dialog is resized.  If a control should be repositioned
	// (e.g. an OK button) then all four sides should move by the same amount
	// of pixels, as the dialog is resized.  If a control should be resized
	// just as much as the dialog (e.g. the list control in the file dialog),
	// then the left and top sides shouldn't move, and the right and bottom
	// sides should move by the same amount of pixels as the dialog.
	//
	void SetResize(
		// Specifies the control's ID.
		const UINT nID,
		// HWND of the dialog item to be sized.
		const HWND hWnd,
		// How much the left, top, right and bottom sides will move when 
		// the dialog is resized.
		const XT_RESIZERECT& rrcSizing);

	// The SetResize function specifies how much each side of a control will
	// move when the dialog is resized.  If a control should be repositioned
	// (e.g. an OK button) then all four sides should move by the same amount
	// of pixels, as the dialog is resized.  If a control should be resized
	// just as much as the dialog (e.g. the list control in the file dialog),
	// then the left and top sides shouldn't move, and the right and bottom
	// sides should move by the same amount of pixels as the dialog.
	//
	void SetResize(
		// Specifies the control's ID.
		const UINT nID,
		// HWND of the dialog item to be sized.
		const HWND hWnd,
		// How much the top and left sides will move when the dialog is resized.
		const XT_RESIZEPOINT& rpTopLeft,
		// How much the bottom and right sides will move when the dialog is resized.
		const XT_RESIZEPOINT& rpBottomRight);

	// The SetResize function specifies how much each side of a control will
	// move when the dialog is resized.  If a control should be repositioned
	// (e.g. an OK button) then all four sides should move by the same amount
	// of pixels, as the dialog is resized.  If a control should be resized
	// just as much as the dialog (e.g. the list control in the file dialog),
	// then the left and top sides shouldn't move, and the right and bottom
	// sides should move by the same amount of pixels as the dialog.
	//
	void SetResize(
		// Specifies the control's ID.
		const UINT nID,
		// How much the top and left sides will move when the dialog is resized.
		const XT_RESIZEPOINT& rpTopLeft,
		// How much the bottom and right sides will move when the dialog is resized.
		const XT_RESIZEPOINT& rpBottomRight);

	// The SetResize function specifies how much each side of a control will
	// move when the dialog is resized.  If a control should be repositioned
	// (e.g. an OK button) then all four sides should move by the same amount
	// of pixels, as the dialog is resized.  If a control should be resized
	// just as much as the dialog (e.g. the list control in the file dialog),
	// then the left and top sides shouldn't move, and the right and bottom
	// sides should move by the same amount of pixels as the dialog.
	//
	void SetResize(
		// Array of XT_SIZING structures that specify how much the left, top,
		// right and bottom sides of the dialog item will move when the dialog
		// is resized.
		XT_SIZING arr[]);

	// This member function sets the minimum size explicitly.  Initial size
	// is the default.
	//
	void SetMinSize(
		// Specifies the minimum width and height the dialog can be sized to.
		CSize& sz);

	// This member function sets the maximum size.  No maximum is the default.
	//
	void SetMaxSize(
		// Specifies the maximum width and height the dialog can be sized to.
		CSize& sz);

	// This member function saves the window placement to the registry.
	//
	void SavePlacement(
		// Name of a section in the initialization file or a key in the Windows
		// registry where placement information is stored.
		LPCTSTR pszSection);

	// This member function loads saved window placement information from
	// the registry.
	//
	void LoadPlacement(
		// Name of a section in the initialization file or a key in the Windows
		// registry where placement information is stored.
		LPCTSTR pszSection);

	// This member function loads saved window placement information from
	// the registry.  This version is the same as LoadPlacement, but there
	// is no need for calling SavePlacement when the window is destroyed,
	// this will be called automatically.
	//
	void AutoLoadPlacement(
		// Name of a section in the initialization file or a key in the Windows
		// registry where placement information is stored.
		LPCTSTR pszSection);

protected:

	// This member function is used by the resize manager to add a dialog
	// item to the list of items to be resized.
	//
	void SetResize(
		// Points to the dialog item to be resized.
		CWnd* pwnd,
		// How much the left, top, right, and bottom sides will move when the
		// dialog is resized.
		const CXTResizeRect& rrcSizing,
		// Initial size of the dialog item.
		CRect rcWindow);

	// This member function is called to remove the specified dialog item
	// from the list of items to be resized. Returns TRUE if successful, otherwise
	// returns FALSE.
	//
	BOOL RemoveResize(
		// Specifies the control's ID.
		const UINT nID);

	// This member function is called to purge the list that contains dialog
	// items to be sized.
	//
	void RemoveAllControls();

	// This member function is called from OnInitDialog or OnInitialUpdate
	// to initialize the resize manager.
	//
	void Init();

	// This member function is called from OnSize to move and resize the dialog
	// items that are managed.
	//
	void Size();

	// This member function is called, when a property sheet in wizard mode
	// has changed pages, to alert the resize manager that the property sheet
	// (common control) has moved the page back to its original size/position
	// on the sheet.
	//
	void Reset();

	// This member function is called from OnGetMinMaxInfo to get the
	// maximized position or dimensions, or the minimum or maximum tracking
	// size.  The maximized size is the size of the window when its borders
	// are fully extended.  The maximum tracking size of the window is the
	// largest window size that can be achieved by using the borders to size
	// the window.  The minimum tracking size of the window is the smallest
	// window size that can be achieved by using the borders to size the window.
	//
	void GetMinMaxInfo(
		// Points to a MINMAXINFO structure that contains information about a
		// window’s maximized size and position, and its minimum and maximum
		// tracking size.  For more information about this structure, see the
		// MINMAXINFO structure.
		MINMAXINFO* pMMI);

protected: // flags
	
	// Style attributes for the resizing dialog, property sheet, or form view.
	//
	enum EFlags
	{
		// Do not add size icon.
		SZ_NOSIZEICON			= 0x01,
		// No horizontal resizing.
		SZ_NOHORISONTAL			= 0x02,
		// No vertical resizing.
		SZ_NOVERTICAL			= 0x04,
		// Do not require a minimum size.
		SZ_NOMINSIZE			= 0x08,
		// Do not set clip children style.
		SZ_NOCLIPCHILDREN		= 0x10,
		// Do not set transparent style for group boxes.
		SZ_NOTRANSPARENTGROUP	= 0x20,
	};

    // This member function is called to determine if the specified flag
	// has been set for the resize manager.  Returns TRUE if the specified
	// flag has been set, otherwise returns FALSE.
	//
	BOOL HasFlag(
		// Flag to check. It can be one of the following:
		//[ul]
        //[li][b]SZ_NOSIZEICON[/b] - Do not add size icon.[/li]
        //[li][b]SZ_NOHORISONTAL[/b] - No horizontal resizing.[/li]
        //[li][b]SZ_NOVERTICAL[/b] - No vertical resizing.[/li]
        //[li][b]SZ_NOMINSIZE[/b] - Do not require a minimum size.[/li]
        //[li][b]SZ_NOCLIPCHILDREN[/b] - Do not set clip children style.[/li]
        //[li][b]SZ_NOTRANSPARENTGROUP[/b] - Do not set transparent style
		//       for group boxes.[/li]
		//[/ul]
		EFlags eFlag);

    // This member function is called to set a specific flag for the resize
	// manager.
	//
	void SetFlag(
		// Flag to set. It can be one of the following:
		//[ul]
        //[li][b]SZ_NOSIZEICON[/b] - Do not add size icon.[/li]
        //[li][b]SZ_NOHORISONTAL[/b] - No horizontal resizing.[/li]
        //[li][b]SZ_NOVERTICAL[/b] - No vertical resizing.[/li]
        //[li][b]SZ_NOMINSIZE[/b] - Do not require a minimum size.[/li]
        //[li][b]SZ_NOCLIPCHILDREN[/b] - Do not set clip children style.[/li]
        //[li][b]SZ_NOTRANSPARENTGROUP[/b] - Do not set transparent style
		//       for group boxes.[/li]
		//[/ul]
		EFlags eFlag);

protected: // helper methods

	//:Ignore
	BOOL Defer(HDWP&, CXTItem*, int dx, int dy);
	//:End Ignore

protected: // helper data

	//:Ignore
	typedef CArray<CXTItem*, CXTItem*&> CXTItemArray;
	// array of controls
	CXTItemArray m_arrItems;
	// the associative relation to the window to be resized
	CWnd* m_pwnd;
	// flags passed from constructor
	UINT m_nFlagsXX;
	// last dialog size
	CRect m_rcWindow;
	// Initial dialog size
	CRect m_rcInitWindow;
	// smallest size allowed
	CSize m_szMin;
	// largest size allowed
	CSize m_szMax;
	// size icon window
	CXTSizeIcon m_scSizeIcon;
	// section in registry where window placement information is saved.
	CString m_strSection;
	//:End Ignore
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTResize::SetMinSize(CSize& sz) { 
	m_szMin = sz; 
}
AFX_INLINE void CXTResize::SetMaxSize(CSize& sz) { 
	m_szMax = sz; 
}
AFX_INLINE BOOL CXTResize::HasFlag(EFlags eFlag) {
	return (m_nFlagsXX & eFlag) != 0;
}
AFX_INLINE void CXTResize::SetResize(const UINT nID, const HWND hWnd, const XT_RESIZEPOINT& rpTopLeft, const XT_RESIZEPOINT& rpBottomRight) {
	SetResize(nID, hWnd, CXTResizeRect(rpTopLeft.x, rpTopLeft.y, rpBottomRight.x, rpBottomRight.y));
}
AFX_INLINE void CXTResize::SetResize(const UINT nID, const XT_RESIZERECT& rrcSizing) {
	SetResize(nID,NULL,rrcSizing);
}
AFX_INLINE void CXTResize::SetResize(const UINT nID, const XT_RESIZEPOINT& rpTopLeft, const XT_RESIZEPOINT& rpBottomRight) {
	SetResize(nID, CXTResizeRect(rpTopLeft.x, rpTopLeft.y, rpBottomRight.x, rpBottomRight.y));
}
AFX_INLINE void CXTResize::SetResize(const UINT nID, XT_RESIZE left, XT_RESIZE top, XT_RESIZE right, XT_RESIZE bottom) {
	SetResize(nID, CXTResizeRect(left, top, right, bottom));
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

#endif // !defined(__XTRESIZE_H__)
