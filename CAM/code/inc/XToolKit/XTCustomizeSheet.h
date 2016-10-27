//:Ignore
// XTCustomizeSheet.h interface for the CXTCustomizeSheet class.
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

#if !defined(__XTCUSTOMIZESHEET_H__)
#define __XTCUSTOMIZESHEET_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTCustomizeSheet is a multiple inheritance class derived from CXTDialogState
// and CXTResizePropertySheet. It is a property sheet that is displayed during
// toolbar customization.
//
class _XT_EXT_CLASS CXTCustomizeSheet : CXTDialogState, public CXTResizePropertySheet
{
	DECLARE_DYNAMIC(CXTCustomizeSheet)

//:Ignore
	// A map to store active custom bars that get mouse input.
	CMap<HWND, HWND, bool, bool> m_map;
//:End Ignore

public:

	// Constructs a CXTCustomizeSheet object.
	//
	CXTCustomizeSheet(
		// Pointer to the frame window on which to execute customizations.
		CFrameWnd* pFrameWnd,
		// Specifies which pages to include when the customize dialog is
        // displayed.  It can be one or more of the following:
        //[ul]
        //[li][b]CUST_XT_TOOLBARS[/b] - Displays the Toolbars tab in the toolbar customize property sheet.[/li]
        //[li][b]CUST_XT_COMMANDS[/b] - Displays the Commands tab in the toolbar customize property sheet.[/li]
        //[li][b]CUST_XT_KEYBOARD[/b] - Displays the Keyboard tab in the toolbar customize property sheet.[/li]
        //[li][b]CUST_XT_TOOLS[/b] - Displays the Tools tab in the toolbar customize property sheet.[/li]
        //[li][b]CUST_XT_OPTIONS[/b] - Displays the Options tab in the toolbar customize property sheet.[/li]
        //[li][b]CUST_XT_DEFAULT[/b] - Same as CUST_XT_TOOLBARS|CUST_XT_COMMANDS|CUST_XT_KEYBOARD|CUST_XT_OPTIONS.[/li]
        //[/ul]
        // If no flags are defined the constructor will ASSERT.
		DWORD dwCustStyle);

	// Destroys a CXTCustomizeSheet object, handles cleanup and de-allocation.
	//
	virtual ~CXTCustomizeSheet();

	// Current context.
	CXTCustomizeContext m_context;
	// First property page.
	CXTCustToolBarPage* m_pPage1;
	// Second property page.
	CXTCustCommandsPage* m_pPage2;
	// Third property page.
	CXTCustAccelerators* m_pPage3;
	// Fourth property page.
	CXTCustTools* m_pPage4;
	// Fifth property page.
	CXTCustOptions* m_pPage5;
	// Size for each page displayed.
	CRect m_rcPage;
	// 'true' if the application is currently active, or 'false' otherwise.
	bool m_bAppActive;

	// This member function sets the customization mode on or off in all applicable
	// control bars.
	//
	void SetCustMode(
		// 'true' to enable toolbar customization.
		bool bMode);

//:Ignore
	//{{AFX_VIRTUAL(CXTCustomizeSheet)
	public:
	virtual BOOL OnInitDialog();
	virtual int DoModal();
	//}}AFX_VIRTUAL
//:End Ignore

//:Ignore
protected:
	//{{AFX_MSG(CXTCustomizeSheet)
	afx_msg void OnDestroy();
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnCloseBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//:End Ignore

//:Ignore
private:
	
    CFrameWnd*         m_pWndParent;

	bool FilterMouse(LPARAM lParam);
	bool FilterKey(LPARAM lParam);
	void StoreInfo(CXTCustomControlBarInfo* pInfo);
	void OnBarDestroyed(CControlBar* pBar);
//:End Ignore

	friend class CXTCustomizeContext;
	friend class CXTCustToolBarPage;
	friend class CXTCustCommandsPage;
	friend class CXTCustAccelerators;
	friend class CXTCustTools;
	friend class CXTCustOptions;
};

//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTCUSTOMIZESHEET_H__)
