//:Ignore
// XTToolBar.h interface for the CXTToolBar class.
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

#if !defined(__XTTOOLBAR_H__)
#define __XTTOOLBAR_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

// forwards
class CXTColorPopup;
class CXTMenu;
class CXTFrameWnd;
class CXTToolBarCtrl;
class CXTExpButton;
class CXTExpMenuWnd;
class CXTCustTbHandler;
class CXTCustomControlBarInfo;
class CXTCustomGroupItemInfo;
class CXTCustomGroups;
class CXTExpButton;
typedef CTypedPtrArray<CPtrArray, CXTCustomGroupItemInfo*> XT_CUSTOMITEMS;
class CXTIconMap;
struct XT_DROPDOWNBUTTON;

//////////////////////////////////////////////////////////////////////
// XT_HIDDENBUTTON is a stand alone helper structure class.  It is used
// to define, dynamically hidden button info.
//
struct XT_HIDDENBUTTON
{
	// Width of the hidden button.
	int nWidth;
};

//////////////////////////////////////////////////////////////////////
// XT_TBBUTTON is a TBBUTTON helper structure class.  It is used to create
// an XT_TBBUTTON structure object.
//
struct XT_TBBUTTON : public TBBUTTON
{
    // Constructs an XT_TBBUTTON object.
	//
	XT_TBBUTTON();

    // Destroys an XT_TBBUTTON object, handles cleanup and de-allocation.
	//
	~XT_TBBUTTON();

    // Points to a CWnd object that represents the control associated with
	// this button.
	CWnd* pWndCtrl;
    // Points to an XT_DROPDOWNBUTTON object that holds information about
	// the dropdown button.
	XT_DROPDOWNBUTTON* pDDButton;
    // 'true' if this button is the owner of the control or dropdown.
	bool bOwnsButton;
    // 'true' if a bold font is to be set.
    bool bBoldFont;
    // 'true' if the button is hidden.
	bool bHidden;
};

//:Ignore
//////////////////////////////////////////////////////////////////////
AFX_INLINE XT_TBBUTTON::XT_TBBUTTON() {
	bBoldFont = false; bHidden   = false; bOwnsButton = true; pWndCtrl  = NULL; pDDButton = NULL;
}
AFX_INLINE XT_TBBUTTON::~XT_TBBUTTON() {
	if (bOwnsButton) { SAFE_DELETE( pDDButton ); }
}
//////////////////////////////////////////////////////////////////////
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTToolBar is a CXTControlBar derived class.  Objects of the class CXTToolBar
// are control bars that have a row of bitmapped buttons and optional separators.
// The buttons can act like pushbuttons, check-box buttons, or radio buttons.
// CXTToolBar objects are usually embedded members of frame-window objects
// derived from the class CFrameWnd or CMDIFrameWnd.
//
// CXTToolBar::GetToolBarCtrl, allows you to take advantage of the Windows
// common control's support for toolbar customization and additional functionality.
// CXTToolBar member functions give you most of the functionality of the
// Windows common controls.  However, when you call GetToolBarCtrl, you
// can give your toolbars even more of the characteristics of Windows 95
// toolbars. When you call GetToolBarCtrl, it will return a reference to
// a CXTToolBarCtrl object. See CXTToolBarCtrl for more information about
// designing toolbars using Windows common controls.  For more general
// information about common controls, see Common Controls in the Windows
// 95 SDK Programmer’s Reference.
//
// Visual C++ provides you with two methods to create a toolbar.  To create
// a toolbar resource using the Resource Editor, follow these steps: 
// [ol]
// [li]Create a toolbar resource.[/li]
// [li]Construct the CXTToolBar object.[/li]
// [li]Call the Create, or CreateEx, function to create the Windows toolbar
// and attach it to the CXTToolBar object.[/li]
// [li]Call LoadToolBar to load the toolbar resource.[/li]
// [/ol]
// Otherwise, follow these steps: 
// [ol]
// [li]Construct the CXTToolBar object.[/li]
// [li]Call the Create, or CreateEx, function to create the Windows toolbar
// and attach it to the CXTToolBar object.[/li]
// [li]Call LoadBitmap to load the bitmap that contains the toolbar button
// images.[/li]
// [li]Call SetButtons to set the button style and associate each button
// with an image in the bitmap.[/li]
// [/ol]
// All the button images in the toolbar are taken from one bitmap, which
// must contain one image for each button.  All images must be the same
// size.  The default is 16 pixels wide and 15 pixels high.  Images must
// be side by side in the bitmap.
//
// The SetButtons function takes a pointer to an array of control IDs and
// an integer that specifies the number of elements in the array.  The
// function sets each button’s ID to the value of the corresponding element
// of the array, and assigns each button an image index, which specifies
// the position of the button’s image in the bitmap.  If an array element
// has the value ID_SEPARATOR, no image index is assigned.
//
// The order of the images in the bitmap is typically the order in which
// they are drawn on the screen, but you can use the SetButtonInfo function
// to change the relationship between image order and drawing order.
//
// All buttons in a toolbar are the same size.  The default is 24 x 22
// pixels, in accordance with Windows Interface Guidelines for Software
// Design.  Any additional space between the image and button dimensions
// is used to form a border around the image.
//
// Each button has one image.  The various button states and styles (pressed,
// up, down, disabled, disabled down, and indeterminate) are generated
// from that one image.  Although bitmaps can be any color, you can achieve
// the best results with images in black and shades of gray.
//
// Toolbar buttons imitate pushbuttons by default.  However, toolbar buttons
// can also imitate check-box buttons or radio buttons.  Check-box buttons
// have three states: checked, cleared, and indeterminate.  Radio buttons
// have only two states: checked and cleared.
//
// To set an individual button or separator style without pointing to an
// array, call GetButtonStyle to retrieve the style, and then call SetButtonStyle
// instead of SetButtons.  SetButtonStyle is most useful when you want
// to change a button’s style at run time.
//
// To assign text to appear on a button, call GetButtonText to retrieve
// the text to appear on the button, and then call SetButtonText to set
// the text. 
//
// To create a check-box button, assign it the style TBBS_CHECKBOX or use
// a CCmdUI object’s SetCheck member function in an ON_UPDATE_COMMAND_UI
// handler.  Calling SetCheck turns a pushbutton into a check-box button.
// Pass SetCheck an argument of 0 for unchecked, 1 for checked, or 2 for
// indeterminate.
//
// To create a radio button, call a CCmdUI object’s SetRadio member function
// from an ON_UPDATE_COMMAND_UI handler.  Pass SetRadio an argument of
// zero for unchecked or nonzero for checked.  In order to provide a radio
// group’s mutually exclusive behavior, you must have ON_UPDATE_COMMAND_UI
// handlers for all of the buttons in the group.
//
class _XT_EXT_CLASS CXTToolBar : public CXTControlBar
{

	DECLARE_DYNAMIC(CXTToolBar)

public:
	
    // Constructs a CXTToolBar object.
    //
	CXTToolBar();

	// Destroys a CXTToolBar object, handles cleanup and de-allocation.
    //
    virtual ~CXTToolBar();

protected:

	// Contains the toolbar button array.
	CList<XT_TBBUTTON, XT_TBBUTTON&> m_listTBButtons;
	// Dynamically hidden buttons.
	CMap<int, int, XT_HIDDENBUTTON, XT_HIDDENBUTTON&> m_mapHiddenButtons;
	// Toolbar button text to command map.
	CMap<int, int, CString, CString&> m_mapBtnTextCmd;
	// array of popup item indices that currently have its popup dropped 
	CArray<int, int> m_arrPopupDropped;
    // Current image size.
	CSize m_sizeImage;
	// Current button size.
	CSize m_sizeButton;
	// Registry name for the subkey.
	CString m_strSubKey;
	// Registry name for the value.
	CString m_strValueName;
	// Used to manage when the button layout should be done.
	BOOL m_bDelayedButtonLayout;
	// Handle to the loaded resource for the image well.
	HRSRC m_hRsrcImageWell;
	// Instance handle to load the image well from.
	HINSTANCE m_hInstImageWell;
	// Contains color mapped button images.
	HBITMAP m_hbmImageWell;
	// Used as CMapStringToUInt.
	CMapStringToPtr* m_pStringMap;
	// Used with a rebar for window updates.
	BOOL m_bNoEntry;
	// TRUE to draw a disabled button in color.
	BOOL m_bDisabledColor;
	// Pointer to a color popup window.
	CXTColorPopup* m_pColorPopup;
	// Last command ID used.
	UINT m_uiLastCmd;
	// DrawText custom flags mask.
	DWORD m_dwDrawTextMask;
	// DrawText custom flags.
	DWORD m_dwDrawTextFlags;
	
//:Ignore
private:
	CXTExpButton* m_pExpButton;
	CXTCustTbHandler* m_pCustomHandler;
	DWORD m_dwSignatureFreshenAfter;
	DWORD m_dwRestoreSignature;
	DWORD m_dwInitialCRC;
	LPVOID m_pvOriginalState;
	DWORD  m_cbOriginalState;
    bool m_bInvalidateOnMouseUp;
	bool m_bOwnsDropDowns;
	// TRUE to enable toolbar customization.
	bool m_bCustomize;
	// tells if toolbar customization is currently on
	bool m_bAdjusting;  
	bool m_bRestoringState;
	bool m_bFirstDrawPass;
	bool m_bForcedListStyle;
	// true to create text for commands drooped on this toolbar during toolbar
	// customization
	bool m_bDropCommandText;
	bool m_bIdealSizeChanged;
//:End Ignore

public:

	// This member function is used to set the command ID for the dropped menu
	// item.  This is the command ID of the toolbar button that was pressed to
	// display the popup window.
	//
	void SetDroppedItem(
		// Command ID of the button pressed.
		int iDroppedItem, 
		// tells if this item is currently dropped down
		bool bDropped);

