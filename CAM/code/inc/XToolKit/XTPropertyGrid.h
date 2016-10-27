//:Ignore
// XTPropertyGrid.h interface for the CXTPropertyGrid class.
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

#if !defined(__XTPROPERTYGRID_H__)
#define __XTPROPERTYGRID_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

// XT_PROPERTYSORT is an enumeration used by CXTPropertyGrid for determining of grouping style
// 
enum XT_PROPERTYSORT
{
	// to group items by category.
	Categorized,
	// to sort items alphabetical.
	Alphabetical,
	// disable sorting
	NoSort
};

//:Ignore
//////////////////////////////////////////////////////////////////////
// CXTPropertyGridView is a CListBox derived class.
// It is an internal class used by Property Grid control
//
class _XT_EXT_CLASS CXTPropertyGridView : public CListBox
{
	DECLARE_DYNAMIC(CXTPropertyGridView)

public:
	CXTPropertyGridView();
	virtual ~CXTPropertyGridView();

	int GetDividerPos();
	CXTPropertyGridItem* ItemFromPoint(CPoint point);

private:

	CXTPropertyGridItem* GetSelectedItem();
	CXTPropertyGridItem* AddCategory(CString strCaption);
	void SetPropertySort(XT_PROPERTYSORT sort, BOOL bRrefresh = FALSE);
	int	InsertItem(CXTPropertyGridItem* pItem, int nIndex);
	int _DoExpand(CXTPropertyGridItem* pItem, int nIndex);
	void _DoCollapse(CXTPropertyGridItem* pItem);
	void _RefreshIndexes();
	void SwitchExpandState(int nItem);
	CXTPropertyGridItem* GetItem(int nIndex);
	int HitTest(CPoint point);
	void ResetContent();

	void ShowToolTip(CPoint pt);
	void _ShowToolTip(CRect rcBound, CRect rcText);
	CSize _GetTextExtent(CString str);
	void RelayToolTipEvent(MSG* pMsg);


private:

    CXTListItems           m_lstCategories;
    XT_PROPERTYSORT      m_properetySort;
    COLORREF             m_clrLine;
    COLORREF             m_clrFore;
    COLORREF             m_clrBack;
    double               m_dDivider;
    CFont                m_fontNormal;
    CFont                m_fontBold;
    BOOL                 m_bTracking;
    CXTPropertyGridItem* m_pSelected;
	
	CToolTipCtrl		 m_wndTip;
	CString m_strTipText;
	CRect m_rcToolTip;


protected:

    //{{AFX_VIRTUAL(CXTPropertyGridView)
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT) {}
	virtual int CompareItem(LPCOMPAREITEMSTRUCT) { return 0;}
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL

    //{{AFX_MSG(CXTPropertyGridView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSelectionChanged();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
    //}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	friend class CXTPropertyGridItem;
	friend class CXTPropertyGrid;
	friend class CXTPropertyGridInplaceEdit;
};
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTPropertyGrid is a CWnd derived class.  It is used to create a property grid
// control similar to the property window in VS.NET.
//
// See the "PropertyGrid" demo for a example of usage.
//
class _XT_EXT_CLASS CXTPropertyGrid : public CWnd
{
	DECLARE_DYNAMIC(CXTPropertyGrid)

public:
	// Constructs a CXTPropertyGrid object.
    //
	CXTPropertyGrid();

	// Destroys a CXTPropertyGrid object, handles cleanup and de-allocation.
	//
	virtual ~CXTPropertyGrid();

	// Call this member function to create the Property Grid control.  Returns
	// TRUE if successful, otherwise returns FALSE.
	//
	virtual BOOL Create(
		// Size of the control.
		const RECT& rect, 
		// Parent of the control.			
		CWnd* pParentWnd, 
		// Identifier of the Property Grid control.			
		UINT nID);

public:

	// Call this member function to add new category to the Property Grid control.
	// Returns the pointer to the item object of the newly inserted category.
	//
	CXTPropertyGridItem* AddCategory(
		// Name of the category to add.		
		CString strCaption);
	
	// Call this member function to modify the way of items grouping.
	//
	void SetPropertySort (
		// style of grouping. It can be one of the following:
		//[ul]
		//[li][b]Categorized[/b] - to group items by category.[/li] 
		//[li][b]Alphabetical[/b] - to sort items alphabetical.[/li] 
		//[li][b]NoSort[/b] - disable sorting[/li] 
		//[/ul]
		//		
		XT_PROPERTYSORT sort);

	// Call this member function to get the current grouping style for the Property Grid control.
	// Returns a XT_PROPERTYSORT value representing the current grouping style.
	//
	XT_PROPERTYSORT GetPropertySort();

	// Call this member function to show or hide toolbar control in Property Grid window.
	//
	void ShowToolBar(
		// TRUE to show toolbar control, or FALSE to hide it.		
		BOOL bShow = TRUE);
	
	// Call this member function to show or hide description pane in Property Grid window.
	//
	void ShowHelp(
		// TRUE to show description pane, or FALSE to hide it.		
		BOOL bShow = TRUE);

