//:Ignore
// XTTreeView.h interface for the CXTTreeView class.
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

#if !defined(__XTTREEVIEW_H__)
#define __XTTREEVIEW_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTTreeView is a CTreeView derived class.  It extends the CTreeView
// class to add additional functionality, including tree traversal, searching,
// color, and settings.
//
class _XT_EXT_CLASS CXTTreeView : public CTreeView
{
	DECLARE_DYNCREATE(CXTTreeView)

protected:

    // Protected constructor used by dynamic creation. Constructs a CXTTreeView
	// object.
    //
	CXTTreeView();

    // Destroys a CXTTreeView object, handles cleanup and de-allocation.
    //
	virtual ~CXTTreeView();

protected:

    // TRUE for a multi-selection tree control.
	BOOL m_bMultiSelect;
	// For shift selection.
	HTREEITEM m_hSelect;
	// TRUE to include the label when selecting tree items.
	BOOL m_bBandLabel;
    // Struct that contains the color and logfont for the tree item.
    CMap<void*,void*,XT_CLRFONT,XT_CLRFONT&> m_mapColorFont;

//:Ignore
private:
    bool m_bActionDone;
//:End Ignore

public:

    // This member function will retrieve the tree view item that has the
	// specified relationship, indicated by the 'nCode' parameter, to 'hItem'.
	// Returns the handle of the next item if successful, otherwise returns
	// NULL.
    //
    virtual HTREEITEM GetNextItem(
		//  Handle of a tree item.
        HTREEITEM hItem,        
		// A flag indicating the type of relation to 'hItem'. This flag can be
		// one of the following values:
        //[ul]
        //[li][b]TVGN_CARET[/b] - Retrieves the currently selected item.[/li]
        //[li][b]TVGN_CHILD[/b] - Retrieves the first child item. The 'hItem'
		//       parameter [b]must[/b] be NULL.[/li]
        //[li][b]TVGN_DROPHILITE[/b] - Retrieves the item that is the target
		//       of a drag-and-drop operation.[/li]
        //[li][b]TVGN_FIRSTVISIBLE[/b] - Retrieves the first visible item.[/li]
        //[li][b]TVGN_NEXT[/b] - Retrieves the next sibling item.[/li]
        //[li][b]TVGN_NEXTVISIBLE[/b] - Retrieves the next visible item
		//       that follows the specified item.[/li]
        //[li][b]TVGN_PARENT[/b] - Retrieves the parent of the specified
		//       item.[/li]
        //[li][b]TVGN_PREVIOUS[/b] - Retrieves the previous sibling item.[/li]
        //[li][b]TVGN_PREVIOUSVISIBLE[/b] - Retrieves the first visible
		//       item that precedes the specified item.[/li]
        //[li][b]TVGN_ROOT[/b] - Retrieves the first child item of the
		//       root item of which the specified item is a part.[/li]
        //[/ul]
        UINT nCode);

    // This member function gets the next item as if the outline was completely
	// expanded. Returns the handle of the item immediately below the reference item.
	//
    virtual HTREEITEM GetNextItem(
		// Handle of the reference item.
        HTREEITEM hItem);

    // This member function gets the previous item as if the outline was
	// completely expanded. Returns the handle of the item immediately above
	// the reference item.
	//
    virtual HTREEITEM GetPrevItem(
		// Handle of the reference item.
        HTREEITEM hItem);

    // This member function gets the last item in the branch. Returns the
	// handle of the last item.
	//
    virtual HTREEITEM GetLastItem(
		// Node identifying the branch.  NULL will return the last item in the
		// outline.
        HTREEITEM hItem);

    // This member function finds an item that contains the search string.
	// Returns the handle to the item, or NULL.
    //
    virtual HTREEITEM FindItem(
		// String to search for.
        LPCTSTR lpszSearch,
        // TRUE if the search should be case sensitive.
        BOOL bCaseSensitive=FALSE,        
		// TRUE for down.
        BOOL bDownDir=TRUE,
		// TRUE if search should match whole words.
        BOOL bWholeWord=FALSE,
        // Handle of the tree item to start searching from, NULL to use
		// the currently selected tree item.
        HTREEITEM hItem=NULL);

    // This member function sets the reference tree item font.
    //
    virtual void SetItemFont(
		// The reference item.
        HTREEITEM hItem,
		// New font for the tree item.
        LOGFONT& logfont);