    // This member function creates a Windows toolbar, a child window,
	// and associates it with the CXTToolBar object.  It also sets the toolbar
	// height to a default value. Returns nonzero if successful, otherwise returns zero.
    // 
    virtual BOOL Create(
		// Pointer to the window that is the toolbar’s parent.
        CWnd* pParentWnd,
		// The toolbar style.  Additional toolbar styles supported are: 
        //[ul]
        //[li][b]CBRS_TOP[/b] - Control bar is at the top of the frame window.[/li]
        //[li][b]CBRS_BOTTOM[/b] - Control bar is at the bottom of the frame
		//       window.[/li]
        //[li][b]CBRS_NOALIGN[/b] - Control bar is not repositioned when
		//       the parent is resized.[/li]
        //[li][b]CBRS_TOOLTIPS[/b] - Control bar displays tool tips.[/li]
        //[li][b]CBRS_SIZE_DYNAMIC[/b] - Control bar is dynamic.[/li]
        //[li][b]CBRS_SIZE_FIXED[/b] - Control bar is fixed.[/li]
        //[li][b]CBRS_FLOATING[/b] - Control bar is floating.[/li]
        //[li][b]CBRS_FLYBY[/b] - Status bar displays information about
		//       the button.[/li]
        //[li][b]CBRS_HIDE_INPLACE[/b] - Control bar is not displayed to
		//       the user.[/li]
        //[/ul]
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP,
		// The toolbar’s child-window ID.
		UINT nID = AFX_IDW_TOOLBAR);

    // Call this function to create a Windows toolbar, a child window,
	// and associate it with the CXTToolBar object.  It also sets the toolbar
	// height to a default value.
    //
    // Use CreateEx, instead of Create, when certain styles need to be
	// present during the creation of the embedded toolbar control.  For
	// example, set 'dwCtrlStyle' to TBSTYLE_FLAT | TBSTYLE_TRANSPARENT to create
	// a toolbar that resembles the Internet Explorer 4 toolbars. Returns
	// nonzero if successful, otherwise returns zero.
    // 
    virtual BOOL CreateEx(
		// Pointer to the window that is the toolbar’s parent.
        CWnd* pParentWnd,
		// Additional styles for the creation of the embedded CXTToolBarCtrl
		// object.  By default, this value is set to TBSTYLE_FLAT.  For a complete
		// list of toolbar styles, see 'dwStyle'.
        DWORD dwCtrlStyle = TBSTYLE_FLAT,
		// The toolbar style.  See Toolbar Control and Button Styles in the Platform
		// SDK for a list of appropriate styles.
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,
		// A CRect object that defines the widths of the toolbar window borders.
		// These borders are set to (0,0,0,0) by default, resulting in a toolbar
		// window with no borders.
		CRect rcBorders = CRect(0, 0, 0, 0),
		// The toolbar’s child-window ID.
		UINT nID = AFX_IDW_TOOLBAR);

    // Call this member function to set the toolbar's buttons to the size,
	// in pixels, specified in 'sizeButton'.  The 'sizeImage' parameter must contain
	// the size, in pixels, of the images in the toolbar's bitmap.  The dimensions
	// in 'sizeButton' must be sufficient to hold the image plus 7 pixels extra
	// in width and 6 pixels extra in height.  This function also sets the
	// toolbar height to fit the buttons.
    //
    // Only call this member function for toolbars that do not follow Windows
	// Interface Guidelines for Software Design recommendations for button
	// and image sizes.
    // 
    void SetSizes(
		// The size, in pixels, of each button.
        SIZE sizeButton,
		// The size, in pixels, of each image.
        SIZE sizeImage);

    // This member function retrieves the current image size and button size.
	//
    void GetSizes(
		// The size, in pixels, of each button.
        SIZE& sizeButton,
		// The size, in pixels, of each image.
        SIZE& sizeImage);

    // This member function sets the toolbar's height to the value, in
	// pixels, specified in 'cyHeight'.
    //
    // After calling SetSizes, use this member function to override the
	// standard toolbar height. If the height is too small, the buttons will
	// be clipped at the bottom.
    //
    // If this function is not called, the framework uses the size of the
	// button to determine the toolbar height.
    // 
    void SetHeight(
		// The height, in pixels, of the toolbar.
        int cyHeight);

    // Call this member function to load the toolbar specified by 'lpszResourceName'.
	// See toolbar editor in the MSDN Visual C++ User’s Guide for more information
	// about creating a toolbar resource. Returns nonzero if successful, otherwise
	// returns zero.
    // 
    BOOL LoadToolBar(
		// Pointer to the resource name of the toolbar to be loaded.
        LPCTSTR lpszResourceName);

    // Call this member function to load the toolbar specified by 'nIDResource'.
	// See toolbar editor in the MSDN Visual C++ User’s Guide for more information
	// about creating a toolbar resource. Returns nonzero if successful, otherwise
	// returns zero.
    // 
    BOOL LoadToolBar(
		// Resource ID of the toolbar to be loaded.
        UINT nIDResource);

    // Call this member function to load the bitmap specified by 'lpszResourceName'.
	// The bitmap should contain one image for each toolbar button.  If the
	// images are not of the standard size (16 pixels wide and 15 pixels high),
	// call SetSizes to set the button sizes and their images. Returns nonzero
	// if successful, otherwise returns zero.
    // 
    BOOL LoadBitmap(
		// Pointer to the resource name of the bitmap to be loaded.
        LPCTSTR lpszResourceName);

    // Call this member function to load the bitmap specified by 'nIDResource'.
	// The bitmap should contain one image for each toolbar button.  If the
	// images are not of the standard size (16 pixels wide and 15 pixels high),
	// call SetSizes to set the button sizes and their images. Returns nonzero
	// if successful, otherwise returns zero.
    // 
    BOOL LoadBitmap(
		// Resource ID of the bitmap to be loaded.
        UINT nIDResource);

    // Call this member function to set the bitmap image for the toolbar.
	// For example, call SetBitmap to change the bitmapped image after the
	// user takes an action on a document that changes the action of a button.
    // Returns nonzero if successful, otherwise returns zero.
    // 
    BOOL SetBitmap(
		// Handle of a bitmap image that is associated with a toolbar.
        HBITMAP hbmImageWell);

    // This member function sets each toolbar button's command ID to the
	// value specified by the corresponding element of the array 'lpIDArray'.
	// If an element of the array has the value ID_SEPARATOR, a separator
	// is created in the corresponding position of the toolbar.  This function
	// also sets each button's style to TBBS_BUTTON and each separator's style
	// to TBBS_SEPARATOR, and assigns an image index to each button.  The
	// image index specifies the position of the button’s image within the
	// bitmap.
    //
    // You do not need to account for separators in the bitmap because
	// this function does not assign image indexes for separators.  If your
	// toolbar has buttons at positions 0, 1, and 3 and a separator at position
	// 2, the images at positions 0, 1, and 2 in your bitmap are assigned
	// to the buttons at positions 0, 1, and 3, respectively.
    //
    // If 'lpIDArray' is NULL, this function allocates space for the number
	// of items specified by 'nIDCount'.  Use SetButtonInfo to set each item’s
	// attributes. Returns nonzero if successful, otherwise returns zero.
    // 
    BOOL SetButtons(
		// Pointer to an array of command IDs.  It can be NULL to allocate empty
		// buttons.
        const UINT* lpIDArray,
		// Number of elements in the array pointed to by 'lpIDArray'.
        int nIDCount);

    // This member function returns the index of the first toolbar button,
	// starting at position 0, whose command ID matches 'nIDFind'. Returns the
	// index of the button, or –1, if no button has the given command ID.
    // 
    int CommandToIndex(
		// Command ID of a toolbar button.
        UINT nIDFind) const;

    // This member function retrieves the command ID of the button or separator
	// specified by 'nIndex'.  Separators return ID_SEPARATOR. Returns the command
	// ID of the button or separator specified by 'nIndex'.
    // 
    UINT GetItemID(
		// Index of the item whose ID is to be retrieved.
        int nIndex) const;

    // This member function fills the RECT structure whose address is contained
	// in 'lpRect' with the coordinates of the button or separator specified
	// by 'nIndex'.  Coordinates are in pixels relative to the upper-left corner
	// of the toolbar.
    //
    // Use GetItemRect to get the coordinates of a separator you want to
	// replace with a combo box or other control.
    // 
    virtual void GetItemRect(
		// Index of the item (button or separator) whose rectangle coordinates
		// are to be retrieved.
        int nIndex,
		// Address of the RECT structure that will contain the item’s coordinates.
        LPRECT lpRect) const;

    // Call this member function to retrieve the style of a button or separator
	// on the toolbar.  A button’s style determines how the button appears
	// and how it responds to user input.  See SetButtonStyle for examples
	// of button styles. Returns the style of the button or separator specified
	// by 'nIndex'.
    // 
    UINT GetButtonStyle(
		// The index of the toolbar button or separator style to be retrieved.
        int nIndex) const;

    // Call this member function to set the style of a button or separator,
	// or to group buttons.  A button's style determines how the button appears
	// and how it responds to user input.
    //
    // Before calling SetButtonStyle, call the GetButtonStyle member function
	// to retrieve the button or separator style.
    // 
    void SetButtonStyle(
		// Index of the button or separator whose information is to be set.
        int nIndex,
		// The button style.  The following button styles are supported: 
        //[ul]
        //[li][b]TBBS_BUTTON[/b] - Standard pushbutton (default).[/li]
        //[li][b]TBBS_SEPARATOR[/b] - Separator.[/li]
        //[li][b]TBBS_CHECKBOX[/b] - Auto check-box button.[/li]
        //[li][b]TBBS_GROUP[/b] - Marks the start of a group of buttons.[/li]
        //[li][b]TBBS_CHECKGROUP[/b] - Marks the start of a group of check-box
		//       buttons.[/li]
        //[/ul]
        UINT nStyle);

