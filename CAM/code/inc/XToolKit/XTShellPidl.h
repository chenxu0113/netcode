//:Ignore
// XTShellPidl.h : header file
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

#ifndef __XTSHELLPIDL_H__
#define __XTSHELLPIDL_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//:Ignore
LRESULT CALLBACK HookWndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTShellPidl is a stand alone base class.  This class is used by the
// shell tree and list controls to handle PIDL creation and management.
//
class _XT_EXT_CLASS CXTShellPidl
{
public:
	
    // Constructs a CXTShellPidl object.
    //
	CXTShellPidl();

	// Destroys a CXTShellPidl object, handles cleanup and de-allocation.
    //
    virtual ~CXTShellPidl();

public:

	// This member function gets the fully qualified PIDL for the path string.
	// Returns a pointer to an item ID list.  Returns NULL if it fails.
	//
	LPITEMIDLIST IDLFromPath(
		// File system path string.
		CString path);

	// This member function performs the OneUp or back function. Returns a
	// fully qualified parent PIDL.
	//
	LPITEMIDLIST OneUpPIDL(
		// Fully qualified PIDL.
		LPITEMIDLIST);

	// This member function gets the parent folder using PIDLs. Returns a
	// path string to the parent.
	//
	CString	OneUpPATH(
		// Path string.
		CString path);

	// This member function walks an ITEMIDLIST and points to the last one.
	// Returns a pointer to the last IDL in the list.
	//
	LPITEMIDLIST GetLastITEM(
		// PIDL list.
		LPITEMIDLIST pidl);

	// This member function copies a whole ITEMIDLIST.  Remember to Free()
	// the old one if it is no longer needed. Returns a new pointer to a copy
	// of the PIDL.
	//
	LPITEMIDLIST CopyIDList(
		// Pointer to an ITEMIDLIST.
		LPITEMIDLIST pidl);

	// This member function concatenates two PIDLs. Returns a pointer to an
	// item ID list.
    //
    LPITEMIDLIST ConcatPidls(
		// Pointer to an item ID list.
        LPCITEMIDLIST pidl1,
		// Pointer to an item ID list.
        LPCITEMIDLIST pidl2);

	// This member function gets the fully qualified PIDLs for the specified
	// folder. Returns a pointer to an item ID list.
    //
    LPITEMIDLIST GetFullyQualPidl(
		// Pointer to the parent shell folder.
        LPSHELLFOLDER lpsf,
		// Pointer to the item ID that is relative to 'lpsf'.
        LPITEMIDLIST lpi);

	// This member function copies the ITEMID. Returns a pointer to an item
	// ID list.
    //
    LPITEMIDLIST DuplicateItem(
		// Points to the shell’s IMalloc interface.
        LPMALLOC lpMalloc,
		// Pointer to item ID that is to be copied.
        LPITEMIDLIST lpi);

	// This member function gets the friendly name for the folder or file.
	// Returns TRUE if successful, otherwise returns FALSE.
    //
    BOOL GetName(
		// Pointer to the parent shell folder.
        LPSHELLFOLDER lpsf,
		// Pointer to the item ID that is relative to 'lpsf'.
        LPITEMIDLIST  lpi,
		// Flags to determine which value to return.  See SHGNO for more details.
        DWORD dwFlags,
		// Buffer to receive the friendly name of the folder.
        TCHAR* lpFriendlyName);

	// This member function allocates a PIDL. Returns a pointer to an item
	// ID list.
    //
    LPITEMIDLIST CreatePidl(
		// Initial size of the PIDL.
        UINT cbSize);

	// This member function computes the number of item IDs in an item ID
	// list. Returns the number of item IDs in the list.
    //
    UINT GetPidlItemCount(
		// Pointer to an item ID list.
        LPCITEMIDLIST pidl);

	// This member function gets the next PIDL in the list. Returns a pointer
	// to the next PIDL item in the list.
	//
    LPITEMIDLIST GetNextPidlItem(
		// Pointer to an item ID list.
        LPCITEMIDLIST pidl);

	// This member function displays a popup context menu, given a parent
	// shell folder, relative item ID, and screen location. Returns TRUE on
	// success, or FALSE on failure.
    //
	BOOL ShowContextMenu(
		// Context menu owner.
        HWND hwnd,
		// Pointer to the parent shell folder.
        LPSHELLFOLDER lpsfParent,
		// Pointer to the item ID that is relative to 'lpsfParent'.
        LPCITEMIDLIST *lpi,
		// Number of PIDLs.
        int nCount,
		// Screen location of where to popup the menu.
        LPPOINT lppt);

	// This member function displays a popup context menu, given a parent
	// shell folder, relative item ID, and screen location. Returns TRUE on
	// success, or FALSE on failure.
    //
	BOOL ShowContextMenu(
		// Context menu owner.
        HWND hwnd,
		// Pointer to the parent shell folder.
        LPSHELLFOLDER lpsfParent,
		// Pointer to the item ID that is relative to 'lpsfParent'.
        LPITEMIDLIST lpi,
		// Screen location of where to popup the menu.
        LPPOINT lppt);

	// This member function gets the index for the current icon. Index is
	// the index into the system image list. Returns an icon index for the current
	// item.
    //
	int GetItemIcon(
		// Fully qualified item ID list for the current item.
        LPITEMIDLIST lpi,
		// Flags for SHGetFileInfo().
        UINT uFlags);

    // This member function gets the IContextMenu, IContextMenu2 or IContextMenu3
	// interface. Returns an HRESULT value.
	//
	HRESULT GetSHContextMenu(
		// A pointer to a valid IShellFolder data type.
		LPSHELLFOLDER psfFolder,
		// A pointer to a valid _ITEMIDLIST structure.
		LPCITEMIDLIST *localPidl,
		// Number of items in the context menu.
		int nCount,
		// Long pointer to a CONTEXTMENU struct.
		void** ppCM,
		// A pointer to a valid int data type that represents the version number
		// of the context menu.
		int* pcmType);

//:Ignore
private:
    
	// regular window proc
	static WNDPROC m_pOldWndProc;
	// active shell context menu
	static LPCONTEXTMENU2 m_pIContext2;

    static LRESULT CALLBACK HookWndProc(
		HWND hWnd, 
		UINT msg, 
		WPARAM wp, 
		LPARAM lp);

//:End Ignore
};

//////////////////////////////////////////////////////////////////////

#endif // __XTSHELLPIDL_H__
