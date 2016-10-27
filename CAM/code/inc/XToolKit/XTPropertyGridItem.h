//:Ignore
// XTPropertyGridItem.h interface for the CXTPropertyGridItem class.
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

#if !defined(__XTPROPERTYGRIDITEM_H__)
#define __XTPROPERTYGRIDITEM_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//:Ignore
// class forwards.
class CXTPropertyGridView;
class CXTPropertyGridItem;

const int PGI_EXPAND_BORDER = 14;
//:End Ignore

// Flags that indicate what styles to set for the grid item.
// 
enum XT_PROPERTYGRID_ITEMFLAGS
{
	// Item has edit control.
	pgiHasEdit         = 1,
	// Item has expand button.
	pgiHasExpandButton = 2,
	// Item has combo button.
	pgiHasComboButton  = 4
};

//////////////////////////////////////////////////////////////////////
// CXTListItems is a CList derived class. It represents items' collection 
// with some internal functionality.
//
class _XT_EXT_CLASS CXTListItems : public CList<CXTPropertyGridItem*, CXTPropertyGridItem*> 
{
public:
	// Call this member function to sort items alphabetical 
	//
	void Sort();

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
};

//////////////////////////////////////////////////////////////////////
// CXTItemConstraints is a CList derived class. It represents item 
// constraints' collection.
//
class _XT_EXT_CLASS CXTItemConstraints : public CStringList
{
public:

	// Call this member function to add new item's constraint.
	// Returns the integer value that represents the index of the added constraint.
	//
	int AddConstraint(
		// new constraint
		CString str);
	
	
	// This method sets current constraint.
	//
	void SetCurrent(
		// Index of needed constraint.
		int nIndex);

	// Call this member function to get current constraint.
	// Returns the index of current constraint.
	//
	int GetCurrent();

	
	// Call this member function to find constraint in the list.
	// Returns the zero-based index of the finded constraint; -1 if the constraint is not found.
	//
	int FindConstaint(
		// A constraint to search for
		CString str);
private:		
	
	//:Ignore
	int m_nCurrent;
	//:End Ignore
};

//////////////////////////////////////////////////////////////////////
// CXTPropertyGridItem is the base class representing an item of the 
// Property Grid Control 
// 
class _XT_EXT_CLASS CXTPropertyGridItem
{
public:

	// Constructs a CXTPropertyGridItem object.
	//
	CXTPropertyGridItem(
		// Caption of the item.
		CString strCaption, 
		// Initial value
		LPCTSTR strValue = NULL);
	
	// Constructs a CXTPropertyGridItem object.
	//
	CXTPropertyGridItem(
		// Identifier of the item.
		UINT nID, 
		// Initial value
		LPCTSTR strValue = NULL);

	// NB: class CXTPropertyGridItem hasn't default constructor. 
	// Identifier of second constructor can be linked with 
	// STRINGTABLE resource with same id in such form "Caption\nDescription". 

	// Destroys a CXTPropertyGridItem object, handles cleanup and de-allocation.
	//
	virtual ~CXTPropertyGridItem(void);

public:

	// Call this method to set the caption of the item. 
	//
	void SetCaption(
		// The new caption of the item. 	
		CString strCaption);
	
	// Call this method to set the description of the item. 
	//
	void SetDescription(
		// The new description of the item. 	
		CString strDescription);

	// Calls this method to set the read-only state of the item
	//
	void SetReadOnly(
		// Specifies whether to set or remove the read-only state of the item. 
		// A value of TRUE sets the state to read-only; a value of FALSE sets the state to read/write. 
		BOOL bReadOnly);

	// Calls this method to set the identifier of the item
	//
	void SetID(
		// The new identifier of the item. 	
		UINT m_nID);

	// This member function is called to set a specific flag for the item.
	//
	void SetFlags(
		// Flag to set. It can be one or more of the following:
		//[ul]
        //[li][b]pgiHasEdit[/b] - Item has edit control.[/li]
        //[li][b]pgiHasExpandButton[/b] - Item has expand button.[/li]
        //[li][b]pgiHasComboButton[/b] - Item has combo button.[/li]
		//[/ul]		
		UINT nFlags);

	// Call this method to get the caption of the item.
	// 
	CString GetCaption();

