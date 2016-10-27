//:Ignore
// XTOutBarCtrl.h interface for the CXTOutBarCtrl class.
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

#if !defined(__XTOUTBARCTRL_H__)
#define __XTOUTBARCTRL_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

// forwards
class CXTBarItem;
class CXTBarFolder;
class CXTEditItem;

//////////////////////////////////////////////////////////////////////
// CXTOutBarCtrl is a CWnd derived class.  It is used to create a shortcut
// bar window similar to the shortcut bar seen in Outlook&trade;.
//
class _XT_EXT_CLASS CXTOutBarCtrl : public CWnd
{
	DECLARE_DYNCREATE(CXTOutBarCtrl)

public:

    // Constructs a CXTOutBarCtrl object.
	//
	CXTOutBarCtrl();

	// Destroys a CXTOutBarCtrl object, handles cleanup and de-allocation.
	//
    virtual ~CXTOutBarCtrl();

protected:

//:Ignore
	// used internally
	int m_nFolderHeight;
	// used internally
	int m_nSelFolder;
	// used internally
	int m_nFolderHilighted;
	// used internally
	int m_nItemHilighted;
	// used internally
	int m_nLastFolderSelected;
	// used internally
	int m_nLastItemSelected;
	// used internally
	int m_nFirstItem;
	// used internally
	int m_nIconSpacingLarge;
	// used internally
	int m_nIconSpacingSmall;
	// used internally
	int m_nHitInternal1;
	// used internally
	int m_nHitInternal2;
	// used internally
	int m_nLastDragItemDraw;
	// used internally
	int m_nLastDragItemDrawType;
	// used internally
	int m_nSelAnimCount;
	// used internally
	int m_nSelAnimTiming;
	// used internally
	int m_nAnimationTickCount;
	// used internally
	BOOL m_bUpArrow;
	// used internally
	BOOL m_bDownArrow;
	// used internally
	BOOL m_bUpPressed;
	// used internally
	BOOL m_bDownPressed;
    // used internally
	BOOL m_bIconPressed;
	// used internally
	BOOL m_bLooping;
	// used internally
	BOOL m_bPressedHighlight;
	// used internally
	CFont m_font;
	// used internally
	CPen m_penBlack;
	// used internally
	CImageList* m_pLargeImageList;
	// used internally
	CImageList* m_pSmallImageList;
	// used internally
	DWORD m_dwFlags;
	// used internally
	CRect m_rcUpArrow;
	// used internally
	CRect m_rcDownArrow;
	// used internally
	CSize m_sizeOffset;
	// used internally
	CSize m_sizeMargin;
	// used internally
	CPtrArray m_arFolder;
	// used internally
	COLORREF m_clrBack;
	// used internally
	COLORREF m_clrText;
    // used internally
	BOOL m_bUserClrBack;
    // used internally
	BOOL m_bUserClrText;
//:End Ignore

//:Ignore
    // used internally
	typedef enum { F_NORMAL, F_SELECT, F_HILIGHT } FOLDER_HILIGHT; 
//:End Ignore

public:

	// Call this member function to set an animation effect for the currently
	// selected item.  Not to be used with OBS_XT_SELHIGHLIGHT flag.
	//
	virtual void SetAnimSelHighlight(
		// Specifies the time, in milliseconds, that the selected item will animate.
		const int iTime);

	// Call this member function to retrieve the item data that was set for
	// the specified folder. Returns a DWORD value.
	//
	virtual DWORD GetFolderData(
		// The index of the folder to retrieve item data for.  If -1, the currently
		// selected folder item data is returned.
		int iFolder = -1);

	// Call this member function to retrieve the CWnd object that has been set
	// for the folder specified by 'iFolder'.  If 'iFolder' is -1, the child of
	// the currently selected folder is returned.  If no object has been set
	// for the folder, the return value is NULL.
	//
	virtual CWnd* GetFolderChild(
		// Index of the folder to retrieve the CWnd object for, if -1 the currently
		// selected folder CWnd object is used.
		int iFolder = -1);

	// Call this member function to add a folder with a CWnd child nested
	// inside of it.  You can insert a folder with any CWnd object, such as
	// a tree control (see the OutlookBar sample). Returns the integer value that
	// represents the index of the added folder.
	//
	virtual int AddFolderBar(
		// Name of the folder to add.
		LPCTSTR lpszFolderName,
		// Points to a valid CWnd object.  The object must be created before
		// inserting.
		CWnd* pWndChild,
		// Item data (lParam) for the folder.
		const DWORD dwData = 0);

	// Call this member function to get the text of the specified item for
	// the currently selected folder. Returns a CString object containing
	// the retrieved text.
	//
	virtual CString GetItemText(
		// Index of the item to retrieve the text for.
		const int iIndex);

