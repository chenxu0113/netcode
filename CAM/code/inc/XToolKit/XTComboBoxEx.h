//:Ignore
// XTComboBoxEx.h : interface for the CXTComboBoxEx class.
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

#if !defined(__XTCOMBOBOXEX_H__)
#define __XTCOMBOBOXEX_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// The CXTComboBoxEx class is a CComboBox derived class.  It extends the
// combo box control by providing support for image lists.  By using CXTComboBoxEx
// to create combo box controls, you no longer need to implement your own
// image drawing code.  Instead, use CXTComboBoxEx to access images from
// an image list.
//
class _XT_EXT_CLASS CXTComboBoxEx : public CComboBox
{
    DECLARE_DYNAMIC(CXTComboBoxEx)

public:

    // Constructs a CXTComboBoxEx object.
    //
    CXTComboBoxEx();

    // Destroys a CXTComboBoxEx object, handles cleanup and de-allocation.
    //
    virtual ~CXTComboBoxEx();

    // This member function creates the combo box and attaches it to the
	// CXTComboBoxEx object. Returns nonzero if the operation was successful,
	// otherwise returns zero.
    //
    BOOL Create(
        // Specifies the combination of combo box styles applied to the
		// combo box.
        DWORD dwStyle,
        // A reference to a CRect object or RECT structure that specifies
		// the position and size of the combo box.
        const RECT& rect,
        // A pointer to a CWnd object that is the parent window of the
		// combo box (usually a CDialog). It must [b]not[/b] be NULL.
        CWnd* pParentWnd,
        // Specifies the combo box’s control ID.
        UINT nID);

#if _MSC_VER > 1200 //MFC 7.0
    using CComboBox::DeleteItem;
#endif //MFC 7.0

    // This member function removes an item from a ComboBoxEx control.
	// Returns the number of items remaining in the control.  If 'iIndex' is
	// invalid, the function returns CB_ERR. 
    //
    int DeleteItem(
        // Zero-based index of the item to be removed. 
        int iIndex);

    // This member function retrieves item information for a given ComboBoxEx
	// item. Returns nonzero if the operation was successful, otherwise returns zero.
    //
    BOOL GetItem(
        // A pointer to a COMBOBOXEXITEM structure that will receive the
		// item information.
        COMBOBOXEXITEM* pCBItem);

    // This member function inserts a new item in a ComboBoxEx control.
	// Returns the index at which the new item was inserted if successful,
	// otherwise returns -1.
    //
    int InsertItem(
        // A pointer to a COMBOBOXEXITEM structure that will receive the
		// item information.  This structure contains callback flag values for
		// the item.
		const COMBOBOXEXITEM* pCBItem);

    // This member function inserts a new item in a ComboBoxEx control.
	// Returns the index at which the new item was inserted if successful,
	// otherwise returns -1.
    //
    int InsertItem(
        // Item index.
        int iItem,
        // String resource.
        UINT nStringID,
        // Amount, in pixels, image is to be indented.
        int iIndent=0,
        // Image index.
        int iImage=0,
        // Selected image index.
        int iSelectedImage=0,
        // Style mask.
        UINT mask=CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE|CBEIF_INDENT);

    // This member function inserts a new item in a ComboBoxEx control.
	// Returns the index at which the new item was inserted if successful,
	// otherwise returns -1.
	//
    int InsertItem(
        // Item index.
        int iItem,
        // Item string.
        LPCTSTR lpszItem,
		// Amount, in pixels, image is to be indented.
        int iIndent=0,
        // Image index.
        int iImage=0,
        // Selected image index.
        int iSelectedImage=0,
        // Style mask.
        UINT mask=CBEIF_TEXT|CBEIF_IMAGE|CBEIF_SELECTEDIMAGE|CBEIF_INDENT);

    // This member function sets the attributes for an item in a ComboBoxEx
	// control. Returns nonzero if the operation was successful, otherwise
	// returns zero.
    //
    BOOL SetItem(
        // A pointer to a COMBOBOXEXITEM structure that will receive the
		// item information.
        const COMBOBOXEXITEM* pCBItem);

    // This member function retrieves the extended styles control.  Returns
	// the DWORD value that contains the extended styles that are used for
	// the combo box control.
    //
    DWORD GetExtendedStyle() const;

    // This member function sets extended styles within a combo box control.
	// Returns a DWORD value that contains the extended styles previously
	// used for the control.
    //
    DWORD SetExtendedStyle(
        // A DWORD value that indicates which styles in 'dwExStyles' are
		// to be affected. Only the extended styles in 'dwExMask' will be changed.
		// All other styles will be maintained as is. If this parameter is zero,
		// then all of the styles in 'dwExStyles' will be affected.
        DWORD dwExMask,
        // A DWORD value that contains the combo box control extended styles
		// to set for the control.  
        DWORD dwExStyles);