	// Call this method to get the description of the item.
	// 
	CString GetDescription();

	// Call this method to get the read-only state of the item.
	// 
	BOOL GetReadOnly();

	// Call this method to get the identifier of the item.
	// 
	UINT GetID();

	// Call this method to get current value of the item.
	// 
	CString GetValue();

	// Call this method to get flags of the item.
	// 
	UINT GetFlags();

	// The IsCategory function determines whether the item is a category. 
	// 
	BOOL IsCategory();

	// Call this method to add child item.
	// 
	CXTPropertyGridItem* AddChildItem(
		// The item to be added as a child.
		CXTPropertyGridItem* pItem);

	// Gets the position of the head element of child's list.
	//
	POSITION GetFirstChild() const;

	// Gets the child item identified by rPosition, then sets rPosition to the POSITION value of the next entry in the list.
	//
	CXTPropertyGridItem* GetNextChild(
		// A reference to a POSITION value returned by a previous GetNextChild, GetFirstChild, or other member function call.
		POSITION& rPosition) const;

	// The HasChilds function determines whether the item has children. 
	// 
	BOOL HasChilds();
	
	// The HasParent function determines whether the item has pParent item as parent. 
	// Returns TRUE if pParent is parent of the item.
	BOOL HasParent(
		// item  to be tested. 
		CXTPropertyGridItem* pParent);

	// Call this method to get constraints's list
	//
	// [b]Example:[/b]
	// 
	//[pre]CXTItemConstraints* pList = pItem->GetConstraints();[/pre]
	//
	CXTItemConstraints* GetConstraints();

	// Call this method to expand the item.
	// 
	void Expand();

	// Call this method to collapse the item.
	// 
	void Collapse();

	// Call this method to select the item.
	// 
	void Select();

	// Retrieves the bounding rectangle of the item
	// 
	CRect GetItemRect();

	// Retrieves the bounding rectangle of the item's value text
	// Override this member function to change it.
	//
	virtual CRect GetValueRect();

	// Call this method to change item's value
	// Override this method to add new functionality.
	// You should call the base class version of this function from your override.
	//
	virtual void SetValue(
		// New value of the item
		CString strValue);

	// This method is called when item's value is changed. Override this function if needed.
	//
	virtual void OnValueChanged(
		// New value of the item
		CString strValue);
	
	// Returns the parent window. 
	//
	CWnd* GetGrid();

	// Call this member function to determine if the item is visible.  Returns
	// TRUE if the item is visible, otherwise returns FALSE.
	//
	BOOL IsVisible() const;

	// Call this member function to determine if the item is expanded.  Returns
	// TRUE if the item is expanded, otherwise returns FALSE.
	//
	BOOL IsExpanded() const;

	// Call this member function to retreive the parent property grid item.  Returns
	// a CXTPropertyGridItem pointer that represents the parent item, can be NULL.
	//
	CXTPropertyGridItem* GetParentItem();

protected:

	// This method is called when item is drawing. Override this function if needed.
	// Returns TRUE if item is self-drawing.
	//
	virtual BOOL OnDrawItemValue(
		// Reference to the device context to be used for rendering an image of the item.
		CDC& dc,
		// Bounding rect of the item
		CRect rcValue);

	// This method is called when item is selecting. 
	//
	virtual void OnSelect();

	// This method is called when item is deselecting. 
	//
	virtual void OnDeselect();

	// This method is called when key is pressed.
	//
	virtual BOOL OnChar(
		// Contains the character code value of the key.
		UINT nChar);

	// This method is called when the user double-clicks the left mouse button. 
	//
	virtual void OnLButtonDblClk();

	// This method is called when the user presses the left mouse button.
	//
	virtual BOOL OnLButtonDown(
		// Indicates whether various virtual keys are down (see Visual C++ documentation)
		UINT nFlags,
		// Specifies the x- and y-coordinate of the cursor. 
		CPoint point);

	// This method is called when item is added to the parent.
	//
	virtual void OnAddChildItem();

	// This method is called when the user presses inplace button.
	// Override the method to show item-specific dialog.
	//
	virtual void OnInplaceButtonDown();

//:Ignore
	void OnValidateEdit();

protected:

