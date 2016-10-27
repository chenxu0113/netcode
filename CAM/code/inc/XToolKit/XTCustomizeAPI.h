//:Ignore
// XTCustomizeAPI.h APIs used in toolbar customization
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

#if !defined(__XTCUSTOMIZEAPI_H__)
#define __XTCUSTOMIZEAPI_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// f.forwards
struct XT_DROPDOWNBUTTON;
class CXTIconMap;

//
// Descriptor of a control bar that supports customizations
// 
class CXTCustomControlBarInfo
{
public:
	// A control bar that supports customizations
	CControlBar* m_pBar;

	// true if this control bar can toggle its visibility on/off
	bool m_bCanShowHide;

	// true if this bar was dynamically created
	bool m_bDynamic;

	// callback for renaming the control bar or NULL if operation is not supported
	typedef CXTDelegate1<const CString&> RENAME;
	RENAME Rename;

	// callback for deleting the control bar or NULL if operation is not supported	
	typedef CXTDelegate0 REMOVE;
	REMOVE Remove;

	// callback for resetting the control bar or NULL if operation is not supported
	typedef CXTDelegate0 RESET;
	RESET Reset;

	CXTCustomControlBarInfo()
	: m_pBar(0)
	, m_bCanShowHide(false)
	, m_bDynamic(false)
	{}

};

// A delegate to store CXTCustomControlBarInfo's
typedef CXTDelegate1<CXTCustomControlBarInfo*> STORECUSTOMINFO;

//
// A request to create a new customizable control bar
//
class CXTNewCustomBarRequest
{
public:
	// Frame window on which to create the toolbar
	CFrameWnd* m_pFrame;

	// ID of the control bar to create
	UINT m_nBarID;

	// The toolbar title
	CString m_strTitle;

	// Newly created custom bar
	CControlBar* m_pNewBar;

	CXTNewCustomBarRequest()
	: m_pFrame(0)
	, m_nBarID(0)
	, m_pNewBar(0)
	{ }
};

//
// An item included in the custom group
//
class CXTCustomGroupItemInfo
{
public:
	// item title, will be displayed in commands listbox in customize dialog
	CString m_strTitle;

	// known types of items being transferred
	enum Type 
	{
		// toolbar button
		Button,

		// a control on the toolbar
		Control

	};
	const Type m_type;

	// Button label
	CString m_strLabel;

	// Command or control identifier
	UINT m_nCmdID;

	// Control width, valid for controls
	UINT m_nWidth; 

	// icon assigned to the command
	CXTIconHandle m_icon;

	// disabled icon assigned to the command
	CXTIconHandle m_disabledIcon;

	// hot icon assigned to the command
	CXTIconHandle m_hotIcon;

	// descriptor of the dropdown button
	XT_DROPDOWNBUTTON* m_pDropDown;

	// true if owning a private copy of XT_DROPDOWNBUTTON
	bool m_bOwnsDropDown;

// Construction
	CXTCustomGroupItemInfo(Type type);
	virtual ~CXTCustomGroupItemInfo();

// Operations
	// implement if you derive from this class
	virtual CXTCustomGroupItemInfo* Clone() const;

private: 
	// no imp
	CXTCustomGroupItemInfo(const CXTCustomGroupItemInfo& other);
	CXTCustomGroupItemInfo& operator=(const CXTCustomGroupItemInfo& other);	
};


//
// Customizable group descriptor
//
// The group is identified by a group owner (can later
// be queried for group content) and group ID
// to uniquely identify the group with its owner	
class CXTCustomGroupInfo
{
public:
	// group title
	CString m_strTitle;

	// items included in this group
	CTypedPtrArray<CPtrArray, CXTCustomGroupItemInfo*> m_items;

	// sort priority
	// customize dialog sorts groups by this number
	int m_nSortPriority;
	

// Construction
	CXTCustomGroupInfo();
	virtual ~CXTCustomGroupInfo();

	void Copy(const CXTCustomGroupInfo* other);
	void DeleteItems();
	void DeleteItem(int nIndex);

private:
	// no implementation
	CXTCustomGroupInfo(const CXTCustomGroupInfo& other);
	CXTCustomGroupInfo& operator=(const CXTCustomGroupInfo& other);
};

// Array of custom groups
class CXTCustomGroups : public CTypedPtrArray<CPtrArray, CXTCustomGroupInfo*> 
{
public:
	CXTCustomGroups();
	virtual ~CXTCustomGroups();
	void RemoveAll();
};