    // This member function is used to determine if the text in the combo
	// has changed by typing.  Returns nonzero if the user has typed in the
	// control's edit box, otherwise returns zero.
    //
    BOOL HasEditChanged();

    // Call this member function to get a pointer to the edit control for
	// a combo box. Returns a pointer to a CEdit object.
    //
    CEdit* GetEditCtrl();

    // Call this member function to get a pointer to a combo box control
	// within a CXTComboBoxEx object.  Returns a pointer to a CComboBox object.
    //
    CComboBox* GetComboBoxCtrl();

    // Call this member function to retrieve a pointer to the image list
	// used by a CXTComboBoxEx control.  Returns a pointer to a CImageList
	// object. If it fails, this member function returns NULL.
    //
    CImageList* GetImageList() const;

    // Sets an image list for a ComboBoxEx control.  Returns a pointer
	// to a CImageList object containing the images previously used by the
	// CXTComboBoxEx control.  Returns NULL if no image list was previously
	// set.
    //
    CImageList* SetImageList(
        // A pointer to a CImageList object containing the images to use
		// with the CXTComboBoxEx control.
        CImageList* pImageList);

//:Ignore
    // Un-supported base class message.
    int Dir(UINT attr, LPCTSTR lpszWildCard);

    // Un-supported base class message.
    int FindString(int nIndexStart, LPCTSTR lpszFind) const;

    // Un-supported base class message.
    int AddString(LPCTSTR lpszString);

    // Un-supported base class message.
    BOOL SetEditSel(int nStartChar, int nEndChar);

    // Un-supported base class message.
    int InsertString(int nIndex, LPCTSTR lpszString);
//:End Ignore
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTComboBoxEx::CXTComboBoxEx() {

}
AFX_INLINE DWORD CXTComboBoxEx::GetExtendedStyle() const {
    ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, CBEM_GETEXTENDEDSTYLE, 0, 0);
}
AFX_INLINE DWORD CXTComboBoxEx::SetExtendedStyle(DWORD dwExMask, DWORD dwExStyles) {
    ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, CBEM_SETEXTENDEDSTYLE, (DWORD) dwExMask, (LPARAM) dwExStyles);
}
AFX_INLINE BOOL CXTComboBoxEx::HasEditChanged() {
    ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, CBEM_HASEDITCHANGED, 0, 0);
}
AFX_INLINE CEdit* CXTComboBoxEx::GetEditCtrl() {
    ASSERT(::IsWindow(m_hWnd)); return (CEdit*) CEdit::FromHandle((HWND) ::SendMessage(m_hWnd, CBEM_GETEDITCONTROL, 0, 0));
}
AFX_INLINE CComboBox* CXTComboBoxEx::GetComboBoxCtrl() {
    ASSERT(::IsWindow(m_hWnd)); return (CComboBox*) CComboBox::FromHandle((HWND) ::SendMessage(m_hWnd, CBEM_GETCOMBOCONTROL, 0, 0));
}
AFX_INLINE CImageList* CXTComboBoxEx::SetImageList(CImageList* pImageList) {
    ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, CBEM_SETIMAGELIST, 0, (LPARAM)pImageList->GetSafeHandle()));
}
AFX_INLINE CImageList* CXTComboBoxEx::GetImageList() const {
    ASSERT(::IsWindow(m_hWnd)); return CImageList::FromHandle((HIMAGELIST) ::SendMessage(m_hWnd, CBEM_GETIMAGELIST, 0, 0));
}
//---------------------------------------------------------------------------
// While CXTComboBoxEx derives from CComboBox, there are some
// CB_messages the underlying ComboBoxEx control doesn't support.
//---------------------------------------------------------------------------
AFX_INLINE int CXTComboBoxEx::Dir(UINT attr, LPCTSTR lpszWildCard) {
    UNUSED_ALWAYS(attr); UNUSED_ALWAYS(lpszWildCard); ASSERT(FALSE); return CB_ERR;
}
AFX_INLINE int CXTComboBoxEx::FindString(int nIndexStart, LPCTSTR lpszFind) const {
    UNUSED_ALWAYS(nIndexStart); UNUSED_ALWAYS(lpszFind); ASSERT(FALSE); return CB_ERR;
}
AFX_INLINE int CXTComboBoxEx::AddString(LPCTSTR lpszString) {
    UNUSED_ALWAYS(lpszString); ASSERT(FALSE); return CB_ERR;
}
AFX_INLINE BOOL CXTComboBoxEx::SetEditSel(int nStartChar, int nEndChar) {
    UNUSED_ALWAYS(nStartChar); UNUSED_ALWAYS(nEndChar); ASSERT(FALSE); return FALSE;
}
AFX_INLINE int CXTComboBoxEx::InsertString(int nIndex, LPCTSTR lpszString) {
    UNUSED_ALWAYS(nIndex); UNUSED_ALWAYS(lpszString); ASSERT(FALSE); return CB_ERR;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTCOMBOBOXEX_H__)
