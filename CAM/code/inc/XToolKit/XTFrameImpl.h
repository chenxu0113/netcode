//:Ignore
// XTFrameImpl.h interface for the CXTFrameImpl class.
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

#if !defined(__XTFRAMEIMPL_H__)
#define __XTFRAMEIMPL_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

// forwards
class CXTDockState;
class CXTCustomizeSheet;

//////////////////////////////////////////////////////////////////////
// CXTFrameImpl is a stand alone helper class.  It is used to provide additional
// support for cool menu and other control bar functionality.  It is an
// additional base class for CXTFrameWnd, CXTMDIChildWnd, CXTMDIFrameWnd,
// and CXTOleIPFrameWnd.
//
class _XT_EXT_CLASS CXTFrameImpl
{

public:

    // Constructs a CXTFrameImpl object.
	//
	CXTFrameImpl();

	// Destroys a CXTFrameImpl object, handles cleanup and de-allocation.
	//
    virtual ~CXTFrameImpl();

    // Handles the display of old-style 'cool' menus.
	CXTCoolMenu m_coolMenu;

protected:

	// Menu bar.
	CXTMenuBar m_wndMenuBar;
	// Customize dialog style, you can set this style in your frame's constructor,
	// see CXTCustomizeSheet for more details.
	static DWORD m_dwCustStyle;
	// Pointer to the toolbar customize dialog.
	static CXTCustomizeSheet* m_pCustomizeSheet;

protected:

	// Call this member function to retrieve a pointer to the cool menu object
	// associated with the frame. Returns a CXTCoolMenu pointer to the cool
	// menu object associated with the frame.
    //          
    CXTCoolMenu* GetCoolMenuImpl(
		// A CFrameWnd pointer to the calling frame window.
        CFrameWnd* pFrameWnd);

	// Call this member function to retrieve a pointer to the application's
	// menu bar.  To use this function, the menu bar must have been created using
	// the default control ID AFX_IDW_MENUBAR.  Returns a CXTMenuBar pointer
	// to the menu bar associated with the frame.
    //          
    CXTMenuBar* GetMenuBarImpl(
		// A CFrameWnd pointer to the calling frame window.
        CFrameWnd* pFrameWnd) const;

    // Call this member function to install cool menus for your application.
	// Cool menus are menus that appear with icons next to the menu titles.
	// Pass in your toolbar resource array to initialize.
    //
    void InstallCoolMenusImpl(
		// Array of toolbar resource IDs.  The cool menu will use the toolbar
		// commands to map the icons placed next to the corresponding menu commands.
        const UINT* nIDToolBars,
		// Size of the array of toolbars.
        int nSize,
		// A CFrameWnd pointer to the calling frame window.
        CFrameWnd* pFrameWnd);

    // Call this member function to install cool menus for your application.
	// Cool menus are menus that appear with icons next to the menu titles.
	// Pass in your toolbar resource to initialize.
    //
    void InstallCoolMenusImpl(
		// Toolbar resource ID.  The cool menu will use the toolbar commands
        // to map the icons placed next to the corresponding menu commands.
        const UINT nIDToolBar,
		// A CFrameWnd pointer to the calling frame window.
        CFrameWnd* pFrameWnd);

    // This member function is called by the frame window to restore the
	// settings of the control bar.
    //
    void LoadBarStateImpl(
		// Name of a section in the initialization file or a key in the Windows
		// registry where state information is stored.
        LPCTSTR lpszProfileName,
		// A CFrameWnd pointer to the calling frame window.
        CFrameWnd* pFrameWnd);

    // This member function is called by the frame window to save the settings
	// of the control bar.
    //
    void SaveBarStateImpl(
		// Name of a section in the initialization file or a key in the Windows
		// registry where state information is stored.
        LPCTSTR lpszProfileName,
		// A CFrameWnd pointer to the calling frame window.
        CFrameWnd* pFrameWnd) const;

