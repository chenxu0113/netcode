//:Ignore
// XTCoolMenu.h : interface for the CXTCoolMenu class.
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

#if !defined(__XTCOOLMENU_H__)
#define __XTCOOLMENU_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTItemData is a stand alone helper class.  It is used by CXTCoolMenu.
//
class _XT_EXT_CLASS CXTItemData
{
public:

    // Constructs a CXTItemData object.
	//
    CXTItemData ();

    // Destroys a CXTItemData object, handles cleanup and de-allocation.
	//
    virtual ~CXTItemData();

    // Item text.
	CString m_strText;
	// TRUE if the item has been removed.
	BOOL m_bHidden;
	// Handle to the menu this item belongs to.
	HMENU m_hMenu;
	// Handle to a submenu, if any, that belongs to this item.
	HMENU m_hSubMenu;
	// Unique identifying number.
	long m_itemID;
	// Menu item position.
	UINT m_nPosition;
	// Menu item (command) ID.
	UINT m_commandID;
	// Original item type flags.
	DWORD m_dwType;
	// Item state.
	DWORD m_dwState;
	// Used for drawing hidden items.
	DWORD m_dwHideType;
	// Alignment of the associated menu bar.
	DWORD m_iMBAlign;

	// This member function is called to determine if the item data for the
	// cool menu belongs to to the cool menu object. Returns TRUE if the cool
	// menu belongs to to the cool menu object.
	//
    BOOL IsXTItemData();
};

//:Ignore
//////////////////////////////////////////////////////////////////////

class CXTMemDC;
class CXTCoolMenu;
class CXTWndShadow;

class CXTPopupMenu : public CXTWndHook
{
protected:

	bool          m_bOffset;
	HMENU         m_hMenu;
	CXTCoolMenu  *m_pParent;
	BOOL m_bAnimationFinished;
	CRect m_rectExclude;
	
	static BOOL      m_bSysMenuAnimation;
	static BOOL      m_bSysDropShadows;
	static BOOL      m_bSysDropShadowsAvailable;
	static int       m_iRefCount;

	enum { WM_XT_FIX_XP_PAINT = WM_APP + 10, WM_XT_DEFER_DRAW_SHADOW };

public:

	CXTPopupMenu();
	virtual ~CXTPopupMenu();

	HWND GetHWnd();
	HMENU GetHMenu();

	void SetParams(HMENU hMenu);
	void SetCoolMenuParent(CXTCoolMenu *pParent);
	void HideBeforeExpand();
	void DisableXPEffects();
	void OnDrawBorder(CDC* pDC);
	void ShowShadows();

	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	friend class CXTCoolMenu;
};
 
typedef CList<CXTPopupMenu*, CXTPopupMenu*&> CXTPopupMenuList;
//////////////////////////////////////////////////////////////////////
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTCoolMenu is a CXTWndHook derived class.  CXTCoolMenu manages the
// cool menus for the application.  The images are drawn based upon toolbar
// resources that are passed in and are displayed next to the menu text
// associated with the toolbar command ID.
//
// Some restrictions apply to cool menus if the XT menu bar (CXTMenuBar) is
// not used:
// [ol]
//   [li]Intelligent menus cannot be used.[/li]
//   [li]XP skinning and shadows can only be used on Win2k or later operating
//       systems.[/li]
// [/ol]
//
class _XT_EXT_CLASS CXTCoolMenu : public CXTWndHook
{
    friend class CXTColorPopup;
    friend class CXTMenu;

    DECLARE_DYNAMIC(CXTCoolMenu)

public:

    // Constructs a CXTCoolMenu object.
    //
    CXTCoolMenu(
		// DEPRECATED: The value of this parameter is ignored.
        BOOL bNoToolbar=FALSE);

    // Destroys a CXTCoolMenu object, handles cleanup and de-allocation.
    //
    virtual ~CXTCoolMenu();

protected:

