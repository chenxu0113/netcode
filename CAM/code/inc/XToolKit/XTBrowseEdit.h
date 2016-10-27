//:Ignore
// XTBrowseEdit.h : interface for the CXTBrowseEdit class.
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

#if !defined(__XTBROWSEEDIT_H__)
#define __XTBROWSEEDIT_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

class CXTBrowseBtn;

//////////////////////////////////////////////////////////////////////
// CXTBrowseEdit is a CXTEdit derived class.  This class also has a push
// button associated with it.  The control can be used to search for directories
// and files, or activate a popup menu.
//
class _XT_EXT_CLASS CXTBrowseEdit : public CXTEdit
{
	DECLARE_DYNAMIC(CXTBrowseEdit)

public:

    // Constructs a CXTBrowseEdit object.
    //
	CXTBrowseEdit();

	// Destroys a CXTBrowseEdit object, handles cleanup and de-allocation.
    //
    virtual ~CXTBrowseEdit();

protected:

	// Distance between the button and edit control.
	int m_nGap;
	// TRUE for Open File dialog, FALSE for Save as.  See SetDlgOpenFile(...).
	BOOL m_bOpenFileDialog;
	// Points to a valid CFileDialog object.
	CFileDialog* m_pFileDialog;
	// File dialog styles.
	DWORD m_dwFlags;
	// Default file extension.
	CString m_strDefExt;
	// Default file name.
	CString m_strFileName;
	// Default file filter.
	CString m_strFilter;
	// Directory dialog title.
	CString m_strTitle;
	// Pointer to a push button.
	CXTBrowseBtn* m_pBrowseBtn;
	// Search type.
	DWORD m_dwBStyle;
	// Popup menu ID.
	UINT m_nMenu;
	// Index of a popup submenu.
	UINT m_nSubMenuIndx;
	// A CWnd pointer that represents the parent of the edit control.
	CWnd* m_pParentWnd;
	// 'true' if in browse operation
	bool m_bBrowsing;

	//:Ignore

	// used for one-time initialization.
	DWORD m_dwInitSignature;
	//:End Ignore

public:

	// Call this member function to determine if the browse edit control is in
	// the middle of a browse operation.  Returns 'true' if the control is displaying
	// a File Open dialog or popup menu.
	//
	bool IsBrowsing();

    // Call this member function to set the gap between the edit window and the
    // browse button.
	//
	void SetGap(
		// Gap, in pixels, between the browse button and edit window.
        int nGap);

	// This member function will set a CFileDialog derived class object to be 
    // the file open dialog.
    //
	virtual void SetFileDialog(
		// Points to the CFileDialog object that will replace the standard File
		// Open dialog.
        CFileDialog *pFileDialog);

	// This member function will set the File Open dialog style.
    //
	virtual void SetDlgOpenFile(
		// Set to TRUE to construct a File Open dialog box, or FALSE to construct a 
        // File Save as dialog box.
        BOOL bOpenFileDialog=TRUE);

	// This member function sets the default extension for the File Open dialog.
    //
	virtual void SetDlgDefExt(
		// Points to a NULL terminated string that represents the default file 
        // extension to be used with the File Open dialog.
        LPCTSTR strDefExt=NULL);

	// This member function sets the default file name for the File Open dialog.
    //
	virtual void SetDlgFileName(
		// Points to a NULL terminated string that represents the default file 
        // name to be used with the File Open dialog.
        LPCTSTR strFileName=NULL);

	// This member function sets the style flags for the File Open dialog.
    //
	virtual void SetDlgStyle(
		// The desired OFN_ styles for the File Open dialog.
        DWORD dwFlags=OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT);

	// This member function sets the file filter for the File Open dialog.
    //
	virtual void SetDlgFilter(
		// Points to a NULL terminated string that represents the file filter used 
        // by the File Open dialog.
        LPCTSTR strFilter=NULL);

    // This member function sets the owner for the File Open dialog.
	// 		
	virtual void SetDlgOwner(
		// Points to a CWnd object that represents the owner window for this control.
        CWnd* pParentWnd=NULL);

	// This member function sets the title for the directory dialog.
	// 		
	virtual void SetDlgTitle(
		// Points to a NULL terminated string the represents the title of the "browse 
        // for directory" dialog.
        LPCTSTR strTitle=NULL);

	// This member function sets the current search type for the control.
	//
	virtual void SetBrowseStyle(
		// A DWORD value that represents the type of search to perform. It can
		// be any one of the following styles:
		//[ul]
		//[li][b]BES_XT_CHOOSEDIR[/b] - Display the choose folder dialog.[/li]
		//[li][b]BES_XT_CHOOSEFILE[/b] - Display the choose file dialog.[/li]
		//[li][b]BES_XT_POPUPMENU[/b] - Display a user defined context menu.[/li]
		//[/ul]
        DWORD dwBStyle,
		// If 'dwBStyle' contains the BES_XT_POPUPMENU flag, then 'nMenu' represents
		// the resource ID of a popup menu. Otherwise this value is ignored.
        UINT nMenu=0,
		// Index of submenu to display.
        int nSubMenuIndx=0);

	// This member function is called whenever the browse button is pressed,
	// and can be overridden to perform custom browse functions.
    //
	virtual void OnBrowse();

	//:Ignore
    //{{AFX_VIRTUAL(CXTBrowseEdit)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
    virtual void ChooseDirectory();
    virtual void ChooseFile();
    virtual void PopupMenu();
    virtual void PositionBrowseButton(bool bSizeEdit=false);
    //:End Ignore

	//:Ignore

	// defers control initialization
	void DeferInitialUpdate();

    //{{AFX_MSG(CXTBrowseEdit)
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	//}}AFX_MSG
	afx_msg LRESULT OnInitControl(WPARAM wParam, LPARAM lParam);
    //:End Ignore

	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTBrowseEdit::SetGap(int nGap) {
    ASSERT(nGap >= 0); m_nGap = nGap;
}
AFX_INLINE void CXTBrowseEdit::SetFileDialog(CFileDialog *pFileDialog/*=NULL*/) {
	m_pFileDialog = pFileDialog;
}
AFX_INLINE void CXTBrowseEdit::SetDlgOpenFile(BOOL bOpenFileDialog/*=TRUE*/) {
	m_bOpenFileDialog = bOpenFileDialog;
}
AFX_INLINE void CXTBrowseEdit::SetDlgDefExt(LPCTSTR strDefExt/*=NULL*/) {
	m_strDefExt = strDefExt;
}
AFX_INLINE void CXTBrowseEdit::SetDlgFileName(LPCTSTR strFileName/*=NULL*/) {
	m_strFileName = strFileName;
}
AFX_INLINE void CXTBrowseEdit::SetDlgStyle(DWORD dwFlags/*=OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT*/) {
	m_dwFlags = dwFlags;
}
AFX_INLINE void CXTBrowseEdit::SetDlgFilter(LPCTSTR strFilter/*=NULL*/) {
	m_strFilter = strFilter;
}
AFX_INLINE void CXTBrowseEdit::SetDlgOwner(CWnd* pParentWnd/*=NULL*/) {
	m_pParentWnd = pParentWnd;
}
AFX_INLINE void CXTBrowseEdit::SetDlgTitle(LPCTSTR strTitle/*=NULL*/) {
	m_strTitle = strTitle;
}
AFX_INLINE void CXTBrowseEdit::SetBrowseStyle(DWORD dwBStyle, UINT nMenu, int nSubMenuIndx) {
	m_dwBStyle = dwBStyle; m_nMenu = nMenu; m_nSubMenuIndx = nSubMenuIndx;
}
AFX_INLINE bool CXTBrowseEdit::IsBrowsing() {
	return m_bBrowsing;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTItemEdit is a CXTBrowseEdit derived class.  It is used to create
// a CXTItemEdit window that can be used as an "in-place" edit field that
// can be dynamically created for controls such as a list box.
//
class _XT_EXT_CLASS CXTItemEdit : public CXTBrowseEdit
{
	DECLARE_DYNAMIC(CXTItemEdit)
	
	// Used internally to determine if a WM_CLOSE message has been sent.
	bool m_bClosePosted;

public:

	// Constructs a CXTItemEdit object that can be used as an "in-place" edit
	// field, and can be dynamically created for controls such as a list box.
    //
	CXTItemEdit(
		// Points to the parent window.
        CWnd* pParent,
		// Size of the edit item.
        const CRect& rect,
		// Text to be initially displayed in the edit field.
        CString& strWindowText,
		// Specifies the browse edit style for the in-place edit field. See 
        // CXTBrowseEdit::SetBrowseStyle for available styles.
        DWORD dwBStyle=BES_XT_CHOOSEDIR,
		// Set to 'true' if the object is to be self deleting.
        bool bAutoDelete=true);

	// Destroys a CXTItemEdit object, handles cleanup and de-allocation.
	//
    virtual ~CXTItemEdit();

public:

	// 'true' if the item was modified.
	bool m_bModified;
	// 'true' if self deleting.
	bool m_bAutoDelete;
	// 'true' if the edit window was closed with the escape key.
	bool m_bEscapeKey;
	// The edit controls text.
	CString& m_strWindowText;

public:

	// This member function is called whenever the control looses focus. 
	// This will destroy the window, and notify the parent via WM_COMMAND
	// that the editing has been completed. The two possible commands are:
	// ON_BEN_XT_LABELEDITEND and ON_BEN_XT_LABELEDITCANCEL;
    //
	virtual void EndLabelEdit();

	//:Ignore
    //{{AFX_VIRTUAL(CXTItemEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
    //:End Ignore

	//:Ignore
protected:
    //{{AFX_MSG(CXTItemEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
    //:End Ignore

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTBROWSEEDIT_H__)
