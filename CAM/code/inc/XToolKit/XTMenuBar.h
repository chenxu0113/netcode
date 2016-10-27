//:Ignore
// XTMenuBar.h interface for the CXTMenuBar class.
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

#if !defined(__XTMENUBAR_H__)
#define __XTMENUBAR_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

// forwards
class CXTMenu;
class CXTMenuBarItem;
class CXTToolsManager;
class CXTSysMenuBarItem;
class CXTSubMenuBarItem;
class CXTControlMenuBarItem;
class CXTMenuCustomHandler;

//////////////////////////////////////////////////////////////////////
// XT_TRACK_STATE - Enumeration used by CXTMenuBar to determine the current
// state of the menu bar.
//
typedef enum XT_TRACK_STATE
{
	// Normal, not tracking anything.
	TRACK_NONE = 0, 
	// Tracking buttons (F10/Alt mode).
	TRACK_BUTTON,   
	// Tracking popups.
	TRACK_POPUP     
};

//:Ignore
//////////////////////////////////////////////////////////////////////
// CXTMenuBarItem - Descriptor of a generic menu bar item
//
class _XT_EXT_CLASS CXTMenuBarItem : public CObject
{
	// Origin of this item (its NW corner)
	CPoint m_ptOrigin;
	// Owner of this item
	CXTMenuBar* const m_pMenuBar;
	// Tells if this item is a line wrap
	bool m_bWrapped;

public:
	// Constructor initializes with default values
	//
	CXTMenuBarItem(CXTMenuBar* pMenuBar);

	// Tells if this item is currently hidden.
	//
	virtual bool IsHidden() const;

	CXTMenuBar* GetMenuBar() const;

	// Renders this item.
	//
	virtual void Render(CDC* pDC) = 0;

	// Retrieves accelerator for this item if one exists .
	//
	virtual bool GetAccelerator(TCHAR* /*pchAccelerator*/);

	// Gets this item extent.
	//
	virtual CSize GetExtent() = 0;
	
	bool IsWrapped() const;

	virtual bool IsWrappable() const;

	void SetWrapped(bool bWrapped);

	// Gets this item origin (its NW point)
	//
	CPoint GetOrigin() const;

	// Gets this item origin (its NW point)
	// 
	void SetOrigin(CPoint ptOrigin);

	// Spacer specifies a distance between this and the following
	// menu bar item, applicable if there is next item and this one is not
	// wrapped.
	//
	virtual int GetSpacer() const;

	// Tracks associated popup menu, if any.
	//
	virtual void TrackMenu();

	// Handles double click on this item.
	//
	virtual void OnDblClick();

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE bool CXTMenuBarItem::IsHidden() const {
	/*// not hidden by default*/return false;
}
AFX_INLINE CXTMenuBar* CXTMenuBarItem::GetMenuBar() const {
	return m_pMenuBar;
}
AFX_INLINE bool CXTMenuBarItem::GetAccelerator(TCHAR* /*pchAccelerator*/) {
	return false;
}
AFX_INLINE bool CXTMenuBarItem::IsWrapped() const {
	return IsWrappable() && m_bWrapped;
}
AFX_INLINE bool CXTMenuBarItem::IsWrappable() const {
	return true;
}
AFX_INLINE void CXTMenuBarItem::SetWrapped(bool bWrapped) {
	ASSERT(IsWrappable()); m_bWrapped = bWrapped;
}
AFX_INLINE CPoint CXTMenuBarItem::GetOrigin() const {
	return m_ptOrigin;
}
AFX_INLINE void CXTMenuBarItem::SetOrigin(CPoint ptOrigin) {
	m_ptOrigin = ptOrigin;
}
AFX_INLINE int CXTMenuBarItem::GetSpacer() const {
	return 0;
}
AFX_INLINE void CXTMenuBarItem::TrackMenu() {
}
AFX_INLINE void CXTMenuBarItem::OnDblClick() {
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTMenuBar is a CXTControlBar derived class.  It is used to create an
// Office&trade; style menu bar.  Use it the way you would a CToolBar, only you
// need to call LoadMenuBar instead of LoadToolbar.
//
class _XT_EXT_CLASS CXTMenuBar : public CXTControlBar
{
	DECLARE_DYNAMIC(CXTMenuBar)

public:

	// Constructs a CXTMenuBar object.
	//
	CXTMenuBar();

	// Destroys a CXTMenuBar object, handles cleanup and de-allocation.
	//
	virtual ~CXTMenuBar();

protected:

	// Array of menu bar items.
	CTypedPtrArray<CObArray, CXTMenuBarItem*> m_arrItems;
	// Null-terminated string that specifies the value name in the registry.
	CString m_strValueName;
	// Null-terminated string that specifies the key name in the registry.
	CString m_strSubKey;
	// Array of hidden menu commands.
	CUIntArray m_arrHiddenCommands;
	// OLE menu item routing map.
	CMap<long, long, BOOL, BOOL> m_mapMenu2OLEItem;
	// Hooks frame window messages.
	CXTMBarWndHook* m_pFrameHook;
	// Hooks MDI client messages.
	CXTMBarMDIWndHook* m_pChildFrameHook;
	// Mouse location when tracking the popup.
	CPoint m_ptMouse;
	// Index of the currently depressed item.
	int m_iDepressedItem;
	// Index of the frame control button being tracked.
	int m_iTracking;
	// Index of the currently hot item.
	int m_iHotItem;
	// Menu timer ID.
	int m_nIDEvent;
	// Time-out value, in milliseconds.
	int m_nTimeOut;
	// Index of which popup is being tracked, if any.
	int m_iPopupTracking;
	// Index of the next menu to track.
	int m_iNewPopup;
	// Index of the currently selected menu bar item.
	int m_iButton;
	// Menu bar resource ID.
	int m_nMenuID;
	// Most recently used max width, or -1 if not set.
	int m_nMRUMaxWidth;
	// Win2000, TRUE if 'use menu underlines' is turned off in the control
	// panel.
	BOOL m_bMenuUnderlines;
	// TRUE, if the application has activation.
	BOOL m_bActive;
	// TRUE, if the window is maximized.
	BOOL m_bMDIMaximized;
	// If this is an MDI application.
	HWND m_hWndMDIClient;
	// Handle to the currently active menu.
	HMENU m_hMenu;
	// Pointer to the current popup being tracked.
	CXTMenu* m_pMenuPopup;
	// Handle to the menu loaded via LoadMenuBar.
	HMENU m_hMenuDefault;
	// Handle to the "Window" menu.
	HMENU m_hMenuShared;
	// Current tracking state.
	XT_TRACK_STATE m_iTrackingState;
	// 'true' when the timer is activated.
	bool m_bTimerActive;
	// 'true' to display the "More Windows" menu command for MDI document
	// management.
	bool m_bMoreWindows;
	// 'true' if the menu bar is stretched to fit the entire window space.
	bool m_bStretchToFit;
	// Tells if the last mouse position (m_ptMouse) has been initialized.
	bool m_bPtMouseInit;
	// Tells if MDI buttons shall be rendered when the MDI child is maximized.
	bool m_bShowMDIButtons;
	// 'true' when the button is pressed.
	bool m_bDown;
	// 'true' to process left/right arrow keys.
	bool m_bProcessRightArrow;
	// 'true' to move to prev/next popup.
	bool m_bProcessLeftArrow;
	// 'true' if the user pressed escape to exit the menu.
	bool m_bEscapeWasPressed;
	// Tells if idle update shall check for the mouse position to reset the hot
	// item once the mouse leaves the window.
	bool m_bDelayCheckMouse;
	// Holds a 'this' pointer.  Set during menu tracking.
	static CXTMenuBar* m_pMenuBar;
	// Handle to the message hook.  Set during menu tracking.
	static HHOOK m_hMsgHook;
	// Counts number of times the hook has been set.
	static int m_iHookRefCount;
	// MDI frame button tooltip.
	CToolTipCtrl m_toolTip;
	// Image list used by the MDI frame buttons.
	CImageList m_imageList;
//:Ignore
private:
	CXTMenuCustomHandler* m_pCustomHandler;
	CMap<HMENU,HMENU,HMENU,HMENU> m_mapTools;
//:End Ignore

public:

	// 'true' when hidden menu items are displayed.
	static bool m_bShowAll;
	// 'true' when the alt key is pressed.
	static bool m_bAltKey;

	// This function translates special menu keys and mouse actions, and is
	// called from CXTFrameWnd's PreTranslateMessage. Returns TRUE if successful,
	// otherwise returns FALSE.
	//
	virtual BOOL TranslateFrameMessage(
		// Pointer to an MSG structure.
		MSG* pMsg);

	// Call this function to load a different menu.  The HMENU must not belong
	// to any CMenu, and you must free it when you are done.  Returns a handle
	// to the old menu.  'hMenuShared' is the MDI "Window" menu, if any (similar
	// to WM_MDISETMENU).
	//
	HMENU LoadMenu(
		// Handle to the new menu.
		HMENU hMenu,
		// Handle to the new shared menu.
		HMENU hMenuShared);

	// This member function retrieves a pointer to the currently active menu.
	// Returns a CMenu pointer object to the currently active menu.
	//
	CMenu* GetMenu() const;

	// Call this member function to load the menu bar specified by 'nMenuID'.
	// Returns nonzero if successful, otherwise returns zero.
	//          
	BOOL LoadMenuBar(
		// Resource ID of the menu to load.
		UINT nMenuID);

	// Call this member function to load the menu bar specified by 'lpszMenuName'.
	// Returns nonzero if successful, otherwise returns zero.
	//          
	BOOL LoadMenuBar(
		// Pointer to the resource name of the menu bar to be loaded.
		LPCTSTR lpszMenuName);

	// This member function creates a Windows menu bar, a child window, and
	// associates it with the CXTMenuBar object.  It also sets the menu bar
	// height to a default value. Returns nonzero if successful, otherwise
	// returns zero.
	//          
	virtual BOOL Create(
		// Pointer to the window that is the menu bar’s parent.
		CWnd* pParentWnd,
		// The menu bar style. Additional menu bar styles supported are: 
		//[ul]
		//[li][b]CBRS_TOP[/b] - Control bar is at the top of the frame window.[/li]
		//[li][b]CBRS_BOTTOM[/b] - Control bar is at the bottom of the frame window.[/li]
		//[li][b]CBRS_NOALIGN[/b] - Control bar is not repositioned when the
		//       parent is resized.[/li]
		//[li][b]CBRS_TOOLTIPS[/b] - Control bar displays tool tips.[/li]
		//[li][b]CBRS_SIZE_DYNAMIC[/b] - Control bar is dynamic.[/li]
		//[li][b]CBRS_SIZE_FIXED[/b] - Control bar is fixed.[/li]
		//[li][b]CBRS_FLOATING[/b] - Control bar is floating.[/li]
		//[li][b]CBRS_FLYBY[/b] - Status bar displays information about the button.[/li]
		//[li][b]CBRS_HIDE_INPLACE[/b] - Control bar is not displayed to the user.[/li]
		//[/ul]
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP,
		// The menu bar’s child-window ID.
		UINT nID = AFX_IDW_MENUBAR);

	// Call this member function to create a Windows menu bar, a child window,
	// and associate it with the CXTToolBar object.  It also sets the menu bar
	// height to a default value.
	//
	// Use CreateEx, instead of Create, when certain styles need to be present
	// during the creation of the embedded menu bar control.  For example, set
	// 'dwCtrlStyle' to TBSTYLE_FLAT | TBSTYLE_TRANSPARENT to create a menu bar
	// that resembles the Internet Explorer 4 menu bars.
	// Returns nonzero if successful, otherwise returns zero.
	// 
	virtual BOOL CreateEx(
		// Pointer to the window that is the menu bar’s parent.
		CWnd* pParentWnd,
		// Not used, should be zero
		DWORD dwUnused = 0,
		// The menu bar style.  See Toolbar Control and Button Styles in the Platform
		// SDK for a list of appropriate styles.
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,
		// A CRect object that defines the widths of the menu bar window borders.
		// These borders are set to (0,0,0,0) by default, thereby resulting in
		// a menu bar window with no borders.
		CRect rcBorders = CRect(0, 0, 0, 0),
		// The menu bar’s child-window ID.
		UINT nID = AFX_IDW_MENUBAR);

	// This member function will set the timer identifier and time-out value
	// for the cool menu.  The timer is activated when the mouse hovers over a
	// menu bar button when using intelligent menus.  NOTE:  You will need to also
	// call CXTCoolMenu::SetTimerInfo() if you are changing the 'nIDEvent' parameter.
	//
	void SetTimerInfo(
		// Specifies the time-out value, in milliseconds.
		UINT nElapse,
		// Specifies a nonzero timer identifier.
		UINT nIDEvent=1000);

	// Call this member function to enable the menu bar to occupy the entire 
	// application area or size the menu bar to the area occupied by menu items
	// only.
	//
	void SetStretchToFit(
		// TRUE to stretch to fit the entire application area, FALSE to fit only
		// the area occupied by menu items.
		BOOL bStretchToFit);

	// Call this member function to get the stretch state of the menu bar.  Returns
	// TRUE if the menu occupies the entire application area, or FALSE if it only
	// occupies the area the size of the menu items.
	//
	BOOL GetStretchToFit();

	// Call this member function to enable or disable rendering of the MDI buttons
	// when an MDI frame is maximized.  Call this function after you have created
	// your menubar.  The button layout will be automatically adjusted. 
	//
	void ShowMDIButtons(
		// TRUE to display MDI buttons, FALSE to hide them.
		BOOL bShowMDIButtons);

	// This member function tells if MDI buttons are currently rendered.
	// Returns TRUE if the MDI buttons are rendered, otherwise returns FALSE.
	//
	BOOL IsShowMDIButtons() const;

	// This member function adds the specified command to the list of
	// menu items to hide until activated by clicking on the chevron.
	// Returns TRUE if successful, otherwise returns FALSE.        
	//
	virtual BOOL HideCommand(
		// The command ID of a menu item to hide.
		UINT nCommandID);

	// This member function adds the specified commands to the list of
	// menu items to hide until activated by clicking on the chevron.
	// Returns TRUE if successful, otherwise returns FALSE.        
	//
	virtual BOOL HideCommands(
		// An array of command IDs, of menu items, to hide.
		const UINT* nCommandIDs,
		// Size of the array passed in.
		int nSize);

	// This function checks if the object passed in as 'pObject' is a CXTMenuBar
	// object. Returns TRUE if the object passed in is a CXTMenuBar object,
	// otherwise returns FALSE.
	//
	static BOOL IsMenuBar(
		// Represents a valid CObject pointer.
		CObject* pObject);

	// This function tells if the active MDI frame is currently maximized.
	// Returns TRUE if the active MDI frame is maximized, otherwise returns
	// FALSE.
	//
	BOOL IsMDIMaximized() const;

	// Call this member function to enable or disable the "More Windows" menu
	// item that is displayed in the "Windows" pull down menu.
	//
	void EnableMoreWindows(
		// 'true' to show "More Windows", or 'false' to hide.
		bool bEnable=true);

	// This menu function is called to determine the window handle for command
	// routing. Returns an HWND data type that represents the window handle.
	//
	HWND OleMenuDescriptor(
		// A reference to a valid BOOL value.
		BOOL& bSend,
		// Specifies the message to be sent. 
		UINT nMsg,
		// Specifies additional message-specific information.
		WPARAM wParam,
		// Specifies additional message-specific information.
		LPARAM lParam);

	// This member function returns a pointer to the menu bar owner frame.
	// Returns a pointer to a valid CFrameWnd object.
	//
	CFrameWnd* GetOwnerFrame();
	
	// This member function returns a pointer to the OLE document that sent
	// the last command if the menu bar is used with an OLE framework. Returns
	// a pointer to a valid COleDocument object.
	//
	COleDocument* GetCmdSentOleDoc();
	
	// This member function returns a pointer to the OLE window that sent
	// the last command if the menu bar is used with an OLE framework. Returns
	// a pointer to a valid CWnd object.
	//
	CWnd* GetCmdSentOleWnd();

	// This member function is called to fill the routing map for the OLE
	// framework. Returns TRUE if successful, otherwise returns FALSE.
	//
	BOOL FillCommandRoutingMap();

protected:

	// This member function will insure the menu always appears inside the
	// window. It will, given a button rectangle, compute point and "exclude
	// rect" for TrackPopupMenu, based on the current docking style, so that
	// the menu will always appear inside the window. Returns a CPoint object.
	//
	virtual CPoint ComputeMenuTrackPoint(
		// A reference to a valid CRect object that represents the size of the
		// button.
		const CRect& rcButton,
		// Reference to a TPMPARAMS struct.
		TPMPARAMS& tpm,
		// A reference to TrackPopupMenu flags
		UINT &nFlags
		);

	// This member function initializes an MDI "Window" menu by adding names
	// of all the MDI children.  This duplicates what the default handler
	// for WM_MDISETMENU does, but it is necessary to reinvent the wheel since
	// menu bars manage the menus themselves.  This function is called when
	// the frame gets WM_INITMENUPOPUP.
	//
	virtual void OnInitWindowMenu();

	// Call this member function to handle a mouse message.  It looks for
	// a click on one of the buttons.  Returns TRUE if handled; i.e., caller
	// should eat the mouse message.
	//
	virtual BOOL OnMouseMessage(
		// Message identifier.
		UINT message,
		// Indicates whether various virtual keys are down.  This parameter 
		// can be any combination of the following values: 
		//[ul]
		//[li][b]MK_CONTROL[/b] - Set if the CTRL key is down.[/li]
		//[li][b]MK_LBUTTON[/b] - Set if the left mouse button is down.[/li]
		//[li][b]MK_MBUTTON[/b] - Set if the middle mouse button is down.[/li]
		//[li][b]MK_RBUTTON[/b] - Set if the right mouse button is down.[/li]
		//[li][b]MK_SHIFT[/b] - Set if the SHIFT key is down.[/li]
		//[/ul]
		UINT nFlags,
		// Specifies the x- and y-coordinate of the cursor.  These coordinates 
		// are always relative to the upper-left corner of the window.
		CPoint pt);

	// This member function makes the frame recalculate the control bar sizes
	// after a menu change.
	//
	void DoLayout();

	// This member function gets the button index of the button before, or
	// after, a given button specified by 'iButton'. Returns the index of the
	// previous, or next, button found.
	//
	int GetNextOrPrevButton(
		// Index of the starting button.
		int iButton,
		// TRUE to search for the previous button.  FALSE to search for the next
		// button.
		BOOL bPrev) const;

	// This member function sets the tracking state for a button to either none,
	// button, or popup.
	//
	void SetTrackingState(
		// Passed in XT_TRACT_STATE structure.
		XT_TRACK_STATE iState,
		// Index of the button to set the state for.
		int iButton=-1);

	// This member function sets an 'iButton' item as hot-selected.
	//
	void SetHotItem(
		// The index of the button item to make hot.
		int iButton);

	// This member function makes an 'iButton' item appear pressed.  Pass in
	// -1 to release all buttons.
	//
	void PressItem(
		// The index of the button item to depress.
		int iButton);

	// This member function searches for a CXTMenuBarItem object. Returns
	// the index of the item, or -1 if the item is not found.
	//
	int FindItem(
		// A pointer to a valid CXTMenuBarItem object.
		CXTMenuBarItem* pItem);

	// This member function tracks the popup submenu associated with the active 
	// button in the menu bar. This function actually goes into a loop, tracking
	// different menus until the user selects a command or exits the menu.
	//
	void TrackPopup(
		// Index of the button to track.
		int iButton);

	// This member function determines which menu bar item, within the range of
	// 'itemsBegin' to 'itemsEnd', a point is in ('pt' in client coordinates).
	// Returns the index of the item that contains 'pt', or -1 if there
	// was no match.
	//
	virtual int HitTest(
		// Specifies the x- and y-coordinate of the cursor.  These coordinates 
		// are always relative to the upper-left corner of the window.
		CPoint pt,
		// Pointer to the first entry in an array of item indices that need to be
		// checked.
		int* itemsBegin,
		// Pointer past the last entry in an array of item indices that need
		// to be checked. 
		int* itemsEnd
		) const;

	// This member function toggles the state from home state to button-tracking,
	// and back.
	//
	void ToggleTrackButtonMode();

	// This member function cancels the current popup menu by posting WM_CANCELMODE,
	// and tracks a new menu.
	//
	void CancelMenuAndTrackNewOne(
		// Index of the new popup to track, or -1 to quit tracking.
		int iButton);

	// This member function is called by the menu bar when a user selects a
	// new menu item.  This will determine if the selected item is a submenu
	// and or parent menu item.  This way the menu bar knows whether the right
	// or left arrow key should move to the next menu popup.
	//
	void OnMenuSelect(
		// Handle to the menu.
		HMENU hMenu,
		// Menu item ID or submenu index of the item selected.
		UINT nItemID);

	// This member function checks whether the MDI maximized state has changed.
	// If so, add or delete the min/max/close buttons to or from the menu bar.
	//
	void CheckMinMaxState(
		// Tells if the layout shall be recalculated if a change is detected.
		bool bDoLayout);

	// This member function is called to check to see if the button specified
	// by 'iButton' is a valid index. Returns TRUE if it is a valid index, otherwise
	// returns FALSE.
	//
	BOOL IsValidButton(
		// Index of the button to check.
		int iButton) const;

	// This member function handles a menu input event.  It looks for a left
	// or right arrow key to change the popup menu, or mouse movement over
	// a different menu button for "hot" popup effect. Returns TRUE if message
	// is handled (to eat it).
	//
	virtual BOOL OnMenuInput(
		// A Reference to a valid tagMSG structure.
		MSG& m,
		// A pointer to a valid CWnd object.
		CWnd *pWndMenu = NULL);

	// This member function is called to update the display for Windows 2000 menus.
	//
	void UpdateDisplay(
		// 'true' if the alt key was used to activate menu.
		bool bAltKey);

	// The control bar calls this method to return the window style for the
	// active MDI child. Returns the window styles for the active MDI client
	// window.
	//
	long GetMDIWindowStyle();

	// This member function removes all menu bar items.
	//
	void RemoveAll();

	// This member function gets the indices of all the items currently visible.
	// Returns a pointer past the last initialized buffer item.	
	//
	int* CXTMenuBar::GetItems(
		// Beginning of the buffer to store indices.
		int* itemsBegin,
		// Points past the last item in the buffer.
		int* itemsEnd,
		// OR'ed (|) combination of the ITEMTYPE_... flags.
		int flags) const;

	// This member function computes the item rectangle, as per current menu bar
	// orientation. Returns a CRect object that represents the item.
	//
	CRect GetItemRect(
		// A pointer to a valid CXTMenuBarItem object.
		CXTMenuBarItem* pItem) const;

	// This member function computes the total width and height of all the
	// borders. Returns a CSize object that represents the width and height
	// of all the borders.
	//
	CSize GetBorderSize(
		// 'true' if horizontal border size is to be calculated.
		bool bHorz) const;

	// This member function resets the state tracking variables.
	//
	void ResetTrackingState();

	// This member function tells if this menu bar shall be visualized when
	// dragging or if the system shall render it with a wire frame.  Default
	// implementation uses full window drag. Returns 'true' to enable full
	// window drag, or 'false' to use the wire frame.
	//
	virtual bool IsFullWindowDrag();

	// This member function calculates the position of the menu bar hot spot,
	// i.e., the point that is used to pin the control bar rect to the mouse
	// cursor when dragging it. Returns 'true' if the menu bar defines a hot
	// spot (the default) in which case a buffer, pointed to by 'pSize', is
	// filled with the hot spot offset from the top-left corner.  Returns
	// 'false' to indicate that no hot spot is defined.
	//
	virtual bool GetHotSpot(
		//[ul]
		//[li][b]IN[/b] - Extent of the rectangle in which the hot spot must be defined.[/li]
		//[li][b]OUT[/b] - Offset of the hot spot from the rect's top-left corner.[/li]
		//[/ul]
		LPSIZE pSize);

	//:Ignore
	// returns a menu popup by its CRC32-based ID
	HMENU GetMenuPopupByCrc(DWORD dwMenuPopupID);

	//{{AFX_VIRTUAL(CXTMenuBar)
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);
	virtual CSize CalcFixedLayout(BOOL bStretch, BOOL bHorz);
	virtual CSize CalcDynamicLayout(int nLength, DWORD nMode);
	//}}AFX_VIRTUAL
	virtual XT_TRACK_STATE GetTrackingState(int& iPopup);
	virtual void OnInitMenuPopup();
	virtual int OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	virtual void OnBarStyleChange(DWORD dwOldStyle,  DWORD dwNewStyle);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	//{{AFX_MSG(CXTMenuBar)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
 
	DECLARE_MESSAGE_MAP()

	// Menu filter hook just passes to virtual CXTMenuBar function
	static LRESULT CALLBACK MenuInputFilter(int code, WPARAM wParam, LPARAM lParam);

	// drawing helpers
	void OnDrawIcon(CDC* pDC, CXTSysMenuBarItem* pItem);
	void OnDrawMenuItem(CDC* pDC, CXTSubMenuBarItem* pItem);
	void OnRenderControl(CDC* pDC, CXTControlMenuBarItem* pItem);
	void DrawMenuItemText(CDC* pDC, CRect& rcItem, CXTSubMenuBarItem* pItem, bool bHorz);
	void DrawVertText(CDC* pDC, CRect& rcItem, CXTSubMenuBarItem* pItem, COLORREF crText);
	void DrawHorzText(CDC* pDC, CRect& rcItem, CXTSubMenuBarItem* pItem);

	// these track menu popups
	void OnTrackSubMenu(CXTSubMenuBarItem* pItem);
	void OnTrackWindowMenu(CXTSysMenuBarItem* pItem);
	void OnDblClickWindowMenu(CXTSysMenuBarItem* pItem);

	// Layout helpers
	CSize CalcLayout(DWORD nMode, int nLength = -1);
	CSize CalcItemExtent(bool bHorz);
	int WrapMenuBar(int nWidth);
	void SizeMenuBar(int nLength, bool bHorz);
	void CalcItemLayout(bool bHorz); 
	int GetClipBoxLength(bool bHorz);

	// alignment (CBRS_ALIGN_...) with regard to floating status
	DWORD GetAlignmentStyle() const;
    void DrawXPFrameControl(CDC* pDC, CRect& r, UINT uStyle, bool bHilite=false, bool bPushed=false);
	CFont& GetTextFont() const;
    CSize GetTextSize(CDC* pDC, CString strMenuText) const;
    TCHAR GetHotKey(LPCTSTR lpszMenuName) const;
	virtual void ActivateToolTips(CPoint point, UINT uState);

	virtual BOOL IsFloating() const;
	virtual BOOL IsHorzDocked() const;

	// This notification is called whenever this menu bar is added to a CXTReBar object.
	virtual bool OnAddedToRebar(
		// Pointer to a rebar bar descriptor to use to fill in the sizeing information
		REBARBANDINFO* lp,
		// Tells if this control bar must be oriented horizontally
		bool bHorz);

	// Called whenever this menu bar is embedded in CXTReBar control
	// that has just resized the band in which this menu bar resides.
	virtual void OnRebarBandResized(
		// Descriptor of the band 
		XT_REBARSIZECHILDINFO* pInfo, 		
		// Tells if horizontally oriented
		bool bHorz);

	// sets customization mode on/off
	void SetCustMode(
        bool bMode);

	//:End Ignore

	// This member function is used to determine the correct background fill
	// color to be used during paint operations. Returns an RGB color value.
	//
	virtual COLORREF GetBackgroundColor() const;

	friend class CXTMenu;
	friend class CXTCustTools;
	friend class CXTMBarWndHook;
    friend class CXTToolsManager;
	friend class CXTMBarMDIWndHook;
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE BOOL CXTMenuBar::IsValidButton(int iButton) const {
	ASSERT(::IsWindow(m_hWnd)); return 0 <= iButton && iButton < m_arrItems.GetSize();
}
AFX_INLINE XT_TRACK_STATE CXTMenuBar::GetTrackingState(int& iPopup) {
	ASSERT(::IsWindow(m_hWnd)); iPopup = m_iPopupTracking; return m_iTrackingState;
}
AFX_INLINE CMenu* CXTMenuBar::GetMenu() const {
	ASSERT(::IsWindow(m_hWnd)); return CMenu::FromHandle(m_hMenu);
}
AFX_INLINE void CXTMenuBar::SetStretchToFit(BOOL bStretchToFit) {
	m_bStretchToFit = (bStretchToFit != 0);
}
AFX_INLINE BOOL CXTMenuBar::GetStretchToFit() {
	return m_bStretchToFit;
}
AFX_INLINE void CXTMenuBar::ShowMDIButtons(BOOL bShowMDIButtons) {
	ASSERT(::IsWindow(m_hWnd)); m_bShowMDIButtons = (bShowMDIButtons != 0); DoLayout();
}
AFX_INLINE BOOL CXTMenuBar::IsShowMDIButtons() const {
	return m_bShowMDIButtons;
}
AFX_INLINE BOOL CXTMenuBar::IsMenuBar(CObject* pObject) {
	return pObject->IsKindOf(RUNTIME_CLASS(CXTMenuBar));
}
AFX_INLINE void CXTMenuBar::EnableMoreWindows(bool bEnable/*=true*/) {
	m_bMoreWindows = bEnable;
}
AFX_INLINE BOOL CXTMenuBar::IsMDIMaximized() const {
	return m_bMDIMaximized;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTMENUBAR_H__)