	// Call this member function to set custom colors of Property Grid control.
	//
	void SetCustomColors( 
		// An RGB value that represents the background color of description pane.
		COLORREF clrHelpBack, 
		// An RGB value that represents the text color of description pane.
		COLORREF clrHelpFore, 
		// An RGB value that represents the color of separating lines in control.
		COLORREF clrViewLine,
		// An RGB value that represents the background color of Property Grid control.
		COLORREF clrViewBack, 
		// An RGB value that represents the text color of Property Grid control.
		COLORREF clrViewFore );

	// Call this member function to set default colors of Property Grid control.
	//
	void SetStandardColors();

	// Call this member function to find item by caption.
	// Returns the pointer to the Item object.
	//
	CXTPropertyGridItem* FindItem(
		// Caption for the item to find
		CString strCaption);

	// Call this member function to find item by identifier.
	// Returns the pointer to the Item object.
	//
	CXTPropertyGridItem* FindItem(
		// Identifier for the item to find
		UINT nID);

	// Removes all items from the list box of a property grid control.
	//
	void ResetContent();

	// Call this member function to determine if the help pane is visible.  Returns
	// TRUE if the help pane is visible, otherwise returns FALSE.
	//
	BOOL IsHelpVisible() const;

	// Call this member function to determine if the toolbar is visible.  Returns
	// TRUE if the toolbar is visible, otherwise returns FALSE.
	//
	BOOL IsBarVisible() const;

	// Call this member function to return the height of the help pane when visible.
	// Returns an integer value that represents the height in pixels.
	//
	int GetHelpHeight() const;

	// Call this member function to return the background color for the property grid.
	// Returns an RGB value.
	//
	COLORREF GetHelpBackColor();

	// Call this member function to return the foreground color for the property grid.
	// Returns an RGB value.
	//
	COLORREF GetHelpForeColor();

	//:Ignore
	CXTPropertyGridView& GetGridView();
	CXTToolBar& GetToolBar();
	//:End Ignore

private:

	//:Ignore
	void OnInvertTracker(CRect rc);
	int HitTest(CPoint pt);
	void OnSelectionChanged(CXTPropertyGridItem* pItem);
	void OnSortChanged();
	void Reposition(int cx, int cy);
	void CreateToolbar();
	void RefreshToolBarButtons();
	 //:End Ignore

private:
	
	//:Ignore
	CXTPropertyGridView m_wndView;
	BOOL				m_bHelpVisible;
	BOOL				m_bToolBarVisible;
	int					m_nHelpHeight;
	COLORREF			m_clrHelpBack;
	COLORREF			m_clrHelpFore;
	CRect				m_rectTracker;
	CXTToolBar			m_wndToolbar;
	//:End Ignore

protected:

    //:Ignore
    //{{AFX_VIRTUAL(CXTPropertyGrid)
    //}}AFX_VIRTUAL
    //:End Ignore

	//:Ignore
    //{{AFX_MSG(CXTPropertyGrid)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSortAlphabetic();
	afx_msg void OnSortCategorized();
    //}}AFX_MSG
	//:End Ignore

	DECLARE_MESSAGE_MAP()

	friend class CXTPropertyGridView;
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPropertyGridItem* CXTPropertyGrid::FindItem(CString strCaption) {
	return m_wndView.m_lstCategories.FindItem(strCaption);
}
AFX_INLINE CXTPropertyGridItem* CXTPropertyGrid::FindItem(UINT nID) {
	return m_wndView.m_lstCategories.FindItem(nID);
}
AFX_INLINE CXTPropertyGridItem* CXTPropertyGrid::AddCategory(CString strCaption) { 
	return m_wndView.AddCategory(strCaption); 
}
AFX_INLINE void CXTPropertyGrid::SetPropertySort (XT_PROPERTYSORT sort) { 
	m_wndView.SetPropertySort(sort); 
}
AFX_INLINE XT_PROPERTYSORT CXTPropertyGrid::GetPropertySort () { 
	return m_wndView.m_properetySort; 
}
AFX_INLINE BOOL CXTPropertyGrid::IsHelpVisible() const {
	return m_bHelpVisible;
}
AFX_INLINE BOOL CXTPropertyGrid::IsBarVisible() const {
	return m_bToolBarVisible;
}
AFX_INLINE int CXTPropertyGrid::GetHelpHeight() const {
	return m_nHelpHeight;
}
AFX_INLINE COLORREF CXTPropertyGrid::GetHelpBackColor() {
	return m_clrHelpBack;
}
AFX_INLINE COLORREF CXTPropertyGrid::GetHelpForeColor() {
	return m_clrHelpFore;
}
AFX_INLINE CXTPropertyGridView& CXTPropertyGrid::GetGridView() {
	return m_wndView;
}
AFX_INLINE CXTToolBar& CXTPropertyGrid::GetToolBar() {
	return m_wndToolbar;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

#endif // #if !defined(__XTPROPERTYGRID_H__)
