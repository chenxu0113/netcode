//:Ignore
// XTMDIWndTab.h : header file
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

#if !defined(__XTMDIWNDTAB_H__)
#define __XTMDIWNDTAB_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

// forwards
class CXTMDIWndTab;

//////////////////////////////////////////////////////////////////////
// CXTMDIClientWnd is a CWnd derived helper class for the CXTMDIWndTab tab
// control.  This class routes messages sent to the client window back
// to the tab control.
//
class _XT_EXT_CLASS CXTMDIClientWnd : public CWnd
{
public:

    // Constructs a CXTMDIClientWnd object.
	//
	CXTMDIClientWnd(
        // Pointer to a CXTMDITabWnd object.
        CXTMDIWndTab* pMDIWndTab);

	// Destroys a CXTMDIClientWnd object, handles cleanup and de-allocation.
    //
    virtual ~CXTMDIClientWnd();

protected:

	// Pointer to the MDI tab control.
	CXTMDIWndTab* m_pMDIWndTab;
	// Amount, in pixels, between the client and the tab control.
	int m_iBorderGap;

public:

	// This member function will set the size of the gap between the client
	// area and the tab control.
    //
	void SetBorderGap(
        // Amount, in pixels, of gap between the tab control and the client.
        int iSize);

	//:Ignore
    //{{AFX_VIRTUAL(CXTMDIClientWnd)
	protected:
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL
    //:End Ignore

