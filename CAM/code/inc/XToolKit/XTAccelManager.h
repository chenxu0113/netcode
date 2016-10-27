//:Ignore
// XTAccelManager.h : interface for the CXTAccelManager class.
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

#if !defined(__XTACCELMANAGER_H_)
#define __XTACCELMANAGER_H_

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//:Ignore
//////////////////////////////////////////////////////////////////////
// CXTAccelSwapOutItemList is a CMap derived class.  This class is used
// to hold a list of swap-out commands.
//
class _XT_EXT_CLASS CXTAccelSwapOutItemList : public CMap<UINT, UINT, CString, LPCTSTR> {};
//:End Ignore

//////////////////////////////////////////////////////////////////////
// XT_EXTRA_ACCELITEM is a stand alone structure class.  It is used to
// create a structure that represents an accelerator item.
//
struct XT_EXTRA_ACCELITEM
{
	// The category, or top level menu name, that this command belongs to.
	CString szCategory;
	// The name, or menu item text, that this item represents.
	CString szName;	
	// The command ID that this item represents.
	UINT nCommandID;
};

//////////////////////////////////////////////////////////////////////
// XT_VKEYMAP is a stand alone structure class.  It is used to define a
// structure used to map virtual key codes to their human readable names.
//
struct XT_VKEYMAP
{
	// Virtual Key Code.
	WORD wKey;
	// Display Name (i.e "CTRL").
	LPCTSTR szKeyName; 

};

//////////////////////////////////////////////////////////////////////
// XT_CATEGORY is a stand alone structure class.  It is used by the
// accelerator manager to map a category title with an associated HMENU
// handle.
//
struct XT_CATEGORY
{
    // Title of the menu category
    CString strCategory;
    // Handle to the menu associated with the category.
    HMENU hSubMenu;
};

//:Ignore
typedef CList<XT_CATEGORY,XT_CATEGORY&> CXTCategoryList;
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTMapString is a CObject derived helper class.  It is used to map IDs
// to command strings.
//
class CXTMapString : public CObject
{
public:
	
	// ID of the command to map.
	UINT m_nID;
	// A NULL terminated string.
	CString m_szName;

	// Use this assignment operator to initialize a CXTMapString object.
	// Returns a reference to a valid CXTMapString object.
	//
    const CXTMapString& operator=(
		// A reference to a valid CXTMapString object.
		const CXTMapString& item);
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE const CXTMapString& CXTMapString::operator=(const CXTMapString& item) {
	m_nID = item.m_nID; m_szName = item.m_szName; return *this;
}

typedef CList<CXTMapString, CXTMapString&> CXTMapStringList;

////////////////////////////////////////////////////////////////////// 
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTAccelManager is a stand alone class.  It encapsulates the configurable
// keyboard accelerator functions.  
//
class _XT_EXT_CLASS CXTAccelManager
{
private:

    // Constructs a CXTAccelManager object.
    //
	CXTAccelManager();

public:

	// Destroys a CXTAccelManager object, handles cleanup and de-allocation.
    //
	virtual ~CXTAccelManager();

protected:
	
	//:Ignore
	//////////////////////////////////////////////////////////////////////
	// Frame window binding information
	//////////////////////////////////////////////////////////////////////
	//:End Ignore

	// A CFrameWnd pointer to the frame window that the accelerators are connected to.
	CFrameWnd* m_pFrameWnd;

	//:Ignore
	//////////////////////////////////////////////////////////////////////
	// String list of command categories (top level menu items).
	//////////////////////////////////////////////////////////////////////
	//:End Ignore

	// A list used to map command IDs to display strings.
	CXTCategoryList m_comboCategoryList;
	// A list of command IDs to display categories.
	CXTMapStringList m_mapAccelString;
	// A map of command IDs to exclude from the string list.
	CMap<UINT, UINT, CString, LPCTSTR> m_mapAccelCategory;
	// A map of swap-out command IDs.
	CMap<UINT, UINT, UINT, UINT> m_MapCommandExcludeList;
	// A map of extra commands for each category.
	CMap<UINT, UINT, CXTAccelSwapOutItemList*, CXTAccelSwapOutItemList*&> m_SwapOutList;
	// A list of additional commands that have accelerator definitions.
	CList<XT_EXTRA_ACCELITEM*, XT_EXTRA_ACCELITEM*> m_ExtraItems;