    // This member function retrieves the control ID, style, and image
	// index of the toolbar button or separator at the location specified
	// by 'nIndex'.  Those values are assigned to the variables referenced by
	// 'nID', 'nStyle', and 'iImage'.  The image index is the position of the image
	// within the bitmap that contains images for all the toolbar buttons.
	// The first image is at position zero.
    //
    // If 'nIndex' specifies a separator, 'iImage' is set to the separator
	// width, in pixels.
    // 
    void GetButtonInfo(
		// Index of the toolbar button or separator whose information is to be
		// retrieved.
        int nIndex,
		// Reference to a UINT that is set to the command ID of the button.
        UINT& nID,
		// Reference to a UINT that is set to the style of the button.
        UINT& nStyle,
		// Reference to an integer that is set to the index of the button’s image
		// within the bitmap.
        int& iImage) const;

    // Call this member function to set the button's command ID, style,
	// and image number. For separators, which have the style TBBS_SEPARATOR,
	// this function sets the separator's width, in pixels, to the value stored
	// in 'iImage'.
    //
    // For information on bitmap images and buttons, see the CXTToolBar
	// Overview and CXTToolBar::LoadBitmap.
	// 
    void SetButtonInfo(
		// Index of the button or separator whose information is to be set.
        int nIndex,
		// The value to which the button’s command ID is set.
        UINT nID,
		// The new button style. The following button styles are supported: 
        //[ul]
        //[li][b]TBBS_BUTTON[/b] - Standard pushbutton (default).[/li]
        //[li][b]TBBS_SEPARATOR[/b] - Separator.[/li]
        //[li][b]TBBS_CHECKBOX[/b] - Auto check-box button.[/li]
        //[li][b]TBBS_GROUP[/b] - Marks the start of a group of buttons.[/li]
        //[li][b]TBBS_CHECKGROUP[/b] - Marks the start of a group of check-box
		//       buttons.[/li]
        //[/ul]
        UINT nStyle,
		// New index for the button’s image within the bitmap.
        int iImage);

    // Call this member function to retrieve the text that appears on a
	// button. Returns a CString object containing the button text.
    // 
    CString GetButtonText(
		// Index of the text to be retrieved.
        int nIndex) const;

    // Call this member function to retrieve the text that appears on a
	// button and fill the CString object with the string text.
    // 
    void GetButtonText(
		// Index of the text to be retrieved.
        int nIndex,
		// A reference to a CString object that will contain the text to be retrieved.
        CString& rString) const;

    // Call this function to set the text on a button. Returns nonzero
	// if successful, otherwise return zero.
    // 
    BOOL SetButtonText(
		// Index of the button whose text is to be set.
        int nIndex,
		// Points to the text to be set on a button.
        LPCTSTR lpszText);

    // This member function allows direct access to the underlying common control.
    //
    // Use GetToolBarCtrl to take advantage of the functionality of the
	// Windows toolbar common control,  and to take advantage of the support
	// CXTToolBarCtrl provides for toolbar customization. 
    //
    // For more information about using common controls, see the article,
	// "Control Topics" in MSDN Visual C++ Programmer’s Guide and "Common
	// Controls" in the Windows 95 SDK Programmer’s Reference. Returns a reference
	// to a CXTToolBarCtrl object.
    // 
    CXTToolBarCtrl& GetToolBarCtrl() const;

    // Call this function to determine whether the specified button, in
	// a toolbar control, is enabled.  Consider calling GetState if you want
	// to retrieve more than one button state. Returns nonzero if the button
	// is enabled, otherwise returns zero.
    // 
    BOOL IsButtonEnabled(
		// Command identifier of the button in the toolbar.
        int nID) const;

    // Call this function to determine whether the specified button, in
	// a toolbar control, is checked.  Consider calling GetState if you want
	// to retrieve more than one button state. Returns nonzero if the button
	// is checked, otherwise returns zero.
    // 
    BOOL IsButtonChecked(
		// Command identifier of the button in the toolbar.
        int nID) const;

    // Call this function to determine whether the specified button, in
	// a toolbar control, is pressed.  Consider calling GetState if you want
	// to retrieve more than one button state. Returns nonzero if the button
	// is pressed, otherwise returns zero.
    // 
    BOOL IsButtonPressed(
		// Command identifier of the button in the toolbar.
        int nID) const;

    // Call this function to determine whether the specified button, in
	// a toolbar control, is hidden.  Consider calling GetState if you want
	// to retrieve more than one button state. Returns nonzero if the button
	// is hidden, otherwise returns zero.
    // 
    BOOL IsButtonHidden(
		// Command identifier of the button in the toolbar.
        int nID) const;

    // Call this function to determine whether the specified button, in
	// a toolbar control, is indeterminate. Indeterminate buttons are displayed
	// grayed, such as the way the bold button on the toolbar of a word processor
	// would look when the text selected contains both bold and regular characters.
	// 
    // Consider calling GetState if you want to retrieve more than one
	// button state. Returns nonzero if the button is indeterminate, otherwise
	// returns zero.
    // 
    BOOL IsButtonIndeterminate(
		// Command identifier of the button in the toolbar.
        int nID) const;

    // Call this function to set the state for the specified button in
	// a toolbar control.
    //
    // This function is especially handy if you want to set more than one
	// of the button states.  To just set one state, use one of the following
	// member functions: EnableButton, CheckButton, HideButton, Indeterminate,
	// or PressButton. Returns nonzero if successful, otherwise returns zero.
    // 
    BOOL SetState(
		// Command identifier of the button in the toolbar.
        int nID,
		// State flags.  It can be a combination of the values listed for button
		// states in CXTToolBar::AddButtons.
        UINT nState);

    // Call this function to retrieve information about the state of the
	// specified button in a toolbar control, such as whether it is enabled,
	// pressed, or checked.
    //
    // This function is especially handy if you want to retrieve more than
	// one of the button states. To just retrieve one state, use one of the
	// following member functions: IsButtonEnabled, IsButtonChecked, IsButtonPressed,
	// IsButtonHidden, or IsButtonIndeterminate. However, the GetState member
	// function is the only way to detect the TBSTATE_WRAP button state. Returns
	// the button state information if successful, or returns –1, otherwise.
	// The button state information can be a combination of the values listed
	// in CXTToolBar::AddButtons.
    // 
    int GetState(
		// Command identifier of the button in the toolbar.
        int nID) const;

    // Call this function to retrieve information about the specified button
	// in a toolbar control. Returns nonzero if successful, otherwise returns
	// zero.
    // 
    BOOL GetButton(
		// Zero-based index of the button for which to retrieve information.
        int nIndex,
		// Address of the TBBUTTON structure that is to receive a copy of the
		// button information. See CXTToolBar::AddButtons for information
		// about the TBBUTTON structure.
        TBBUTTON* pButton) const;

    // Call this function to retrieve a count of the buttons currently
	// in the toolbar control. Returns an integer value that represents the count
	// of the toolbar control buttons.
    // 
    int GetButtonCount() const;

    // This member function retrieves the bounding rectangle for a specified
	// toolbar button. Returns nonzero if successful, or zero otherwise.
    // 
    BOOL GetRect(
		// Button identifier.
        UINT nIdButton,
		// Address of a RECT structure that will receive the bounding rectangle
		// information. 
        RECT& rc) const;

    // Call this function to specify the size of the TBBUTTON structure.
	// If you wanted to store extra data in the TBBUTTON structure, you could
	// either derive a new structure from TBBUTTON, adding the members you
	// needed, or create a new structure that contains a TBBUTTON structure
	// as its first member.  You would then call this function to tell the
	// toolbar control the size of the new structure.
    //
    // See CXTToolBar::AddButtons for more information on the TBBUTTON
	// structure.
    // 
    void SetButtonStructSize(
		// Size, in bytes, of the TBBUTTON structure.
        int nSize);

    // Call this member function to get the size of a toolbar button. Returns
	// a CSize value that contains the width and height values.
    // 
    virtual CSize GetButtonSize(
		// Address of the TBBUTTON structure that is to receive a copy of the
		// button information.
        TBBUTTON* pData,
		// Zero-based index of the button for which to retrieve information.
        int iButton);

    // Call this function to set the size of the buttons in the toolbar
	// control.  The button size must always be at least as large as the bitmap
	// size it encloses.
    // 
    // This function must be called only before adding any bitmaps to the
	// toolbar.  If the application does not explicitly set the button size,
	// it defaults to 24 by 22 pixels. Returns nonzero if successful, otherwise
	// returns zero.
    // 
    BOOL SetButtonSize(
		// Width and height, in pixels, of the buttons.
        CSize size);

    // Call this function to set the size of the actual bitmapped images
	// to be added to a toolbar control.
    //
    // This function must be called only before adding any bitmaps to the
	// toolbar.  If the application does not explicitly set the bitmap size,
	// it defaults to 16 by 15 pixels. Returns nonzero if successful, otherwise
	// returns zero.
    // 
    BOOL SetBitmapSize(
		// Width and height, in pixels, of the bitmapped images.
        CSize size);

    // Call this function to retrieve the handle of the tooltip control,
	// if any, associated with the toolbar control.  Since the toolbar control
	// normally creates and maintains its own tooltip control, most programs
	// do not need to call this function. Returns a pointer to the CToolTipCtrl
	// object associated with this toolbar, or NULL, if the toolbar has no
	// associated tooltip control.
    // 
    CToolTipCtrl* GetToolTips() const;

    // Call this function to associate a tooltip control with a toolbar
	// control.
    // 
    void SetToolTips(
		// Pointer to the CToolTipCtrl object.
        CToolTipCtrl* pTip);

    // Call this function to set the owner window for the toolbar control.
	// The owner window is the window that receives notifications from the
	// toolbar.
    // 
    virtual void SetOwner(
		// Pointer to the CWnd or CWnd-derived object that will be the new owner
		// window for the toolbar control.
        CWnd* pOwnerWnd);

