//:Ignore
// XTTrayIcon.h: interface for the CXTTrayIcon class.
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

#if !defined(__XTTRAYICON_H__)
#define __XTTRAYICON_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// XT_TRAYICON is a stand alone helper structure.  It is used by CXTTrayIcon
// to maintain an animation icon list.
//
struct XT_TRAYICON
{
	// A NULL terminated string that represents the tooltip displayed for the
	// tray icon.
	CString strToolTip;
	// Handle to the icon displayed.
	HICON hIcon;
};

//:Ignore
// Animated icon array.
typedef CList<XT_TRAYICON, XT_TRAYICON&> CXTTrayIconList;
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTTrayIcon is a CWnd derived class of the NOTIFYICONDATA structure.
//  It is used to display a system tray icon with animation.
//
class _XT_EXT_CLASS CXTTrayIcon : public CWnd
{
public:

	// Constructs a CXTTrayIcon object.
    //
    CXTTrayIcon();

    // Destroys a CXTTrayIcon object, handles cleanup and de-allocation.
    //
    virtual ~CXTTrayIcon();

protected:

	// Default icon.
	CXTIconHandle m_hIcon;
	// Style settings for icon restore.
	UINT m_uFlags;
	// Resource ID for the default icon.
	UINT m_nIconID;
	// Timer event ID.
	UINT m_nIDEvent;
	// Holds the current position in the timer loop.
	UINT m_nCounter;
	// Default popup menu item ID.
	UINT m_uDefMenuItemID;
	// Determines if the default menu item is a command or index.
	bool m_bDefMenuItemByPos;
	// State of the icon. 'true' to indicate the icon is hidden.
	bool m_bHidden;
	// 'true' if the icon has been removed from the system tray.
	bool m_bRemoved;
	// 'true' if the icon display is pending.
	bool m_bShowPending;
	// Maximum size for tooltip string.
	size_t m_iMaxTipSize;
	// Handle to the window that receives command notification.
	HWND m_hWndNotify;
	// Hidden window used during minimize and restore functions.
	CWnd m_wndMinimize;
	// Tooltip for the default icon.
	CString	m_strToolTip;
	// Tray icon structure.
	NOTIFYICONDATA m_NIData;
	// Array of icons and text that are displayed during animation.
	CXTTrayIconList m_arTrayIcons;

public:
	
    // This member function will create the system tray icon. Returns 'true'
	// if successful, otherwise returns 'false'.
    //
    virtual bool Create(
        // Default tooltip text to display for the icon.
        LPCTSTR lpszCaption,
        // Pointer to the window that will receive notification messages
		// associated with an icon in the taskbar status area.
        CWnd* pParentWnd,
        // Resource ID for the default tray icon.
        UINT nIconID,
		// Popup menu resource identifier.
		UINT uMenuID=0,
		// Command ID that represents the default item for the menu.
		UINT uDefMenuItemID=0,
		// 'true' if the default is defined by its position. 'false' if it is
		// defined by its command ID.
		bool bDefMenuItemByPos=false);

    // Call this member function to display a balloon tooltip for the system
	// tray icon. Must have Windows 2000 or later.  Returns 'true' if successful,
	// otherwise returns 'false'.
    // 
    bool ShowBalloonTip(
        // Pointer to a null-terminated string with the text for a balloon
		// ToolTip.  It can have a maximum of 255 characters.
        LPCTSTR lpszInfo=NULL,
        // Pointer to a null-terminated string containing a title for a
		// balloon ToolTip.  This title appears in boldface above the text. 
		// It can have a maximum of 63 characters.
        LPCTSTR lpszInfoTitle=NULL, 
        // Flags that can be set to add an icon to a balloon ToolTip. 
		// It is placed to the left of the title.  If the 'lpszInfoTitle' member
		// is zero-length, the icon is not shown, can be any of the following:
        //[ul]
        //[li][b]NIIF_ERROR[/b] - An error icon.[/li]
        //[li][b]NIIF_INFO[/b] - An information icon.[/li]
        //[li][b]NIIF_NONE[/b] - No icon.[/li]
        //[li][b]NIIF_WARNING[/b] - A warning icon.[/li]
        //[/ul]
        DWORD dwInfoFlags=NIIF_NONE,
        // The timeout value, in milliseconds, for a balloon ToolTip. 
		// The system enforces minimum and maximum timeout values. 'uTimeout'
		// values that are too large are set to the maximum value, and values
		// that are too small default to the minimum value. The system minimum
		// and maximum timeout values are currently set at 10 seconds and 30
		// seconds, respectively.
        UINT uTimeout=10);