    // Call this function to enable a control bar to be docked.  The sides
	// specified must match one of the sides enabled for docking in the destination
	// frame window, or the control bar cannot be docked to that frame window.
    //
    void EnableDockingImpl(
		// Specifies whether the control bar supports docking and the sides of
		// its parent window to which the control bar can be docked, if supported.
        // The style can be one or more of the following: 
        //[ul]
        //[li][b]CBRS_ALIGN_TOP[/b] - Allows docking at the top of the
		//       client area.[/li]
        //[li][b]CBRS_ALIGN_BOTTOM[/b] - Allows docking at the bottom of
		//       the client area.[/li]
        //[li][b]CBRS_ALIGN_LEFT[/b] - Allows docking on the left side
		//       of the client area.[/li]
        //[li][b]CBRS_ALIGN_RIGHT[/b] - Allows docking on the right side
		//       of the client area.[/li]
        //[li][b]CBRS_ALIGN_ANY[/b] - Allows docking on any side of the
		//       client area.[/li]
        //[li][b]CBRS_FLOAT_MULTI[/b] - Allows multiple control bars to
		//       be floated in a single mini-frame window.[/li]
        //[/ul]
        //  If 0 (that is, indicating no flags), the control bar will not dock.
        DWORD dwDockStyle,
		// A CFrameWnd pointer to the calling frame window.
        CFrameWnd* pFrameWnd);

    // Call this function to enable a control bar to be docked.  The sides
	// specified must match one of the sides enabled for docking in the destination
	// frame window, or the control bar cannot be docked to that frame window.
    //
    void EnableDockingExImpl(
		// Specifies whether the control bar supports docking and the sides of
		// its parent window to which the control bar can be docked, if supported.
        // The style can be one or more of the following: 
        //[ul]
        //[li][b]CBRS_ALIGN_TOP[/b] - Allows docking at the top of the
		//       client area.[/li]
        //[li][b]CBRS_ALIGN_BOTTOM[/b] - Allows docking at the bottom of
		//       the client area.[/li]
        //[li][b]CBRS_ALIGN_LEFT[/b] - Allows docking on the left side
		//       of the client area.[/li]
        //[li][b]CBRS_ALIGN_RIGHT[/b] - Allows docking on the right side
		//       of the client area.[/li]
        //[li][b]CBRS_ALIGN_ANY[/b] - Allows docking on any side of the
		//       client area.[/li]
        //[li][b]CBRS_FLOAT_MULTI[/b] - Allows multiple control bars to
		//       be floated in a single mini-frame window.[/li]
        //[/ul]
        // If 0 (that is, indicating no flags), the control bar will not dock.
        DWORD dwDockStyle,
		// Specifies the splitter's, inside dockbars, look. The style can be
		// one of the following: 
        //[ul]
        //[li][b]CBRS_XT_NONFLAT[/b] - Thick devstudio like non-flat splitters.[/li]
        //[li][b]CBRS_XT_SEMIFLAT[/b] - Thin 3D non-flat splitters.[/li]
        //[li][b]CBRS_XT_FLAT[/b] - Flat splitters.[/li]
        //[/ul]
        DWORD dwFlatStyle,
		// A CFrameWnd pointer to the calling frame window.
        CFrameWnd* pFrameWnd);

    // This member function causes a control bar to be docked to the frame
	// window.  The control bar will be docked to one of the sides of the
	// frame window specified in the calls to both CXTDockWindow::EnableDocking
	// and CXTFrameWnd::EnableDocking.  The side chosen is determined by the
	// dockbar represented by 'pDockBar'.
    //
    void DockControlBarImpl(
		// A CControlBar pointer to the control bar to be docked.
        CControlBar* pBar,
		// A CDockBar pointer to the dockbar the control bar is docked to.
        CDockBar* pDockBar,
		// Determines, in screen coordinates, where the control bar will be docked
		// in the non-client area of the destination frame window.
        LPCRECT lpRect,
		// A CFrameWnd pointer to the calling frame window.
        CFrameWnd* pFrameWnd);

    // This member function causes a control bar to be docked to the frame
	// window. The control bar will be docked to one of the sides of the frame
	// window specified in the calls to both CXTDockWindow::EnableDocking
	// and CXTFrameWnd::EnableDocking.  The side chosen is determined by 'nDockBarID'.
    //
    void DockControlBarImpl(
		// A CControlBar pointer to the control bar to be docked.
        CControlBar* pBar,
		// Determines which sides of the frame window to consider for docking.
		// It can be 0, or one or more of the following: 
        //[ul]
        //[li][b]AFX_IDW_DOCKBAR_TOP[/b] - Dock to the top side of the
		//       frame window.[/li]
        //[li][b]AFX_IDW_DOCKBAR_BOTTOM[/b] - Dock to the bottom side of
		//       the frame window.[/li]
        //[li][b]AFX_IDW_DOCKBAR_LEFT[/b] - Dock to the left side of the
		//       frame window.[/li]
        //[li][b]AFX_IDW_DOCKBAR_RIGHT[/b] - Dock to the right side of
		//       the frame window.[/li]
        //[/ul]
        // If 0, the control bar can be docked to any side enabled for
		// docking in the destination frame window.
        UINT nDockBarID,
		// Determines, in screen coordinates, where the control bar will be docked
		// in the non-client area of the destination frame window.
        LPCRECT lpRect,
		// A CFrameWnd pointer to the calling frame window.
        CFrameWnd* pFrameWnd);

    // This member function will redock a control bar specified by 'pBar2'
    // to the left of a newly docked control bar specified by 'pBar1'. 
    //
    void DockControlBarLeftOfImpl(
        // A CControlBar pointer to the control bar to be docked.
        CControlBar* pBar1,
        // A CControlBar pointer to the already docked control bar to be
        // redocked on the left of 'pBar1'.
        CControlBar* pBar2,
        // A CFrameWnd pointer to the calling frame window.
        CFrameWnd* pFrameWnd);

    // This member function retrieves a pointer to the menu for the frame
	// window. Returns a pointer to a CMenu object that represents the active
	// menu for the frame.
    //
    CMenu* GetMenuImpl(
		// A CFrameWnd pointer to the calling frame window.
        CFrameWnd* pFrameWnd) const;

    // This member function adds the specified command to the list of menu
	// items to hide until activated by clicking on the chevron. Returns TRUE
	// if successful, otherwise returns FALSE.        
    //
    BOOL HideMenuItemImpl(
		// The command ID of a menu item to hide.
        UINT nCommandID,
		// A CFrameWnd pointer to the calling frame window.
        CFrameWnd* pFrameWnd);

    // This member function adds the specified menu item to the list of
	// menu items to hide until activated by clicking on the chevron. Returns
	// TRUE if successful, otherwise returns FALSE.
    //
    BOOL HideMenuItemImpl(
		// Text string representing the popup menu item to hide.
        LPCTSTR lpszItem,
		// A CFrameWnd pointer to the calling frame window.
        CFrameWnd* pFrameWnd);

    // This member function adds the specified commands to the list of
	// menu items to hide until activated by clicking on the chevron. Returns
	// TRUE if successful, otherwise returns FALSE.        
    //
    BOOL HideMenuItemsImpl(
		// An array of command IDs, of menu items, to hide.
        const UINT* nCommandIDs,
		// Size of the array passed in.
        int nSize,
		// A CFrameWnd pointer to the calling frame window.
        CFrameWnd* pFrameWnd);

    // This member function adds the specified menu items to the list of
	// menu items to hide until activated by clicking on the chevron. Returns
	// TRUE if successful, otherwise returns FALSE.
    //
    BOOL HideMenuItemsImpl(
		// An array of command IDs, of menu items, to hide.
        const LPCTSTR lpszItems,
		// Size of the array passed in.
        int nSize,
		// A CFrameWnd pointer to the calling frame window.
        CFrameWnd* pFrameWnd);