    // Call this function to ask the toolbar control to resize itself to
	// the requested number of rows.
    // 
    // If the toolbar cannot resize itself to the requested number or rows,
	// it will resize itself to either the next larger or next smaller valid
	// size, depending on the value of 'bLarger'.  If 'bLarger' is TRUE, the new
	// number of rows will be larger than the number requested.  If 'bLarger'
	// is FALSE, the new number of rows will be smaller than the number requested.
    // 
    // A given number of rows is valid for the toolbar, if the buttons
	// can be arranged such that all of the rows have the same number of buttons,
	// except perhaps the last row.  For example, a toolbar that contains
	// four buttons could not be sized to three rows because the last two
	// rows would have to be shorter.  If you attempted to size it to three
	// rows, you would get four rows if 'bLarger' was TRUE and two rows if 'bLarger'
	// was FALSE.
    // 
    // If there are separators in the toolbar, the rules for when a given
	// number of rows is valid are more complicated.  The layout is computed
	// such that button groups, buttons with a separator before the first
	// and the last button in the group, are never broken up on several rows
	// unless the group cannot fit on one row.
    // 
    // If a group does not fit on one row, the next group will start on
	// the next row even if it would fit on the row where the large group
	// ended.  The purpose of this rule is to make the separation between
	// large groups more noticeable.  The resulting vertical separators are
	// counted as rows.
    // 
    // Note, also, that the SetRows member function will always choose the
	// layout that results in the smallest toolbar size.  Creating a toolbar
	// with the TBSTYLE_WRAPABLE style and then resizing the control will
	// simply apply the method outlined above given the width of the control.
    // 
    // This function can only be called for toolbars that were created
	// with the TBSTYLE_WRAPABLE style.
    // 
    void SetRows(
		// Requested number of rows.
        int nRows,
		// Tells whether to use more rows (TRUE) or fewer rows (FALSE) if the
		// toolbar cannot be resized to the requested number of rows.
        BOOL bLarger,
		// Points to the CRect object or RECT structure that will receive the
		// new bounding rectangle of the toolbar.
        LPRECT lpRect);

    // Call this function to retrieve the number of rows of buttons currently
	// displayed by the toolbar control.  Note that the number of rows will
	// always be one, unless the toolbar was created with the TBSTYLE_WRAPABLE
	// style. Returns the number of rows of buttons currently displayed on
	// the toolbar. 
    // 
    int GetRows() const;

    // Call this function to set the command identifier which will be sent
	// to the owner window when the specified button is pressed. Returns nonzero
	// if successful, otherwise returns zero.
    // 
    BOOL SetCmdID(
		// The zero-based index of the button whose command ID is to be set.
        int nIndex,
		// The command ID to set the selected button to.
        UINT nID);

    // Call this function to retrieve the bitmap flags from the toolbar.
	// You should call it after creating the toolbar but before adding bitmaps
	// to the toolbar. 
    //
    // The return value indicates whether the display supports large bitmaps
	// or not.  If the display supports large bitmaps, and if you choose to
	// use them, call SetBitmapSize and SetButtonSize before adding your large
	// bitmap using AddBitmap. Returns a UINT that has the TBBF_LARGE flag
	// set if the display can support large toolbar bitmaps, clear otherwise.
    // 
    UINT GetBitmapFlags() const;
    
    // This member function implements the behavior of the Win32 message
	// TB_GETIMAGELIST, as described in the Platform SDK. Returns a pointer
	// to a CImageList object, or NULL if no disabled image list is set.
    // 
    HIMAGELIST GetImageList() const;

    // Call this member function to get the styles currently applied to
	// a toolbar control. Returns a DWORD containing a combination of toolbar
	// control styles, as described in the Platform SDK.
    // 
    DWORD GetToolbarStyle() const;

    // This member function implements the behavior of the Win32 message
	// TB_SETIMAGELIST, as described in the Platform SDK.
    //
    // The MFC implementation of SetImageList uses a CImageList object
	// containing the toolbar control's button images, rather than a handle
	// to an image list. Returns a pointer to an HIMAGELIST handle that was
	// previously used by the toolbar control to display button images in
	// their default state.
    // 
    HIMAGELIST SetImageList(
		// A handle to an HIMAGELIST object containing the images to be used
		// by the toolbar control to display button images in their default state.
        HIMAGELIST hImgList,
		// 'FALSE' if image list is set for disabled icons
		BOOL bEnabled = TRUE
		);

    // Call this member function to set the indentation for the first button
	// in a toolbar control. Returns nonzero if successful, otherwise returns
	// zero.
    // 
    BOOL SetIndent(
		// The value specifying the indentation, in pixels.
        int indent);

    // Call this member function to set the styles for a toolbar control.
    // 
    void SetToolbarStyle(
		// A DWORD containing a combination of toolbar control styles, as described
		// in the Platform SDK.
        DWORD dwStyle);

    // This member function implements the behavior of the Win32 message
	// TB_GETHOTITEM, as described in the Platform SDK. Returns the zero-based
	// index of the hot item in a toolbar.
    // 
    int GetHotItem() const;

    // This member checks to see if the item at given position has its
	// popup dropped down.  Returns the true if the index matches an item 
	// that has its popup dropped down, or false, if the item does not.
	//
    bool IsDroppedItem(
	 	// The zero-based index of the button whose command ID is to be set.
	 	int nIndex) const;

    // This member checks to see if there are item(s) that has its
	// popup dropped down.  Returns the true if so or false otherwise.
	//
    bool IsDroppedItem() const;

    // This member function implements the behavior of the Win32 message
	// TB_SETHOTITEM, as described in the Platform SDK. Returns the index
	// of the previous hot item, or -1, if there was no hot item.
    // 
    int SetHotItem(
		// The zero-based index number of the item that will be made hot.  If
		// this value is -1, none of the items will be hot.
        int iHot);

    // This member function implements the behavior of the Win32 message
	// TB_GETEXTENDEDSTYLE, as described in the Platform SDK. Returns a DWORD
	// that represents the extended styles currently in use for the toolbar
	// control.  For a list of styles, see Toolbar Extended Styles, in the
	// Platform SDK.
    // 
    DWORD GetExtendedStyle() const;

    // This member function implements the behavior of the Win32 message
	// TB_SETEXTENDEDSTYLE, as described in the Platform SDK. Returns a DWORD
	// that represents the previous extended styles.  For a list of styles,
	// see Toolbar Extended Styles, in the Platform SDK.
    // 
    DWORD SetExtendedStyle(
		// A value specifying the new extended styles.  This parameter can be
		// a combination of the toolbar extended styles.
        DWORD dwStyle);

    // This member function implements the behavior of the Win32 message
	// TB_MAPACCELERATOR, as described in the Platform SDK. Returns nonzero
	// if successful, otherwise returns zero.
    // 
    BOOL MapAccelerator(
        // Accelerator character to be mapped.  This character is the same
		// character that is underlined in the button's text.
        TCHAR ch,
        // A reference to a UINT that receives the command identifier of
		// the button that corresponds to the accelerator specified in 'ch'.
        UINT& nID) const;

    // This member function implements the behavior of the Win32 message
	// TB_HITTEST, as described in the Platform SDK. Returns an integer value
	// indicating the location of a point on a toolbar.  If the value is 
	// zero or a positive value, this return value is the zero-based index
	// of the non-separator item in which the point lies.  If the return value
	// is negative, the point does not lie within a button.  The absolute
	// value of the return value is the index of a separator item or the nearest
	// non-separator item.
    // 
    int HitTest(
		// A CPoint object that contains the x-coordinate of the hit test in
		// the x member and the y-coordinate of the hit test in the y member.
		// The coordinates are relative to the toolbar's client area.
        CPoint p) const;

    // Call this function to enable or disable the specified button in
	// a toolbar control.  When a button has been enabled, it can be pressed
	// and checked.  If you want to change more than one button state, consider
	// calling SetState instead. Returns nonzero if successful, otherwise
	// returns zero.
    // 
    BOOL EnableButton(
		// Command identifier of the button to enable or disable.
        int nID,
		// TRUE to enable the button, or FALSE to disable the button.
        BOOL bEnable);

    // Call this function to check or clear a given button in a toolbar
	// control.  When a button has been checked, it appears to have been pressed.
	// If you want to change more than one button state, consider calling
	// SetState instead. Returns nonzero if successful, otherwise returns zero.
    // 
    BOOL CheckButton(
		// Command identifier of the button to check or clear.
        int nID,
		// TRUE to check the button, or FALSE to clear it.
        BOOL bCheck);

    // Call this function to press or release the specified button in a
	// toolbar control.  If you want to change more than one button state,
	// consider calling SetState instead. Returns nonzero if successful, otherwise
	// returns zero.
    // 
    BOOL PressButton(
		// Command identifier of the button to press or release.
        int nID,
		// TRUE to press the specified button, or FALSE to release the specified
		// button.
        BOOL bPress);

    // Call this function to hide or show the specified button in a toolbar
	// control.  If you want to change more than one button state, consider
	// calling SetState instead. Returns nonzero if successful, otherwise
	// returns zero.
    // 
    BOOL HideButton(
        // Command identifier of the button to hide or show.
        int nID,
		// TRUE to hide the button, or FALSE to show it.
        BOOL bHide);

    // Call this function to set or clear the indeterminate state of the
	// specified button in a toolbar control.  Indeterminate buttons are displayed
	// grayed, such as the way the bold button on the toolbar of a word processor
	// would look when the text selected contains both bold and regular characters.
    //
    // If you want to change more than one button state, consider calling
	// SetState instead. Returns nonzero if successful, otherwise returns
	// zero.
    // 
    BOOL Indeterminate(
		// Command identifier of the button whose indeterminate state is to be
		// set or cleared.
        int nID,
		// TRUE to set the indeterminate state for the specified button, or FALSE
		// to clear it.
        BOOL bIndeterminate);