// definition of the drop target
interface IXTCustDropTarget
{
	// Checks to see if cursor is over the target
	// Returns true to indicate the drop is possible
	virtual bool IsDropPoint(
		// location in screen coordinates
		CPoint ptCursor) = 0;

	// Drag cursor is over the target notification	
	// Returns true if the data has been accepted for the drop
	virtual bool DragEnter(
		// item being dragged
		CXTCustomGroupItemInfo* pInfo,
		// location in screen coordinates
		CPoint ptCursor,
		// OUT: tells if copy operation is acceptable
		bool& bCopyAllowed) = 0;

	// Mouse is still over the target but position has changed
	virtual void DragMove(
		// location in screen coordinates
		CPoint ptCursor) = 0;

	// mouse has left the building
	virtual void DragLeave() = 0;

	// mouse dropped, must accept the data
	virtual void Drop(
		// item being dropped
		CXTCustomGroupItemInfo* pInfo,
		// location in screen coordinates
		CPoint ptCursor) = 0;
};


// a sink to report a drop target
interface IXTCustDropSink
{
	virtual void Add(IXTCustDropTarget* target) = 0;
};

// A connection point for objects involved in customize toolbar operations
class CXTCustomizeConnection : public CObject
{
	// Last known point of a newly created toolbar.
	CPoint m_startPoint; 

	bool m_bEnableCmdUI;
	bool m_bCustMode;
	
// Construction
private:
	CXTCustomizeConnection();

public:
	static _XT_EXT_CLASS CXTCustomizeConnection* GetInstance();

// Operations
public:

	// A delegate to check if the command shall be displayed in the customization
	// dialog	
	typedef CXTDelegate1Ret<bool, UINT> ISCUSTCMD;
	ISCUSTCMD IsCustCmd;

	// a delegate to create new customizable control bar
	typedef CXTDelegate1<CXTNewCustomBarRequest*> CREATEBAR;
	CREATEBAR CreateBar;

	// a delegate to properly place newly created customizable control bar
	typedef CXTDelegate2<CFrameWnd*, CControlBar*> PLACEBAR;
	PLACEBAR PlaceBar;

	// A delegate for to get info about custom command groups
	typedef CXTMultiCastDelegate1<CXTCustomGroups&> GETCUSTOMGROUPS;
	GETCUSTOMGROUPS GetCustomGroups;

	// A delegate to collect all command-to-icon associations
	typedef CXTMultiCastDelegate1<CXTIconMap*> GETICONS;
	GETICONS GetIcons;

	// A delegate for to get info about customizable control bars
	typedef CXTMultiCastDelegate1<const STORECUSTOMINFO&> GETCUSTOMBARINFO;
	GETCUSTOMBARINFO GetCustomBarInfo;

	// A delegate for resetting tracking state
	typedef CXTMultiCastDelegate0 RESETTRACKING;
	RESETTRACKING ResetTracking;

	// A delegate to receive notifications on customize mode on/off events
	typedef CXTMultiCastDelegate1<bool> SETCUSTMODE;
	SETCUSTMODE SetCustMode;

	// A delegate to obtain current drop targets
	typedef CXTMultiCastDelegate1<IXTCustDropSink*> GETTARGETS;
	GETTARGETS GetTargets;

	// Notifies that a control bar has been hidden/shown
	typedef CXTMultiCastDelegate2<CControlBar*, bool> ONSHOWHIDE;
	ONSHOWHIDE OnShowHide;

	// Notifies a new customizable bar has been created
	typedef CXTMultiCastDelegate1<CXTCustomControlBarInfo*> ONNEWBAR;
	ONNEWBAR OnNewBar;

	// Notifies customizable bar has been renamed
	typedef CXTMultiCastDelegate2<CControlBar*, LPCTSTR> ONBARRENAMED;
	ONBARRENAMED OnBarRenamed;

	// Notifies customizable bar has been destroyed
	typedef CXTMultiCastDelegate1<CControlBar*> ONBARDESTROYED;
	ONBARDESTROYED OnBarDestroyed;

	// Returns a menu handle given its ID (a DWORD)
	typedef CXTDelegate1Ret<HMENU, DWORD> GETMENUPOPUP;
	GETMENUPOPUP GetMenuPopup;

	// Application command removed event, gets ID of the command removed
	typedef CXTMultiCastDelegate1<UINT> ONCMDCHANGE;
	ONCMDCHANGE OnCmdRemoved;

	// A delegate to define operation for persisting toolbars to/from the registry
	// Takes a parent frame pointer and a profile name to load from
	// Returns false if no data in the profile
	// Default refers to LoadBarsImp()
	typedef CXTDelegate2Ret<bool, CFrameWnd*, LPCTSTR> PERSISTER;
	
