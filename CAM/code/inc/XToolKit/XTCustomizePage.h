//:Ignore
// XTCustomizePage.h interface for the CXTCustToolBarPage class.
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

#if !defined(__XTCUSTOMIZEPAGE_H__)
#define __XTCUSTOMIZEPAGE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// forwards
class CXTCustomizeSheet;

//////////////////////////////////////////////////////////////////////
// CXTCustomizeContext is a CObject derived class. It creates a customization
// context.
//
class CXTCustomizeContext : public CObject
{

private:
	// A frame on which customizations are executed.
	CFrameWnd* const m_pFrameWnd;

public:
	// Constructs a CXTCustomizeContext object.
	//
	CXTCustomizeContext(
		// A pointer to a CFrameWnd object.
		CFrameWnd* pFrameWnd);

	// Destroys a CXTCustomizeContext object, handles cleanup and de-allocation.
	//
	virtual ~CXTCustomizeContext();

	// This member function gets the frame being customized.  Returns a pointer
	// to a CFrameWnd.
	//
	CFrameWnd* GetFrame() const;

};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CFrameWnd* CXTCustomizeContext::GetFrame() const {
	return m_pFrameWnd;
}

//////////////////////////////////////////////////////////////////////
// CXTCustToolBarPage is a multiple inheritance class derived from CXTDialogState
// and CXTResizePropertyPage. It is displayed during toolbar customization as the
// first page in the customize tab control.
//
class _XT_EXT_CLASS CXTCustToolBarPage : CXTDialogState, public CXTResizePropertyPage
{
    DECLARE_DYNAMIC(CXTCustToolBarPage)

	// Descriptor of a toolbar rendered on this page.
	typedef CXTCustomControlBarInfo TBINFO;
	// Array of toolbars rendered on this page.
	// NB: Can contain zeroed spots for destroyed control bars.
	CArray<TBINFO, TBINFO&> m_infos;
	// Current customization context.
	CXTCustomizeContext* const m_context;

public:
    
    // Constructs a CXTCustToolBarPage object.
    //
	CXTCustToolBarPage(
		// Current customization context.
        CXTCustomizeContext* context);

    // Destroys a CXTCustToolBarPage object, handles cleanup and de-allocation.
    //
    virtual ~CXTCustToolBarPage();

	//{{AFX_DATA(CXTCustToolBarPage)
	enum { IDD = XT_IDD_CUSTPAGE1 };
	CXTCheckListBox m_checkList;
	CButton m_resetButton;
	CButton m_renameButton;
	CButton m_deleteButton;
	CButton m_newButton;
    //}}AFX_DATA

    //{{AFX_VIRTUAL(CXTCustToolBarPage)
    protected:
    
	// DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);    
    virtual BOOL OnInitDialog();
    //}}AFX_VIRTUAL

protected:
    //{{AFX_MSG(CXTCustToolBarPage)
    afx_msg void OnChkChange();
    afx_msg void OnNew();
    afx_msg void OnSelchangeChecklist();
    afx_msg void OnRename();
    afx_msg void OnDelete();
	afx_msg void OnDestroy();
	afx_msg void OnClear();
	//}}AFX_MSG

    DECLARE_MESSAGE_MAP()

    // This member function is called by the page to set the selection
    // in the toolbar list box.
    //
    void SetSelectItem(
        // Index of the list box item to select.
        int nSel=0);

	// This member function adds an item. Returns an int value.
	//
	int AddItem(
		// A pointer to a CXTCustomControlBarInfo object.
		CXTCustomControlBarInfo* pInfo);

	// This member function removes an item by its position in the context.
	//
	void OnBarDestroyed(
		// A pointer to a CControlBar object.
		CControlBar* pBar);

	// This member function finds an item index by its position in the context.
	//
	int CXTCustToolBarPage::FindItem(
		// A pointer to a CControlBar object.
		CControlBar* pBar) const;

	// This member function is called when a control bar's visibility changes.
	//
	void OnShowHideBar(
		// A pointer to a CControlBar object.
		CControlBar* pBar,
		bool bVisible);

	// This member function is called when a control bar is renamed.
	//
	void OnBarRenamed(
		// A pointer to a CControlBar object.
		CControlBar* pBar, 
		// A pointer to a string.
		LPCTSTR pszText);

	// This member function adds custom bar information.
	//
	void StoreInfo(
		// A pointer to a CXTCustomControlBarInfo object.
		CXTCustomControlBarInfo* pInfo);

	// This member function adds custom bar information.
	//
	void StoreNewInfo(
		// A pointer to a CXTCustomControlBarInfo object.
		CXTCustomControlBarInfo* pInfo);
};

//////////////////////////////////////////////////////////////////////
// CXTCustCommandsPage is a multiple inheritance class derived from CXTDialogState,
// CXTResizePropertyPage, and CXTCommandsListBox::ISite. It is displayed during
// toolbar customization as the second page in the customize tab control.
//
class _XT_EXT_CLASS CXTCustCommandsPage : CXTDialogState, 
										  public CXTResizePropertyPage,
										  public CXTCommandsListBox::ISite
{
    DECLARE_DYNAMIC(CXTCustCommandsPage)

	// Array of groups.
	CXTCustomGroups m_groups;
	// A map of command IDs to their icons.
	CXTIconMap m_iconMap;
	// Current customization context.
	CXTCustomizeContext* const m_context;

public:
    
    // Constructs a CXTCustCommandsPage object.
    //
    CXTCustCommandsPage(
		// Current customization context.
        CXTCustomizeContext* context);


    // Destroys a CXTCustCommandsPage object, handles cleanup and de-allocation.
    //
    virtual ~CXTCustCommandsPage();

    //{{AFX_DATA(CXTCustCommandsPage)
	enum { IDD = XT_IDD_CUSTPAGE2 };
	CXTCommandsListBox  m_listCommands;
	CXTListBox  m_listGroups;
	CButton   m_btnDescription;
	CStatic m_txtSelected;
    //}}AFX_DATA

	// Displays information about the selected command.
	CXTTipWindow m_tipWindow;

    // The page calls this member function to update the commands list
	// box contents.
    //          
    void UpdateListContents();

    // The page calls this member function to enable or disable the displayed
    // dialog controls.
    //
    void SetEnableControls(    
		// TRUE to enable, FALSE to disable.
        BOOL bEnable);

	// This member function gets icons of a command . Returns an true if found, false otherwise
	// Implements CXTCommandsListBox::ISite interface.
	virtual bool GetCommandIcon(
		// Position (item data) of the item in the list box.
		UINT nCmdID,
		// destination to store command icon, valid if function returns true.
		HICON& hIcon, 
		// destination to store disabled command icon, can be NULL, valid if function returns true.
		HICON& hDisabledIcon,
		// destination to store hot command icon, can be NULL, valid if function returns true.
		HICON& hHotIcon);

	// This member function gets a command ID by its position. Returns a UINT
	// value. Implements CXTCommandsListBox::ISite interface.
	virtual UINT GetItemCommand(
		// Position (item data) of the item in the list box.
		int pos);

	// This member function performs drag and drop of an item at the position
	// provided. Implements CXTCommandsListBox::ISite interface.
	virtual void DragNDrop(
			// Position (item data) of the item in the list box.
			int pos);

	// notification handler for a command removed event
	void OnCmdRemoved(UINT nCmdID);

	//{{AFX_VIRTUAL(CXTCustCommandsPage)
    protected:
    // DDX/DDV support
	virtual void DoDataExchange(CDataExchange* pDX);    
    virtual BOOL OnInitDialog();
    //}}AFX_VIRTUAL

    //{{AFX_MSG(CXTCustCommandsPage)
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    afx_msg void OnSelchangeListCategories();
    afx_msg void OnBtnDescription();
    afx_msg void OnSelchangeListCommands();
    //}}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////
// CXTCustAccelerators is a multiple inheritance class derived from CXTDialogState
// and CXTResizePropertyPage. It is displayed during toolbar customization as the
// keyboard page in the customize tab control.  This class is used to create a 
// shortcut assignments dialog page for the keyboard tab in the customize dialog.
//
class _XT_EXT_CLASS CXTCustAccelerators : CXTDialogState, public CXTResizePropertyPage
{
    DECLARE_DYNAMIC(CXTCustAccelerators)

	// Current customization context
	CXTCustomizeContext* const m_context;

public:

    // Constructs a CXTCustAccelerators object.
    //
    CXTCustAccelerators(
		// Current customization context.
		CXTCustomizeContext* const context);

    // Destroys a CXTCustAccelerators object, handles cleanup and de-allocation.
    //
    virtual ~CXTCustAccelerators();

    //{{AFX_DATA(CXTCustAccelerators)
    enum { IDD = XT_IDD_CUSTPAGE3 };
	CXTAccelKeyEdit	m_editShortcutKey;
	CXTListBox		m_lboxKeys;
	CXTListBox		m_lboxCommands;
	CComboBox		m_comboCategory;
	CString			m_strTitle;
	CString			m_strDesc;
	int		m_iCategory;
    //}}AFX_DATA

    //{{AFX_VIRTUAL(CXTCustAccelerators)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

	// This member function is called by the page to reload the list items.
	//
	void ReloadList();

	// This member function is called by the page to enable the assignment buttons.
	//
	void EnableControls();

protected:

    //{{AFX_MSG(CXTCustAccelerators)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCategory();
	afx_msg void OnSelchangeCommands();
	afx_msg void OnSelchangeCurKeys();
	afx_msg void OnChangeShortcutKey();
	afx_msg void OnAssign();
	afx_msg void OnResetAll();
	afx_msg void OnRemove();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

typedef CList<XT_TOOL*,XT_TOOL*> CXTToolsList;

//////////////////////////////////////////////////////////////////////
// CXTCustTools is a multiple inheritance class derived from CXTDialogState
// and CXTResizePropertyPage. It is displayed during toolbar customization as the
// second page in the customize tab control.
//
class _XT_EXT_CLASS CXTCustTools : CXTDialogState, public CXTResizePropertyPage
{
    DECLARE_DYNAMIC(CXTCustTools)

	// Current customization context
	CXTCustomizeContext* const m_context;

public:
    
    // Constructs a CXTCustTools object.
    //
    CXTCustTools(
		// Current customization context.
		CXTCustomizeContext* const context);

    // Destroys a CXTCustTools object, handles cleanup and de-allocation.
    //
    virtual ~CXTCustTools();

    //{{AFX_DATA(CXTCustTools)
    enum { IDD = XT_IDD_CUSTPAGE4 };
	CStatic	m_txtToolsDir;
	CStatic	m_txtToolsArg;
	CStatic	m_txtToolsCmd;
	CString m_strToolsDir;
	CString	m_strToolsArg;
	CString	m_strToolsCmd;
	CXTBrowseEdit	m_editToolsDir;
	CXTBrowseEdit	m_editToolsArg;
	CXTBrowseEdit	m_editToolsCmd;
	CXTEditListBox	m_lboxToolsList;
    //}}AFX_DATA

    CXTToolsList m_arTools;
	XT_TOOL*     m_pOldData;

	enum { XT_TOOLCMD, XT_TOOLARG, XT_TOOLDIR };

	void UpdateToolsItem(LPCTSTR lpszText, int iWhich, bool bAppend);
	void UpdateToolData();
    void EnableControls(bool bEnable);
    void UpdateToolsList();
    void FreeToolsList(CXTToolsList& arTools, bool bAddTool=false);
	bool ValidateTools();

    //{{AFX_VIRTUAL(CXTCustTools)
	public:
	virtual BOOL OnKillActive();
	protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
    //{{AFX_MSG(CXTCustTools)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeEditList();
	afx_msg void OnChangeToolsCmd();
	afx_msg void OnChangeToolsArg();
	afx_msg void OnChangeToolsDir();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnLabelEditEnd();
	afx_msg void OnLabelEditCancel();
	afx_msg void OnNewItem();
	afx_msg void OnPreDeleteItem();
	afx_msg void OnDeleteItem();
	afx_msg void OnMoveItemUp();
	afx_msg void OnMoveItemDown();
    DECLARE_MESSAGE_MAP()
};

// forwards
class CXTOptionsManager;

//////////////////////////////////////////////////////////////////////
// CXTCustOptions is a multiple inheritance class derived from CXTDialogState
// and CXTResizePropertyPage. It is displayed during toolbar customization as the
// second page in the customize tab control.
//
class _XT_EXT_CLASS CXTCustOptions : CXTDialogState, public CXTResizePropertyPage
{
    DECLARE_DYNAMIC(CXTCustOptions)

	// Current customization context
	CXTCustomizeContext* const m_context;

public:
    
    // Constructs a CXTCustOptions object.
    //
    CXTCustOptions(
		// Current customization context.
		CXTCustomizeContext* const context);

    // Destroys a CXTCustOptions object, handles cleanup and de-allocation.
    //
    virtual ~CXTCustOptions();

    //{{AFX_DATA(CXTCustOptions)
	enum { IDD = XT_IDD_CUSTPAGE5 };
	CComboBox	m_comboAnimationType;
    CButton m_chkShowFull;
    CButton m_chkToolBarAccelTips;
	BOOL	m_bMenuShadows;
	BOOL	m_bMenuRecentCommands;
	BOOL	m_bToolBarScreenTips;
	BOOL	m_bShowFullAfterDelay;
	BOOL	m_bToolBarAccelTips;
	BOOL    m_bToolBarVisualize;
	int		m_nAnimationType;
	//}}AFX_DATA

    //{{AFX_VIRTUAL(CXTCustOptions)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

protected:
    //{{AFX_MSG(CXTCustOptions)
	afx_msg void OnMenuShadows();
	afx_msg void OnRecentMenu();
	afx_msg void OnShowFull();
	afx_msg void OnBtnReset();
	afx_msg void OnScreenTips();
	afx_msg void OnShowShortcutKeys();
	afx_msg void OnVisualize();
	afx_msg void OnAnimation();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP()

	void AddString( UINT nIDResource );
};

//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTCUSTOMIZEPAGE_H__)
//:End Ignore