    // Call this function to add one or more buttons to a toolbar control.
    // 
    // The 'lpButtons' pointer points to an array of TBBUTTON structures.
	// Each TBBUTTON structure associates the button being added with the
	// button’s style, image and/or string, command ID, state, and user-defined
    // data:
    //
    //[pre]typedef struct _TBBUTTON {
    //    int iBitmap;    // Zero-based index of the button image
    //    int idCommand;  // Command to be sent when the button is pressed
    //    BYTE fsState;   // Button state--see below
    //    BYTE fsStyle;   // Button style--see below
    //    DWORD dwData;   // Application-defined value
    //    int iString;    // Zero-based index of the button label string
    //} TBBUTTON;[/pre]
    // 
    // The members are as follows:
    //[ul]
    //[li][b]iBitmap[/b] - Zero-based index of the button image.  NULL if there is no
	//       image for this button.[/li]
    //[li][b]idCommand[/b] - Command identifier associated with the button.
	//       This identifier is sent in a WM_COMMAND message when the button
	//       is chosen.  If the 'fsStyle' member has the TBSTYLE_SEP value,
	//       this member must be zero.[/li]
    //[li][b]fsState[/b] - Button state flags.  It can be a combination
	//       of the values listed below:
	//   [ul]
    //   [li][b]TBSTATE_CHECKED[/b] - The button has the TBSTYLE_CHECKED
	//          style and is being pressed.[/li]
    //   [li][b]TBSTATE_ENABLED[/b] - The button accepts user input.  A
	//          button that does not have this state does not accept user
	//          input and is grayed.[/li]
    //   [li][b]TBSTATE_HIDDEN[/b] - The button is not visible and cannot
	//          receive user input.[/li]
    //   [li][b]TBSTATE_INDETERMINATE[/b] - The button is grayed.[/li]
    //   [li][b]TBSTATE_PRESSED[/b] - The button is being pressed.[/li]
    //   [li][b]TBSTATE_WRAP[/b] - A line break follows the button.  The
	//          button must also have the TBSTATE_ENABLED state.[/li]
	//   [/ul][/li]
    //[li][b]fsStyle[/b] - Button style.  It can be a combination of the
	//       values listed below: 
    //   [ul]
    //   [li][b]TBSTYLE_BUTTON[/b] - Creates a standard push button.[/li]
    //   [li][b]TBSTYLE_CHECK[/b] - Creates a button that toggles between
	//          the pressed and unpressed states each time the user clicks
	//          it.  The button has a different background color when it is
	//          in the pressed state.[/li]
    //   [li][b]TBSTYLE_CHECKGROUP[/b] - Creates a check button that stays
	//          pressed until another button in the group is pressed.[/li]
    //   [li][b]TBSTYLE_GROUP[/b] - Creates a button that stays pressed
	//          until another button in the group is pressed.[/li]
    //   [li][b]TBSTYLE_SEP[/b] - Creates a separator, providing a small
	//          gap between button groups.  A button that has this style does
	//          not receive user input.[/li]
    //   [/ul][/li]
    //[li][b]dwData[/b] - User-defined data.[/li]
    //[li][b]iString[/b] - Zero-based index of the string to use as the
	//       button’s label.  NULL if there is no string for this button.[/li]
    //[/ul]
    // The image and/or string whose index you provide must have previously
	// been added to the toolbar control’s list using AddBitmap, AddString,
	// and/or AddStrings. Returns nonzero if successful, otherwise returns
	// zero.
    // 
    BOOL AddButtons(
		// Number of buttons to add.
        int nNumButtons,
		// Address of an array of TBBUTTON structures that contain information
		// about the buttons to add.  There must be the same number of elements
		// in the array as buttons specified by 'nNumButtons'. 
        LPTBBUTTON lpButtons);

    // Call this function to insert a button in a toolbar control.
    // 
    // The image and/or string whose index you provide must have previously
	// been added to the toolbar control’s list using AddBitmap, AddString,
	// and/or AddStrings. Returns nonzero if successful, otherwise returns zero.
    // 
    BOOL InsertButton(
		// Zero-based index of a button.  This function inserts the new button
        // to the left of this button.
        int nIndex,
		// Address of a TBBUTTON structure containing information about the button
		// to insert.  See CXTToolBar::AddButtons for a description of the
		// TBBUTTON structure.
        LPTBBUTTON lpButton);

    // Call this function to delete a button from the toolbar control.
	// Returns nonzero if successful, otherwise returns zero.
    // 
    BOOL DeleteButton(
		// Zero-based index of the button to delete.
        int nIndex);

    // Call this function to restore the state of the toolbar control from
	// the location in the registry.
    // 
    void RestoreState();

    // Call this function to save the state of the toolbar control to the
	// location in the registry.
    // 
    void SaveState();

    // Call this function to display the Customize Toolbar dialog box.
	// This dialog box allows the user to customize the toolbar by adding
	// and deleting buttons.
    //
    void Customize();

    // Call this function to add a new string or strings to the list of
	// strings available for a toolbar control.  Strings in the buffer must
	// be separated by a null character.
    //
    // You must ensure that the last string has two null terminators. 
	// To properly format a constant string, you might write it as:
    //
    //[pre]// one null added automatically
    //lpszStrings = "Only one string to add\0";[/pre]
    //
    // or:
    //
    //[pre]// adds three strings with one call
    //lpszStrings = "String 1\0String 2\0String 3\0";[/pre]
    //
    // You should not pass a CString object to this function since it is
	// not possible to have more than one null character in a CString. Returns
	// the zero-based index of the first new string added if successful, otherwise
	// returns –1. 
    // 
    int AddStrings(
        // Address of a buffer that contains one or more null-terminated
		// strings to add to the toolbar’s string list.  The last string must
		// be terminated with two null characters.
        LPCTSTR lpszStrings);

    // Call this function to resize the entire toolbar control.  You should
	// call this function when the size of the parent window changes, or when
	// the size of the toolbar changes, such as when you set the button or
	// bitmap size, or add strings.
    // 
    void AutoSize();

	//Operations
public:

	// Call this function to save all toolbars in the same list as 
	// CMainFrame::m_wndToolBar. This function will also write the XT library
	// version number into the root key of the toolbar subsection, for future
	// compatibility reasons.  Returns 'true' to indicate success, or 'false'
	// to indicate failure.
	//
	bool SaveToolbars();

	// Call this function to load toolbars and their button layouts.  This
	// function returns the number of toolbars successfully loaded.  It is
	// called [b]after[/b] the standard toolbar loading code and [b]before[/b]
	// LoadBarState. First, the standard toolbars are loaded, so that their
	// variables, in CMainFrame, are properly initialized.  Then, this function
	// adds any missing toolbars and resets the buttons of the standard toolbars
	// to what they were saved as.  Finally, LoadBarState restores the positions
	// and so on.
	//
	// The two parameters passed in are the master menu and toolbar, from
	// which all icons and strings come.  Both have the resource ID, IDR_XTMASTER,
	// in the main application resources. Returns the number of toolbars loaded successfully.
	//
	int LoadToolbars(
		// A pointer to a valid CXTToolBar object.
		CXTToolBar * tbMaster, 
		// A pointer to a valid CXTMenu object.
		CXTMenu * menuMaster);

	// This function allows you to gain access to the toolbar list when you
	// do not have a toolbar instance available. This is the reason it is static.
	// The reason for having a CXTToolbar list inside the toolbars is to allow
	// stepping only through toolbars, without having to step through the
	// control bar list and check each to see if it is a toolbar.  
	// CXTFrameWnd::m_listControlBars is in fact where the pointer comes from.
	// Returns a pointer to a CXTToolbar object.
	//
	static CXTToolBar * GetTBListHead();

	// This member function tells if this toolbar is currently being customized.
	// Returns 'true' if it is being customized, otherwise returns 'false'.
	//
	bool IsAdjustingToolbar() const;
	
	// This member function tells if this toolbar shall render an 'expansion
	// available' chevron button when the bar is only partially visible. 
	// Default is to display the button. Returns TRUE to display the chevron
	// button, otherwise returns FALSE.
	//
    virtual BOOL IsExpansionButton();

	// This member function tells if expansion button contains a 'Customize'
	// toolbar command. Returns 'true' if expansion button contains a 'Customize'
	// toolbar command, otherwise returns 'false'.
	//
	bool IsExpansionCustomizes();

    // This member function tells if the expansion button will display
	// a chevron showing more buttons available. Valid if IsExpansionButton()
	// returns TRUE. Returns TRUE if the expansion button will display a chevron,
	// otherwise returns FALSE.
	//
    virtual BOOL IsExpansionChevron();

	// This member function tells if the expansion chevron points horizontally.
	// Applicable if IsExpansionChevron() returns TRUE. Returns 'true' if
	// the expansion chevron points horizontally, otherwise returns 'false'.
	//
	virtual bool IsChevronHorizontal();

    // This member function is called by the toolbar to auto-hide toolbar
	// buttons when the framework is sizing. Returns an integer value that represents
	// the current compaction of the buttons.
	//
    int CompactButtons(
		// Current total size of the buttons hidden.
		int dimesionCurrent,
		// Current visible toolbar size.
		int dimensionMin,
		// 'true' if the toolbar is horizontally oriented.
		bool bHorz);
    
    // This member function is called by the toolbar to auto-display toolbar
	// buttons when the framework is sizing. Returns an integer value that represents
	// the current compaction of the buttons.
	//
    int ExpandButtons(
		// Current total size of the buttons hidden.
		int currentDimension,
		// Current visible toolbar size.
		int dimesion,
		// 'true' if the toolbar is horizontally oriented.
		bool bHorz);

	// This member function calculates the minimum extent for this toolbar
	// (at least one button visible). Returns a CSize object.
	//
	CSize GetMinExtent();

	// This member function adjusts DrawText() flags, as specified with 
	// SetDrawTextFlags(). See also: TB_SETDRAWTEXTFLAGS. Returns a DWORD
	// value containing the adjusted flags.
	//
	DWORD GetDrawTextFlags(
		// Default DrawText() flags.
		DWORD dwFlags) const;