	// Array of command IDs to hide.
	CUIntArray m_arCommandID;
	// Size of the button bitmap.
	CSize m_sizeBmp;
	// Size of the button, including shadow.
	CSize m_sizeBtn;
	// Number of items the recent item list cycles through.
	int m_nRecentList;
	// Menu timer ID.
	int m_nIDEvent;
	// Time-out value, in milliseconds.
	int m_nTimeOut;
	// TRUE if intelligent menus are enabled.
	BOOL m_bIntelligent;
	// 'true' when the timer has been activated.
	bool m_bTimerActive;
	// (Win2K) TRUE if 'use menu underlines' is turned off in the control panel.
	BOOL m_bHasMnemonics;
	// TRUE if the current menu was activated through TrackPopupMenu.
	BOOL m_bIsTrackPopup;
	// Incremented when user calls IgnoreNextPopup();.
	static int m_iIgnoreCount;
    // List of the CMenu objects initialized.
	CList<CMenu*,CMenu*> m_menuList;
	// List of the item data pointers.
	CList<CXTItemData*,CXTItemData*> m_itemList;
    // Size of the border of popup menus.
	static CSize m_szPopupBorder;
	// Size of the icon margins. 
	static CSize m_szIconMargin;
	// 'true' if last message was WM_MOUSEMOVE and not WM_KEYDOWN
	static bool m_bLastMouseEvent;

public:

    // 'true' when the cool menu displays hidden commands.
	static bool m_bShowAll;
	// Set to 'true' when the popup is generated through CXTMenu which implements
	// part of the intelligent menu's logic.
	static bool m_bAllowIntelligent;
	// 'true' if the menu is not owned by CXTMenuBar.
	static bool m_bIsPopup;
	// The number of milliseconds for animation effect. Default value is 300.
	static int m_nAnimationDelay;
	// Type of animation to perform. 
	// This parameter can be one of the following values:
	// [ul]
	// [li][b]animateWindowsDefault[/b] - Animation as defined in the "Display" settings.
	// [li][b]animateRandom[/b] - Any of the first three in random selection.
	// [li][b]animateUnfold[/b] -  Unfold top to bottom.
	// [li][b]animateSlide[/b] - Slide in from left.
	// [li][b]animateFade[/b] - Fade-in.
	// [li][b]animateNone[/b] - No animation.
	// [/ul]
	// Default value is animateWindowsDefault. 
	// You can add new animation effect, see CXTAnimationMemDC description.
	static int m_nAnimationType;

    //:Ignore
    // Used by menu bar for linking menu bar button to menu (XP style)
    static int m_iMenuBarAlign;
	static CRect m_rectExclude;
    //:End Ignore


    // This member function returns the index for the specified command.
    // Returns the index of the specified command, or -1 if the command
	// is not found.
    //
    int GetButtonIndex(
		// The command ID to get the index for.
        WORD nID,
		// 'true' if the command ID is from the enabled image list.
		bool bEnabled=true);
    
    // This member function will set the timer identifier and time-out value
    // for the cool menu. The timer is activated when the mouse hovers over a
    // chevron when using intelligent menus. NOTE: if you are not using
    // the CXTMenuBar with your application you should set this to a
    // value of 100, otherwise you will need to also call CXTMenuBar::SetTimerInfo()
    // if you are changing the 'nIDEvent' parameter.
    //
    void SetTimerInfo(
		// Specifies the time-out value, in milliseconds.
        UINT nElapse,
		// Specifies a nonzero timer identifier.
        UINT nIDEvent=1000);

    // This member function will enable or disable the intelligent menu feature.
    //
    void SetIntelligentMode(
		// TRUE to enable intelligent menus. FALSE to disable intelligent menus.
        BOOL bIntelligent=TRUE);

    // This member function is called to install cool menus for the application.
    //
    virtual void Install(
		// Pointer to a CFrameWnd object that represents the application's main
		// window. If NULL, this will uninstall the cool menus.
        CFrameWnd* pFrame);

    // This member function will load a toolbar resource and extract
    // each button image to be used with its associated menu command.
    // Returns TRUE if successful, otherwise returns FALSE.        
    //
    virtual BOOL LoadToolbar(
		// The resource ID of a toolbar to extract images
        // from, to be displayed in cool menus.
        UINT nToolbarID,
		// 'true' if the toolbar is for enabled images.
		bool bEnabled=true);