    // This member function will stop the tray icon animation timer.
    //
    virtual void StopAnimation();

    // This member function will start the tray icon animation timer.
    //
    virtual void StartAnimation(
        // Specifies the time-out value, in milliseconds, between frames.
        UINT uElapse=500);

    // This member function will set the icons used for animated system
	// tray icons.  To use, call SetAnimationIcons, then SetTimer.  Returns
	// 'true' if successful, otherwise returns 'false'.
	//
    virtual bool SetAnimationIcons(
        // An array of resource IDs that represent the icons to display
		// in the caption area.
        const UINT* lpIDArray,
        // Size of the array 'lpStrTipArray'.
        int nIDCount,
        // An array of tooltips that match the icons passed in as 'lpStrTipArray'.
		// If NULL, the default tooltip will be used.
        const CString* lpStrTipArray=NULL);

	// Call this member function to set the tooltip text for the tray icon.
	// Returns 'true' if successful, otherwise returns 'false'.
	//
    bool SetTooltipText(
        // Null terminated string that represents the tooltip text to display
		// for the icon.
        LPCTSTR lpszTipText);

	// Call this member function to set the tooltip text for the tray icon.
	// Returns 'true' if successful, otherwise returns 'false'.
	//
	bool SetTooltipText(
        // Windows string resource ID that represents the tooltip text
		// to display for the icon.
		UINT nTipText);

	// Call this member function to retrieve the tooltip text that is displayed
	// by the tray icon. Returns a CString object that represents the tooltip text.
	//
	CString GetTooltipText() const;

	// This member function is called to reset the tray icon control settings
	// to their default value.
	//
	void SetDefaultValues();

	// This member function will set the callback message to be used by the
	// tray icon. Returns 'true' if successful, otherwise returns 'false'.
	//
	bool SetCallbackMessage(
		// Notification message ID.
		UINT uNewCallbackMessage);

	// This member function will retrieve the notification message ID used
	// by the tray icon. Returns an ID to a notification message.
	//
	UINT GetCallbackMessage();

	// This member function will set the default menu item for the tray icon's
	// popup menu if the tray icon handles notification messages. Returns
	// 'true' if successful, otherwise returns 'false'.
	//
	bool SetDefaultMenuItem(
		// Command identifier or position of the default menu item.
		UINT uItem,
		// 'true' if 'uItem' is the menu item's index, 'false' if it is the menu
		// item's command ID.
		bool bByPos);

	// Call this member function to retrieve the item ID and position flag
	// for the tray icon.
	//
	void GetDefaultMenuItem(
		// Reference to a UINT that is to receive the default menu item ID.
		UINT& uItem,
		// Reference to a bool that is to receive the default menu item position
		// flag.
		bool& bByPos);

	// Call this member function to set the window that is to receive notification
	// messages from the tray icon. Returns 'true' if successful, otherwise
	// returns 'false'.
	//
	bool SetNotificationWnd(
		// Points to a valid CWnd object.
		CWnd* pWndNotify);

	// Call this member function to retrieve a pointer to the window that
	// receives notification messages from the tray icon. Returns a pointer
	// to a valid CWnd object.
	//
	CWnd* GetNotificationWnd();

	// Call this member function to set the icon that is displayed in the
	// system tray. Returns 'true' if the icon was set, otherwise returns 'false'.
	//
	bool SetIcon(
		// Handle to the icon to display.
		HICON hIcon);

	// Call this member function to set the icon that is displayed in the
	// system tray. Returns 'true' if the icon was set, otherwise returns 'false'.
	//
	bool SetIcon(
		// Resource name of the icon to display.
		LPCTSTR lpszIconName);

