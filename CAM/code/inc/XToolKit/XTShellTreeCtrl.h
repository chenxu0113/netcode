//:Ignore
// XTShellTreeCtrl.h : header file
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

#if !defined(__XTSHELLTREECTRL_H__)
#define __XTSHELLTREECTRL_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTShellTreeCtrl is a multiple inheritance class derived from CXTTreeCtrl
// and CXTShellPidl. CXTShellTreeCtrl is used to create a tree control
// that displays an explorer style tree, and can be associated with a combo box
// and list control.
//
class _XT_EXT_CLASS CXTShellTreeCtrl : public CXTTreeCtrl, public CXTShellPidl
{

public:

    // Constructs a CXTShellTreeCtrl object.
    //
    CXTShellTreeCtrl();

    // Destroys a CXTShellTreeCtrl object, handles cleanup and de-allocation.
    //
    virtual ~CXTShellTreeCtrl();

protected:
    
	// Contains SHELLFLAGSTATE info.
	CXTShellSettings m_shSettings;
	// Current path to tunnel if tree initialization is still pending.
	// Valid if 'm_bTunnelPath' is TRUE.
	CString m_strTunnelPath;
    // Flags indicating which items to include in the enumeration.
	UINT m_uFlags;
	// Window that receives the update notification, usually a CXTShellListCtrl.
	CWnd* m_pListCtrl;
	// CComboBox that is associated with this control.  See AssociateCombo(...)
	CWnd* m_pComboBox;
	// TRUE to display the shell context menu on right item click.
	BOOL m_bContextMenu;
	// TRUE if the tree control is to initialize when created.
	BOOL m_bAutoInit;
	// TRUE if content initialization is pending.
	BOOL m_bContentInitPending;
	// TRUE if tunnel initialization is pending.
	BOOL m_bTunnelPath;
	// 'true' if tree is currently traversing.
	bool m_bTunneling;

	// Enumeration that holds the type of object, folder or drive.
	//
    enum FindAttribs
	{ 
		// The object type is a drive.
		type_drive, 
		// The object type is a folder.
		type_folder 
	};

    // This member function fills a branch of the TreeView control.  Given
	// the shell folder, it enumerates the subitems of this folder, and adds
	// the appropriate  items to the tree. Returns TRUE if successful, otherwise
	// returns FALSE.
    //
    virtual BOOL InitTreeViewItems(
        // Pointer to the parent shell folder.
        LPSHELLFOLDER lpsf,
        // Fully qualified item ID list to the item having items enumerated.
		// This is the PIDL to the item identified by the 'lpsf' parameter.
        LPITEMIDLIST lpifq,
        // Parent tree node.
        HTREEITEM hParent);

    // This member function gets the index for the normal and selected
	// icons for the current item.
    //
    virtual void GetNormalAndSelectedIcons(
        // Fully qualified item ID list for the current item.
        LPITEMIDLIST lpifq,
        // Pointer to the tree view item being added to the tree.
        LPTV_ITEM lptvitem);

public:

    // Call this member function to enable or disable auto-initialization
	// of the shell tree control.
    //
    void EnableAutoInit(
        // TRUE if the tree initializes upon creation.
        BOOL bEnable);

    // Call this member function to enable or disable the display of the
	// shell context menu on right item click.
    //
    void EnableContextMenu(
        // TRUE to display a context menu.
        BOOL bEnable);

    // Call this member function to determine the type of items included
	// in the shell enumeration. The default is SHCONTF_FOLDERS | SHCONTF_NONFOLDERS.
    //
    void SetEnumFlags(
        // Determines the type of items included in an enumeration. It can
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
		//       displayed prior to the first IEnumIDList::Next call.  For a
		//       user interface to be presented, 'hwndOwner' must be set to a valid
		//       window handle.[/li]
        //[li][b]SHCONTF_NETPRINTERSRCH[/b] - The caller is looking for
		//       printer objects.[/li]
        //[/ul]
        UINT uFlags);

    // This member function obtains a handle to the system image list and
	// attaches it to the tree control. Returns TRUE if successful, otherwise
	// returns FALSE.
    //
    virtual BOOL InitSystemImageLists();

    // This member function adds items to the tree view.
    //
    virtual void PopulateTreeView();

    // This member function responds to a TVN_ITEMEXPANDING message in
	// order to fill up subdirectories.
    //
    virtual void OnFolderExpanding(
        // Address of an NM_TREEVIEW struct.
        NM_TREEVIEW* pNMTreeView);

    // This member function displays the system popup menu for the selected
	// item or folder. Returns a handle to the currently selected HTREEITEM.
    //
    virtual HTREEITEM GetContextMenu();

    // This member function responds to a TVN_SELCHANGED message to retrieve
	// the specified folder path. Returns TRUE if the folder path was found,
	// otherwise returns FALSE.
    //
    virtual BOOL OnFolderSelected(
        // Address of an NM_TREEVIEW struct.
        NM_TREEVIEW* pNMTreeView,
        // Address of a CString object to receive the file system path.
        CString &strFolderPath);

    // This member function retrieves the path of the currently selected
	// tree item. Returns TRUE if successful, otherwise returns FALSE.
    //
    virtual BOOL GetSelectedFolderPath(
        // Address of a CString object to receive the file system path.
        CString &strFolderPath);

    // This member function recursively searches the tree control, beginning
	// at 'hItem', to find the item specified by 'lplvid'. This is typically used when
	// the user double clicks an item in the list view. Returns TRUE if successful,
	// otherwise returns FALSE.
    //
    virtual BOOL FindTreeItem(
        // Handle to the tree item to search from.
        HTREEITEM hItem,
        // Pointer to the list view item data.
        XT_LVITEMDATA* lplvid);

    // Call this member function to associate the list control with the tree.
    //
    virtual void AssociateList(
        // Points to the list control that is associated with the tree.
        CWnd* pWnd);

    // This member function is used to associate a CComboBox object with
	// the control.  Whenever the path changes, the combo is updated.
    //
    virtual void AssociateCombo(
        // Points to the combo box that is associated with the tree.
        CWnd* pWnd);

	// This member function is called to initialize a branch of the shell
	// tree.
    //
    virtual void InitTreeNode(
        // Handle to a tree node.
        HTREEITEM hItem,
        // Pointer to the list view item data.
        XT_TVITEMDATA* lptvid);

    // This member function searches the tree for a specified folder. Returns
	// TRUE if successful, otherwise returns FALSE.
    //
    virtual BOOL SearchTree(
        // Tree item to begin the search from.
        HTREEITEM hItem,
        // String to search for.
        CString strSearchName,
        // Looking for folder or drive.
        FindAttribs attr,
		// TRUE to look for the drive letter only.
		BOOL bFindRoot=FALSE);

    // This member function will "tunnel" the tree to find the specified
	// path.  This will only work when the PopulateTreeView() method is used
	// to populate the tree.
    //
    virtual void TunnelTree(
        // Path to find.
        CString strFindPath);

    // This member function retrieves the path of a folder item, which
	// does not have to be selected.  Pass a CString object that will hold
	// the folder path. Returns TRUE if the path is not in the file system(e.g.
	// MyComputer), or if none is selected, it returns FALSE.
    //
    virtual BOOL GetFolderItemPath(
        // Tree item to get path for.
        HTREEITEM hItem,
        // Reference to a CString object to contain the folder path.
        CString &strFolderPath);

    // This member function populates a tree based upon a path.
    //
    virtual void PopulateTree(
        // Path to populate.
        LPCTSTR lpszPath);

    //:Ignore
    //{{AFX_VIRTUAL(CXTShellTreeCtrl)
    protected:
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL
	virtual void OnDragDrop(NM_TREEVIEW* pNMTreeView);
    //:End Ignore

protected:

	// This member function sets the shell attribute flags for the specified
	// tree item.
    //
	void SetAttributes(
        // Handle to HTREEITEM node.
        HTREEITEM hItem,
        // Flags retrieved from SHELLFOLDER::GetAttributesOf.
        DWORD dwAttributes);

    //:Ignore
    virtual void DelayContentInit();
    virtual void ProcessContentInit();
	virtual HTREEITEM InsertDesktopItem();

    //{{AFX_MSG(CXTShellTreeCtrl)
    afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginRDrag(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
    afx_msg LRESULT OnUpdateShell(WPARAM wParam, LPARAM lParam);
    //:End Ignore

    DECLARE_MESSAGE_MAP()
};

#define CXTShellTree    CXTShellTreeCtrl

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTShellTreeCtrl::SetEnumFlags(UINT uFlags) {
    m_uFlags = uFlags;
}
AFX_INLINE void CXTShellTreeCtrl::EnableContextMenu(BOOL bEnable) {
    m_bContextMenu = bEnable;
}
AFX_INLINE void CXTShellTreeCtrl::EnableAutoInit(BOOL bEnable) {
    m_bAutoInit = bEnable;
}
AFX_INLINE void CXTShellTreeCtrl::AssociateList(CWnd* pWnd) {
    ASSERT_VALID(pWnd); m_pListCtrl = pWnd;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__XTSHELLTREECTRL_H__)