    // Call this function to enable a control bar to be docked.  The sides
	// specified must match one of the sides enabled for docking in the destination
	// frame window, or the control bar cannot be docked to that frame window.
    //
    void EnableDocking(
		// Specifies whether the control bar supports docking and the sides of
		// its parent window to which the control bar can be docked, if supported.
        // It can be one or more of the following: 
        //[ul]
        //[li][b]CBRS_ALIGN_TOP[/b] - Allows docking at the top of the
		//       client area.[/li]
        //[li][b]CBRS_ALIGN_BOTTOM[/b] - Allows docking at the bottom of
		//       the client area.[/li]
        //[li][b]CBRS_ALIGN_LEFT[/b] - Allows docking on the left side
		//       of the client area.[/li]
        //[li][b]CBRS_ALIGN_RIGHT[/b] - Allows docking on the right side
		//       of the client area.[/li]
        //[li][b]CBRS_ALIGN_ANY[/b] - Allows docking on any side of the
		//       client area.[/li]
        //[li][b]CBRS_FLOAT_MULTI[/b] - Allows multiple control bars to
		//       be floated in a single mini-frame window.[/li]
        //[/ul]
        // If 0 (that is, indicating no flags), the control bar will not
		// dock.
        DWORD dwDockStyle);

    // Call this function to enable a control bar to be docked.  The sides
	// specified must match one of the sides enabled for docking in the destination
	// frame window, or the control bar cannot be docked to that frame window.
    //
    void EnableDockingEx(
		// Specifies whether the control bar supports docking and the sides of
		// its parent window to which the control bar can be docked, if supported.
        // It can be one or more of the following: 
        //[ul]
        //[li][b]CBRS_ALIGN_TOP[/b] - Allows docking at the top of the
		//       client area.[/li]
        //[li][b]CBRS_ALIGN_BOTTOM[/b] - Allows docking at the bottom of
		//       the client area.[/li]
        //[li][b]CBRS_ALIGN_LEFT[/b] - Allows docking on the left side
		//       of the client area.[/li]
        //[li][b]CBRS_ALIGN_RIGHT[/b] - Allows docking on the right side
		//       of the client area.[/li]
        //[li][b]CBRS_ALIGN_ANY[/b] - Allows docking on any side of the
		//       client area.[/li]
        //[li][b]CBRS_FLOAT_MULTI[/b] - Allows multiple control bars to
		//       be floated in a single mini-frame window.[/li]
        //[/ul]
        // If 0 (that is, indicating no flags), the control bar will not
		// dock.
        DWORD dwDockStyle,
		// Specifies the splitters, inside dockbars, look. It can be one of
		// the following: 
        //[ul]
        //[li][b]CBRS_XT_NONFLAT[/b] - Thick devstudio like non-flat splitters.[/li]
        //[li][b]CBRS_XT_SEMIFLAT[/b] - Thin 3D non-flat splitters.[/li]
        //[li][b]CBRS_XT_FLAT[/b] - Flat splitters.[/li]
        //[/ul]
        DWORD dwFlatStyle);

    // This member function replaces an existing bitmap with a new bitmap.
	// Returns TRUE if successful, otherwise returns FALSE.
    // 
    BOOL AddReplaceBitmap(
		// An HBITMAP structure that contains the information for the new bitmap.
        HBITMAP hbmImageWell);

    // This member function replaces the button at the specified index.
    // 
    void SetButton(
		// The index at which to set the button.
        int nIndex,
		// Points to a valid TBBUTTON struct.
        TBBUTTON* pButton);

    // Call this member function to add a dropdown arrow to a toolbar.
	// A dropdown arrow can be used to display a popup menu or popup window.
	// Returns TRUE if successful, otherwise returns FALSE.
    // 
    BOOL AddDropDownButton(
		// Command ID for the toolbar button.
        UINT nIDButton,
		// Resource ID of the popup menu to display.  If none is specified, you
		// can use the CBRN_XT_DROPDOWN message that is sent from the toolbar
		// to its owner to handle the dropdown event.
        UINT nIDMenu=0,
		// TRUE if an arrow is to be displayed.
        BOOL bArrow=TRUE);

    // Call this member function to add a dropdown arrow to a toolbar
	// that contains a color picker.  A dropdown arrow can be used to display
	// a popup menu or popup window as well. Returns TRUE if successful, otherwise
	// returns FALSE.
    // 
    BOOL AddDropDownButton(
		// Command ID for the toolbar button.
        UINT nIDButton,
		// An RGB value that represents the current color for the color picker.
        COLORREF clrColor,
		// An RGB value that represents the default color for the color picker.
        COLORREF clrDefault=CLR_DEFAULT,
		// Color picker style.  Styles to be added can be combined by using the
		// bitwise OR (|) operator and can be one or more of the following:
		//[ul]
		//[li][b]CPS_XT_NOFILL[/b] - The color picker will display a No Fill
		//       button rather than the default Automatic Color button.[/li]
		//[li][b]CPS_XT_EXTENDED[/b] - The color picker will display 40 extended
		//       colors rather than the default 16 colors.[/li]
		//[li][b]CPS_XT_MORECOLORS[/b] - The color picker will display a More
		//       Colors button which will display a CXTColorDialog.[/li]
		//[li][b]CPS_XT_SHOW3DSELECTION[/b] - Displays the color selection box
		//       with a 3D raised border in CXTColorDialog.[/li]
		//[li][b]CPS_XT_SHOWHEXVALUE[/b] - Displays the hex equivalent of the
		//       selected color.[/li]
		//[/ul]
        DWORD dwPopup=CPS_XT_EXTENDED|CPS_XT_MORECOLORS,
		// TRUE to display the color on the toolbar button.
        BOOL bShowColor=TRUE,
		// TRUE if an arrow is to be displayed.
        BOOL bArrow=TRUE);

    // This member function is called to retrieve a pointer to the XT_DROPDOWNBUTTON
	// for the toolbar button specified by 'nID'. Returns a pointer to a valid
	// XT_DROPDOWNBUTTON structure.
    // 
    XT_DROPDOWNBUTTON* FindDropDownButton(
		// Command ID of the toolbar button.
        UINT nID);

    // Call this member function to calculate the horizontal size of a
	// control bar.  Control bars, such as toolbars, can stretch horizontally
	// or vertically to accommodate the buttons contained in the control bar.
	// Returns the control bar size, in pixels, of a CSize object.
	//
    virtual CSize CalcFixedLayout(
		// Indicates whether the bar should be stretched to the size of the frame.
		// The 'bStretch' parameter is nonzero when the bar is not a docking bar
		// (not available for docking) and is zero when it is docked or floating
		// (available for docking). 
        BOOL bStretch,
		// Indicates that the bar is horizontally or vertically oriented. The
		// 'bHorz' parameter is nonzero if the bar is horizontally oriented, and
		// is zero if it is vertically oriented. 
        BOOL bHorz);

    // The framework calls this member function to calculate the dimensions
	// of a dynamic toolbar.  Override this member function to provide your
	// own dynamic layout in classes you derive from CControlBar. MFC classes
	// derived from CControlBar, such as CToolbar, override this member function
	// and provide their own implementation. Returns a CSize object that represents
	// the control bar size, in pixels.
	//
    virtual CSize CalcDynamicLayout(
		// The requested dimension of the control bar, either horizontal or vertical,
		// depending on 'nMode'. 
        int nLength,
		// The following predefined flags are used to determine the height and
		// width of the dynamic control bar. Use the bitwise-OR (|) operator to
		// combine the flags. 
		//[ul]
		//[li][b]LM_STRETCH[/b] - Indicates whether the control bar should be
		//       stretched to the size of the frame. Set if the bar is not a
		//       docking bar (not available for docking). Not set when the bar
		//       is docked or floating (available for docking). If set, LM_STRETCH
		//       ignores 'nLength' and returns dimensions based on the LM_HORZ
		//       state. LM_STRETCH works similarly to the 'bStretch' parameter
		//       used in CalcFixedLayout. See that member function for more information
		//       about the relationship between stretching and orientation.[/li]
		//[li][b]LM_HORZ[/b] - Indicates that the bar is horizontally or vertically
		//       oriented. Set if the bar is horizontally oriented, and if it
		//       is vertically oriented, it is not set. LM_HORZ works similarly
		//       to the 'bHorz' parameter used in CalcFixedLayout. See that member function
		//       for more information about the relationship between stretching
		//       and orientation.[/li]
		//[li][b]LM_MRUWIDTH[/b] - Most Recently Used Dynamic Width. Ignores the 
		//       'nLength' parameter and uses the remembered most recently used
		//       width.[/li]
		//[li][b]LM_HORZDOCK[/b] - Horizontal Docked Dimensions. Ignores the 'nLength'
		//       parameter and returns the dynamic size with the largest width.[/li]
		//[li][b]LM_VERTDOCK[/b] - Vertical Docked Dimensions. Ignores the 'nLength'
		//       parameter and returns the dynamic size with the largest height.[/li]
		//[li][b]LM_LENGTHY[/b] - Set if 'nLength' indicates height (Y-direction)
		//       instead of width.[/li]
		//[li][b]LM_COMMIT[/b] - Resets LM_MRUWIDTH to the current width of the floating
		//       control bar.[/li]
		// [/ul]
        DWORD nMode);

    // The framework calls this member function to determine whether a point
	// is in the bounding rectangle of the specified tool. If the point is
	// in the rectangle, it retrieves information about the tool. If the area
	// with which the tooltip is associated is not a button, OnToolHitTest
	// sets the structure flags to TTF_NOTBUTTON and TTF_CENTERTIP. Override
	// OnToolHitTest to provide different information than the default provides.
	// See TOOLINFO, in the Win32 SDK Programmer’s Reference, for more information
	// about the structure. Returns 1, if the tooltip control was found, or
	// -1, if the tooltip control was not found.
    // 
    virtual int OnToolHitTest(
		// Specifies the x- and y-coordinate of the cursor. These coordinates
		// are always relative to the upper-left corner of the window.
        CPoint point,
		// A pointer to a TOOLINFO structure. The following structure values
		// are set by default: 
		// [ul]
        // [li][b]hwnd = m_hWnd[/b] - Handle to a window.[/li]
		// [li][b]uId = (UINT)hWndChild[/b] - Handle to a child window.[/li]
		// [li][b]uFlags |= TTF_IDISHWND[/b] - Handle of the tool.[/li]
		// [li][b]lpszText = LPSTR_TEXTCALLBACK[/b] - Pointer to the string that
		//        is to be displayed in the specified window.[/li]
		// [/ul]
        TOOLINFO* pTI) const;