    // This member function will load a toolbar resource and extract
    // each button image to be used with its associated menu command.
    // Returns TRUE if successful, otherwise returns FALSE.        
    //
    virtual BOOL LoadToolbar(
		// Reference to a CToolBarCtrl object.
        CToolBarCtrl& toolbar,
		// 'true' if the toolbar is for enabled images.
		bool bEnabled=true);

    // This member function will load a toolbar resource and extract
    // each button image to be used with its associated menu command.
    // Returns TRUE if successful, otherwise returns FALSE.        
    //
    virtual BOOL LoadToolbars(
		// An array of toolbar resource IDs used to extract 
        // images from, to be displayed in cool menus.
        const UINT* nToolbarIDs,
		// Size of the array passed in.
        int nSize,
		// 'true' if the toolbar array is for enabled images.
		bool bEnabled=true);

    // This member function adds the specified command to the list of
    // menu items to hide until activated by clicking on the chevron.
    // Returns TRUE if successful, otherwise returns FALSE.        
    //
    virtual BOOL HideCommand(
		// The command ID of a menu item to hide.
        UINT nCommandID);

    // This member function adds the specified menu item to the list of
    // menu items to hide until activated by clicking on the chevron.
    // Returns TRUE if successful, otherwise returns FALSE.        
    //
    virtual BOOL HideCommand(
		// Text string representing the popup menu item to hide.
        LPCTSTR lpszItem);

    // This member function adds the specified commands to the list of
    // menu items to hide until activated by clicking on the chevron.
    // Returns TRUE if successful, otherwise returns FALSE.        
    //
    virtual BOOL HideCommands(
		// An array of command IDs, of menu items, to hide.
        const UINT* nCommandIDs,
		// Size of the array passed in.
        int nSize);

    // This member function adds the specified menu items to the list of
    // menu items to hide until activated by clicking on the chevron.
    // Returns TRUE if successful, otherwise returns FALSE.        
    //
    virtual BOOL HideCommands(
		// An array of command IDs, of menu items, to hide.
        const LPCTSTR lpszItems,
		// Size of the array passed in.
        int nSize);

    // This member function will append the specified image resource (hIcon)
    // to the existing cool menu image list, and map the associated
    // command ID (uCmdID) to the newly added image.
    // Returns TRUE if successful, otherwise returns FALSE.
    //
	virtual BOOL AppendImageList(
		// Handle to the icon to add.
		HICON hIcon,
		// Command id of the icon.
		UINT uCmdID,
		// 'true' if the image is to be added to the enabled image list.
		bool bEnabled/*=true*/);

    // This member function will append the specified image resource (bitmap)
    // to the existing cool menu image list, and map the associated
    // command IDs to the newly added images.
    // Returns TRUE if successful, otherwise returns FALSE.
    //
    virtual BOOL AppendImageList(
		// Resource ID of the new bitmap to add to the cool
        // menu's image list.
        UINT nBitmapID,
		// Array of menu command(s) to be associated with the 
        // newly added image.
        UINT* arCmdIDs,
		// Size of the array passed in.
        int nSize,
		// 'true' if the image is to be added to the enabled image list.
		bool bEnabled=true);

    // This member function will append the specified image resource (bitmap)
    // to the existing cool menu image list, and map the associated
    // command IDs to the newly added images.
    // Returns TRUE if successful, otherwise returns FALSE.
    //
    virtual BOOL AppendImageList(
		// Handle of the new bitmap to add to the cool menu's image list.
        HBITMAP hBitmap,
		// Array of menu command(s) to be associated with the 
        // newly added image.
        UINT* arCmdIDs,
		// Size of the array passed in.
        int nSize,
		// 'true' if the image is to be added to the enabled image list.
		bool bEnabled=true);

    // Call this member function to check for the existence of a menu item.
    // Returns the position of the menu item if it exists, otherwise returns -1.
    //
    virtual int MenuItemExists(
		// Points to the active menu.
        CMenu* pMenu,
		// Command ID of a menu item.
        UINT nCommandID);

