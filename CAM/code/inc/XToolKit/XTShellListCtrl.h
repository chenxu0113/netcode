//:Ignore
// XTShellListCtrl.h : header file
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

#if !defined(__XTSHELLLISTCTRL_H__)
#define __XTSHELLLISTCTRL_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTShellListCtrl is a multiple inheritance class derived from CXTListCtrl
// and CXTShellPidl. It is used to create a CXTShellListCtrl window.
//
class _XT_EXT_CLASS CXTShellListCtrl : public CXTListCtrl, public CXTShellPidl
{

public:

    // Constructs a CXTShellListCtrl object.
    //
    CXTShellListCtrl();

    // Destroys a CXTShellListCtrl object, handles cleanup and de-allocation.
    //
    virtual ~CXTShellListCtrl();

protected:

    // Flags indicating which items to include in the enumeration.
	UINT m_uFlags;
	// Window that receives update notification, usually a CXTShellTreeCtrl.
	CWnd* m_pTreeCtrl;
	// TRUE to display the shell context menu on right item click.
	BOOL m_bContextMenu;
	// Contains SHELLFLAGSTATE info.
	CXTShellSettings m_shSettings;
	// Points to the CSIDL_INTERNET folder location.
	LPITEMIDLIST m_pidlINet;

	// This member function adds items to the list view. Returns TRUE if successful,
	// otherwise returns FALSE.
    //
    virtual BOOL InitListViewItems(
        // Pointer to tree view item data.
        XT_TVITEMDATA* lptvid,
        // Pointer to the parent shell folder.
        LPSHELLFOLDER lpsf);

    // This member function gets the index for the normal and selected icons
	// of the current item.
    //
    virtual void GetNormalAndSelectedIcons(
        // Fully qualified item ID list for the current item.
        LPITEMIDLIST lpifq,
        // Pointer to a tree view item about to be added to the tree.
        LPTV_ITEM lptvitem);

public:

    // Call this member function to enable or disable the display of the shell
    // context menu on the right click of an item.
    //
    virtual void EnableContextMenu(
        // TRUE to display a context menu.
        BOOL bEnable);

    // Call this member function to determine the type of items included
	// in the shell enumeration.  Default is SHCONTF_FOLDERS | SHCONTF_NONFOLDERS.
    //
    virtual void SetEnumFlags(
        // Determines the type of items included in an enumeration, and can
		// be one or more of the following:
        //[ul]
        //[li][b]SHCONTF_FOLDERS[/b] - Include items that are folders in
		//       the enumeration.[/li]
        //[li][b]SHCONTF_NONFOLDERS[/b] - Include items that are not folders
		//       in the enumeration.[/li]
        //[li][b]SHCONTF_INCLUDEHIDDEN[/b] - Include hidden items in the
		//       enumeration.[/li]
        //[li][b]SHCONTF_INIT_ON_FIRST_NEXT[/b] - IShellFolder::EnumObjects
		//       can return without validating the enumeration object. Validation
		//       can be postponed until the first call to IEnumIDList::Next.
		//       This flag is intended to be used when a user interface may be
		//       displayed prior to the first IEnumIDList::Next call. For a
		//       user interface to be presented, 'hwndOwner' must be set to a valid
		//       window handle.[/li]
        //[li][b]SHCONTF_NETPRINTERSRCH[/b] - The caller is looking for
		//       printer objects.[/li]
        //[/ul]
        UINT uFlags);

    // This member function associates the system image list with the list
	// control. Returns TRUE if successful, otherwise returns FALSE.
    //
    virtual BOOL InitSystemImageLists();

    // This member function populates the list view control. Returns TRUE
	// if successful, otherwise returns FALSE.
    //
    virtual BOOL PopulateListView(
        // Pointer to TreeView item data
        XT_TVITEMDATA* lptvid,
        // Pointer to the parent shell folder.
        LPSHELLFOLDER lpsf);

    // This member function returns the index of the list view item that
	// was double clicked on.  Returns the index of the item that was double
	// clicked, or -1, if the item was not found.
    //
    virtual int GetDoubleClickedItem();

    // This member function displays the system popup menu for the selected
	// item or folder.  Returns the index of the item selected, or -1, if
	// the item was not found. 
	//
    virtual int GetContextMenu();

    // This member function executes, via the Shell, the item clicked on
	// in the list control.  Returns 'true' if successful, otherwise returns 'false'.
    //
    virtual bool ShellOpenItem(
        // Index of the list view item clicked on.
        int iItem);