	//:Ignore
    //{{AFX_MSG(CXTMDIClientWnd)
	//}}AFX_MSG
	afx_msg LRESULT OnMDICreate(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMDIDestroy(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMDIActivate(WPARAM wParam, LPARAM lParam);
    //:End Ignore

	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTMDIClientWnd::SetBorderGap(int iBorderGap) {
	m_iBorderGap = iBorderGap;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//////////////////////////////////////////////////////////////////////
// XT_MDICHILD is a stand alone structure class.  It is used to create
// an XT_MDICHILD structure.
//
struct XT_MDICHILD
{
	// Index of the item in the tab control.
	int iItem;
	// Window handle of the tab item.
	HWND hWnd;
	// Tab label.
	CString strItem;
};

//:Ignore
// Array of XT_MDICHILD structs that represent each item in the tab control.
typedef CList<XT_MDICHILD*,XT_MDICHILD*> CMDIChildArray;
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTMDIWndTab is a multiple inheritance class derived from CTabCtrl and
// CXTTabCtrlBase. CXTMDIWndTab is used to create a tab control for a multiple
// document interface (MDI) application. 
//
class _XT_EXT_CLASS CXTMDIWndTab : public CTabCtrl, public CXTTabCtrlBase
{
	DECLARE_DYNAMIC(CXTMDIWndTab)

public:

    // Constructs a CXTMDIWndTab object.
    //
	CXTMDIWndTab();

    // Destroys a CXTMDIWndTab object, handles cleanup and de-allocation.
    //
	virtual ~CXTMDIWndTab();

protected:

	// Tab image list.
	CImageList m_imageList;
	// Array of the MDI windows added to the tab control.
	CMDIChildArray m_arMDIChildern;
	// Points to the owner frame.
	CMDIFrameWnd* m_pMDIFrameWnd;
	// Window that receives messages on behalf of the MDI client.
	CXTMDIClientWnd* m_pMDIClientWnd;
	// Initialization state signature to synch up posted init requests.
	DWORD m_dwInitSignature;
	// Active MDI child.
	HWND m_hActiveChild;
	// Amount, in pixels, between the client and the tab control.
	int m_iBorderGap;
	// Index of the tab that received a right click.
	int m_iHitTest;
	// Index of the popup menu contained in the menu.
	int m_nPos;
	// Command ID of the default menu item for the popup menu.
	UINT m_nDefCmd;
	// Popup menu resource ID.
	UINT m_popupMenuID;
	// TRUE if no icons are used.
	BOOL m_bNoIcons;
	// Tells if a request to recalc the parent frame is pending.
	BOOL m_bRecalcLayoutPending;
	// Hash table that maps labels to the MDI child frame windows.
	CMap<HWND, HWND, CString, CString&> m_mapTabLabels;
	// Hash table that maps icons to the MDI child frame windows.
	CMap<HWND, HWND, HICON, HICON&> m_mapTabIcons;

public:

	// Call this member function to set the icon for the tab item specified
	// by 'iIndex'. Returns 'true' if successful, otherwise returns 'false'.
	//
	bool SetTabIcon(
		// Zero-based index of the tab to set the tab icon for.
		int iIndex,
		// Reference to an HICON handle that represents the tab icon.
		HICON& hIcon);

	// Call this member function to set the icon for the tab item specified
	// by 'hChildWnd'. Returns 'true' if successful, otherwise returns 'false'.
	//
	bool SetTabIcon(
		// Valid HWND of the MDI child window to set the tab icon for.
		HWND hChildWnd,
		// Reference to an HICON handle that represents the tab icon.
		HICON& hIcon);

	// Call this member function to set the label for the tab item specified
	// by 'iIndex'. Returns 'true' if successful, otherwise returns 'false'.
	//
	bool SetTabLabel(
		// Zero-based index of the tab to set the tab text for.
		int iIndex,
		// Reference to a CString object that represents the tab label.
		CString& strLabel);
	
	// Call this member function to set the label for the tab item specified
	// by 'hChildWnd'. Returns 'true' if successful, otherwise returns 'false'.
	//
	bool SetTabLabel(
		// Valid HWND of the MDI child window to set the tab text for.
		HWND hChildWnd,
		// Reference to a CString object that represents the tab label.
		CString& strLabel);

	// This member function will install the MDI tab views with your application.
	// Returns TRUE if successful, otherwise returns FALSE.
    //
	virtual BOOL Install(
        // Points to the parent MDI frame window.
        CMDIFrameWnd* pMDIFrameWnd,
        // Style for the tab control. It can be any of the TCS_ values.
        DWORD dwStyle=TCS_BOTTOM|TCS_HOTTRACK,
		// Set to TRUE for no icon to display
		BOOL bNoIcons=xtAfxData.bXPMode);

	// This member function will uninstall the MDI tab views from your application.
	// Returns TRUE if successful, otherwise returns FALSE.
    //
	virtual BOOL UnInstall();

	// Call this member function to retrieve the tab index from the current
	// cursor position. Returns the zero-based index of the tab, or -1 if no tab
	// is at the specified point.
    //
	int TabFromPoint(
        // Pointer to a CPoint object that contains the cursor screen 
	    // coordinates.  Use default for the current cursor position.
        CPoint point) const;

	// This member function will return a pointer to the child frame associated
	// with the tab item. Returns a CWnd object if successful, otherwise returns
	// NULL.
	//
	CMDIChildWnd* GetFrameWnd(
		// Index of the tab control.  If -1, then the tab under the mouse when
		// the last right click was performed will be used.
		int iIndex=-1) const;

	// This member function will return a pointer to the child frame associated
	// with the tab item. Returns a CWnd object if successful, otherwise returns
	// NULL.
	//
	CMDIChildWnd* GetFrameWnd(
		// CPoint object that represents the cursor position.
		CPoint point) const;

	// This member function is used to set the resource ID for the popup menu
	// used by the tab control.
    //
	void SetMenuID(
        // ID for the tab control popup menu.
        UINT popupMenuID,
        // Index position in the menu resource.
        int nPos=0,
        // ID of the default menu command, will display bold.
        UINT nDefCmd=-1);

	// This member function will set the size of the gap between the client
	// area and the tab control.
    //
	void SetBorderGap(
        // Amount, in pixels, of gap between the tab control and the client.
        int iSize);

	// This member function will get the text for the specified MDI child
	// window.  It returns a const CString object that represents the document
	// title.
	//
	CString GetChildWndText(
		// Handle to a valid CMDIChildWnd object.
		HWND hWnd) const;

protected:

	// This member function is called to initialize the font for the tab control
	// associated with this view.
    //
	virtual void InitializeFont();

	// This member function is called to insert a child frame into the MDI
	// tab control.
    //
	virtual void InsertTabWnd(
        // A pointer to a valid child frame window.
        CMDIChildWnd* pChildFrame,
        // TRUE to force the MDI frame window to recalc the layout.
        BOOL bRecalcLayout=TRUE);

	// This member function is called by the tab control to ensure the current
	// selection matches the active document.  Returns FALSE if there is no
	// active document, otherwise returns TRUE.
	//
	BOOL RefreshActiveSel();

	// This member function is called by the tab control to ensure the tab
	// labels match their corresponding views.
	//
	void RefreshTabLabels();

	// This member function is called by the tab control to ensure the stored
	// indexes match their corresponding tab indexes.
	//
	void RefreshIndexes();

	//:Ignore
    //{{AFX_VIRTUAL(CXTMDIWndTab)
	//}}AFX_VIRTUAL
	virtual void OnMDICreate(HWND hWnd);
	virtual void OnMDIDestroy(HWND hWnd);
	virtual void OnMDIActivate(HWND hWnd);
    //:End Ignore

	//:Ignore
    //{{AFX_MSG(CXTMDIWndTab)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnIdleUpdateCmdUI();
	afx_msg LRESULT OnXtUpdate(WPARAM, LPARAM);
	afx_msg void OnPaint();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSysColorChange();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTabClose();
    //:End Ignore

	DECLARE_MESSAGE_MAP()

	friend class CXTMDIClientWnd;
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTMDIWndTab::SetMenuID(UINT popupMenuID, int nPos, UINT nDefCmd) {
	m_popupMenuID = popupMenuID; m_nPos = nPos; m_nDefCmd = nDefCmd;
}
AFX_INLINE void CXTMDIWndTab::SetBorderGap(int iBorderGap) {
	m_iBorderGap = iBorderGap; m_pMDIClientWnd->SetBorderGap(iBorderGap + ::GetSystemMetrics(SM_CXSIZEFRAME));
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__XTMDIWNDTAB_H__)
