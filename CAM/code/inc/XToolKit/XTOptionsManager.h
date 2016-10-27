//:Ignore
// XTOptionsManager.h: interface for the CXTOptionsManager class.
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

#if !defined(__XTOPTIONSMANAGER_H__)
#define __XTOPTIONSMANAGER_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

// forwards
class CXTCoolMenu;
class CXTAccelManager;
class CXTToolsManager;

//////////////////////////////////////////////////////////////////////
// XT_OPTIONS is a stand alone structure class.  It is used by CXTOptionsManager
// to manage option data settings.
//
struct XT_OPTIONS
{
	// 'true' to render the toolbar while dragging or resizing.
	bool bToolBarVisualize;
    // 'true' to use shadows under the menus.
	bool bMenuShadows;
    // 'true' to use intelligent menus to hide selected menu commands.
	bool bMenuRecentCommands;
    // 'true' to show tooltips on toolbar commands.
	bool bToolBarScreenTips;
    // 'true' to display hidden menu commands after a short delay.
	bool bShowFullAfterDelay;
    // 'true' to add accelerator keys to toolbar tips.
	bool bToolBarAccelTips;
	// Animation type
	int nAnimationType;
};

//////////////////////////////////////////////////////////////////////
// CXTOptionsManager is a stand alone class. It is used to create a CXTOptionsManager
// object that is used to manage toolbar and menu options for the application.
//
class CXTOptionsManager  
{
//:Ignore
private:

	CXTOptionsManager();

//:End Ignore
	
public:

	// Destroys a CXTOptionsManager object, handles cleanup and de-allocation.
	//
	virtual ~CXTOptionsManager();

protected:

    // Option value structure.
	XT_OPTIONS m_options;
	// 'true' if the options data has been initialized for the frame object.
	bool m_bInitialized;
	// 'true' if the options data is automatically saved when this class is destroyed.
	bool m_bAutoSave;
	// 'true' if the contents of 'm_options' has been changed.
	bool m_bChanged;
    // 'true' if all options data is to be removed from the system registry.
    bool m_bRemoveAll;
    // Pointer to the frame window for the application.
    CFrameWnd* m_pFrameWnd;
    // Pointer to the cool menu manager for the frame specified by 'm_pFrameWnd'.
    CXTCoolMenu* m_pCoolMenu;

public:

	// This static member function will retrieve a reference to the one
	// and only CXTOptionsManager object.  You can use this function to access
	// data members for the CXTOptionsManager class.
	//
	// [b]Example:[/b]
	//
	//[pre]CXTOptionsManager::Get().Init( this, GetCoolMenu() );[/pre]
	//
	// Returns a reference to the one and only CXTOptionsManager object.
	//
    static CXTOptionsManager& Get();

	// This member function is called to save the options data to the system registry.
	// Returns 'true' if successful, otherwise returns 'false'.
	//
	bool Save();

	// This member function is called to load the options data from the system
	// registry. Returns 'true' if successful, otherwise returns 'false'.
	//
	bool Load();

	// This member function is called to initialize the Options manager. 
	// This function is called by the framework, usually from an overrode
	// LoadFrame handler. Returns 'true' if successful, otherwise returns
	// 'false'.
	//
	bool Init(
        // A pointer to a valid CFrameWnd object.
        CFrameWnd* pFrameWnd,
        // A pointer to a valid CXTCoolMenu object.
        CXTCoolMenu* pCoolMenu);

	// This member function is called to determine if the Options manager
	// has been initialized. Returns 'true' if it has been initialized, otherwise
	// returns 'false'.
	//
	bool IsInitialized();

	// This member function is called to update the option data for the Options
	// manager.
	//
	void SetOptions(
        // A reference to a valid XT_OPTIONS structure.
		XT_OPTIONS& options);

	// This member function is called to return a reference to the options
	// data for the Options manager.  Returns a reference to an XT_OPTIONS
	// structure.
	//
	XT_OPTIONS& GetOptions();

	// This member function is called to reset the the cool menus usage data
	// for recently used commands. Calling this function will remove the list
	// of recently used commands for intelligent menus. 
	//
	void Reset();

protected:

    // This member function is called by the Options manager during load
	// operations.  Returns 'true' if successful, otherwise returns 'false'.
	//
	virtual bool OnLoad();

    // This member function is called by the Options manager during save
	// operations.  Returns 'true' if successful, otherwise returns 'false'.
	//
	virtual bool OnSave();
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE bool CXTOptionsManager::IsInitialized() {
	return m_bInitialized;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(__XTOPTIONSMANAGER_H__)