    // This member function executes, via the Shell, the item clicked on
	// in the list control.  Returns 'true' if successful, otherwise returns 'false'.
    //
	virtual bool ShellOpenItem(
        // Pointer to the list view item data.
        XT_LVITEMDATA* lplvid);

    // This member function retrieves the path of the specified list view
	// item. Returns TRUE if successful, otherwise returns FALSE.
    //
    virtual BOOL GetItemPath(
        // Index of the list view item to get the path of.
        int iItem,
        // Reference to a CString object that receives the path string.
        CString &strItemPath);

    // This member function creates default columns for the list view.
    //
    virtual void BuildDefaultColumns();

    // Override this member function in your derived class to perform custom
	// sort routines. Returns 'true' if successful, otherwise returns 'false'.
	//
    virtual bool SortList(
        // Passed in from the control. The index of the column clicked.
        int nCol,
        // Passed in from the control,  'true' if the sort order should
		// be ascending.
        bool bAscending );

    // Call this member function to associate the tree control with the list.
    //
    virtual void AssociateTree(
        // Points to the tree control that is associated with the list.
        CWnd* pWnd);

	// Call this member function to initialize the shell list control.
	//
	virtual void InitializeControl();

    //:Ignore
    //{{AFX_VIRTUAL(CXTShellListCtrl)
    protected:
    //}}AFX_VIRTUAL
	virtual void OnDragDrop(NM_LISTVIEW* pNMListView);
    //:End Ignore

protected:

    // This member function sets the shell attribute flags for the specified
	// list item.
    //
    virtual void SetAttributes(
        // Index of the item to set the attributes for.
        int iItem,
        // Flags retrieved from SHELLFOLDER::GetAttributesOf.
        DWORD dwAttributes);

    //:Ignore
    //{{AFX_MSG(CXTShellListCtrl)
    afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginRDrag(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
    afx_msg LRESULT OnUpdateShell(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInitControl(WPARAM wParam, LPARAM lParam);
    //:End Ignore

    DECLARE_MESSAGE_MAP()
};

#define CXTShellList CXTShellListCtrl

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTShellListCtrl::SetEnumFlags(UINT uFlags) {
    m_uFlags = uFlags;
}
AFX_INLINE void CXTShellListCtrl::AssociateTree(CWnd* pWnd) {
    ASSERT_VALID(pWnd); m_pTreeCtrl = pWnd;
}
AFX_INLINE void CXTShellListCtrl::EnableContextMenu(BOOL bEnable) {
    m_bContextMenu = bEnable;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

/////////////////////////////////////////////////////////////////////////////
// CXTShellListCtrlEx is a CXTShellListCtrl derived class.  It is used to create
// a stand-alone shell list control that is not dependant on a CXTShellTreeCtrl
// for initialization. It is used to create a CXTShellListCtrlEx window for 
// displaying the contents of file folders.
//
class _XT_EXT_CLASS CXTShellListCtrlEx : public CXTShellListCtrl
{

public:

    // Constructs a CXTShellListCtrlEx object.
    //
	CXTShellListCtrlEx();

    // Destroys a CXTShellListCtrlEx object, handles cleanup and de-allocation.
    //
	virtual ~CXTShellListCtrlEx();

protected:

	// Points to the window to synchronize with the shell list control.
	CWnd* m_pSyncWnd;
	// A NULL terminated string that represents the currently selected folder.
	CString m_strItemPath;

public:

	// This member function is called to browse to a particular file folder.  Returns
	// 'true' if successful, otherwise returns 'false'.
	//
	bool BrowseToFolder(
		// A NULL terminated string that represents the folder to select.
		LPCTSTR lpszPath);
	
	// This member function will associate a CWnd object with the shell list control.  This
	// window is usually a CEdit control, but can be any CWnd object.  This window will have
	// its window text updated whenever the selected folder is changed.
	//
	void SetSyncWnd(
		// Points to a valid CWnd object.
		CWnd* pSyncWnd);

    //:Ignore
	//{{AFX_VIRTUAL(CXTShellListCtrlEx)
	//}}AFX_VIRTUAL
    //:End Ignore

protected:

    //:Ignore
	//{{AFX_MSG(CXTShellListCtrlEx)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
    //:End Ignore

	DECLARE_MESSAGE_MAP()
};

//:Ignore
/////////////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTShellListCtrlEx::SetSyncWnd(CWnd* pSyncWnd) {
	m_pSyncWnd = pSyncWnd;
}

/////////////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__XTSHELLLISTCTRL_H__)