	int						m_nIndex;
	int						m_nIndent;
	UINT					m_nID;
	UINT					m_nFlags;
	BOOL					m_bReadOnly;
	BOOL					m_bVisible;
	BOOL					m_bCategory;
	BOOL					m_bExpanded;
	CString					m_strValue;
	CString					m_strCaption;
	CString					m_strDescription;
	CXTPropertyGridItem*	m_pParent;
	CXTPropertyGridView*	m_pGrid;
	CXTListItems			m_lstChilds;
	CXTItemConstraints		m_lstContraints;

	static CXTPropertyGridInplaceEdit	m_wndEdit;
	static CXTPropertyGridInplaceButton m_wndButton;
	static CXTPropertyGridInplaceList	m_wndListBox;

private:

	void Init();
	void SetEditText(CString str);

	friend class CXTListItems;
	friend class CXTPropertyGridView;
	friend class CXTPropertyGridInplaceEdit;
	friend class CXTPropertyGridInplaceButton;
//:End Ignore
	
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE POSITION CXTPropertyGridItem::GetFirstChild() const { 
	return m_lstChilds.GetHeadPosition();
}
AFX_INLINE CXTPropertyGridItem* CXTPropertyGridItem::GetNextChild(POSITION& rPosition) const { 
	return m_lstChilds.GetNext(rPosition); 
}
AFX_INLINE BOOL CXTPropertyGridItem::HasChilds() { 
	return !m_lstChilds.IsEmpty(); 
}
AFX_INLINE void CXTPropertyGridItem::SetCaption(CString strCaption) { 
	m_strCaption = strCaption; 
}
AFX_INLINE CString CXTPropertyGridItem::GetCaption() { 
	return m_strCaption; 
}
AFX_INLINE void CXTPropertyGridItem::SetDescription(CString strDescription) {
	m_strDescription = strDescription; 
}
AFX_INLINE CString CXTPropertyGridItem::GetDescription() {
	return m_strDescription;
}
AFX_INLINE void CXTPropertyGridItem::SetReadOnly(BOOL bReadOnly) {
	m_bReadOnly = bReadOnly;
}
AFX_INLINE BOOL CXTPropertyGridItem::GetReadOnly() {
	return m_bReadOnly;
}
AFX_INLINE void CXTPropertyGridItem::SetID(UINT nID) {
	m_nID = nID;
}
AFX_INLINE UINT CXTPropertyGridItem::GetID() {
	return m_nID;
}
AFX_INLINE CString CXTPropertyGridItem::GetValue() {
	return m_strValue;
}
AFX_INLINE BOOL CXTPropertyGridItem::IsCategory() { 
	return m_bCategory; 
}
AFX_INLINE CWnd* CXTPropertyGridItem::GetGrid() {
	return (CWnd*)m_pGrid;
}
AFX_INLINE BOOL CXTPropertyGridItem::OnDrawItemValue(CDC&, CRect) { 
	return FALSE; 
}
AFX_INLINE void CXTPropertyGridItem::OnAddChildItem() {

}
AFX_INLINE void CXTPropertyGridItem::SetFlags(UINT nFlags) {
	ASSERT(!m_bCategory); ASSERT((nFlags & (pgiHasComboButton | pgiHasExpandButton)) != (pgiHasComboButton | pgiHasExpandButton)); m_nFlags = nFlags;
}
AFX_INLINE UINT CXTPropertyGridItem::GetFlags() {
	return m_nFlags;
}
AFX_INLINE BOOL CXTPropertyGridItem::IsVisible() const {
	return m_bVisible;
}
AFX_INLINE BOOL CXTPropertyGridItem::IsExpanded() const {
	return m_bExpanded;
}
AFX_INLINE CXTPropertyGridItem* CXTPropertyGridItem::GetParentItem() {
	return m_pParent;
}

//////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTItemConstraints::AddConstraint(CString str){
	AddTail(str); return (int)GetCount() - 1;
}
AFX_INLINE CXTItemConstraints* CXTPropertyGridItem::GetConstraints(){
	return &m_lstContraints;
}
AFX_INLINE void CXTItemConstraints::SetCurrent(int nIndex){
	m_nCurrent = nIndex;
}
AFX_INLINE int CXTItemConstraints::GetCurrent(){
	return m_nCurrent;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTPROPERTYGRIDITEM_H__)