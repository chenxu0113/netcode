//:Ignore
// XTEditListBox.h interface for the CXTEditGroup class.
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

#if !defined(__XTEDITLISTBOX_H__)
#define __XTEDITLISTBOX_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTListBox is a CListBox derived class.  CXTListBox extends the standard
// list box control to enable flicker free drawing.
//
class _XT_EXT_CLASS CXTListBox : public CListBox
{
    DECLARE_DYNAMIC(CXTListBox)

public:
  
    // Constructs a CXTListBox object.
    //
    CXTListBox();

    // Destroys a CXTListBox object, handles cleanup and de-allocation.
    //
    virtual ~CXTListBox();

    // If 'true,' the font will automatically be set for the control.
	bool m_bAutoFont;
	// 'true' for initialization.
	bool m_bInitControl;

    //:Ignore
    //{{AFX_VIRTUAL(CXTListBox)
    public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    protected:
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL
    //:End Ignore

protected:

    //:Ignore
    //{{AFX_MSG(CXTListBox)
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	afx_msg LRESULT OnInitControl(WPARAM wParam, LPARAM lParam);
    //:End Ignore

    DECLARE_MESSAGE_MAP()
};

//:Ignore
// List used for enumerating XT_LOGFONT structures.
typedef CList<XT_LOGFONT, XT_LOGFONT&> CXTFontList;
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTFontEnum is a stand alone singleton font enumeration class.  It is
// used to enumerate a list of fonts found installed for the operating
// system.  CXTFontEnum is a singleton class, which means it can only be
// instantiated a single time.  The constructor is private, so the only
// way to access members of this class is to use the objects Get() method,
// to retrieve a list of available fonts for your operating system, you
// would make the following call:
//
//[pre]CXTFontEnum::Get().GetFontList()[/pre]
//
class _XT_EXT_CLASS CXTFontEnum  
{
    // Constructs a CXTFontEnum object.  CXTFontEnum is a singleton
	// class, to instantiate an object, use the static method Get().
    //
	CXTFontEnum();

public:

    // Destroys a CXTFontEnum object, handles cleanup and de-allocation.
    //
	virtual ~CXTFontEnum();

protected:

	// List of fonts found during enumeration.
	CXTFontList m_listFonts;

public:

	// This static member function will return a reference to the one
	// and only CXTFontEnum object.  You can use this function to access
	// data members for the CXTFontEnum class.
	//
	// [b]Example:[/b]
	// 
	//[pre]CXTFontEnum::Get().GetFontList()[/pre]
	// 
	// Returns a reference to a CXTFontEnum object.
	//
	static CXTFontEnum& Get();

	// This member function is used to determine the existence of the font
	// specified by 'strFaceName'.  Returns 'true' if the font exists, otherwise
	// returns 'false'.
	//
	bool DoesFontExist(
		// Reference to a NULL terminated string that represents the font name.
		CString& strFaceName);
	
	// This member function is used to get a pointer to the font specified
	// by 'strFaceName'. Returns a pointer to the XT_LOGFONT structure for the
	// specified item, or NULL if no font was found.
	//
	XT_LOGFONT* GetLogFont(
		// A NULL terminated string that represents the font name.
		CString strFaceName);

	// This member function is used by the callback function to retrieve the
	// current width for the longest font name in the list. Returns an integer
	// value that represents the width for the longest font in the list.
	//
	int GetMaxWidth();

	// This member function is used to get a reference to the font list. 
	// Returns a reference to the CXTFontList used by this class.
	//
	CXTFontList& GetFontList();

	// This member function is called by the CXTFontEnum class to initialize
	// the font list. You can also call this member function to reinitialize
	// the font enumeration. For example, if you changed printers and you want
	// to enumerate printer fonts, or you wanted to use a different character
	// set.
	//
	void Init(
		// Points to a valid device context, if NULL, the screen device context
		// is used.
		CDC* pDC=NULL,
		// Represents the character set to enumerate.
		BYTE nCharSet=DEFAULT_CHARSET);

protected:

    // The EnumFontFamExProc function is an application defined–callback
	// function used with the EnumFontFamiliesEx function. It is used to process
	// the fonts. It is called once for each enumerated font. The FONTENUMPROC
	// type defines a pointer to this callback function. EnumFontFamExProc
	// is a placeholder for the application defined–function name. The return
	// value must be a nonzero value to continue enumeration.  To stop enumeration,
	// the return value must be zero.
    //
    static BOOL CALLBACK EnumFontFamExProc(
		// Pointer to an ENUMLOGFONTEX structure that contains information 
		// about the logical attributes of the font.
        ENUMLOGFONTEX* pelf,
		// Pointer to a structure that contains information about the physical 
		// attributes of a font. The function uses the NEWTEXTMETRICEX structure 
		// for TrueType fonts; and the TEXTMETRIC structure for other fonts.
        NEWTEXTMETRICEX* lpntm,
		// Specifies the type of the font. This parameter can be a combination
		// of these values: 
		// [ul]
		// [li][b]DEVICE_FONTTYPE[/b] - The font is a device font.[/li] 
		// [li][b]RASTER_FONTTYPE[/b] - The font is a raster font.[/li]
		// [li][b]TRUETYPE_FONTTYPE[/b] - The font is a TrueType font.[/li]
		// [/ul]
        DWORD dwFontType,
		// Specifies the application-defined data passed by the EnumFontFamiliesEx
		// function.
        LPARAM lParam);

	// This member function is called by the font enumeration callback to
	// add a font to the font list. Returns 'true' if successful, otherwise
	// returns 'false'.
	//
	bool AddFont(
		// Points to a valid LOGFONT structure.
		const LOGFONT* pLF,
		// Specifies the type of the font. This parameter can be a combination
		// of these values: 
		// [ul]
		// [li][b]DEVICE_FONTTYPE[/b] - The font is a device font.[/li] 
		// [li][b]RASTER_FONTTYPE[/b] - The font is a raster font.[/li]
		// [li][b]TRUETYPE_FONTTYPE[/b] - The font is a TrueType font.[/li]
		// [/ul]
		DWORD dwType);
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTFontList& CXTFontEnum::GetFontList() {
	return m_listFonts;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//////////////////////////////////////////////////////////////////////
// Enumerated style that is used to define how the font list box will display
// the font listings.
//
enum eSTYLE
{
	// Display font name with GUI font style.
	XT_FLB_NAME_GUI = 0,
	// Display font name with its own font style.
	XT_FLB_NAME_SAMPLE,
	// Display font name with GUI font style, then a sample display to the
	// right.
	XT_FLB_BOTH,
};

// forwards
class CXTFontListBoxWndHook;

//////////////////////////////////////////////////////////////////////
// CXTFontListBox is a CXTListBox derived class.  It is used to create
// a font selection list box.  You can choose to display the font name
// with the GUI font style, display the font name with its own font style,
// or display the font name with the default GUI font style and a sample
// display to the right.
//
class _XT_EXT_CLASS CXTFontListBox : public CXTListBox
{
public:

    // Constructs a CXTFontListBox object.
    //
	CXTFontListBox();

    // Destroys a CXTFontListBox object, handles cleanup and de-allocation.
    //
	virtual ~CXTFontListBox();

protected:
	
	// String displayed for the symbol characters.
	CString m_strSymbol;
	// Enumerated style indicating how to display the font list.
	eSTYLE m_eStyle;
	// True type font image list.
	CImageList m_ilFontType;

public:

    // Call this member function to get the logfont for the currently selected
	// item. Returns 'true' if successful, otherwise returns 'false'.
    //
    virtual bool GetSelFont(
		// Reference to an XT_LOGFONT structure.
        XT_LOGFONT& lf);

    // Call this member function to get the logfont for the currently selected
	// item. Returns 'true' if successful, otherwise returns 'false'.
    //
	virtual bool GetSelFont(
		// A reference to a valid CString object to receive the logfont face
		// name.
		CString& strFaceName);

    // Call this member function to select the logfont for the list box.
	// Returns 'true' if successful, otherwise returns 'false'.
    //
    virtual bool SetSelFont(
		// Reference to an XT_LOGFONT structure.
        XT_LOGFONT& lf);

    // Call this member function to select the logfont for the list box.
    // Returns 'true' if successful, otherwise returns 'false'.
    //
	virtual bool SetSelFont(
		// A NULL terminated string that represents the logfont face name.
		CString strFaceName);

	// Call this member function to set the font display style for the font
	// list box.  There are three styles to choose from.  They include displaying
	// the font in the default GUI font, displaying the font in its own font
	// style, or displaying both the font name in the default GUI font and
	// a sample to the right.
	// 
	void SetListStyle(
		// Specifies the style for the font list box.  Styles can be any one of
		// the following combinations:
		// [ul]
		// [li][b]XT_FLB_NAME_GUI[/b] - Display font name in GUI font style.[/li]
		// [li][b]XT_FLB_NAME_SAMPLE[/b] - Display font name with its own font
		//        style.[/li]
		// [li][b]XT_FLB_BOTH[/b] - Display font name in GUI font style, then
		//        a sample display to the right.[/li]
		// [/ul]
		eSTYLE eStyle);

	// Call this member function to initialize the font list box and populate it
	// with a list of avaliable fonts.
	//
	virtual void InitControl();

    //:Ignore
	//{{AFX_VIRTUAL(CXTFontListBox)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	//}}AFX_VIRTUAL
    //:End Ignore

protected:
    //:Ignore
	//{{AFX_MSG(CXTFontListBox)
	//}}AFX_MSG
    //:End Ignore

	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTFontListBox::SetListStyle(eSTYLE eStyle) {
    m_eStyle = eStyle;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTEditGroup is a CStatic derived class.  It used by the CXTEditListBox
// class to create a toolbar above the edit list box to display icons for
// editing.  It can be used for other classes by setting the notify window
// in Initialize.  This window will receive notification messages whenever
// the new, delete, up, and down buttons are pressed. You can handle these
// messages by adding an ON_BN_CLICKED handler for each of the buttons
// XT_IDC_BTN_NEW, XT_IDC_BTN_DELETE, XT_IDC_BTN_UP and XT_IDC_BTN_DOWN.
//
class _XT_EXT_CLASS CXTEditGroup : public CStatic
{
    DECLARE_DYNAMIC(CXTEditGroup)

public:

    // Constructs a CXTEditGroup object.
    //
    CXTEditGroup();

    // Destroys a CXTEditGroup object, handles cleanup and de-allocation.
    //
    virtual ~CXTEditGroup();

protected:

    // Points to a CWnd object where notification messages are sent.
	CWnd* m_pNotifyWnd;
	// New edit button.
	CXTButton m_btnNew;
	// Delete edit button.
	CXTButton m_btnDelete;
	// Move Up edit button.
	CXTButton m_btnUp;
	// Move Down edit button.
	CXTButton m_btnDown;
	// Holds the images for the edit buttons.
	CImageList m_ImageList;
	// Tooltip control for edit buttons.
	CToolTipCtrl m_tooltip;
	// Initial size of the edit buttons.
	CSize m_sizeBtn;
	// Size of each button in the edit group.
	CRect m_rClip[4];
	// Down button icon handle.
	CXTIconHandle m_hIconDown;
	// Up button icon handle.
	CXTIconHandle m_hIconUp;
	// Delete button icon handle.
	CXTIconHandle m_hIconDelete;
	// New button icon handle.
	CXTIconHandle m_hIconNew;

public:
    
    // This member function will initialize the edit group control.
    //
    virtual void Initialize(
		// A CWnd object that represents the window that is to receive notification
		// messages.
        CWnd* pNotifyWnd);

    //:Ignore
    //{{AFX_VIRTUAL(CXTEditGroup)
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL
	virtual void MoveButtons();
    //:End Ignore

    //:Ignore
    //{{AFX_MSG(CXTEditGroup)
    afx_msg void OnButtonNew();
    afx_msg void OnButtonDelete();
    afx_msg void OnButtonUp();
    afx_msg void OnButtonDown();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
    //:End Ignore

    DECLARE_MESSAGE_MAP()
};

// forwards
class CXTItemEdit;

//////////////////////////////////////////////////////////////////////
// CXTEditListBox is a CXTListBox derived class. It is used to create an
// editable list box.  This list box can be configured to display a toolbar
// for editing.  You can define browse styles to search for files or folders.
// Each entry is made editable with a double mouse click.
//
class _XT_EXT_CLASS CXTEditListBox : public CXTListBox
{
    DECLARE_DYNAMIC(CXTEditListBox)

public:

    // Constructs a CXTEditListBox object.
    //
    CXTEditListBox();

    // Destroys a CXTEditListBox object, handles cleanup and de-allocation.
    //
    virtual ~CXTEditListBox();

    // This member function will create the edit group control.
    //
    void SetListEditStyle(
		// NULL terminated string that represents the caption title.
        LPCTSTR lpszTitle,
		// Style for the list edit control.  Pass in LBS_XT_NOTOOLBAR
        // if you do not wish the caption edit navigation control
        // bar to be displayed.
        DWORD dwLStyle=LBS_XT_DEFAULT);

    // This member function will create the edit group control.
    //
    void SetListEditStyle(
		//  Resource ID of the string to load for the caption title.
        UINT nTitle,
		// Style for the list edit control.  Pass in LBS_XT_NOTOOLBAR
        // if you do not wish the caption edit navigation control
        // bar to be displayed.
        DWORD dwLStyle=0x0);

	// Call this member function to get the current index for the edit control.
	// Similar to GetCurSel, however the current index is the index of the
	// last item to be modified or added to the edit list box, and not necessarily
	// the selected item. Returns an integer value that represents the edit control
	// index.
	//
	int GetCurrentIndex();

	// This member function will enable editing for the list box item.
	//
	void EditItem(
		// Index of the item to edit.
		int iItem);

	// Call this member function to return a reference to the CXTEditGroup
	// control that is associated with the edit list box. Returns a reference
	// to a CXTEditGroup object.
	//
	CXTEditGroup& GetEditGroup();

	// Call this member function to set the default filter for the file dialog.
	//
	virtual void SetDlgFilter(
		// Points to a NULL terminated string that represents the file filter
		// used by the file open dialog.
		LPCTSTR strFilter=NULL);

	// Call this member function to determine if the edit list has a toolbar.
	// Returns 'true' if the toolbar is turned on, otherwise returns 'false.'
	//
	bool HasToolbar();

protected:

    // Current index when edit functions are performed.
	int m_nIndex;
	// 'true' for initialization.
	bool m_bInitControl;
	// TRUE if a new item is being entered into the list box.
	BOOL m_bNewItem;
	// Current text of a selected item during edit.
	CString m_strItemText;
	// The edit group (toolbar) that appears above the list box.
	CXTEditGroup m_editGroup;
	// List edit styles.
	DWORD m_dwLStyle;
	// Caption area title.
	CString m_strTitle;
	// Default file filter.
	CString m_strFilter;
	// Points to the in-place edit item.
	CXTItemEdit* m_pItemEdit;

    // This member function will create the edit group control.
    // Returns TRUE if successful, otherwise returns FALSE.
    //
    virtual BOOL CreateEditGroup();

    // This member function will enable editing for the currently selected
	// list box item.  If 'bNewItem' is TRUE, a new item is added to the end
	// of the list box.
    //
    void EditListItem(
		// TRUE to add a new item.
        BOOL bNewItem);

    //:Ignore
    //{{AFX_VIRTUAL(CXTEditListBox)
    public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual void PreSubclassWindow();
    //}}AFX_VIRTUAL
	virtual void PositionEditGroup(bool bSizeList=false);
    //:End Ignore

    //:Ignore
    //{{AFX_MSG(CXTEditListBox)
    afx_msg void OnEndLabelEdit();
    afx_msg void OnNewItem();
    afx_msg void OnDeleteItem();
    afx_msg void OnMoveItemUp();
    afx_msg void OnMoveItemDown();
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcMButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	afx_msg LRESULT OnInitControl(WPARAM wParam, LPARAM lParam);
    //:End Ignore

    DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTEditListBox::GetCurrentIndex() {
	return m_nIndex;
}
AFX_INLINE CXTEditGroup& CXTEditListBox::GetEditGroup() {
	return m_editGroup;
}
AFX_INLINE void CXTEditListBox::SetDlgFilter(LPCTSTR strFilter/*=NULL*/) {
	m_strFilter = strFilter;
}
AFX_INLINE bool CXTEditListBox::HasToolbar() {
	return ((m_dwLStyle & LBS_XT_NOTOOLBAR) == 0);
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTEDITLISTBOX_H__)