	//:Ignore
	//////////////////////////////////////////////////////////////////////
	// Our accelerator buffer
	//////////////////////////////////////////////////////////////////////
	//:End Ignore

	// The array of accelerators.
	LPACCEL m_pAccelBuffer;
	// The array of default accelerators.
	LPACCEL m_pAccelDefault;
	// The count of accelerators pointed to by 'm_pAccelBuffer'.
	int m_nAccelBufferCount;
	// The count of accelerators pointed to by 'm_pAccelDefault'.
	int m_nAccelDefaultCount;
	// 'true' if the 'm_pAccelBuffer' currently contains a copy of the default
	// accelerators.
	bool m_bDefaultInUse;
	// 'true' if the contents of 'm_pAccelBuffer' has been changed.
	bool m_bChanged;
	// 'true' if the accelerator data is automatically saved when this class
	// is destroyed.
	bool m_bAutoSave;
	// 'true' if the accelerator data has been initialized for the frame object.
	bool m_bInitialized;
	// Virtual System Key ID to Name mappings.
	static XT_VKEYMAP m_VirtSysKeys[];
	// Virtual Key ID to Name mappings.
	static XT_VKEYMAP m_VirtKeys[];

public:

	// This static member function will retrieve a reference to the one
	// and only CXTAccelManager object.  You can use this function to access
	// data members for the CXTAccelManager class.
	//
	// [b]Example:[/b]
	//
	//[pre]CXTAccelManager::Get().Init( this );[/pre]
	//
	// Returns a reference to the one and only CXTAccelManager object.
	//
    static CXTAccelManager& Get();

	// This member function will check to see if the accelerator data has been
	// initialized. Returns 'true' if the accelerator data has been initialized,
	// otherwise it returns 'false'.
	//
	bool IsInitialized();

	// This member function initializes the accelerator manager and loads default
	// and user defined accelerator tables.  Returns 'true' if successful, otherwise
	// returns 'false'.
	//
	bool Init(
		// A CFrameWnd pointer to the frame window whose keyboard shortcuts should
		// be managed.
		CFrameWnd* pWnd);

	// This member function saves the current keyboard accelerator.
	// Returns 'true' if successful, otherwise returns 'false'.
	//
	bool Save(
		// 'true' if the object is to be un-initialized.
		bool bUnInitialize=false);

	// This member function loads a new keyboard accelerator.
	// Returns 'true' if successful, otherwise returns 'false'.
	//
	bool Load(
		// 'true' to, when loaded, apply the configuration to the current frame window.
		bool bUpdateWindowAccel = true);

	// This member function is called to get a pointer to the frame's
	// active menu.  Returns a valid CMenu pointer if successful, otherwise 
	// returns NULL.
	//
	CMenu* GetFrameMenu();

	// Call this member function to enable or disable the autosave feature.  Autosave
	// will automatically save the accelerator information when this object is destroyed.
	//
	void EnableAutoSave(
		// 'true' to enable autosave.
		bool bEnable);

	// This member function is called to initialize the default and user
	// defined accelerator tables for the manager, and must be called after the
	// Init(...) method has been called.
	//
	void InitAccelerators(
        HMENU hMenu=NULL);
	
	// This member function is used to exclude a specific command ID. To exclude
	// a specific command ID, call this method before calling Init(...).
	//
	void ExcludeCommandID(
		// The command ID to exclude.
		UINT nCommand);

	// Call this method to set one or more swapout IDs, in the case where
	// there is a dynamic menu item. An example of where this can be used would
	// be the MRU in a standard doc/view application. Add a single command
	// to the menu resource that can be swapped out with the commands you
	// set here. Call this method once for each command you want to replace.
	// For example, if you had a command ID called IDC_MRUPLACEHOLDER, and
	// you wanted to add four new items in place of this which are IDC_MRUFILE1,
	// IDC_MRUFILE2, IDC_MRUFILE3, IDC_MRUFILE4 you should do the following: -
	//
	//[pre]kbsm.AddSwapoutCommand(IDC_MRUPLACEHOLDER, IDC_MRUFILE1, "File 1");
	//kbsm.AddSwapoutCommand(IDC_MRUPLACEHOLDER, IDC_MRUFILE2, "File 2");
	//kbsm.AddSwapoutCommand(IDC_MRUPLACEHOLDER, IDC_MRUFILE3, "File 3");
	//kbsm.AddSwapoutCommand(IDC_MRUPLACEHOLDER, IDC_MRUFILE4, "File 4");[/pre]
	//
	void AddSwapoutCommand(
		// The command ID of the menu item, defined in the menu resource, that
		// should be replaced.
		UINT nSwapOutID,
		// The command ID to add to the menu.
		UINT nCommandID,
		// The menu string to add.
		LPCTSTR szName);

