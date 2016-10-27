//:Ignore
// XTShellTreeView.h : header file
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

#ifndef __XTSHELLTREEVIEW_H__
#define __XTSHELLTREEVIEW_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTShellTreeView is a multiple inheritance class derived from CXTTreeView
// and CXTShellPidl. It is used to create a CXTShellTreeView class object.
//
class _XT_EXT_CLASS CXTShellTreeView : public CXTTreeView, public CXTShellPidl
{
	DECLARE_DYNCREATE(CXTShellTreeView)

public:

    // Constructs a CXTShellTreeView object.
    //
    CXTShellTreeView();

    // Destroys a CXTShellTreeView object, handles cleanup and de-allocation.
    //
    virtual ~CXTShellTreeView();

protected:

	// Flags indicating which items to include in the enumeration.
	UINT m_uFlags;
	// TRUE to display the shell context menu on right item click.
	BOOL m_bContextMenu;
	// CComboBox that is associated with this control.  See AssociateCombo(...)
	CWnd* m_pComboBox;
	// Contains SHELLFLAGSTATE info.
	CXTShellSettings m_shSettings;
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
	// the appropriate items to the tree. Returns TRUE if successful, otherwise
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
        // Determines the type of items included in an enumeration. It
		// can be one or more of the following:
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
		// TRUE to look for a drive letter only.
		BOOL bFindRoot=FALSE);

    // This member function will "tunnel" the tree to find the specified
	// path. This will only work when the PopulateTreeView() method is used
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
        // Tree item to get the path for.
        HTREEITEM hItem,
        // Reference to a CString object to contain the folder path.
        CString &strFolderPath);

    // This member function populates a tree based upon a path.
    //
    virtual void PopulateTree(
        // Path to populate.
        LPCTSTR lpszPath);

	//:Ignore
    //{{AFX_VIRTUAL(CXTShellTreeView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL
	virtual void OnDragDrop(NM_TREEVIEW* pNMTreeView);
	virtual HTREEITEM InsertDesktopItem();
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
    //{{AFX_MSG(CXTShellTreeView)
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginRDrag(NMHDR* pNMHDR, LRESULT* pResult);
    //:End Ignore
	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTShellTreeView::SetEnumFlags(UINT uFlags) {
	m_uFlags = uFlags;
}
AFX_INLINE void CXTShellTreeView::EnableContextMenu(BOOL bEnable) {
	m_bContextMenu = bEnable;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __XTSHELLTREEVIEW_H__