    // This member function is called by the framework to update the status
	// of the toolbar or status bar.  To update an individual button or pane,
	// use the ON_UPDATE_COMMAND_UI macro in your message map to set an update
	// handler appropriately. See ON_UPDATE_COMMAND_UI for more information
	// about using this macro.  OnUpdateCmdUI is called by the framework when
	// the application is idle. The frame window to be updated must be a child
	// window, at least indirectly, of a visible frame window. OnUpdateCmdUI
	// is an advanced overridable.
    // 
    virtual void OnUpdateCmdUI(
		// A pointer to the main frame window of the application. This pointer
		// is used for routing update messages.
        CFrameWnd* pTarget,
		// Flag that indicates whether a control that has no update handler should
		// be automatically displayed as disabled. 
        BOOL bDisableIfNoHndler);

    // This member function is called to insert a control into a toolbar.
	// You can add a control to your toolbar by adding a place holder button
	// to your toolbar.  This button will represent where in the toolbar you
	// want your control to be placed.  Once you have done this, you can create
	// your control using the command ID for the toolbar button you just created.
	// Returns TRUE if successful, otherwise returns FALSE.
    // 
    virtual BOOL InsertControl(
		// A pointer to a valid CWnd object. 
        CWnd* pWnd,
		// 'true' if the newly inserted control should have a bold font.
        bool bBoldFont=false);

    // Call this member function to enable or disable customization for
	// the toolbar.  If enabled you can right click on the toolbar or choose
	// the expansion button at the end of the toolbar to display the customize
	// button.
    // 
	void EnableCustomization(
		// TRUE to enable customization.
        bool bEnable=true);

    // Call this member function to determine if customization is enabled
	// for the toolbar. Returns 'true' if customization has been enabled, otherwise
	// returns 'false'.
    // 
    bool IsCustomizable() const;
	
	//:Ignore
	void SetCustomBar(bool bCustomBar=true); // obsolete
    bool GetCustomBar() const; // obsolete
	//:EndIgnore

    // Call this member function to display disabled toolbar icons colorized. 
    // 
    void ColorizeDisabledItems(
		// TRUE to display disabled toolbar buttons in color.
        BOOL bDisabledColor=TRUE);

    // Call this member function to retrieve the index into the toolbar's
	// image list for the button specified by 'nIdButton'. Returns an integer value
	// index from the toolbar's image list for the specified button.
    // 
    int GetBitmap(
		// Command ID of a toolbar button.
        UINT nIdButton) const;

    // This member function retrieves the amount of horizontal and vertical
	// space (padding) around the text and image displayed on the buttons
	// for this toolbar. Returns a CSize object.
    // 
    CSize GetPadding() const;

    // This member function sets the amount of horizontal and vertical
	// space (padding) around the text and image displayed on the buttons
	// for this toolbar. Returns a CSize object.
    // 
    CSize SetPadding(
		// A CSize object.
        CSize size);

    // Call this member function to auto-size the toolbar.  This will adjust
	// the toolbar to the correct size whenever a control or text has been
	// added. Returns TRUE if successful, otherwise returns FALSE.
    // 
	BOOL AutoSizeToolbar();

	// This member function sets a flag telling that command buttons dropped
	// during toolbar customization on this toolbar shall be created with
	// text and icon or with icon only.
	//
	void SetDropCommandWithText(
		// Buttons shall be created with text and icons if 'true', or with an
		// icon only if 'false'.
		bool bWithText);

	// This member function is used to determine the correct background fill
	// color to be used during paint operations. Returns an RGB color value.
	//
	virtual COLORREF GetBackgroundColor() const;

	// This member function is used to display a popup menu associated with a
	// toolbar button drop arrow.
	//
	void TrackMenuPopup(
		// Points to a valid CXTMenu object.
		CXTMenu* pPopup,
		// Size and location of the popup menu.
		const CRect& rect);

	// This member function is called to determine the popup tracing point for a
	// toolbar button that has a popup window.  Returns a CPoint object that represents
	// the tracking point for the popup associated with the drop down toolbar button.
	//
	CPoint ComputePopupTrackPoint(
		// Area that the popup should not overlap.
		CRect rcExclude,
		// Size of the popup.
		CSize szPopup);

protected:
	
	// This member function creates and initializes a color picker popup window.
	// Returns a pointer to the color picker popup window just created.
	//
	virtual CXTColorPopup* CreateNewColorPopup(
		// A pointer to a valid XT_DROPDOWNBUTTON structure.
		XT_DROPDOWNBUTTON* pDropButton,
		// A CRect object.
		CRect rect);

	// This notification is called whenever this toolbar is added to a CXTReBar
	// object. Returns 'true' if successful, otherwise returns 'false'.
	//
	virtual bool OnAddedToRebar(
		// Pointer to a rebar bar descriptor to use to fill in the sizing information.
		REBARBANDINFO* pRBBI,
		// 'true' if this toolbar must be oriented horizontally.
		bool bHorz);

	// This member function is called whenever this toolbar is embedded in
	// a CXTReBar control that has just resized the band in which this toolbar
	// resides.
	//
	virtual void OnRebarBandResized(
		// Descriptor of the band.
		XT_REBARSIZECHILDINFO* pInfo,
		// 'true' if the toolbar is horizontally oriented.
		bool bHorz);

	// This member function tells if this toolbar will be visualized when
	// dragging or the system will render it with a wire frame. Default implementation
	// uses full window drag. Returns 'true' to enable full window drag, or
	// 'false' to use the wire frame.
	//
	virtual bool IsFullWindowDrag();

	// This member function is called to turn on or off toolbar customization mode.
	//
	virtual void SetCustMode(
		// 'true' to enable toolbar customization, or 'false' to disable it.
		bool bMode);

	// This member function fills in supported customizable features.
	//
	virtual void GetCustomBarInfo(
		// Pointer to a valid CXTCustomControlBarInfo object.
		CXTCustomControlBarInfo* pInfo);

//:Ignore
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
//:End Ignore

protected:
	//:Ignore
    //{{AFX_VIRTUAL(CXTToolBar)
	public:
	virtual void OnPaint();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	virtual void PositionControl(CWnd* pWnd);
	virtual void PositionControls();
    virtual bool IsFillToolBarClientRect();
    virtual void DrawSeparators(CDC* pDC);
    virtual BOOL DrawItem(CDC* pDC, LPNMCUSTOMDRAW lpNMCD);
    virtual void EraseWrappedSeparators(CDC* pDC);
    virtual void OnBarStyleChange(DWORD dwOldStyle, DWORD dwNewStyle);
    virtual void OnDropDownButton(NMTOOLBAR* pNMTB, UINT nID, CRect rect);
	virtual bool HasCmdHandlers(CWnd* pWnd);
    //:End Ignore