	// Call this member function to set the tick count, in milliseconds, between
	// each animation frame in folder scrolling.  If you set a value of -1,
	// or minor, no animation will be played.  Animation requires the OBS_XT_ANIMATION
	// flag be set.
	//
	virtual void SetAnimationTickCount(
		// Specifies the time, in milliseconds, between animation.  A value of
		// -1 will disable animation playback.
		const long lValue);

	// Call this member function to get the current animation tick count.
	// Returns an integer value representing the current tick count.
	//
	virtual int GetAnimationTickCount();

	// Call this member function to set the image index, in the image list,
	// for the 'iIndex' item of the currently selected folder.
	//
	virtual void SetItemImage(
		// Index of the item in the currently selected folder.
		const int iIndex,
		// Index of the image, in the image list, to use for the specified item.
		const int iImage);

	// Call this member function to set the item data (lParam) for the specified
	// item in the currently selected folder.
	//
	virtual void SetItemData(
		// Index of the item to set item data for.
		const int iIndex,
		// Item data (lParam) to set.
		const DWORD dwData);

	// Call this member function to retrieve the index of the image associated
	// with the specified item in the currently selected folder. Returns an
	// integer value that represents the index of the desired image.
	//
	virtual int  GetItemImage(
		// Index of the item to retrieve the image index for.
		const int iIndex) const;

	// Call this member function to get the item data (lParam) for the specified
	// item in the currently selected folder. Returns a DWORD value.
	//
	virtual DWORD GetItemData(
		// Index of the item to retrieve item data for.
		const int iIndex) const;

	// Call this member function to remove all items from the folder specified
	// by 'iFolder'.
	//
	virtual void RemoveAllItems(
		// Index of the folder to insert the item into.
		int iFolder,
		// 'true' to send an XTWM_OUTBAR_NOTIFY message.
		bool bNofify=false);

	// Call this member function to remove the specified item from the currently
	// selected folder.
	//
	virtual void RemoveItem(
		// Index of the item to remove.
		const int iIndex);

	// Call this member function to set the text for the specified item in the
	// currently selected folder.
	//
	virtual void SetItemText(
		// Index of the item to set the text for.
		const int iIndex,
		// Points to a NULL terminated string.
		LPCTSTR lpszItemName);

	// Call this member function to begin local editing of the specified item
	// in the currently selected folder.
	//
	virtual void StartItemEdit(
		// Index of the item to begin editing for.
		const int iIndex);

	// Call this member function to set the text label for the specified folder.
	//
	virtual void SetFolderText(
		// Index of the folder to set the text label for.
		const int iIndex,
		// Points to a NULL terminated string.
		LPCTSTR lpszFolderName);

	// Call this member function to begin editing of the specified folder
	// item's label.
	//
	virtual void StartGroupEdit(
		// Index of the folder to begin editing.
		const int iIndex);

	// Call this member function to get a pointer to the image list for the
	// specified folder. Returns a CImageList pointer representing the image list
	// for the folder specified by 'iIndex'.
	//
	virtual CImageList* GetFolderImageList(
		// Index of the folder to retrieve the image list for.
		const int iIndex,
		// TRUE to return the small image list.  FALSE to return the large image
		// list.
		const BOOL bSmall) const;

	// Call this member function to return the global image list for the OutlookBar
	// control. Returns a CImageList pointer representing the global image
	// list for the OutlookBar control.
	//
	virtual CImageList* GetImageList(
		// If OBS_XT_SMALLICON, the small image list is returned, if OBS_XT_LARGEICON,
		// the large image list is returned.
		DWORD dwImageList);

	// Call this member function to set the image list for the specified folder.
	// Returns a pointer to the previously set image list, or NULL if no previous
	// image list exists.
	//
	virtual CImageList* SetFolderImageList(
		// Index of the folder to set the image list for.
		const int iFolder,
		// Points to the new image list.
		CImageList* pImageList,
		// If OBS_XT_SMALLICON, the small image list is set, if OBS_XT_LARGEICON,
		// the large image list is set.
		DWORD dwImageList);

	// This member function will set the main image list.  You can link different
	// imagelists to the folders using the SetFolderImageList function.  If a
	// folder has been linked to an image list with the SetFolderImageList function,
	// it will own the linked image list.  Otherwise, it will use the image list
	// set with this function.  Returns a pointer to the previously set image
	// list, or NULL if no previous image list exists.
	//
	virtual CImageList* SetImageList(
		// Points to the new image list.
		CImageList* pImageList,
		// If OBS_XT_SMALLICON, the small image list is set, if OBS_XT_LARGEICON,
		// the large image list is set.
		DWORD dwImageList);