	// This member function adds command IDs that are not found in the menu,
	// but you want to have appear in the keyboard shortcut configuration and assignment
	// property pages.
	//
	void AddExtraCommand(
		// The category that this command should be added under.
		LPCTSTR szCategory,
		// The command ID for this extra command.
		UINT nCommandID,
		// The menu string for this command.
		LPCTSTR szName);

	// This member function applies the current accelerators to the window.
	// Returns 'true' if successful, otherwise returns 'false'.
	//
	bool UpdateWindowAccelerator();

	// This member function is called to restore the default accelerators for
	// the frame window. Returns 'true' if successful, otherwise returns 'false'.
	//
	bool LoadDefaultAccelerator();

	// This member function will, given a command ID, return a string describing
	// the currently assigned keys, if any.
	//
	void GetKeyAssignmentStrings(
		// The command ID.
		UINT nCommand,
		// Retrieves the displayable string(s) for the assigned shortcuts (i.e.
		// "Ctrl+C").
		CString& szKeys);

	// Override this member function to handle custom loading of the accelerator
	// data. Returns 'true' if successful, otherwise returns 'false'.
	//
	virtual bool OnLoad();

	// Override this member function to handle custom saving of the accelerator
	// data. Returns 'true' if successful, otherwise returns 'false'.
	//
	virtual bool OnSave(
		// Pointer to the accelerator table (array) to save.
		LPACCEL pTable,
		// The number of elements in the above array.
		int nCount);

    // This member function is called by the framework to allow the accelerator
	// manager to process any messages before they are forwarded.  Returns 'true'
	// if the message was handled, otherwise returns 'false'.
	//
	bool TranslateAccelerator(
        // A pointer to a valid MSG structure.
		MSG* pMsg);

private:

//:Ignore
	// Helper methods used by the configuration pages
	POSITION GetCommandStringStartPosition();
	void GetNextCommandStringItem(POSITION& rPos, UINT& nCommandID, CString& szString);
	bool LookupCommandStringItem(UINT nCommandID, CString& szString);
	void SetCommandStringItem(UINT nCommandID, CString szName);
	void GetCategoryList(CComboBox& cb);
	bool IsCommandInCategory(UINT nCommandID, LPCTSTR szCategory);
	bool LookupCommandCategory(UINT nCommandID, CString& szCategory);
	bool GetKeyAssignmentInfo(int& nPos, UINT nCommand, CString& szName, BYTE& cVirt, WORD& wKey);
	bool AddKeyAssignment(UINT nCommand, BYTE cVirt, WORD wKey);
	bool GetKeyAssignment(BYTE cVirt, WORD wKey, UINT& nCommand);
	bool DeleteKeyAssignment(BYTE cVirt, WORD wKey);
	void MakeDisplayName(CXTString& str);
	void MakeKeyString(ACCEL Accel, CString& szName);
	void SaveDefaultAcceleratorTable(CFrameWnd* pFrameWnd);
	bool AddMenuItemsToNameList(HMENU hMenu, CString szParentName, LPCTSTR szCategory);
	static LPCTSTR GetVirtualKeyString(WORD wKey);
    bool MenuHasCommands(HMENU hMenu);
    void BuildCategoryList(HMENU hMenu, CString strParentItem);
    CXTMapStringList& GetCommandStringList();
//:End Ignore

	// These classes need access to edit the keyboard accelerators.
	friend class CXTCustAccelerators;
	friend class CXTAccelKeyEdit;
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTAccelManager::ExcludeCommandID(UINT nCommand) {
	m_MapCommandExcludeList.SetAt(nCommand, nCommand);
}
AFX_INLINE POSITION CXTAccelManager::GetCommandStringStartPosition() {
	return m_mapAccelString.GetHeadPosition();
}
AFX_INLINE bool CXTAccelManager::IsInitialized() {
	return m_bInitialized;
}
AFX_INLINE void CXTAccelManager::EnableAutoSave(bool bEnable) {
	m_bAutoSave = bEnable;
}
AFX_INLINE CXTMapStringList& CXTAccelManager::GetCommandStringList() {
    return m_mapAccelString;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__XTACCELMANAGER_H_)
