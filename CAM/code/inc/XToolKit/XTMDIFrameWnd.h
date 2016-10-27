//:Ignore
// XTMDIFrameWnd.h interface for the CXTMDIFrameWnd class.
//
// This file is a part of the Xtreme Toolkit for MFC.
// ?998-2003 Codejock Software, All Rights Reserved.
//
// This source code can only be used under the terms and conditions 
// outlined in the accompanying license agreement.
//
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////
//:End Ignore
#include "BCGPMDIFrameWnd.h"
#if !defined(__XTMDIFRAMEWND_H__)
#define __XTMDIFRAMEWND_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTMDIFrameWnd is a multiple inheritance class derived from CMDIFrameWnd
// and CXTFrameImpl.  CXTMDIFrameWnd extends the standard CMDIFrameWnd
// class to allow CXTDockWindow and CXTToolBar docking, customization,
// and cool menu support.
//
class _XT_EXT_CLASS CXTMDIFrameWnd : public CBCGPMDIFrameWnd, public CXTFrameImpl
{
	DECLARE_DYNCREATE(CXTMDIFrameWnd)

public:

    // Constructs a CXTMDIFrameWnd object.
	//
	CXTMDIFrameWnd();

	// Destroys a CXTMDIFrameWnd object, handles cleanup and de-allocation.
	//
    virtual ~CXTMDIFrameWnd();

public:

    // This member function gets a pointer to the cool menu object associated
	// with the frame. Returns a pointer to a CXTCoolMenu object that represents
	// the cool menu object associated with the frame.
    //          
    CXTCoolMenu* GetCoolMenu();

    // This member function gets a pointer to the menu bar associated with
	// the frame. Returns a pointer to a CXTMenuBar object that represents
	// the menu bar associated with the frame.
    //          
    CXTMenuBar* GetMenuBar();

    // Call this member function to install cool menus for your application.
	// Cool menus are menus that appear with icons next to the menu titles.
	// Pass in your toolbar resource array to initialize.
    //
    void InstallCoolMenus(
        // Array of toolbar resource IDs.  The cool menu will use the toolbar
		// commands to map the icons placed next to the corresponding menu commands.
        const UINT* nIDToolBars,
        // Size of the array of toolbars.
        int nSize);

    // Call this member function to install cool menus for your application.
	// Cool menus are menus that appear with icons next to the menu titles.
	// Pass in your toolbar resource to initialize.
    //
    void InstallCoolMenus(
        // Toolbar resource ID.  The cool menu will use the toolbar commands
        // to map the icons placed next to the corresponding menu commands.
        const UINT nIDToolBar);

    // This member function is called by the frame window to restore the
	// settings of the control bar.
    //
    virtual void LoadBarState(
        // Name of a section in the initialization file or a key in the
		// Windows registry where state information is stored.
        LPCTSTR lpszProfileName);

    // This member function is called by the frame window to save the settings
	// of the control bar.
    //
    virtual void SaveBarState(
        // Name of a section in the initialization file or a key in the
		// Windows registry where state information is stored.
        LPCTSTR lpszProfileName) const;

    // Call this function to enable a control bar to be docked.  The sides
	// specified must match one of the sides enabled for docking in the destination
	// frame window, or the control bar cannot be docked to that frame window.
    //
    void EnableDocking(
        // Specifies whether the control bar supports docking and the sides
		// of its parent window to which the control bar can be docked, if supported.
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
        //  If 0 (that is, indicating no flags), the control 
        //  bar will not dock.
        DWORD dwDockStyle);
    
    // Call this function to enable a control bar to be docked.  The sides
	// specified must match one of the sides enabled for docking in the destination
	// frame window, or the control bar cannot be docked to that frame window.
    //
    void EnableDockingEx(
        // Specifies whether the control bar supports docking and the sides
		// of its parent window to which the control bar can be docked, if supported.
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
        // If 0 (that is, indicating no flags), the control 
        // bar will not dock.
        DWORD dwDockStyle,
        // Specifies the splitters, inside dockbars, look. It can be one
		// of the following: 
        //[ul]
        //[li][b]CBRS_XT_NONFLAT[/b] - Thick devstudio like non-flat splitters.[/li]
        //[li][b]CBRS_XT_SEMIFLAT[/b] - Thin 3D non-flat splitters.[/li]
        //[li][b]CBRS_XT_FLAT[/b] - Flat splitters.[/li]
        //[/ul]
        DWORD dwFlatStyle);

    // This member function causes a control bar to be docked to the frame
	// window.  The control bar will be docked to one of the sides of the
	// frame window specified in the calls to both CXTDockWindow::EnableDocking
	// and CXTFrameWnd::EnableDocking.  The side chosen is determined by the
	// dockbar specified by 'pDockBar'.
    //
    void DockControlBar(
        // A CControlBar pointer to the control bar to be docked.
        CControlBar* pBar,
        // A CDockBar pointer to the dockbar the control bar is docked to.
        CDockBar* pDockBar,
        // Determines, in screen coordinates, where the control bar will
		// be docked in the non-client area of the destination frame window.
        LPCRECT lpRect = NULL);

