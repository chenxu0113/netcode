//:Ignore
// XTToolBarPopupWnd.h : header file
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

#if !defined(__POPUPWND_H__)
#define __POPUPWND_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

// forwards
class CXTToolBarPopupWnd;

//////////////////////////////////////////////////////////////////////
// CXTPopupWndToolbar is a CXTToolBar derived class.  It is a toolbar 
// custom tailored for usage in a toolbar popup window.
//
class _XT_EXT_CLASS CXTPopupWndToolbar : public CXTToolBar
{
    DECLARE_DYNAMIC(CXTPopupWndToolbar)

	// Back reference to the toolbar expansion popup.
	CXTToolBarPopupWnd* m_pPopup;

public:

    // Constructs a CXTPopupWndToolbar object.
    //
	CXTPopupWndToolbar();

	// This member function defines the background color as that used in popup
	// windows. Returns an RGB value.
	//
    virtual COLORREF GetBackgroundColor() const;

	// This member function tells the base class to always fill the background
	// of this toolbar with a predefined popup window background color. Returns
	// 'true' if successful, otherwise returns 'false'.
	//
    virtual bool IsFillToolBarClientRect();

	friend CXTToolBarPopupWnd;
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPopupWndToolbar::CXTPopupWndToolbar() {
	m_pPopup = 0;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTToolBarPopupWndHook is a CXTWndHook derived class.  It is used to
// hook a toolbar owner frame, and to figure out when to close the popup.
// Its WindowProc() filters out the commands the owner frame receives
// and, if applicable, sends a request for the toolbar expansion popup
// to close.
//
class _XT_EXT_CLASS CXTToolBarPopupWndHook : public CXTWndHook
{
    DECLARE_DYNAMIC(CXTToolBarPopupWndHook)

	// Back reference to the toolbar expansion popup.
	CXTToolBarPopupWnd* m_pPopup;

	// true if parent popup completed creation
	bool m_bCreated;

public:

    // Constructs a CXTToolBarPopupWndHook object.
    //
	CXTToolBarPopupWndHook();

	// This member function filters messages, as required, to synch up the
	// hidden toolbar popup.
	//
    virtual LRESULT WindowProc(
        // Specifies the Windows message to be processed.
        UINT message,
        // Provides additional information used in processing the message.
		// The parameter value depends on the message.
        WPARAM wParam,
        // Provides additional information used in processing the message.
		// The parameter value depends on the message.
        LPARAM lParam);

	friend CXTToolBarPopupWnd;
};

//////////////////////////////////////////////////////////////////////
// CXTToolBarPopupWnd is a CWnd derived class.  It is used to display a
// popup window to be activated when the user presses the down arrow on
// a toolbar.  The popup window contains a separate toolbar that will swap
// out the image with the newly selected command. Works similar to MS Word's
// border dropdown button.
//
class _XT_EXT_CLASS CXTToolBarPopupWnd : public CWnd
{
    DECLARE_DYNAMIC(CXTToolBarPopupWnd)

public:

    // Constructs a CXTToolBarPopupWnd object.
    //
	CXTToolBarPopupWnd(
		// Command ID that is currently active for the popup window.
		int& iCmdID);
	
	// Destroys a CXTToolBarPopupWnd object, handles cleanup and de-allocation.
    //
	virtual ~CXTToolBarPopupWnd();

protected:
	
	// Pointer to the parent toolbar.
	CXTToolBar* m_pWndParent;
	// Nested toolbar that is displayed when this window is created.
	CXTPopupWndToolbar m_wndToolBar;
	// Resource ID of the parent toolbar.
	UINT m_uToolbarID;
	// Command ID of the button that created this window.
	int& m_iCmdID;
	// Message hook that properly closes this window.
	CXTToolBarPopupWndHook m_hook;
	// Exclusion rectangle for drawing adjacent borders.
	CRect m_rcExclude;

public:

	// Call this member function to create a CXTToolBarPopupWnd object.  This
	// can be done from a CBRN_XT_DROPDOWN after a dropdown arrow has been
	// added to your toolbar. To do so, add a dropdown button to your toolbar.
	// The button image should represent one of the images contained in the
	// toolbar popup window, for example:
	//
	//[pre]m_wndToolBar.AddDropDownButton(ID_BDR_OUTSIDE);[/pre]
	//
	// Once you have done this you can add a CBRN_XT_DROPDOWN message handler
	// to determine when the toolbar button has been dropped, for example:
	//
	//[pre]BEGIN_MESSAGE_MAP(CMainFrame, CXTFrameWnd)
	//    {{AFX_MSG_MAP(CMainFrame)
	//    ON_MESSAGE(CBRN_XT_DROPDOWN, OnToolbarDropDown)
	//    //}}AFX_MSG_MAP
	//END_MESSAGE_MAP()[/pre]
	//
	//[pre]LRESULT CMainFrame::OnToolbarDropDown(WPARAM wParam, LPARAM lParam)
	//{
	//    NMTOOLBAR* pNMTB = ( NMTOOLBAR* )wParam;
	//    CRect* pRect = ( CRect* )lParam;
	//
	//    if ( pNMTB->iItem == m_uCmdID )
	//    {
	//        CXTToolBarPopupWnd* pwndPopup = new CXTToolBarPopupWnd( m_uCmdID );
	//        pwndPopup->Create( &m_wndToolBar, IDR_BORDERS, 5 );
	//        return 1;
	//    }
	//
	//    return 0;
	//}[/pre]
	// 
	// The toolbar command range should be sequential so you can add a command
	// range handler for your popup toolbar, for example:
	//
	//[pre]BEGIN_MESSAGE_MAP(CMainFrame, CXTFrameWnd)
	//    {{AFX_MSG_MAP(CMainFrame)
	//    ON_COMMAND_EX_RANGE(ID_BDR_OUTSIDE, ID_BDR_INSIDE_VERT, OnBorderCommand)
	//    //}}AFX_MSG_MAP
	//END_MESSAGE_MAP()[/pre]
	//
    //[pre]BOOL CMainFrame::OnBorderCommand(UINT nID)
    //{
    //    // TODO: Add your command handler code here
    //    switch (nID)
    //    {
    //    case ID_BDR_OUTSIDE:
    //        break;
    //    case ID_BDR_ALL:
    //        break;
    //    case ID_BDR_TOP:
    //        break;
    //    case ID_BDR_LEFT:
    //        break;
    //    }
    //
    //    return TRUE;
    //}[/pre]
    // 
    // See the FontCombo sample application for an example use of this class.
	// Returns TRUE if successful, otherwise returns FALSE.
	//
	virtual BOOL Create(
		// Points to a valid parent toolbar.
		CXTToolBar* pParent,
		// Resource ID of the toolbar to display as a popup.
		UINT uToolbarID,
		// Number of columns to display when the toolbar is displayed.
		int iNumCols=5);
	
	// This member function is called to update the currently selected icon
	// for the parent toolbar.  This icon will represent the selection the
	// user made when the popup was open.
	//
	virtual void UpdateToolbarIcon(
		// Command ID of the toolbar button to set active.
		UINT uCmdID);

	// This member function retrieves the currently selected icon.  Returns
	// a CXTIconHandle object that is used to update the parent toolbar's image
	// list.
	//
	virtual CXTIconHandle GetSelectedIcon(
		// Command ID of the toolbar button.
		UINT uCmdID);

    //:Ignore
	//{{AFX_VIRTUAL(CXTToolBarPopupWnd)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
    //:End Ignore

protected:

    //:Ignore
	//{{AFX_MSG(CXTToolBarPopupWnd)
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
    //:End Ignore
	DECLARE_MESSAGE_MAP()

	friend class CXTToolBarPopupWndHook;
};

//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__POPUPWND_H__)