    // This member function retrieves the current LOGFONT font used by
	// the tree item. Returns TRUE if successful, otherwise returns FALSE.
    //
    virtual BOOL GetItemFont(
		// Handle of the reference item.
        HTREEITEM hItem,
		// Item's font.
        LOGFONT* plogfont);

    // This member function sets the reference tree item font to bold.
    //
    virtual void SetItemBold(
		// Handle of the reference item.
        HTREEITEM hItem,
		// TRUE for bold font.
        BOOL bBold=TRUE);

    // This member function checks to see if the tree item has a bold font.
    // Returns TRUE if the tree item has a bold font, otherwise returns FALSE.
    //
    virtual BOOL GetItemBold(
		// Handle of the reference item.
        HTREEITEM hItem);

    // This member function sets the tree item text color.
    //
    virtual void SetItemColor(
		// Handle of the reference item.
        HTREEITEM hItem,
		// RGB value for the tree item's text.
        COLORREF color);

    // This member function gets the RGB text value for the referenced
	// tree item.  Returns the RGB text value for the referenced tree item,
	// or (COLORREF)-1, if the color was not set.
    //          
    virtual COLORREF GetItemColor(
		// Handle of the reference item.
        HTREEITEM hItem);

    // This member function enables or disables multi-selection for the
	// tree control. Returns TRUE if successful, otherwise returns FALSE.
	//
    virtual BOOL EnableMultiSelect(
		// TRUE for a multi-selection tree control.
        BOOL bMultiSelect=TRUE);

    // This member function returns the previously selected tree item in
	// a multi-selection tree control. Returns a handle to the previously
	// selected tree item.
	//
    virtual HTREEITEM GetPrevSelectedItem(
		// Handle of the reference item.
        HTREEITEM hItem);

    // This member function returns the next selected item in a multi-selection
	// tree control. Returns a handle to the next selected tree item.
	//
	HTREEITEM GetNextSelectedItem(
		// Handle of the reference item.
		HTREEITEM hItem) const;

    // This member function returns the first selected item in a multi-selection
	// tree control. Returns a handle to the first selected tree item.
	//
	HTREEITEM GetFirstSelectedItem() const;

    // This member function returns the number of tree items that are selected.
    // Returns a UNIT value that represents the number of tree items selected.
	//
	UINT GetSelectedCount() const;
	
	// This member function is a replacement for the base class function of
	// the same name, to handle TVIS_FOCUSED in a multi-select tree control.
	// It sets the state of the item specified by 'hItem'.  Returns nonzero
	// if successful, otherwise returns zero.
	//
	BOOL SetItemState(
		// Handle of the item whose state is to be set.
		HTREEITEM hItem,
		// Specifies the new states for the item.
		UINT nState,
		// Specifies which states are to be changed.
		UINT nStateMask);

	// This member function is a replacement for the base class function of
	// the same name, to handle TVIS_FOCUSED in a multi-select tree control.
	// Returns the state of the item specified by 'hItem'.
	//
	UINT GetItemState(
		// Handle of the item whose state is to be retrieved.
		HTREEITEM hItem,
		// Mask indicating which states are to be retrieved. For more information
		// on possible values for 'nStateMask', see the discussion of the 'state'
		// and 'stateMask' members of the TVITEM structure in the Platform SDK.
		UINT nStateMask) const;

	// This member function is a replacement for the base class function of
	// the same name, to handle TVIS_FOCUSED in a multi-select tree control.
	// Call this function to select the given tree view item.  If 'hItem' is
	// NULL, then this function selects no item.  Returns nonzero if successful,
	// otherwise returns zero.
	//
	BOOL SelectItem(
		// Handle of a tree item.
		HTREEITEM hItem);

	// This member function returns the handle to the tree item that currently
	// has focus. Returns the handle of the item that has focus, otherwise
	// returns NULL.
	//
	HTREEITEM GetFocusedItem() const;

	// This member function will set the focus for the tree item specified
	// by 'hItem'.  Returns TRUE if successful, otherwise returns FALSE.
	//
	BOOL FocusItem(
		// Handle of a tree item.
		HTREEITEM hItem);

	// Call this member function to clear, or select, all of the visible items
	// in the tree control.  This will not effect the focus of the tree items.
	//
	void SelectAll(
		// TRUE to select all the items, or FALSE to clear the selection.
		BOOL bSelect = TRUE);