	// Call this member function to remove the specified folder and its items.
	//
	virtual void RemoveFolder(
		// Index of the folder to remove.
		const int iIndex);

	// This member function will get the index of the currently selected folder.
	// Returns an integer value representing the index of the currently selected
	// folder.
	//
	virtual int GetSelFolder() const;

	// This member function will get the total number of folders found in
	// the Outlook bar. Returns an integer value representing the number
	// of folders in the Outlook bar.
	//
	virtual int GetFolderCount() const;

	// This member function will set the selected folder for the Outlook bar.
	//
	virtual void SetSelFolder(
		// Index of the new selected folder.
		const int iIndex);

	// This member function gets the number of items found in the currently
	// selected folder. Returns an integer value representing the number of items
	// in the current folder.
	//
	virtual int GetItemCount() const;

	// Call this member function to insert an item into the specified folder.
	// Returns the index of the newly inserted item.
	//
	virtual int InsertItem(
		// Index of the folder to insert the item into.
		const int iFolder,
		// Index or position of the item to insert into the folder.
		const int iIndex,
		// A NULL terminated string that represents the item label.  This value
		// [b]cannot[/b] be set to NULL.
		LPCTSTR lpszItemName,
		// Index into the folder's image list.
		const int iImage = -1,
		// User defined item data that you can assign to the item. Use GetItemData
		// and SetItemData to access and change this data.
		const DWORD dwData = 0);

	// Call this member function to add a folder to the Outlook bar control.
	// Returns the index of the newly inserted folder.
	//
	virtual int AddFolder(
		// A NULL terminated string that represents the folder's label.
		LPCTSTR lpszFolderName,
		// User defined item data for the folder.
		const DWORD dwData);

    // Call this member function to modify an Outlook bar style.  Styles
	// to be added or removed can be combined by using the bitwise OR (|) 
    // operator.
	//
    // The desired styles for the Outlook bar can be one or more of the 
    // following:
    //[ul]
    //[li][b]OBS_XT_EDITGROUPS[/b] - Enables folder local editing (renaming).[/li] 
    //[li][b]OBS_XT_EDITITEMS[/b] - Enables item local editing (renaming).[/li]
    //[li][b]OBS_XT_REMOVEGROUPS[/b] - Enables the "Remove" command for
	//       folders in context menu.[/li]
    //[li][b]OBS_XT_REMOVEITEMS[/b] - Enables the "Remove" command for
	//       items in context menu.[/li]
    //[li][b]OBS_XT_ADDGROUPS[/b] - Enables folder insertion.[/li]
    //[li][b]OBS_XT_DRAGITEMS[/b] - Enables item dragging to rearrange
	//       position.[/li]
    //[li][b]OBS_XT_ANIMATION[/b] - Enables animation while changing folder
	//       selection.[/li] 
    //[li][b]OBS_XT_SELHIGHLIGHT[/b] - Enables dimmed highlight of last
	//       pressed item.[/li]
    //[li][b]OBS_XT_DEFAULT[/b] - Same as OBS_XT_DRAGITEMS | OBS_XT_EDITGROUPS
	//       | OBS_XT_EDITITEMS | OBS_XT_REMOVEGROUPS | OBS_XT_REMOVEITEMS
	//       | OBS_XT_ADDGROUPS.[/li]
    //[/ul]
    //
	virtual void ModifyFlag(
        // Specifies OBS_XT_ styles to be removed during style modification.
		const DWORD& dwRemove,
        // Specifies OBS_XT_ styles to be added during style modification.
		const DWORD& dwAdd,
		// 'true' to redraw the Outlook bar.
		const bool bRedraw = false);

	// Call this member function to get the current style set for the Outlook bar.
	// Returns a DWORD value representing the current style of the Outlook bar.
	//
	virtual DWORD GetFlag() const;

	// Call this member function to set the size of the icons displayed in the 
	// Outlook bar control for the specified folder.
	//
	virtual void SetSmallIconView(
		// TRUE to display small icons, or FALSE to display large icons.
		const BOOL bSet,
		// Index of the folder to set the icon size for.  If -1, all folder icons
		// are set.
		const int iFolder=-1);

	// Call this member function to return the current state of the icon display
	// for the Outlook bar control.  Returns TRUE if small icons are displayed, and
	// FALSE if large icons are displayed.
	//
	virtual BOOL IsSmallIconView(
		// Index of the folder to check.  If -1, the currently selected folder
		// is checked.
		const int iFolder=-1) const;

