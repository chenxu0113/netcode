//:Ignore
// XTBrowseDialog.h: interface for the CXTBrowseDialog class.
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

#if !defined(__XTBROWSEDIALOG_H__)
#define __XTBROWSEDIALOG_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTBrowseDialog is derived from the BROWSEINFO structure.  It is used
// to display a directory chooser dialog using shell extensions.  The CXTBrowseDialog
// class allows you to configure various options, including setting the
// dialog's title, or assigning a callback function for defining folder
// display and file filtering styles.
//
class _XT_EXT_CLASS CXTBrowseDialog : protected BROWSEINFO  
{
public:

	// Constructs a CXTBrowseDialog object.
    //
	CXTBrowseDialog(
		// Points to a CWnd object that represents the parent 
		// window for the browse dialog.
		CWnd* pParent=NULL);

    // Destroys a CXTBrowseDialog object, handles cleanup and de-allocation.
    //
	virtual ~CXTBrowseDialog();

protected:

	// NULL terminated string that represents the selected directory.
	TCHAR m_szSelPath[MAX_PATH];

public:

	// Call this member function to invoke the browse dialog box and return 
	// the dialog box result when done.  Returns IDOK if the OK button was pressed,
	// otherwise returns IDCANCEL.
	//
	UINT DoModal();

	// Call this member function to set the owner window for the dialog box.
	//
	void SetOwner(
		// Handle to the owner window for the dialog box.
		HWND hWnd);

	// Call this member function to get an HWND handle to the owner window
	// for the dialog box. Returns an HWND handle.
	//
	HWND GetOwner();

	// Call this member function to set the address of an ITEMIDLIST structure
	// which specifies the location of the root folder from which to browse.
	//
	void SetPidlRoot(
		// Address of an ITEMIDLIST structure specifying the location
		// of the root folder from which to browse.  Only the specified
		// folder and its subfolders appear in the dialog box.  This member
		// can be NULL, in which case, the namespace root
		// (the desktop folder) is used.
		LPCITEMIDLIST pidl);

	// Call this member function to return the address of the ITEMIDLIST
	// structure that was specified for the location of the root folder.
	//
	LPCITEMIDLIST GetPidlRoot();

	// Call this member function to set the address for the display name
	// the dialog box will use.
	//
	void SetDisplayName(
		// Address of a buffer to receive the display name of the folder
		// selected by the user.  The size of this buffer is assumed to be
		// MAX_PATH bytes. 
		TCHAR* szDisplayName);

	// Call this member function to return the display name that is used
	// by the browse dialog box.
	//
	LPCTSTR GetDisplayName();

	// Call this member function to set the title for the browse dialog box.
	//
	void SetTitle(
		// Address of a null-terminated string that is displayed above the
		// tree view control in the dialog box.  This string can be used to
		// specify instructions to the user. 
		TCHAR* szTitle);

	// Call this member function to return a NULL terminated string that represents
	// the title that was set for the dialog box.
	//
	LPCTSTR GetTitle();

	// Call this member function to set the flags for specifying the options
	// for the browse dialog box.
	//
	void SetOptions(
		// Flags specifying the options for the dialog box.  This member can
		// include zero or a combination of the following values:
		//[ul]
		//[li][b]BIF_BROWSEFORCOMPUTER[/b] - Only return computers.  If the user
		//       selects anything other than a computer, the OK button is grayed.[/li]
		//[li][b]BIF_BROWSEFORPRINTER[/b] - Only return printers.  If the user
		//       selects anything other than a printer, the OK button is grayed.[/li]
		//[li][b]BIF_BROWSEINCLUDEFILES[/b] - The browse dialog will display
		//       files as well as folders.[/li]
		//[li][b]BIF_DONTGOBELOWDOMAIN[/b] - Do not include network folders
		//       below the domain level in the tree view control.[/li]
		//[li][b]BIF_EDITBOX[/b] - Version 4.71. The browse dialog includes an
		//       edit control in which the user can type the name of an item.[/li]
		//[li][b]BIF_RETURNFSANCESTORS[/b] - Only return file system ancestors.
		//       If the user selects anything other than a file system ancestor,
		//       the OK button is grayed.[/li]
		//[li][b]BIF_RETURNONLYFSDIRS[/b] - Only return file system directories.
		//       If the user selects folders that are not part of the file system,
		//       the OK button is grayed.[/li]
		//[li][b]BIF_STATUSTEXT[/b] - Include a status area in the dialog box.
		//       The callback function can set the status text by sending messages
		//       to the dialog box.[/li]
		//[li][b]BIF_VALIDATE[/b] - Version 4.71.  If the user types an invalid
		//       name into the edit box, the browse dialog will call the application's
		//       BrowseCallbackProc with the BFFM_VALIDATEFAILED message.  This
		//       flag is ignored if BIF_EDITBOX is not specified.[/li]
        //[/ul]
		UINT uf);

