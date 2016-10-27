//:Ignore
// XTShellListView.h : header file
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

#ifndef __XTSHELLLISTVIEW_H__
#define __XTSHELLLISTVIEW_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTShellListView is a multiple inheritance class derived from CXTListView
// and CXTShellPidl. It is used to create a CXTShellListView class object.
//
class _XT_EXT_CLASS CXTShellListView : public CXTListView, public CXTShellPidl
{
	DECLARE_DYNCREATE(CXTShellListView)

public:

    // Constructs a CXTShellListView object.
    //
    CXTShellListView();

    // Destroys a CXTShellListView object, handles cleanup and de-allocation.
    //
    virtual ~CXTShellListView();

protected:

	// Flags indicating which items to include in the enumeration.
	UINT m_uFlags;
	// TRUE to display the shell context menu on right item click.
	BOOL m_bContextMenu;
	// Contains SHELLFLAGSTATE info.
	CXTShellSettings m_shSettings;
	// Points to the CSIDL_INTERNET folder location.
	LPITEMIDLIST m_pidlINet;
	// TRUE if OleUninitialize has to be called. 
	BOOL m_bUninitOLE;

    // This member function adds items to the list view. Returns TRUE if
	// successful, otherwise returns FALSE.
    //
    virtual BOOL InitListViewItems(
        // Pointer to tree view item data.
        XT_TVITEMDATA* lptvid,
        // Pointer to the parent shell folder.
        LPSHELLFOLDER lpsf);

    // This member function gets the index for the normal and selected
	// icons of the current item.
    //
    virtual void GetNormalAndSelectedIcons(
        // Fully qualified item ID list for the current item.
        LPITEMIDLIST lpifq,
        // Pointer to the tree view item about to be added to the tree.
        LPTV_ITEM lptvitem);

public:

    // Call this member function to enable or disable the display of the
	// shell context menu on the right click of the item.
    //
    virtual void EnableContextMenu(
        // TRUE to display a context menu.
        BOOL bEnable);

    // Call this member function to determine the type of items included
	// in the shell enumeration. The default is SHCONTF_FOLDERS | SHCONTF_NONFOLDERS.
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
	// if successful, otherwise return FALSE.
    //
    virtual BOOL PopulateListView(
        // Pointer to TreeView item data.
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

    // This member function retrieves the path for the specified list view
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
        // Passed in from the control. 'true' if the sort order should be ascending.
        bool bAscending );

	//:Ignore
    //{{AFX_VIRTUAL(CXTShellListView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
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
    //{{AFX_MSG(CXTShellListView)
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBeginRDrag(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
    //:End Ignore
	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTShellListView::SetEnumFlags(UINT uFlags) {
	m_uFlags = uFlags;
}
AFX_INLINE void CXTShellListView::EnableContextMenu(BOOL bEnable) {
	m_bContextMenu = bEnable;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __XTSHELLLISTVIEW_H__