    // This member function redraws the menu bar. If a menu bar is changed
	// after Windows has created the window, call this function to draw the
	// changed menu bar.  Overrides the CWnd implementation.
    //
    void DrawMenuBarImpl(
		// A CFrameWnd pointer to the calling frame window.
        CFrameWnd* pFrameWnd);

	// This member function will initialize the accelerator manager for the
	// framework.  Returns 'true' if successful, otherwise returns 'false'.
	// 
	bool InitAccelManagerImpl(
		// A CFrameWnd pointer to the calling frame window.
		CFrameWnd* pFrameWnd);

	// Call this member function to initialize the Tools manager for your
	// application. The Tools manager will insert a "Tools" menu into your
	// application's menu which allows the user to customize and add custom
	// commands to the menu.  Typically used with toolbar customization,
	// the Tools manager should be initialized when your frame is loaded by
	// overriding the virtual function CFrameWnd::LoadFrame and can be managed
	// by selecting the 'Tools' tab in the Customize dialog.  Returns 'true'
	// if successful, otherwise returns 'false. 
	//
	bool InitToolsManagerImpl(
		// A pointer to a CFrameWnd object.
		CFrameWnd* pFrameWnd,
        // Index where the 'Tools' menu should be inserted into the Standard menu.
		int iNormalIndex,
        // Index where the 'Tools' menu should be inserted into the MDI window menu.
		int iWindowIndex,
        // Resource ID of the popup menu to be displayed for the Arguments
		// browse edit box.
		int iArgPopupMenu,
        // Resource ID of the popup menu to be displayed for the Initial
		// Directory browse edit box.
		int iDirPopupMenu);

	// Call this member function to initialize the Options manager for your
	// application. Typically used with toolbar customization, the Options
	// manager will allow the user to configure options related to the toolbar
	// and menu behavior for the application.  The options manager should
	// be initialized when your frame is loaded by overriding the virtual
	// function CFrameWnd::LoadFrame and can be managed by selecting the 'Options'
	// tab in the Customize dialog.  Returns 'true' if successful, otherwise
	// returns 'false.
	//
	bool InitOptionsManagerImpl(
		// A pointer to a CFrameWnd object.
		CFrameWnd* pFrameWnd);

	// This member function is called to update the Tools manager argument
	// or directory string by appending the string displayed in the corresponding
	// edit field.  This function is used by the framework whenever the argument
	// or directory browse edit in the Tools manager tab of the toolbar Customize
	// dialog is configured to use a popup menu.  Returns 'true' if successful,
	// otherwise returns 'false.
	//
	bool UpdateToolsItem(
        // String to append to the browse edit string.
		LPCTSTR lpszText,
        // Enumerated value that specifies which browse edit to update.
		// It can be either XT_TOOLARG or XT_TOOLDIR.
		int iWhich,
        // 'true' to append the string, 'false' to replace the string.
		bool bAppend);

	//:Ignore
	void SetDockStateImpl(const CXTDockState& state, CFrameWnd* pFrameWnd);
	void GetDockStateImpl(CXTDockState& state, CFrameWnd* pFrameWnd) const;
	BOOL PreCreateWindowImpl(CREATESTRUCT& cs, UINT uIcon, CFrameWnd* pFrameWnd);
	BOOL PreTranslateMessageImpl(MSG* pMsg, CFrameWnd* pFrameWnd);
	void OnSysColorChangeImpl(CFrameWnd* pFrameWnd);
	void OnSettingChangeImpl(UINT uFlags, LPCTSTR lpszSection, CFrameWnd* pFrameWnd);
	void OnCustomizeBarImpl(CFrameWnd* pFrameWnd);
	void OnInitMenuPopupImpl(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	BOOL OnToolsManagerImpl(UINT nID);
	void OnUpdateToolsManagerImpl(CCmdUI* pCmdUI);
	void OnUpdateChevronImpl(CCmdUI* pCmdUI);
	//:End Ignore

    friend class CXTToolBar;
};

//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTFRAMEIMPL_H__)