	// Call this member function to create the Outlook bar control.  Returns
	// TRUE if successful, otherwise returns FALSE.
	//
	virtual BOOL Create(
		// Style for the Outlook bar. It usually includes the WS_CHILD|WS_VISIBLE
		// flags.
		DWORD dwStyle,
		// Size of the Outlook bar.
		const RECT& rect,
		// Parent of the control.
		CWnd* pParentWnd,
		// Identifier of the Outlook bar control.
		UINT nID,
		// Specifies the style flags for the control.  See ModifyFlag for a list
		// of available styles.
		const DWORD dwFlag = OBS_XT_DEFAULT);

	// Call this member function to create the Outlook bar control.  Returns
	// TRUE if successful, otherwise returns FALSE.
	//
	virtual BOOL CreateEx(
		// Extended style for the Outlook bar such as WS_EX_STATICEDGE. It can
		// be NULL.
		DWORD dwExStyle,
		// Style for the Outlook bar. It usually includes the WS_CHILD|WS_VISIBLE
		// flags.
		DWORD dwStyle,
		// Size of the Outlook bar.
		const RECT& rect,
		// Parent of the control.
		CWnd* pParentWnd,
		// Identifier of the Outlook bar control.
		UINT nID,
		// Specifies the style flags for the control.  See ModifyFlag for a list
		// of available styles.
		const DWORD dwFlag = OBS_XT_DEFAULT);

	// Call this member function to set the font used by the Outlook bar control.
	//
	virtual void SetFontX(
		// Points to the font to be used by the Outlook bar.
		CFont* pFont);

	// Call this member function to retrieve the font used by the Outlook
	// bar control. Returns a pointer to a CFont object representing the font
	// used by the Outlook bar control.
	//
	virtual CFont* GetFontX();

	// Call this member function to return a pointer to the CXTBarFolder data
	// that is associated with the specified folder.  Returns a pointer to
	// a CXTBarFolder object.
	//
	virtual CXTBarFolder* GetBarFolder(
		// Index of the folder to retrieve.
		const int iFolder);

	// Call this member function to return a pointer to the CXTBarItem data
	// that is associated with the specified folder and item.  Returns a pointer
	// to a CXTBarItem object.
	//
	virtual CXTBarItem* GetBarFolderItem(
		// Index of the folder to retrieve.
		const int iFolder,
		// Index of the item to retrieve.
		const int iIndex);

	// Call this member function to set the background color for the Outlook
	// bar control.
	//
	virtual void SetBackColor(
		// An RGB value that represents the background color.
		COLORREF clrBack);

	// Call this member function to set the text color for items in the Outlook
	// bar control.
	//
	virtual void SetTextColor(
		// An RGB value that represents the text item color.
		COLORREF clrText);

	// Call this member function to retrieve the size of the label for the
	// specified item.
	//
	virtual void GetLabelRect(
		// Index of the folder where the item is located.
		const int iFolder,
		// Index of the item.
		const int iIndex,
		// Address of a CRect object that will receive the label size.
		CRect& rect);

	// Call this member function to retrieve the size of the icon for the
	// specified item.
	//
	virtual void GetIconRect(
		// Index of the folder where the item is located.
		const int iFolder,
		// Index of the item.
		const int iIndex,
		// Address of a CRect object that will receive the icon size.
		CRect& rect);

	// Call this member function to retrieve the size of the client area for
	// the Outlook bar.  This is the inside area that contains the folders.
	//
	virtual void GetInsideRect(
		// Address of a CRect object that will receive the size.
		CRect& rect) const;

	// Call this member function to retrieve the size of the specified item.
	// The size includes the area occupied by the item's label and icon.
	//
	virtual void GetItemRect(
		// Index of the folder where the item is located.
		const int iFolder,
		// Index of the item.
		const int iIndex,
		// Address of a CRect object that will receive the item size.
		CRect& rect);

	// Call this member function to retrieve the size of the specified folder.
	// Returns TRUE if successful, otherwise returns FALSE.
	//
	virtual BOOL GetFolderRect(
		// Index of the folder item.
		const int iIndex,
		// Address of a CRect object that will receive the folder size.
		CRect& rect) const;

	// This member function will return the index of the currently selected
	// item for the currently selected folder. Returns an integer value representing
	// the index of the selected item.
	//
    inline int GetCurSel();

	// This member function will set the currently selected item for the currently
	// selected folder.
	//
    inline void SetCurSel(
		// Index of the item to select.
		int iItem,
		// 'true' if the item is to be pressed when selected.
		const BOOL bPressed=false);

	// Call this member function to enable or disable a folder item.
	//
	void EnableItem(
		// Index of the folder that owns the item.
		int iFolder,
		// Index of the item to enable or disable.
		int iItem,
		// 'true' to enable item, 'false' to disable.
		bool bEnable);
	
protected:

	//:Ignore
	virtual void DrawScrollButton(CDC* pDC, CRect rect, UINT uType, UINT uState);
	virtual void DrawItem(CDC* pDC, const int iFolder, CRect rc, const int iIndex, const BOOL bOnlyImage = false);
	virtual void DrawDragArrow(CDC* pDC, const int iFrom, const int iTo);
	virtual void DrawAnimItem(const int iOffsetX, const int iOffsetY, const int iIndex);
	virtual void DrawFolder(CDC* pDC, const int iIndex, CRect rect, const FOLDER_HILIGHT eHilight);
    virtual void DrawIcon(CDC* pDC, int iIcon, int iFolder, bool bHilight);
	virtual void PaintItems(CDC* pDC, const int iFolder, CRect rc);
	virtual void GetVisibleRange(const int iFolder, int& iFirst, int& iLast);
	virtual int GetDragItemRect(const int iIndex, CRect& rect);
	virtual CSize GetItemSize(const int iFolder, const int iIndex, const int iType);
	virtual void AnimateFolderScroll(const int iFrom, const int iTo);
	virtual void HighlightItem(const int iIndex, const BOOL bPressed = false);
	virtual void HighlightFolder(const int iIndex);
	virtual int HitTestEx(const CPoint& point, int& iIndex);
	virtual BOOL IsValidItem(const int iIndex) const;
	virtual void EndLabelEdit(CXTEditItem* pEdit, bool bIsFolder);
	virtual void OnLabelChanged (const XT_OUTBAR_INFO* pObi);
	virtual void DrawItemIcon( CDC* pDC, CPoint pt, CXTIconHandle hIcon, BOOL bEnabled );
	virtual void DrawItemText( CDC* pDC, CRect rc, CString strText, UINT nFormat, BOOL bEnabled );
    //:End Ignore

	//:Ignore
    //{{AFX_VIRTUAL(CXTOutBarCtrl)
	//}}AFX_VIRTUAL
    //:End Ignore

	//:Ignore
    //{{AFX_MSG(CXTOutBarCtrl)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLargeIcon();
	afx_msg void OnUpdateLargeIcon(CCmdUI* pCmdUI);
	afx_msg void OnSmallIcon();
	afx_msg void OnUpdateSmallIcon(CCmdUI* pCmdUI);
	afx_msg void OnRemoveItem();
	afx_msg void OnUpdateRemoveItem(CCmdUI* pCmdUI);
	afx_msg void OnRenameItem();
	afx_msg void OnUpdateRenameItem(CCmdUI* pCmdUI);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
    //:End Ignore
	
	DECLARE_MESSAGE_MAP()