    // This member function selects items from 'hItemFrom' to 'hItemTo' in
	// a multi-selection tree control.  It does not select a child item if
	// the parent is collapsed.  It will remove the selection from all other
	// items if 'bOnly' is set to TRUE.
    //
	void SelectItems(
		// Handle of the item to start selecting from.
		HTREEITEM hItemFrom,
		// Handle of the item to end selection at.
		HTREEITEM hItemTo,
		// TRUE to only select the specified range, or FALSE to keep existing
		// selections.
		BOOL bOnly = TRUE);

	// This member function checks to see if the specified item is selected.
	// Returns TRUE if the specified item is selected, otherwise returns FALSE.
	// 
	BOOL IsSelected(
		// Handle of a tree item.
		HTREEITEM hItem) const;

	// This member function will cause all of the children of the specified
	// tree item to be selected or deselected. Returns TRUE if focus was
	// on a child item, otherwise returns FALSE.
	//
	BOOL SelectChildren(
		// Handle of tree item to begin selection from.
		HTREEITEM hParent,
		// TRUE to only select the child items, or FALSE to keep existing selections.
		BOOL bSelect = TRUE,
		// TRUE to recurse all siblings, or FALSE to only select children of the
		// parent item.
		BOOL bRecurse = TRUE);

	// This member function will retrieve a reference to the typed pointer
	// array that contains the items selected in the tree control.
	//
	void GetSelectedList(
		// Reference to a CTypedPtrList<CPtrList, HTREEITEM> object.
		CTypedPtrList<CPtrList, HTREEITEM>& list) const;

	// This member function checks to see if the tree control is a multi-select
	// tree. Returns TRUE if the tree control is a multi-select tree, otherwise
	// returns FALSE.
	//
	BOOL IsMultiSelect() const;

	// This member function sets the banding mode for a multi-selection tree
	// control.  If 'bLabel' is TRUE, then items are selected only when the
	// banding rect passes over the tree item label. If FALSE, then passing
	// over any part of the tree item will cause selection to be made when
	// the banding rect passes over it.  Returns the previous banding state.
	//
	BOOL SetBandingHit(
		// TRUE to select items only when banding rect passes over the text label,
		// or FALSE to select items when banding rect passes over any part of
		// the tree item.
		BOOL bLabel);

protected:

//:Ignore
	//{{AFX_VIRTUAL(CXTTreeView)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL
	virtual BOOL SendNotify(LPNMHDR pNMHDR);
	virtual void SelectAllIgnore(BOOL bSelect, HTREEITEM hIgnore);
	virtual BOOL OnButtonDown(BOOL bLeft, UINT nFlags, CPoint point);
	virtual void DoPreSelection(HTREEITEM hItem, BOOL bLeft, UINT nFlags);
	virtual void DoAction(HTREEITEM hItem, BOOL bLeft, UINT nFlags, CPoint point);
	virtual void DoBanding(UINT nFlags, CPoint point);
	virtual void UpdateSelectionForRect(LPCRECT pRect, UINT nFlags, CTypedPtrList<CPtrList, HTREEITEM>& list);
    virtual BOOL IsFindValid(HTREEITEM hti);
//:End Ignore

//:Ignore
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
//:End Ignore

protected:
//:Ignore
	//{{AFX_MSG(CXTTreeView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
//:End Ignore

	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE HTREEITEM CXTTreeView::GetNextItem( HTREEITEM hItem, UINT nCode ) {
    ASSERT(::IsWindow(m_hWnd)); return GetTreeCtrl().GetNextItem( hItem, nCode );
}
AFX_INLINE HTREEITEM CXTTreeView::GetFocusedItem() const {
	ASSERT(m_bMultiSelect); return GetTreeCtrl().GetSelectedItem();
}
AFX_INLINE BOOL CXTTreeView::IsSelected(HTREEITEM hItem) const {
	return !!(TVIS_SELECTED & GetTreeCtrl().GetItemState(hItem, TVIS_SELECTED));
}
AFX_INLINE BOOL CXTTreeView::IsMultiSelect() const {
	return m_bMultiSelect;
};
AFX_INLINE BOOL CXTTreeView::SetBandingHit(BOOL bLabel) {
	BOOL bReturn = m_bBandLabel; m_bBandLabel = bLabel; return bReturn;
};

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__XTTREEVIEW_H__)