	// Call this member function to return the flags specifying the options 
	// that have been set for the dialog box.
	//
	UINT GetOptions();

	// Call this member function to define the address for the BrowseCallbackProc
	// function to be called when an event occurs.
	//
	void SetCallback(
		// Address of an application-defined function that the dialog box calls
		// when an event occurs.  For more information, see the BrowseCallbackProc
		// function. This member can be NULL. 
		BFFCALLBACK pf);

	// Call this member function to return the address for the BrowseCallbackProc
	// function that is called when an event occurs.
	//
	BFFCALLBACK GetCallback();

	// Call this member function to set the application data that is passed to
	// the callback function.
	//
	void SetData(
		// Application-defined value that the dialog box passes to the callback 
		// function, if one is specified.
		LPARAM lp);

	// This member function will return the application data that was set to be
	// passed to the callback function, if one is specified.
	//
	LPARAM GetData();

	// Call this member function to set the initial path to select when the 
	// browse dialog is first opened.
	//
	void SetSelPath(
		// A NULL terminated string that represents the directory that is selected
		// when the dialog is initially opened.  If not set, GetCurrentDirectory
		// is called to set the directory.
		TCHAR* szSelPath);

	// Call this member function to get a NULL terminated string that represents
	// the currently selected directory. Returns a NULL terminated string
	// representing the selected directory.
	//
	LPCTSTR GetSelPath();

	// Call this member function to get the index to the system image list
	// of the image associated with the selected folder. Returns the index
	// to the system image list.
	//
	int GetImage();

//:Ignore
private:
	
	// default dialog title.
	CString m_strTitle;

	// Application-defined callback function used with the SHBrowseForFolder function. 
	// The browse dialog box calls this function to notify it about events. You can 
	// define your own callback function by using the SetCallback method.
	static int CALLBACK BrowseCtrlCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
//:End Ignore
};

//:Ignore
/////////////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTBrowseDialog::SetOwner(HWND hWnd) {
	hwndOwner = hWnd;
}
AFX_INLINE HWND CXTBrowseDialog::GetOwner() {
	return hwndOwner;
}
AFX_INLINE void CXTBrowseDialog::SetPidlRoot(LPCITEMIDLIST pidl) {
	pidlRoot = pidl;
}
AFX_INLINE LPCITEMIDLIST CXTBrowseDialog::GetPidlRoot() {
	return pidlRoot;
}
AFX_INLINE void CXTBrowseDialog::SetDisplayName(TCHAR* szDisplayName) {
	pszDisplayName = szDisplayName;
}
AFX_INLINE LPCTSTR CXTBrowseDialog::GetDisplayName() {
	return pszDisplayName;
}
AFX_INLINE void CXTBrowseDialog::SetTitle(TCHAR* szTitle) {
	lpszTitle = szTitle;
}
AFX_INLINE LPCTSTR CXTBrowseDialog::GetTitle() {
	return lpszTitle;
}
AFX_INLINE void CXTBrowseDialog::SetOptions(UINT uf) {
	ulFlags = uf;
}
AFX_INLINE UINT CXTBrowseDialog::GetOptions() {
	return ulFlags;
}
AFX_INLINE void CXTBrowseDialog::SetCallback(BFFCALLBACK pf) {
	lpfn = pf;
}
AFX_INLINE BFFCALLBACK CXTBrowseDialog::GetCallback() {
	return lpfn;
}
AFX_INLINE void CXTBrowseDialog::SetData(LPARAM lp) {
	lParam = lp;
}
AFX_INLINE LPARAM CXTBrowseDialog::GetData() {
	return lParam;
}
AFX_INLINE void CXTBrowseDialog::SetSelPath(TCHAR* szSelPath) {
	_tcscpy(m_szSelPath, szSelPath);
}
AFX_INLINE LPCTSTR CXTBrowseDialog::GetSelPath() {
	return m_szSelPath;
}
AFX_INLINE int CXTBrowseDialog::GetImage() {
	return iImage;
}

/////////////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__XTBROWSEDIALOG_H__)