	friend class CXTEditItem;
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTOutBarCtrl::GetCurSel() {
	return m_nItemHilighted;
}
AFX_INLINE void CXTOutBarCtrl::SetCurSel(int iItem, const BOOL bPressed/*=false*/) {
    HighlightItem(iItem, bPressed);
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTBarItem is a stand alone helper class.  It is used by the CXTOutBarCtrl
// to maintain information about each folder item specified in the Outlook
// bar control.
//
class _XT_EXT_CLASS CXTBarItem
{
public:

    // Constructs a CXTBarItem object.
	//
	CXTBarItem(
		// A NULL terminated string that represents the item name.
        LPCTSTR lpszName,
		// An index into the folders image list.
        const int nImageIndex,
		// User item data (lParam).
        DWORD dwData);

	// Destroys a CXTBarItem object, handles cleanup and de-allocation.
	//
    virtual ~CXTBarItem();

	// Call this member function to retrieve the zero-based index of the folder
	// item. Returns an integer value that represents the zero-based index of
	// the folder item.
	//
	int GetIndex();

	// Call this member function to set the zero-based index of the folder item.
	//
	void SetIndex(
		// New index of the folder item.
		int iIndex);

	// Call this member function to return the user specified item data (lParam)
	// for the folder item.  Returns a DWORD value that represents the user data.
	//
	DWORD GetData();

	// Call this member function to set a user data (lParam) value for the
	// folder item.
	//
	void SetData(
		// Specifies the user data (lparam) value to be associated with the folder
		// item.
		DWORD dwData);

	// Call this member function to retrieve the label of the folder item. 
	// Returns a CString object that contains the folder item label.
	//
	CString GetName();

	// Call this member function to set the label of the folder item.
	//
	void SetName(
		// A NULL terminated string that represents the item label.
		CString strName);

	// Call this member function to set the items selected state.
	//
	void SelectItem(
		// 'true' to set the item state to selected.
		bool bSelected);

	// Call this member function to see if the item is selected. Returns 'true'
	// if the item is selected, otherwise returns 'false'.
	//
	bool IsSelected();

	// Call this member function to toggle the enabled state for the outlook
	// bar item.
	//
	void EnableItem(
		// 'true' to enable, 'false' to disable.
		bool bEnable);

	// Call this member function to determine if the item is enabled.  Returns 'true'
	// if the item is enabled, otherwise returns 'false'.
	//
	bool IsEnabled();

protected:

	//:Ignore
	// true if the icon is enabled.
	bool m_bEnabled;
	// used internally
	bool m_bSelected;
	// used internally
	int m_nIndex;
	// used internally
	DWORD m_dwData;
	// used internally
	CString m_strName;
    //:End Ignore

	friend class CXTOutBarCtrl;
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTBarItem::SelectItem(bool bSelected) {
	m_bSelected = bSelected;
}
AFX_INLINE bool CXTBarItem::IsSelected() {
	return m_bSelected;
}
AFX_INLINE void CXTBarItem::EnableItem(bool bEnable) {
	m_bEnabled = bEnable;
}
AFX_INLINE bool CXTBarItem::IsEnabled() {
	return m_bEnabled;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTBarFolder is a stand alone helper class.  It is used by the CXTOutBarCtrl
// to maintain information about each folder specified in the Outlook bar
// control.
//
class _XT_EXT_CLASS CXTBarFolder
{
public:

    // Constructs a CXTBarFolder object.
	//
	CXTBarFolder(
		// A NULL terminated string that represents the item name.
        LPCTSTR lpszName,
		// User item data (lParam).
        DWORD dwData);

	// Destroys a CXTBarFolder object, handles cleanup and de-allocation.
	//
    virtual ~CXTBarFolder();

	// Call this member function to get the number of folders found in the
	// Outlook bar. Returns the number of folders in the Outlook bar.
	//
	int GetItemCount();

	// Call this member function to insert an item into the Outlook bar folder.
	// Returns an integer value.
	//
	int	InsertItem(
		// Index of the item.
		int iIndex,
		// A NULL terminated string that represents the label of the item.
		LPCTSTR lpszName,
		// Index into the folder's image list.
		const int nImage,
		// User item data (lParam).
		const DWORD dwData);

	// Call this member function to retrieve the specified item.  Returns
	// a pointer to a CXTBarItem object.
	//
	CXTBarItem* GetItemAt(
		// Index of the item to retrieve.
		int iIndex);

	// Call this member function to insert a folder item into the location
	// specified by 'iIndex'.
	//
	void InsertItemAt(
		// Zero-based index of where to insert the new item.
		int iIndex,
		// Points to a valid CXTBarItem object.
		CXTBarItem* pBarItem);

	// Call this member function to remove the specified item from the folder.
	// Returns a pointer to the removed item.
	//
	CXTBarItem* RemoveItemAt(
		// Index of the item to remove.
		int iIndex);

	// Call this member function to return the label of the folder item. 
	// Returns a CString object.
	//
	CString GetName();

	// Call this member function to set the label for the folder item.
	//
	void SetName(
		// A NULL terminated string that represents the folder's new label.
		CString strName);

	// Call this member function to return the user item data (lParam) for
	// the folder. Returns a DWORD value that represents the item data.
	//
	DWORD GetData();

	// Call this member function to set the user item data (lParam) for the
	// folder.
	//
	void SetData(
		// User item data (lParam).
		DWORD dwData);

	// Call this member function to return a pointer to the large image list
	// for the folder.  Returns a pointer to CImageList object if successful,
	// otherwise returns NULL.
	//
	CImageList* GetLargeImageList();

	// Call this member function to set the large image list for the folder.
	//
	void SetLargeImageList(
		// Points to a CImageList object.
		CImageList* pLargeList);

	// Call this member function to return a pointer to the small image list
	// for the folder.  Returns a CImageList object if successful, otherwise
	// returns NULL.
	//
	CImageList* GetSmallImageList();

	// Call this member function to set the small image list for the folder.
	//
	void SetSmallImageList(
		// Points to a CImageList object.
		CImageList* pSmallList);

	// Call this member function to retrieve a CWnd pointer to the child object
	// that is associated with this folder item.  Returns a CWnd pointer to
	// the child associated with the folder, or NULL if no objects were found.
	//
	CWnd* GetChild();

	// Call this member function to set the CWnd child to be associated with this
	// folder item.
	//
	void SetChild(
		// Points to a valid CWnd object.
		CWnd* pChild);

	// Call this member function to set the currently selected item for the folder.
	//
	void SetSelItem(
		// Index of the item to select
		int iItem);

	// Call this member function to return a pointer to the currently selected
	// item. Returns a pointer to a CXTBarItem object if successful, otherwise
	// returns NULL.
	//
	CXTBarItem* GetSelItem();

	// Call this member function to return the index for the currently selected
	// item. Returns the index of the currently selected item if successful,
	// otherwise returns -1.
	//
	int GetSelIndex();

protected:

	//:Ignore
    // used internally
	CString m_strName;
    // used internally
	DWORD m_dwData;
    // used internally
	CImageList* m_pLargeList;
    // used internally
	CImageList* m_pSmallList;
    // used internally
	CWnd* m_pChild;
	// used internally
	BOOL m_bSmallIcons;
    // used internally
	CList<CXTBarItem*, CXTBarItem*> m_barItems;
    //:End Ignore

	friend class CXTOutBarCtrl;
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTOutBarCtrl::SetFontX(CFont* pFont) {
	ASSERT_VALID(pFont); LOGFONT lf; pFont->GetLogFont(&lf); m_font.DeleteObject(); m_font.CreateFontIndirect(&lf);
}
AFX_INLINE CFont* CXTOutBarCtrl::GetFontX() {
	return &m_font;
}
AFX_INLINE void CXTOutBarCtrl::SetAnimationTickCount(const long lValue) {
	m_nAnimationTickCount = lValue;
}
AFX_INLINE int CXTOutBarCtrl::GetAnimationTickCount() {
	return m_nAnimationTickCount;
}
AFX_INLINE CXTBarFolder* CXTOutBarCtrl::GetBarFolder(const int iFolder) {
	return (CXTBarFolder*)m_arFolder.GetAt(iFolder);
}
AFX_INLINE CXTBarItem* CXTOutBarCtrl::GetBarFolderItem(const int iFolder, const int iIndex) {
	return GetBarFolder(iFolder)->GetItemAt(iIndex);
}
AFX_INLINE void CXTOutBarCtrl::SetBackColor(COLORREF clrBack) {
    m_bUserClrBack = TRUE;
	m_clrBack = clrBack;
}
AFX_INLINE void CXTOutBarCtrl::SetTextColor(COLORREF clrText) {
    m_bUserClrText = TRUE;
	m_clrText = clrText;
}

//////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTBarItem::GetIndex() {
	return m_nIndex;
}
AFX_INLINE void CXTBarItem::SetIndex(int iIndex) {
	m_nIndex = iIndex;
}
AFX_INLINE DWORD CXTBarItem::GetData() {
	return m_dwData;
}
AFX_INLINE void CXTBarItem::SetData(DWORD dwData) {
	m_dwData = dwData;
}
AFX_INLINE CString CXTBarItem::GetName() {
	return m_strName;
}
AFX_INLINE void CXTBarItem::SetName(CString strName) {
	m_strName = strName;
}

//////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTBarFolder::GetItemCount() {
	return (int)m_barItems.GetCount();
}
AFX_INLINE CString CXTBarFolder::GetName() {
	return m_strName;
}
AFX_INLINE void CXTBarFolder::SetName(CString strName) {
	m_strName = strName;
}
AFX_INLINE DWORD CXTBarFolder::GetData() {
	return m_dwData;
}
AFX_INLINE void CXTBarFolder::SetData(DWORD dwData) {
	m_dwData = dwData;
}
AFX_INLINE CImageList* CXTBarFolder::GetLargeImageList() {
	return m_pLargeList;
}
AFX_INLINE void CXTBarFolder::SetLargeImageList(CImageList* pLargeList) {
	m_pLargeList = pLargeList;
}
AFX_INLINE CImageList* CXTBarFolder::GetSmallImageList() {
	return m_pSmallList;
}
AFX_INLINE void CXTBarFolder::SetSmallImageList(CImageList* pSmallList) {
	m_pSmallList = pSmallList;
}
AFX_INLINE CWnd* CXTBarFolder::GetChild() {
	return m_pChild;
}
AFX_INLINE void CXTBarFolder::SetChild(CWnd* pChild) {
	m_pChild = pChild;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTEditItem is a helper class derived from CXTEdit.  It is used by 
// CXTOutBarCtrl, an Outlook like control, to create an in-place edit box
// that is used to modify a folder or an item within the control.
//
class _XT_EXT_CLASS CXTEditItem : public CXTEdit
{
	DECLARE_DYNAMIC(CXTEditItem)

public:

    // Constructs a CXTEditItem object.
	//
	CXTEditItem();

	// Destroys a CXTEditItem object, handles cleanup and de-allocation.
	//
    virtual ~CXTEditItem();

protected:

	// Index of the folder or item.
	int m_iIndex;
	// 'true' if the escape key was pressed.
	bool m_bEscapeKey;
	// 'true' if the folder is using small icons.
	bool m_bSmallIcons;
	// 'true' if the edit box is for a folder.
	bool m_bIsFolder;
	// Original size of the edit box when it was first created.
	CRect m_rcOriginal;
	// Original string of the edit box when it was first created, and the new text
	// on edit completion.
	CString m_strText;
	// Parent Outlook bar control.
	CXTOutBarCtrl* m_pParentWnd;

public:
	
	// Call this member function to return the index of the folder or item
	// currently being edited. Returns an integer value that represents the index
	// of the folder or item.
	//
	int GetIndex() const;

	// Call this member function to return the text associated with this edit
	// control.  Returns a CString object that represents the text originally
	// set for the edit control.  If called after the edit is destroyed, it
	// returns the modified text value.
	//
	CString GetText() const;

	// The Outlook bar control calls this member to create an in-place edit
	// control. Returns TRUE if successful, otherwise returns FALSE.
	//
	virtual BOOL Create(
		// NULL terminated string to be displayed in the edit control.
		LPCTSTR lpszText,
		// Window style for the edit control.
		DWORD dwStyle,
		// Size of the edit control.
		const RECT& rect,
		// Owner window.
		CWnd* pParentWnd,
		// Control ID.
		UINT nID,
		// Folder or item index.
		UINT nIndex,
		// 'true' if the edit is for a folder item.
		bool bIsFolder,
		// 'true' if the parent folder is displaying small icons.
		bool bSmallIcons);

	//:Ignore
    //{{AFX_VIRTUAL(CXTEditItem)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
    //:End Ignore

protected:

	//:Ignore
    //{{AFX_MSG(CXTEditItem)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
    //:End Ignore

	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTEditItem::GetIndex() const {
	return m_iIndex;
}
AFX_INLINE CString CXTEditItem::GetText() const {
	return m_strText;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTToolBox is a CXTOutBarCtrl derived class.  It is used to create a
// toolbox control similar to the toolbox control seen in VisualStudio.NET.
//
class _XT_EXT_CLASS CXTToolBox : public CXTOutBarCtrl
{
	DECLARE_DYNCREATE(CXTToolBox)

public:
    
    // Constructs a CXTToolBox object.
	//
	CXTToolBox();
    
	// Destroys a CXTToolBox object, handles cleanup and de-allocation.
	//
	virtual ~CXTToolBox();

protected:

	// Index of the first visible item.
	int m_iFirst;
	// Index of the last visible item.
	int m_iLast;
	// 'true' if the folder selection is changing.
	bool m_bAnimating;

public:

	// Call this member function to set the selection for the item specified
	// by 'iItem'.
	//
	void SetSelItem(
		// Index of the item to select.
		int iItem,
		// Index of the folder the item belongs to.
		int iFolder);

	// Call this member function to create the toolbox control.  Returns TRUE
	// if successful, otherwise returns FALSE.
	//
	virtual BOOL CreateEx(
		// Extended style for the toolbox, such as WS_EX_STATICEDGE. It can be
		// NULL.
		DWORD dwExStyle,
		// Style for the toolbox. It usually includes the WS_CHILD|WS_VISIBLE
		// flags.
		DWORD dwStyle,
		// Size of the toolbox.
		const RECT& rect,
		// Parent of the control.
		CWnd* pParentWnd,
		// Identifier of the toolbox control.
		UINT nID,
		// Specifies the style flags for the control.  See CXTOutBarCtrl::ModifyFlag
		// for a list of available styles.
		const DWORD dwFlag = OBS_XT_DEFAULT);

	// Call this member function to retrieve the size of the icon for the
	// specified item.
	//
	virtual void GetIconRect(
		// Index of the folder where the item is located.
		const int iFolder,
		// Index of the item.
		const int iIndex,
		// Address of a CRect object that will receive the icon size.
		CRect& rect);

	// Call this member function to retrieve the size of the specified item.
	// The size includes the area occupied by the item's label and icon.
	//
	virtual void GetItemRect(
		// Index of the folder where the item is located.
		const int iFolder,
		// Index of the item.
		const int iIndex,
		// Address of a CRect object that will receive the item size.
		CRect& rect);

	// This member function will set the selected folder for the toolbox.
	//
	virtual void SetSelFolder(
		// Index of the new selected folder.
		const int iIndex);

protected:
	//:Ignore
	//{{AFX_VIRTUAL(CXTToolBox)
	//}}AFX_VIRTUAL
    virtual void DrawIcon(CDC* pDC, int iIcon, int iFolder, bool bHilight);
    virtual void DrawItem(CDC* pDC, const int iFolder, CRect rc, const int iIndex, const BOOL bOnlyImage);
	virtual void DrawFolder(CDC* pDC, const int iIndex, CRect rect, const FOLDER_HILIGHT eHilight);
	virtual void DrawScrollButton(CDC* pDC, CRect rect, UINT uType, UINT uState);
    //:End Ignore

	//:Ignore
	//{{AFX_MSG(CXTToolBox)
	afx_msg void OnPaint();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
    //:End Ignore
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTOUTBARCTRL_H__)