	//:Ignore
    //{{AFX_MSG(CXTToolBar)
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnSysColorChange();
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	//}}AFX_MSG
	afx_msg LRESULT OnSetDrawTextFlags(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetButtonSize(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetBitmapSize(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPreserveZeroBorderHelper(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToolBarBtnDropDown(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnToolBarBeginAdjust(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToolBarCustomHelp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToolBarEndAdjust(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToolBarGetButtonInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToolBarQueryDelete(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToolBarQueryInsert(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToolBarReset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnToolBarChange(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnToolBarHotItemChange(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg LRESULT OnSelEndOK(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnCloseUp(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFreshenAfter(WPARAM wParam, LPARAM lParam);
    //:End Ignore

	DECLARE_MESSAGE_MAP()

	//:Ignore
    friend CXTExpButton;
    friend CXTExpMenuWnd;
	friend CXTCustTbHandler;

    int WrapToolBar(TBBUTTON* pData, int nCount, int nWidth);
    CSize CalcLayout(DWORD nMode, int nLength = -1);
    CSize CalcSize(TBBUTTON* pData, int nCount);
	void MoveExpButton(int nDimension, bool bHorz);
    void ResetHiddenButtons();
    void CopyHiddenButtons(CXTToolBar* other, int preferredWidth);
    void SizeToolBar(TBBUTTON* pData, int nCount, int nLength, BOOL bVert = FALSE);
    void Layout();
    void LoadButtonInfo();
	LRESULT OnSetSizeHelper(CSize& size, LPARAM lParam);
    XT_DROPDOWNBUTTON* InitDropDownButton(UINT nIDButton, BOOL bArrow);
	void ResetCustomizations();
	void DoRestoreState();
	void DeferFreshenAfter();
	void ResetFreshenAfter();

	// Create new custom group item descriptor.
	// Used during toolbar customization, override to pass your custom data
	virtual CXTCustomGroupItemInfo* GetCustomItemInfo(
		// Index of the toolbar button being queried
		int nIndex);
	// Do data drop, implementation must create a toolbar button
	// out of data provided at a given position
	virtual void DropCustomItemInfo(
		// data being dragged
		CXTCustomGroupItemInfo* pInfo, 
		// position at which the button must be created
		int nIndex);

	void TrackMenuBarPopup(DWORD dwMenuBarPopupID, const CRect& rect);
	void SaveToolBarState(LPCTSTR pszSubKey, LPCTSTR pszValueName);
	bool LoadToolBarState(LPCTSTR pszSubKey, LPCTSTR pszValueName);
	void GetCurrentBarState(LPVOID* ppvState, DWORD* pcbState);
	bool SetCurrentBarState(LPVOID pvData, DWORD cbData);
	void GetIcons(CXTIconMap* pMap);
	void RegisterCustomizeAPI(bool bRegister);
	bool StartCustomDrag(CPoint point);
	void DoCustomDrag(CPoint point);
	void SetListStyleIfNoText();
	void ResolveCommandIcons();
	bool ReportOriginalCustomGroups(XT_CUSTOMITEMS& items);
	CXTCustomGroupItemInfo* GetCustomItemInfo(const TBBUTTON& button);
	CXTCustomGroupItemInfo* GetCustomButtonInfo(const TBBUTTON& button);
	CXTCustomGroupItemInfo* GetCustomControlInfo(const TBBUTTON& button);
	void NoteIdealSizeChanged();
	virtual void ReportCustomGroups(CXTCustomGroups& groups);
	bool IsSeparator(int nIndex) const;
	static bool IsSeparator(const TBBUTTON& button);
	static bool IsControl(const TBBUTTON& button);
	int LocateHit(const CPoint& point);
	//:End Ignore
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE bool CXTToolBar::IsCustomizable() const {
	return m_bCustomize;
}
AFX_INLINE void CXTToolBar::SetCustomBar(bool bCustomBar/*=true*/) {
	EnableCustomization(bCustomBar);
}
AFX_INLINE bool CXTToolBar::GetCustomBar() const {
	return IsCustomizable();
}
AFX_INLINE void CXTToolBar::ColorizeDisabledItems(BOOL bDisabledColor) {
	ASSERT(::IsWindow(m_hWnd)); m_bDisabledColor = bDisabledColor;
}
AFX_INLINE BOOL CXTToolBar::LoadToolBar(UINT nIDResource) {
	ASSERT(::IsWindow(m_hWnd)); return LoadToolBar(MAKEINTRESOURCE(nIDResource));
}
AFX_INLINE BOOL CXTToolBar::LoadBitmap(UINT nIDResource) {
	ASSERT(::IsWindow(m_hWnd)); return LoadBitmap(MAKEINTRESOURCE(nIDResource));
}
AFX_INLINE CXTToolBarCtrl& CXTToolBar::GetToolBarCtrl() const {
	ASSERT(::IsWindow(m_hWnd)); return *(CXTToolBarCtrl*)this;
}
AFX_INLINE BOOL CXTToolBar::EnableButton(int nID, BOOL bEnable) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_ENABLEBUTTON, nID, MAKELPARAM(bEnable, 0));
}
AFX_INLINE BOOL CXTToolBar::CheckButton(int nID, BOOL bCheck) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_CHECKBUTTON, nID, MAKELPARAM(bCheck, 0));
}
AFX_INLINE BOOL CXTToolBar::PressButton(int nID, BOOL bPress) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_PRESSBUTTON, nID, MAKELPARAM(bPress, 0));
}
AFX_INLINE BOOL CXTToolBar::HideButton(int nID, BOOL bHide) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_HIDEBUTTON, nID, MAKELPARAM(bHide, 0));
}
AFX_INLINE BOOL CXTToolBar::Indeterminate(int nID, BOOL bIndeterminate) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_INDETERMINATE, nID, MAKELPARAM(bIndeterminate, 0));
}
AFX_INLINE BOOL CXTToolBar::IsButtonEnabled(int nID) const {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_ISBUTTONENABLED, nID, 0);
}
AFX_INLINE BOOL CXTToolBar::IsButtonChecked(int nID) const {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_ISBUTTONCHECKED, nID, 0);
}
AFX_INLINE BOOL CXTToolBar::IsButtonPressed(int nID) const {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_ISBUTTONPRESSED, nID, 0);
}
AFX_INLINE BOOL CXTToolBar::IsButtonHidden(int nID) const {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_ISBUTTONHIDDEN, nID, 0);
}
AFX_INLINE BOOL CXTToolBar::IsButtonIndeterminate(int nID) const {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_ISBUTTONINDETERMINATE, nID, 0);
}
AFX_INLINE BOOL CXTToolBar::SetState(int nID, UINT nState) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_SETSTATE, nID, MAKELPARAM(nState, 0));
}
AFX_INLINE int CXTToolBar::GetState(int nID) const {
	ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, TB_GETSTATE, nID, 0L);
}
AFX_INLINE BOOL CXTToolBar::AddButtons(int nNumButtons, LPTBBUTTON lpButtons) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_ADDBUTTONS, nNumButtons, (LPARAM)lpButtons);
}
AFX_INLINE BOOL CXTToolBar::InsertButton(int nIndex, LPTBBUTTON lpButton) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_INSERTBUTTON, nIndex, (LPARAM)lpButton);
}
AFX_INLINE BOOL CXTToolBar::DeleteButton(int nIndex) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_DELETEBUTTON, nIndex, 0);
}
AFX_INLINE int CXTToolBar::GetButtonCount() const {
	ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, TB_BUTTONCOUNT, 0, 0L);
}
AFX_INLINE int CXTToolBar::AddStrings(LPCTSTR lpszStrings) {
	ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, TB_ADDSTRING, 0, (LPARAM)lpszStrings);
}
AFX_INLINE void CXTToolBar::SetButtonStructSize(int nSize) {
	ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, TB_BUTTONSTRUCTSIZE, nSize, 0L);
}
AFX_INLINE BOOL CXTToolBar::SetButtonSize(CSize size) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_SETBUTTONSIZE, 0, MAKELPARAM(size.cx, size.cy));
}
AFX_INLINE BOOL CXTToolBar::SetBitmapSize(CSize size) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(size.cx, size.cy));
}
AFX_INLINE void CXTToolBar::AutoSize() {
	ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, TB_AUTOSIZE, 0, 0L);
}
AFX_INLINE CToolTipCtrl* CXTToolBar::GetToolTips() const {
	ASSERT(::IsWindow(m_hWnd)); return (CToolTipCtrl*)CWnd::FromHandle((HWND)::SendMessage(m_hWnd, TB_GETTOOLTIPS, 0, 0L));
}
AFX_INLINE void CXTToolBar::SetToolTips(CToolTipCtrl* pTip) {
	ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, TB_SETTOOLTIPS, (WPARAM)pTip->GetSafeHwnd(), 0L);
}
AFX_INLINE void CXTToolBar::SetRows(int nRows, BOOL bLarger, LPRECT lpRect) {
	ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, TB_SETROWS, MAKELPARAM(nRows, bLarger), (LPARAM)lpRect);
}
AFX_INLINE int CXTToolBar::GetRows() const {
	ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, TB_GETROWS, 0, 0L);
}
AFX_INLINE BOOL CXTToolBar::SetCmdID(int nIndex, UINT nID) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_SETCMDID, nIndex, nID);
}
AFX_INLINE UINT CXTToolBar::GetBitmapFlags() const {
	ASSERT(::IsWindow(m_hWnd)); return (UINT) ::SendMessage(m_hWnd, TB_GETBITMAPFLAGS, 0, 0L);
}
AFX_INLINE BOOL CXTToolBar::SetIndent(int indent) {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_SETINDENT, indent, 0L);
}
AFX_INLINE HIMAGELIST CXTToolBar::GetImageList() const {
	ASSERT(::IsWindow(m_hWnd)); return (HIMAGELIST) ::SendMessage(m_hWnd, TB_GETIMAGELIST, 0, 0L);
}
AFX_INLINE int CXTToolBar::GetBitmap(UINT nIdButton) const {
	ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, TB_GETBITMAP, nIdButton, 0L);
}
AFX_INLINE DWORD CXTToolBar::GetExtendedStyle() const {
	ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, TB_GETEXTENDEDSTYLE, 0, 0L);
}
AFX_INLINE DWORD CXTToolBar::SetExtendedStyle(DWORD dwStyle) {
	ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, TB_SETEXTENDEDSTYLE, 0, dwStyle);
}
AFX_INLINE BOOL CXTToolBar::GetRect(UINT nIdButton, RECT& rc) const {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_GETRECT, nIdButton, (LPARAM)&rc);
}
AFX_INLINE DWORD CXTToolBar::GetToolbarStyle() const {
	ASSERT(::IsWindow(m_hWnd)); return (DWORD) ::SendMessage(m_hWnd, TB_GETSTYLE, 0, 0L);
}
AFX_INLINE void CXTToolBar::SetToolbarStyle(DWORD dwStyle) {
	ASSERT(::IsWindow(m_hWnd)); ::SendMessage(m_hWnd, TB_SETSTYLE, 0, dwStyle);
}
AFX_INLINE int CXTToolBar::HitTest(CPoint p) const {
	ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, TB_HITTEST, 0, (LPARAM)&p);
}
AFX_INLINE int CXTToolBar::GetHotItem() const {
	ASSERT(::IsWindow(m_hWnd)); return (int) ::SendMessage(m_hWnd, TB_GETHOTITEM, 0, 0L);
}
AFX_INLINE int CXTToolBar::SetHotItem(int iHot) {
	ASSERT(::IsWindow(m_hWnd)); return (int)::SendMessage(m_hWnd, TB_SETHOTITEM, iHot, 0L);
}
AFX_INLINE BOOL CXTToolBar::MapAccelerator(TCHAR ch, UINT& nID) const {
	ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TB_MAPACCELERATOR, (WPARAM)ch, (LPARAM)&nID);
}
AFX_INLINE CSize CXTToolBar::GetPadding() const {
	ASSERT(::IsWindow(m_hWnd)); return (CSize)(DWORD)::SendMessage(m_hWnd, TB_GETPADDING, 0, 0L);
}
AFX_INLINE CSize CXTToolBar::SetPadding(CSize size)  {
	ASSERT(::IsWindow(m_hWnd)); return (CSize)(DWORD)::SendMessage(m_hWnd, TB_SETPADDING, 0, MAKELPARAM(size.cx,size.cy));
}
AFX_INLINE void CXTToolBar::GetSizes(SIZE& sizeButton, SIZE& sizeImage) {
    sizeImage = m_sizeImage, sizeButton = m_sizeButton;	
}
AFX_INLINE bool CXTToolBar::IsAdjustingToolbar() const {        
    return m_bAdjusting;
}
AFX_INLINE void CXTToolBar::SetDropCommandWithText(bool bWithText) {
	m_bDropCommandText = bWithText;
}
//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTTOOLBAR_H__)

