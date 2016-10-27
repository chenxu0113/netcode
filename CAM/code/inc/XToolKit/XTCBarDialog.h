//:Ignore
// XTCBarDialog.h : header file
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

#if !defined(__XTCBARDIALOG_H__)
#define __XTCBARDIALOG_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

/////////////////////////////////////////////////////////////////////////////
// CXTCBarDialog is a, CXTResizeDialog derived class.  The CXTCBarDialog class is used
// to create a dialog that supports toolbars, status bars and menu commands similar to the way
// a CFrameWnd class would handle this.
//
class _XT_EXT_CLASS CXTCBarDialog : public CXTResizeDialog
{
	DECLARE_DYNAMIC(CXTCBarDialog)

public:

    // Constructs a CXTCBarDialog object.  To construct a resource-based modal dialog box, call either 
	// public form of the constructor. One form of the constructor provides access to the dialog resource 
	// by template name. The other constructor provides access by template ID number, usually with an 
	// [b]IDD_[/b] prefix (for example, IDD_DIALOG1).
	//
	// To construct a modal dialog box from a template in memory, first invoke the parameterless, protected 
	// constructor and then call [b]InitModalIndirect[/b].
	// 
	// After you construct a modal dialog box with one of the above methods, call [b]DoModal[/b].
	// 
	// To construct a modeless dialog box, use the protected form of the [b]CXTResizeDialog[/b] constructor. The constructor 
	// is protected because you must derive your own dialog-box class to implement a modeless dialog box. 
	// Construction of a modeless dialog box is a two-step process. First call the constructor; then call the 
	// [b]Create[/b] member function to create a resource-based dialog box, or call [b]CreateIndirect[/b] to 
	// create the dialog box from a template in memory.
    //
	CXTCBarDialog();

    // Constructs a CXTCBarDialog object.  To construct a resource-based modal dialog box, call either 
	// public form of the constructor. One form of the constructor provides access to the dialog resource 
	// by template name. The other constructor provides access by template ID number, usually with an 
	// [b]IDD_[/b] prefix (for example, IDD_DIALOG1).
	//
	// To construct a modal dialog box from a template in memory, first invoke the parameterless, protected 
	// constructor and then call [b]InitModalIndirect[/b].
	// 
	// After you construct a modal dialog box with one of the above methods, call [b]DoModal[/b].
	// 
	// To construct a modeless dialog box, use the protected form of the [b]CXTResizeDialog[/b] constructor. The constructor 
	// is protected because you must derive your own dialog-box class to implement a modeless dialog box. 
	// Construction of a modeless dialog box is a two-step process. First call the constructor; then call the 
	// [b]Create[/b] member function to create a resource-based dialog box, or call [b]CreateIndirect[/b] to 
	// create the dialog box from a template in memory.
    //
	CXTCBarDialog(
		// Contains the ID number of a dialog-box template resource.
		UINT nIDTemplate,
		// Points to the parent or owner window object (of type CWnd) to which the dialog object 
		// belongs. If it is [b]NULL[/b], the dialog object’s parent window is set to the main application window.
		CWnd* pParentWnd = NULL,
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

protected:
	
    // Handles the display of old-style 'cool' menus.
	CXTCoolMenu m_hookMenu;
	// Resource id of the string command currently displayed in the status bar area.
	UINT m_nIDTracking;
	// Resource id of the last string command displayed in the status bar area.
	UINT m_nIDLastMessage;
	// Array of control bars currently owned by the dialog.
	CPtrList m_listControlBars;
	
	//:Ignore
	//{{AFX_VIRTUAL(CXTCBarDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	//:End Ignore

protected:

    // Call this member function to install cool menus for your application.
	// Cool menus are menus that appear with icons next to the menu titles.
	// Pass in your toolbar resource array to initialize.
    //
	virtual void InstallCoolMenus(
        // Array of toolbar resource IDs.  The cool menu will use the toolbar
		// commands to map the icons placed next to the corresponding menu commands.
		const UINT* nIDToolBars,
        // Size of the array of toolbars.
		int nSize);

    // Call this member function to install cool menus for your application.
	// Cool menus are menus that appear with icons next to the menu titles.
	// Pass in your toolbar resource to initialize.
    //
	virtual void InstallCoolMenus(
        // Toolbar resource ID.  The cool menu will use the toolbar commands
        // to map the icons placed next to the corresponding menu commands.
		const UINT nIDToolBar);

	// Called by the dialog when the standard control bars are toggled on or off or when the dialog 
	// window is resized. The default implementation of this member function calls the [b]CWnd[/b] member 
	// function [b]RepositionBars[/b] to reposition all the control bars in the dialog.
	//
	// Override this member function to control the appearance and behavior of control bars after the 
	// layout of the frame window has changed. For example, call it when you turn control bars on or off 
	// or add another control bar.
	//
	virtual void RecalcBarLayout();

    // Call this member function to retrieve a pointer to the status bar associated
	// with the dialog.  Returns a CXTStatusBar pointer that represents the status bar
	// associated with the dialog.
    //          
	virtual CXTStatusBar* GetMessageBar();

    // Call this member function to retrieve a pointer to the menu bar associated
	// with the dialog.  Returns a CXTMenuBar pointer that represents the menu bar
	// associated with the dialog.
    //          
	virtual CXTMenuBar* GetMenuBar();

	// Call this member function to show or hide the control bar.
	//
	virtual void ShowControlBar(
		// Pointer to the control bar to be shown or hidden.
		CControlBar* pBar,
		// If [b]TRUE[/b], specifies that the control bar is to be shown. If [b]FALSE[/b], 
		// specifies that the control bar is to be hidden.
		BOOL bShow,
		// If [b]TRUE[/b], delay showing the control bar. If [b]FALSE[/b], show the control
		// bar immediately.
		BOOL bDelay);

	//:Ignore
	//{{AFX_MSG(CXTCBarDialog)
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnEnterIdle(UINT nWhy, CWnd* pWho);
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg LRESULT OnSetMessageString(WPARAM wParam, LPARAM lParam = 0L);
	afx_msg LRESULT OnPopMessageString(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateKeyIndicator(CCmdUI* pCmdUI);
	afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnBarCheck(UINT nID);
	afx_msg LRESULT OnKickIdle(WPARAM wParam, LPARAM lParam);
	//:End Ignore
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__XTCBARDIALOG_H__)