    // This member function causes a control bar to be docked to the frame
	// window. The control bar will be docked to one of the sides of the frame
	// window specified in the calls to both CXTDockWindow::EnableDocking
	// and CXTFrameWnd::EnableDocking.  The side chosen is determined by 'nDockBarID'.
    //
    void DockControlBar(
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
        UINT nDockBarID = 0,
        // Determines, in screen coordinates, where the control bar will
		// be docked in the non-client area of the destination frame window.
        LPCRECT lpRect = NULL);

    // This member function will redock a control bar specified by 'pBar2'
	// to the left of a newly docked control bar specified by 'pBar1'. 
    //
    virtual void DockControlBarLeftOf(
        // A CControlBar pointer to the control bar to be docked.
        CControlBar* pBar1,
        // A CControlBar pointer to the already docked control bar to be
		// redocked on the left of 'pBar1'.
        CControlBar* pBar2);

    // This member function retrieves a pointer to the menu for the frame
	// window. Returns a pointer to a CMenu object that represents the active
	// menu for the frame.
    //
    virtual CMenu* GetMenu() const;

    // This member function adds the specified command to the list of menu
	// items to hide until activated by clicking on the chevron. Returns TRUE
	// if successful, otherwise returns FALSE.        
    //
    virtual BOOL HideMenuItem(
        // The command ID of a menu item to hide.
        UINT nCommandID);

    // This member function adds the specified menu item to the list of
	// menu items to hide until activated by clicking on the chevron. Returns
	// TRUE if successful, otherwise returns FALSE.
    //
    virtual BOOL HideMenuItem(
        // Text string representing the popup menu item to hide.
        LPCTSTR lpszItem);

    // This member function adds the specified commands to the list of
	// menu items to hide until activated by clicking on the chevron. Returns
	// TRUE if successful, otherwise returns FALSE.        
    //
    virtual BOOL HideMenuItems(
        // An array of command IDs, of menu items, to hide.
        const UINT* nCommandIDs,
        // Size of the array passed in.
        int nSize);

    // This member function adds the specified menu items to the list of
	// menu items to hide until activated by clicking on the chevron. Returns
	// TRUE if successful, otherwise returns FALSE.
    //
    virtual BOOL HideMenuItems(
        // An array of command IDs, of menu items, to hide.
        const LPCTSTR lpszItems,
        // Size of the array passed in.
        int nSize);

    // This member function redraws the menu bar.  If a menu bar is changed
	// after Windows has created the window, call this function to draw the
	// changed menu bar. Overrides the CWnd implementation.
    //
    void DrawMenuBarX();

    // Call this function to display the Customize Toolbar dialog box.
	// This dialog box allows the user to customize the toolbar by adding
	// and deleting buttons.
    //
	void Customize();

	// This member function will initialize the accelerator manager for the
	// framework. Returns 'true' if successful, otherwise returns'false'.
	//
	bool InitAccelManager();

	// Call this member function to initialize the Tools manager for your
	// application. The Tools manager will insert a "Tools" menu into your
	// application's menu which allows the user to customize and add custom
	// commands the the menu.  Typically used with toolbar customization,
	// the Tools manager should be initialized when your frame is loaded by
	// overriding the virtual function CFrameWnd::LoadFrame and can be managed
	// by selecting the 'Tools' tab in the Customize dialog.  Returns 'true'
	// if successful, otherwise returns 'false.
	//
	bool InitToolsManager(
        // Index where the 'Tools' menu should be inserted into the standard menu.
		int iNormalIndex,
        // Index where the 'Tools' menu should be inserted into the MDI window menu.
		int iWindowIndex=-1,
        // Resource ID of the popup menu to be displayed for the Arguments
		// browse edit box.
		int iArgPopupMenu=0,
        // Resource ID of the popup menu to be displayed for the Initial
		// Directory browse edit box.
		int iDirPopupMenu=0);

	// Call this member function to initialize the Options manager for your
	// application. Typically used with toolbar customization, the Options
	// manager will allow the user to configure options related to the toolbar
	// and menu behavior for the application.  The Options manager should
	// be initialized when your frame is loaded by overriding the virtual
	// function CFrameWnd::LoadFrame and can be managed by selecting the 'Options'
	// tab in the Customize dialog.  Returns 'true' if successful, otherwise
	// returns 'false.
	//
	bool InitOptionsManager();

protected:

	//:Ignore
    virtual void SetDockState(const CXTDockState& state);
    virtual void GetDockState(CXTDockState& state) const;

    //{{AFX_VIRTUAL(CXTMDIFrameWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs, UINT uIcon);
	//}}AFX_VIRTUAL

    //{{AFX_MSG(CXTMDIFrameWnd)
	afx_msg void OnSysColorChange();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	//}}AFX_MSG
	afx_msg void OnCustomizeBar();
	afx_msg void OnWindowList();
	afx_msg BOOL OnToolsManager(UINT nID);
	afx_msg void OnUpdateToolsManager(CCmdUI* pCmdUI);
	afx_msg void OnUpdateChevron(CCmdUI* pCmdUI);
    //:End Ignore

	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTMDIFrameWnd::Customize() {
	OnCustomizeBar();
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTMDIFRAMEWND_H__)

