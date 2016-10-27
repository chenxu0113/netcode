//:Ignore
// XTToolsManager.h: interface for the CXTToolsManager class.
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

#if !defined(__XTTOOLSMANAGER_H__)
#define __XTTOOLSMANAGER_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

// forwards
class CXTMenuBar;
class CXTCoolMenu;
class CXTEditListBox;

//////////////////////////////////////////////////////////////////////
// XT_TOOL is a stand alone structure class.  It is used by CXTToolsManager
// to manage tool data settings.
//
struct XT_TOOL
{
	// A NULL terminated string that represents the command title.
	TCHAR szTitle[128];
	// A NULL terminated string that represents the argument passed to the executable.
	TCHAR szArg[128];
	// A NULL terminated string that represents the executable associated with this command.
	TCHAR szCmd[_MAX_PATH];
	// A NULL terminated string that represents the default directory. 
	TCHAR szDir[_MAX_PATH];
	// Command ID to be associated with this tool.
	UINT uCmdID;
};

//:Ignore
typedef CMap<UINT,UINT,XT_TOOL*,XT_TOOL*> CXTToolsMap;
typedef	CMap<HMENU,HMENU,HMENU,HMENU>     CXTToolsMenuMap;
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTColorRef is a stand alone class.  It is used to manage the tools
// configuration page in the Customize dialog.
//
class _XT_EXT_CLASS CXTToolsManager  
{
private:

	// Constructs a CXTToolsManager object.
	//
	CXTToolsManager();
	
public:

    // Destroys a CXTToolsManager object, handles cleanup and de-allocation.
    //
    virtual ~CXTToolsManager();

protected:

	//:Ignore
	//////////////////////////////////////////////////////////////////////
	// Frame window binding information
	//////////////////////////////////////////////////////////////////////
	//:End Ignore

	// Frame window the accelerators are connected to.
	CFrameWnd* m_pFrameWnd;

	//:Ignore
	//////////////////////////////////////////////////////////////////////
	// Our tools buffer
	//////////////////////////////////////////////////////////////////////
	//:End Ignore

	// Array of tool commands.
	XT_TOOL* m_pToolsArray;
	// The count of tools pointed to by 'm_pToolsArray'.
	int m_nToolsCount;
	// 'true' if the contents of 'm_pToolsArray' has been changed.
	bool m_bChanged;
	// 'true' if the tool's data is automatically saved when this class
	// is destroyed.
	bool m_bAutoSave;
	// 'true' if the tool's data has been initialized.
	bool m_bInitialized;
    // 'true' if all tool's have been removed.
    bool m_bRemoveAll;
	// Handle to the system image list.
	HIMAGELIST m_hImageList;
	// Points to the application's cool menu hook.
	CXTCoolMenu* m_pCoolMenu;
	// Hash table of XT_TOOL structures mapped to user defined commands.
	CXTToolsMap m_mapTools;
	// Hash table of application menus mapped to existing tools menus.
	CXTToolsMenuMap m_mapToolsMenu;
	// Hash table to map tool commands with corresponding menu icons.
	CXTIconMap m_mapIconCmd;
	// Points to the menubar for the application.
	CXTMenuBar* m_pMenuBar;
	// Handle to the tools menu inserted into the applications menu.
	HMENU m_hToolsMenu;
    // Index of the HTML icon in the shell image list.
    int m_iHTMLIcon;
    // Resource ID of the popup menu to be displayed for the arguments browse edit box.
    int m_iArgPopup;
    // Resource ID of the popup menu to be displayed for the directory browse edit box.
    int m_iDirPopup;
    // Index where the 'Tools' menu should be inserted into the standard menu.
    int m_iNormalIndex;
    // Index where the 'Tools' menu should be inserted into the MDI window menu.
    int m_iWindowIndex;
	// User defined title of the 'Tools' pulldown menu.
	CString m_strToolsTitle;

public:

	// This static member function will retrieve a reference to the one
	// and only CXTToolsManager object.  You can use this function to access
	// data members for the CXTToolsManager class.
	//
	// [b]Example:[/b]
	//
	//[pre]CXTOptionsManager::Get().Init( this, GetMenuBar(), GetCoolMenu() );[/pre]
	//
	// Returns a reference to the one and only CXTToolsManager object.
	//
    static CXTToolsManager& Get();