	// Call this member function to set the icon that is displayed in the
	// system tray. Returns 'true' if the icon was set, otherwise returns 'false'.
	//
	bool SetIcon(
		// Resource identifier of the icon to display.
		UINT nIDResource);

	// Call this member function to return a handle to the icon displayed
	// in the system tray. Returns a handle to the icon displayed in the system tray.
	//
	HICON GetIcon() const;

	// Call this member function to add the icon to the system tray. Returns
	// 'true' if successful, otherwise returns 'false'.
	//
	bool AddIcon();

	// Call this member function to remove the icon from the system tray.
	// Returns 'true' if successful, otherwise returns 'false'.
	//
	bool RemoveIcon();

    // Call this member function to hide the system tray icon, in Windows
	// 2000 or greater.  Returns 'true' if successful, otherwise returns 'false'.
    //
    bool HideIcon();

    // Call this member function to show the system tray icon, in Windows
	// 2000 or greater.  Returns 'true' if successful, otherwise returns 'false'.
    //
    bool ShowIcon();

	// Call this member function to minimize the specified window to the system
	// tray.  If window animation is supported, the window will glide down
	// to the system tray.
	//
	void MinimizeToTray(
		// Window to minimize.
		CWnd* pWnd);

	// Call this member function to maximize the specified window from the
	// system tray.  If window animation is supported, the window will glide
	// up from the system tray.
	//
	void MaximizeFromTray(
		// Window to maximize.
		CWnd* pWnd);

	//:Ignore
    //{{AFX_VIRTUAL(CXTTrayIcon)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
    virtual void InstallIconPending();
	virtual bool CreateMinimizeWnd(CWnd* pWndApp);
	virtual void RemoveAnimationIcons();
	//:End Ignore

	//:Ignore
	//{{AFX_MSG(CXTTrayIcon)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	//}}AFX_MSG
    afx_msg LRESULT OnTaskbarCreated(WPARAM wParam, LPARAM lParam);
	//:End Ignore

    DECLARE_MESSAGE_MAP()

	//:Ignore
public:
	// OBSOLETE
	virtual bool Create(LPCTSTR lpszCaption, DWORD dwStyle, CWnd* pParentWnd, UINT nIconID);
    virtual void SetTooltip(CString strToolTip);
    virtual void SetTrayIcon(UINT nIcon, DWORD dwMessage=NIM_ADD);
    virtual void SetAnimationIcons(const UINT* lpIDArray,const CString* lpStrTipArray,int nIDCount);
    virtual void KillTimer();
    virtual void SetTimer(UINT nIDEvent, UINT uElapse);
	// END OBSOLETE
	//:End Ignore
};

//////////////////////////////////////////////////////////////////////

//:Ignore
// OBSOLETE
AFX_INLINE bool CXTTrayIcon::Create(LPCTSTR lpszCaption, DWORD dwStyle, CWnd* pParentWnd, UINT nIconID) {
	UNREFERENCED_PARAMETER(dwStyle); return Create(lpszCaption, pParentWnd, nIconID);
}
AFX_INLINE void CXTTrayIcon::SetTooltip(CString strToolTip) {
	SetTooltipText(strToolTip);
}
AFX_INLINE void CXTTrayIcon::SetTrayIcon(UINT nIcon, DWORD dwMessage) {
	UNREFERENCED_PARAMETER(dwMessage); SetIcon(nIcon);
}
AFX_INLINE void CXTTrayIcon::SetAnimationIcons(const UINT* lpIDArray,const CString* lpStrTipArray,int nIDCount) {
	SetAnimationIcons(lpIDArray, nIDCount, lpStrTipArray);
}
AFX_INLINE void CXTTrayIcon::KillTimer() {
	StopAnimation();
}
AFX_INLINE void CXTTrayIcon::SetTimer(UINT nIDEvent, UINT uElapse) {
	m_nIDEvent = nIDEvent; StartAnimation(uElapse);
}
// END OBSOLETE
//:End Ignore

//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__TRAYICON_H__)