	// A delegate to load dynamic custom toolbars from the registry
	// (defaults to LoadBarsImp())
	PERSISTER LoadDynamicBars;

	// A delegate to store dynamically created toolbars to the registry
	// (defaults to SaveBarsImp())
	PERSISTER SaveDynamicBars;

	// A delegate to find out if a command is defined in the app
	// Takes a command ID, returns true if the command is defined in the app
	// Default refers to IsAppCmdImp()
	typedef CXTDelegate1Ret<bool, UINT> ISAPPCMD;
	ISAPPCMD IsAppCmd;

	// Tells if customization is currently active
	bool IsCustMode() const;

	// tells if UI shall be enabled during customization
	// (valid if IsCustMode() returns true)
	bool IsEnableCmdUI() const;

	// sets the flag telling if UI shall be enabled during toolbar customization
	// default (as set when toolbar customization is turned on) is true
	void SetEnableCmdUI(bool bEnableCmdUI);

	// Removes a target 
	void Remove(CObject* pTarget);

	// Processes drag-n-drop during toolbar customization
	// Returns true to indicate a move operation has been requested
	bool DragNDrop(
		// a window to capture the mouse
		CWnd* pWnd,
		// true if the data can be moved to other locations
		bool canMove,
		// item to move/copy
		CXTCustomGroupItemInfo* pInfo);


	// Default implementation of the CreateBar delegate.
	// It creates CXTCustomToolBar objects.
	// To customize, replace CreateBar delegate with you own version such as
	// shown below.
	// BOOL CMyApp::InitInstance()
	// {
	//		CXTCustomizeConnection::GetInstance()->CreateBar =
	//			CXTCustomizeConnection::CREATEBAR(this,
	//			CXTCustomizeConnection::CREATEBAR::CB(OnCreateCustomBar));
	//		..... other stuff ...
	// }
	// void CMyApp::OnCreateCustomBar(CXTNewCustomBarRequest* pRequest)
	// {
	//		// create default toolbar
	//		CXTCustomizeConnection::GetInstance()->CreateBarImp(pRequest);
	//		CXTCustomToolBar* pBar = (CXTCustomToolBar*)pRequest->m_pNewBar;
	//      .... customize pBar ....
	// }
	void CreateBarImp(CXTNewCustomBarRequest* pRequest);

	// Default implementation of the IsCustCmd delegate.
	// This function filters out IDs for MRU file list (ID_FILE_MRU1 thru 16)
	// To customize, replace IsCustomizableCommand delegate with you own 
	// version such as shown below.
	// BOOL CMyApp::InitInstance()
	// {
	//		CXTCustomizeConnection::GetInstance()->IsCustCmd =
	//			CXTCustomizeConnection::ISCUSTCMD(this,
	//			CXTCustomizeConnection::ISCUSTCMD::CB(IsCustCmd));
	//		..... other stuff ...
	// }
	// bool CMyApp::IsCustCmd(UINT nCmdID)
	// {
	//		return CXTCustomizeConnection::GetInstance()->IsCustCmdImp(nCmdID) &&
	//				nCmdID != ID_A_COMMAND;
	// }	
	bool IsCustCmdImp(UINT nCmdID);

	// Default implementation of restoring dynamic custom toolbars
	// from the data persistently stored in the registry.
	bool LoadDynamicBarsImp(CFrameWnd* pFrame, LPCTSTR lpszProfileName);

	// Default implementation of saving dynamic custom toolbars
	// to the data persistently stored in the registry.
	bool SaveDynamicBarsImp(CFrameWnd* pFrame, LPCTSTR lpszProfileName);

	// Default implementation of newly created toolbar placement
	// Floats the control bar.
	void PlaceBarImp(CFrameWnd* pFrame, CControlBar* pBar);

private:
	void OnSetCustMode(bool bMode);
};


//////////////////////////////////////////////////////////////////////////////
AFX_INLINE bool CXTCustomizeConnection::IsCustMode() const {
	return m_bCustMode;
}
AFX_INLINE bool CXTCustomizeConnection::IsEnableCmdUI() const {
	return m_bEnableCmdUI;
}
AFX_INLINE void CXTCustomizeConnection::SetEnableCmdUI(bool bEnableCmdUI) {
	m_bEnableCmdUI = bEnableCmdUI;
}
//////////////////////////////////////////////////////////////////////////////
//:End Ignore

#endif //__XTCUSTOMIZEAPI_H__