    // This member function is called to set the popup menu used by the argument browse
	// edit control in the toolbar customize dialogs tools page.
	//
	void SetArgPopupMenu(
		// Resource ID of the popup menu to be displayed.
		int iArgPopup);

    // This member function retrieves the resource ID of the argument menu
	// associated with the argument browse edit control in the toolbar customize
	// dialog tools page. Returns an integer value that represents the resource ID.
	//
	int GetArgPopupMenu() const;

    // This member function is called to set the popup menu used by the directory browse
	// edit control in the toolbar customize dialogs tools page.
	//
	void SetDirPopupMenu(
		// Resource ID of the popup menu to be displayed.
		int iDirPopup);

    // This member function retrieves the resource ID of the directory
	// menu associated with the argument browse edit control in the toolbar
	// customize dialog tools page. Returns an integer value that represents
	// the resource ID.
	//
	int GetDirPopupMenu() const;

    // This member function is called to retrieve the location where the
	// tools menu is currently inserted into the framework's main menu.  Returns
	// an integer value that represents the zero-based index where the tools
	// menu is inserted.
    //
    int GetMenuIndex(
        // 'true' for normal index, 'false' to return the MDI window index.
        bool bNormal);

    // This member function is called to retrieve a handle to the tools
	// menu that is inserted into the framework's main menu. Returns a handle
	// to the tools menu.
	//
	HMENU GetToolsMenu() const;

    // This member function is called to retrieve a reference to the CXTToolMap
	// object that represents the array of user commands defined for the tools
	// menu. Returns a reference to the CXTToolMap object.
	//
	CXTToolsMap& GetToolsMap();

	// This member function will check to see if the accelerator data has
	// been initialized. Returns 'true' if the accelerator data has been
	// initialized, otherwise it returns 'false'.
	//
	bool IsInitialized();

    // This member function is called to remove all of the custom user
	// defined tools.
	//
	void RemoveAllTools();

	// This member function is called to remove the user tool specified by
	// 'pTool'.  Returns 'true' if successful, otherwise returns 'false'. 
	//
	bool RemoveTool(
		// A pointer to a valid XT_TOOL structure.
		XT_TOOL* pTool);

	// This member function is called to add a user defined tool to the Tools
	// manager command array.
	//
	void AddTool(
		// A pointer to a valid XT_TOOL structure.
		XT_TOOL* pTool);

	// This member function saves the current tool information.
	// Returns 'true' if successful, otherwise returns 'false'.
	//
	bool Save(
		// 'true' if the object is to be un-initialized.
		bool bUnInitialize=false);

	// This member function loads saved tool information.
	// Returns 'true' if successful, otherwise returns 'false'.
	//
	bool Load();

	// This member function is called to initialize the Tools manager.  This
	// function is called by the framework, usually from an overridden LoadFrame
	// handler. Returns 'true' if successful, otherwise returns 'false'.
	//
	bool Init(
        // A pointer to a valid CFrameWnd object.
        CFrameWnd* pFrameWnd,
        // A pointer to a valid CXTMenuBar object.
		CXTMenuBar* pMenuBar,
        // A pointer to a valid CXTCoolMenu object.
		CXTCoolMenu* m_pCoolMenu );

    // This member function is called to initialize the tools list that
	// is displayed in the customize toolbar tools page.
	//
	void GetToolsList(
        // A reference to a valid CXTEditListBox object.
        CXTEditListBox& editListBox);

	// Call this member function to enable or disable the autosave feature.
	// Autosave will automatically save the tool's information when this object
	// is destroyed.
	//
	void EnableAutoSave(
		// 'true' to enable autosave.
		bool bEnable);

    // This member function is called to set the changed flag for the tools manager.
	//
	void HasChanged(
        // 'true' if the tools data has changed.
		bool bChanged);

    // This member function is called to rebuild the tools menu that is
	// displayed in the framework's main menu.
	//
    void BuildToolsMenu(
		// TRUE to remove all menus including items not belonging to us.
		BOOL bDeleteAll);

    // This member function is called to retrieve the index of the HTML
	// icon stored in the system's image list.  Returns a zero-based index
	// value.
	//
    int GetHTMLIcon();

    // This member function is called to set the index location of where
	// the Tools manager will insert the tools menu in the framework's main menu.
	//
    void SetMenuIndex(
        // Index where the 'Tools' menu should be inserted into the standard menu.
        int iNormalIndex,
        // Index where the 'Tools' menu should be inserted into the MDI window menu.
        int iWindowIndex);

    // This member function is called to retrieve the next available command
	// ID to be used when creating a user defined tool.  Returns a UINT value
	// that represents the next available command id.
	//
	UINT GetNextCmdID();

	// This member function will set the title for the 'Tools' pulldown menu.  You can
	// use this function to set the name to something other than the default 'Tools'.  If
	// you set the title to an existing pulldown menu then the existing menu will be
	// appended to include the user defined items normally seen in the 'Tools' menu.
	//
	void SetMenuTitle(
		// NULL terminated string that represents the new tools title.
		CString strToolsText);

	// This member function returns the current title for the 'Tools' pulldown menu.  
	// Returns a CString object that represents the current text for the 'Tools' pulldown
	// menu item.
	//
	CString GetMenuTitle() const;

	// This member function searches the menu specified by 'hMenu' for a submenu that
	// is titled 'Tools'.  Returns the handle of the existing tools menu if found,
	// otherwise returns NULL.
	//
    HMENU ToolsMenuExists(
		// Handle to the menu to search.
		HMENU hMenu,
		// Index of the sub menu found, or -1 if no menu was found.
		int& iMenuItem );

	// This member function will insert the tools menu into the menu specified by 'hNewMenu'
	// at the index specified by iIndex.  Returns 'true' if successful, otherwise returns 'false'.
	//
	bool InsertToolsMenu(
		// Handle of the menu to modify.
		HMENU hNewMenu,
		// Zero based index where to insert the tools submenu.
		int iIndex);

protected:

    // This member function is called by the Tools manager during load
	// operations.  Returns 'true' if successful, otherwise returns 'false'.
	//
	virtual bool OnLoad();

    // This member function is called by the Tools manager during save
	// operations.  Returns 'true' if successful, otherwise returns 'false'.
	//
	virtual bool OnSave(
		// A pointer to a valid XT_TOOL structure.
		XT_TOOL* pToolsArray,
		// Size of the 'pToolsArray'.
		int nCount);

	friend class CXTToolBar;
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE bool CXTToolsManager::IsInitialized() {
	return m_bInitialized;
}
AFX_INLINE void CXTToolsManager::EnableAutoSave(bool bEnable) {
	m_bAutoSave = bEnable;
}
AFX_INLINE void CXTToolsManager::HasChanged(bool bChanged) {
	m_bChanged = bChanged;
}
AFX_INLINE CXTToolsMap& CXTToolsManager::GetToolsMap() {
	return m_mapTools;
}
AFX_INLINE HMENU CXTToolsManager::GetToolsMenu() const {
	return m_hToolsMenu;
}
AFX_INLINE void CXTToolsManager::SetArgPopupMenu(int iArgPopup) {
	m_iArgPopup = iArgPopup;
}
AFX_INLINE int CXTToolsManager::GetArgPopupMenu() const {
	return m_iArgPopup;
}
AFX_INLINE void CXTToolsManager::SetDirPopupMenu(int iDirPopup) {
	m_iDirPopup = iDirPopup;
}
AFX_INLINE int CXTToolsManager::GetDirPopupMenu() const {
	return m_iDirPopup;
}
AFX_INLINE int CXTToolsManager::GetMenuIndex(bool bNormal) {
    return bNormal ? m_iNormalIndex : m_iWindowIndex;
}
AFX_INLINE void CXTToolsManager::SetMenuIndex(int iNormalIndex, int iWindowIndex) {
    m_iNormalIndex = iNormalIndex; m_iWindowIndex = iWindowIndex;
}
AFX_INLINE void CXTToolsManager::SetMenuTitle(CString strToolsText) {
	m_strToolsTitle = strToolsText;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTTOOLSMANAGER_H__)