    // This member function sets the number of hidden items that can be
    // added to the short menu at any given time.
    //
    virtual void SetRecentItemLength(
		// Number of items the recent item list contains.  Set to 0 to
        // disable this feature.
        int nLength);

    // This member function is called to reset the recent item list used to
	// add recently used hidden items to the short menu.
    //
    virtual void ResetUsageData();

    // Call this member function if the next popup menu is not to be 
    // transformed by the CXTCoolMenu class.  For instance, use this before
    // displaying system menus.
	//
    static void IgnoreNextPopup();

	// Call this member function to set the default icon size for cool menus.
	// This will override the size that was set when LoadToolbar was called.
	// This member should be called [b]after[/b] InstallCoolMenus has been called
	// in the frame class.  Standard size is 16x15 pixels.
	//
	virtual void SetIconSize(
		// Represents the new size of the cool menu icons.
		CSize size);

	// Call this member function to get the default icon size for cool menus.
	// Returns a CSize object that represents the current icon size for cool menu
	// images.
	//
	virtual CSize GetIconSize();

	// This member function is called to add an image to the image map for
	// the cool menus.
	//
	virtual void AddImageToMap(
		// Command ID to associate with the newly added image.
		UINT nIDCmd,
		// Icon handle of the image to add.
		HICON hIcon,
		// Width and height of the newly added item.
		CSize sz,
		// 'true' if the image is to be added to the enabled image list.
		bool bEnabled=true);

protected:

    // This member function is called by the cool menu to paint a menu item's
    // text.
    //
    virtual void DrawMenuText(
		// Points to the current device context.
        CDC* pDC,
		// Size of the area to paint.
        CRect rect,
		// A CString object that represents the text to be displayed.
        CString strText,
		// An RGB value that represents the color of the text to be displayed.
        COLORREF clrColor);

    // This member function is called by the cool menu to paint checkmark
    // and radio buttons for each item.
    //
    virtual void Draw3DCheckmark(
		// Points to the current device context.
        CDC* pDC,
		// Size of the area to paint.
        const CRect& rect,
		// TRUE if the checkmark is to be drawn highlighted.
        BOOL bSelected,
		// TRUE if the checkmark is to be drawn disabled.
        BOOL bDisabled,
		// HBITMAP handle to the menu item info checkmark. It can be NULL.
        HBITMAP hbmCheck);

    // This member function is called by the cool menu to paint a chevron
	// that is used to display hidden menu items.
    //
    virtual void DrawChevron(
		// Points to the current device context.
        CDC* pDC,
		// Size of the area to paint.
        CRect rcItem,
		// TRUE if the chevron is to be drawn highlighted.
        BOOL bSelected);

    // This member function is called by the cool menu to convert the specified
	// CMenu object to a 'cool' menu.
    //
    virtual void ConvertMenu(
		// Points to a CMenu object to be made 'cool'.
        CMenu* pMenu,
		// TRUE if the menu has icons.
        BOOL bHasButtons);

    // This member function is called by the cool menu to check to see
	// if the specified menu item is to be hidden. Returns TRUE if the item is
	// hidden, otherwise returns FALSE.        
    //
    virtual BOOL IsItemHidden(
		// Command ID of a menu item.
        UINT nCommandID);

    // This member function is called by the cool menu to check to see
	// if the menu item is a separator. Returns TRUE if the menu item is a
	// separator, otherwise returns FALSE.
    //
	BOOL IsItemSeparator(
		// Points to the active menu.
        CMenu* pMenu,
		// Position of a menu item.
		int nPos);

    // This member function is called by the cool menu to check to see
	// if the previous menu item is hidden. Returns TRUE if the previous menu
	// item is hidden, otherwise returns FALSE.      
    //
    virtual BOOL IsPrevItemHidden(
		// Points to the active menu.
        CMenu* pMenu,
		// Position of a menu item.
        int nPos);

    // This member function is called by the cool menu to check to see
	// if the next menu item is hidden. Returns TRUE if the next menu item
	// is hidden, otherwise returns FALSE.      
    //
    virtual BOOL IsNextItemHidden(
		// Points to the active menu.
        CMenu* pMenu,
		// Position of a menu item.
        int nPos);

    // This member function is called by the cool menu to check to see
	// if the menu item is the first one visible. Returns TRUE if the menu
	// item is the first one visible, otherwise returns FALSE.
    //
    virtual BOOL IsFirstVisibleItem(
		// Points to the active menu.
        HMENU hMenu,
		// Position of a menu item.
        UINT nPos);

    // This member function is called by the cool menu to check to see if
    // the menu item is the last one visible. Returns TRUE if the menu
	// item is the last one visible, otherwise returns FALSE.
    //
    virtual BOOL IsLastVisibleItem(
		// Points to the active menu.
        HMENU hMenu,
		// Position of a menu item.
        UINT nPos,
		// Command ID of the item to check.
        UINT cmd);
    
    // This member function is called by the cool menu to append a chevron
	// menu item if the menu has hidden commands. Returns TRUE if the menu
	// was actually modified.
	//
    virtual BOOL UpdateChevronItem(
		// CMenu pointer to the active menu.
        CMenu* pMenu,
		// TRUE to append the menu.
        BOOL bAppend);

    // This member function is called by the cool menu to verify if the
	// specified menu is a system menu or not. Returns TRUE if the menu item
	// is a system menu, otherwise returns FALSE.        
    //
    virtual BOOL IsSysMenu(
		// CMenu pointer to the active menu.
        CMenu* pMenu);

    // This member function is called by the cool menu to restore previously
	// hidden menu items. Returns TRUE if there was a change to the menu.
	//
    virtual BOOL RestoreMenuItems(
		// CMenu pointer to the active menu.
        CMenu* pMenu);

    // This member function is called by the cool menu to hide menu items.
    //
    virtual void HideMenuItems(
		// CMenu pointer to the active menu.
        CMenu* pMenu);

    // This member function is called by the cool menu to check for hidden
	// items. Returns TRUE if the menu has hidden items, otherwise returns
	// FALSE.        
    //
    virtual BOOL HasHiddenItems(
		// CMenu pointer to the active menu.
        CMenu* pMenu);

    // This member function is called by the cool menu to add a hidden
	// item to the short menu. Returns TRUE if the menu item was added to
	// the recent item list, otherwise returns FALSE.
    //
    virtual BOOL AddRecentItem(
		// Menu item command ID.
        UINT nItem);

    // This member function is called by the cool menu to see if the specified
	// item is in the recent item list. Returns TRUE if the menu item is in
	// the recent item list, otherwise returns FALSE.
    //
    virtual BOOL IsRecentItem(
		// Menu item command ID.
        UINT nItem);

    // This internal member function checks to see if the menus are allowed
	// to use intelligent mode. Returns 'true' if the menus are allowed to
	// use intelligent mode, otherwise returns 'false'.  The default implementation
	// allows intelligent menus only if the menu was created through the CXTMenu
	// class.
	//
    virtual bool IsIntelligentModeAllowed();

    //:Ignore
    // Used by owner-drawn menu overrides
    static void UnhookCBTHook();
    static void HookCBTHook();
    static HHOOK m_hCBTHook;
    static LRESULT CALLBACK CBTHook(int nCode, WPARAM wParam, LPARAM lParam);
    static void RemoveBorder(HWND hWnd);
    friend class CXTCoolMenuInit;
    friend class CXTPopupMenu;
    //:End Ignore

    //:Ignore
    //{{AFX_VIRTUAL(CXTCoolMenu)
    virtual void Destroy();
    virtual void Refresh();
    virtual void OnMenuTimer(int nIDEvent);
    virtual void OnInitMenuPopup(CMenu* pMenu, UINT nIndex, BOOL bSysMenu);
    virtual BOOL OnMeasureItem(LPMEASUREITEMSTRUCT lpms);
    virtual BOOL OnDrawItem(LPDRAWITEMSTRUCT lpds);
    virtual LONG OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
    virtual void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL
    //:End Ignore

	//:Ignore
private:
	static bool				m_bIs95orNT4;
	static bool				m_bChevronSelected;
	static HMENU			m_hMenuLast;
	static CXTPopupMenu*	m_pPopupLast;
	static CXTPopupMenuList m_listPopups;
	int m_nTextHeight;
	//:End Ignore
};

//////////////////////////////////////////////////////////////////////
// CXTMenu is a CMenu derived class.  It is used to create a CXTMenu object.
// Use this class if you want your popup to be 'intelligent'.
//
// Please note that intelligent menus have somewhat more limited capabilities
// under [b]Windows 95 and Windows NT 4[/b] (all other versions of Windows do 
// not suffer from these limitations):
// [ul]
// [li]The menu will not expand automatically if you hover over the chevron.[/li]
// [li]Clicking on a chevron in a submenu will cause the submenu to close.[/li]
// [li]Intelligent menus are not available if you do not use the docking
// XT menu bar (CXTMenuBar).[/li]
// [/ul]
//
class _XT_EXT_CLASS CXTMenu : public CMenu
{
public:

    // Constructs a CXTMenu object.
	//
    CXTMenu();

    //:Ignore
    ~CXTMenu();
    //:End Ignore

    // Call this member function to display a floating popup menu at the
	// specified location and track the selection of items on the popup menu.
    // Returns nonzero if the function is successful, otherwise returns zero.
    //
    BOOL TrackPopupMenu(
		// Specifies a screen-position flag and a mouse-button flag.
        UINT nFlags,
		// Specifies the horizontal position in screen coordinates 
        // of the popup menu.
        int x,
		// Specifies the vertical position, in screen coordinates, of 
        // the top of the menu on the screen.
        int y,
		// Identifies the window that owns the popup menu.
        CWnd* pWnd,
		// Points to a RECT structure or a CRect object that contains 
        // the screen coordinates of a rectangle within which the user 
        // can click, without dismissing the popup menu.
        LPCRECT lpRect = NULL,
		// TRUE to send WM_COMMAND notification back to the owner window.
        BOOL bNotify = TRUE);

    // Call this member function to display a floating popup menu at the
	// specified location and track the selection of items on the popup menu.
    // Returns nonzero if the function is successful, otherwise returns zero.
    //
    BOOL TrackPopupMenuEx(
		// Specifies a screen-position flag and a mouse-button flag.
        UINT nFlags,
		// Specifies the horizontal position, in screen coordinates, 
        // of the popup menu.
        int x,
		// Specifies the vertical position, in screen coordinates, of 
        // the top of the menu on the screen.
        int y,
		// Identifies the window that owns the popup menu.
        CWnd* pWnd,
		// Pointer to a TPMPARAMS structure that specifies an area 
        // of the screen the menu should not overlap.
        LPTPMPARAMS lptpm = NULL,
		// TRUE to send WM_COMMAND notification back to the owner window.
        BOOL bNotify = TRUE);

    // Call this member function to get a pointer to a CMenu object given
	// a Windows handle to a menu. Returns a pointer to a CMenu object.  The
	// pointer may be temporary or permanent. 
    //
    static CXTMenu* PASCAL FromHandle(
		// A Windows handle to a menu.
        HMENU hMenu);

    // Call this member function to get a pointer to a CMenu object when
	// given a handle to a window. Returns a pointer to a CMenu object.  If
	// a CMenu object is not attached to the handle, NULL is returned.
    //
    static CXTMenu* PASCAL FromHandlePermanent(
		// A Windows handle to a menu.
        HMENU hMenu);

    // Call this member function to get a pointer to a CMenu object whose
	// m_hMenu member contains a handle to the popup menu, if a popup menu
	// exists at the given position. Returns a pointer to a CMenu object, if
	// a popup menu exists at the given position, otherwise returns NULL.
    //
    CXTMenu* GetSubMenu(
		// Specifies the position of the popup menu contained in the menu.
        int nPos) const;
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTCoolMenu::SetIntelligentMode(BOOL bIntelligent/*=TRUE*/) {
    m_bIntelligent = bIntelligent;
}
AFX_INLINE void CXTCoolMenu::SetRecentItemLength(int nLength) {
    m_nRecentList = nLength;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTCOOLMENU_H__)

