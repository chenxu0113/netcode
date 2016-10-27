//:Ignore
// XTSkins.h : header file
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

#if !defined(__XTSKINS_H__)
#define __XTSKINS_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

// forwards
class CXTToolBar;
class CXTFlatEdit;
class CXTFlatComboBox;
class CXTCommandsListBox;

//////////////////////////////////////////////////////////////////////
// IXTFlatComboProxy is a stand alone interface class.  It is an API for
// skin related operations for a flat combo.
// 
interface IXTFlatComboProxy
{
    // This member function renders the appearance of the combo box.
	//
    virtual void DrawCombo(
        // CXTFlatComboBox pointer to the combo to draw.
        CXTFlatComboBox* pCombo,
        // CDC pointer to the device context to render on.
        CDC* pDC,
        // Flag that tells if the combo must be highlighted.
        BOOL bHilight
        ) = 0;
};

//////////////////////////////////////////////////////////////////////
// IXTFlatEditProxy is a stand alone interface class.  It is an API for
// skin related operations for a flat edit.
// 
interface IXTFlatEditProxy
{
    // This member function renders the appearance of the combo box.
	//
    virtual void DrawEdit(
        // CXTFlatEdit pointer to the combo to draw.
        CXTFlatEdit* pCombo,
        // CDC pointer to the device context to render on.
        CDC* pDC,
        // Flag that tells if the combo must be highlighted.
        BOOL bHilight
        ) = 0;
};

//////////////////////////////////////////////////////////////////////
// IXTToolbarProxy is a stand alone interface class.  It is used to create
// an API for skin related operations for a toolbar item.
// 
interface IXTToolbarProxy
{
    // This member function fills in a rectangle allocated to a toolbar
	// control.
	//
    virtual void FillControlRect(
        // CDC pointer to the device context to draw on.
        CDC* pDC,
        // CXTToolBar pointer to a toolbar whose item is being drawn.
        CXTToolBar* pOwner,
        // Index of the item (a button) to draw.
        UINT nIndex
        ) = 0;

    // This member function draws a toolbar item.  Returns TRUE if the
	// item has been drawn, or returns FALSE otherwise.
	//
    virtual BOOL DrawButton(
        // CDC pointer to the device context to draw on.
        CDC* pDC,
        // CXTToolBar pointer to a toolbar whose item is being drawn.
        CXTToolBar* pOwner,
        // Index of the item (a button) to draw.
        UINT nIndex
        ) = 0;

    // This member function draws a toolbar separator.
	//
    virtual void DrawSeparator(
        // CDC pointer to the device context to draw on.
        CDC* pDC,
        // CXTToolBar pointer to a toolbar whose item is being drawn.
        CXTToolBar* pOwner,
        // Index of the item (separator) to draw.
        UINT nIndex
        ) = 0;

    // This member function draws special effects for a dropped item (shadow).
	//
    virtual void DrawDroppedEffects(
        // CDC pointer to the device context to draw on.
        CDC* pDC,
        // CXTToolBar pointer to a toolbar whose item is being drawn.
        CXTToolBar* pOwner,
        // Index of the item (button) to draw.
        UINT nIndex
        ) = 0;

    // This member function removes special effects around a selected item.
	//
    virtual void RemoveDroppedEffects(
        // CXTToolBar pointer to a toolbar that needs to be refreshed.
        CXTToolBar* pOwner,
        // Index of the item (button) to refresh.
        UINT nIndex
        ) = 0;

};

//////////////////////////////////////////////////////////////////////
// IXTCommandsListBoxProxy is a stand alone interface class.  It is used to create
// an API for skin related operations for the commands list box.
// 
interface IXTCommandsListBoxProxy
{
    // This member function draws each item in the commands list box.
    //
    virtual void OnDrawItem(
        // LPDRAWITEMSTRUCT pointer that contains information about the
		// item being drawn.
        LPDRAWITEMSTRUCT lpDIS,
        // CXTCommandsListBox pointer to the commands list box whose item
		// is being drawn.
        CXTCommandsListBox* pOwner) = 0;

    // This member function paints the background and handles default
	// drawing for the commands list box.
	//
	virtual void OnPaint(
        // CXTCommandsListBox pointer to the commands list box that needs
		// to be refreshed.
		CXTCommandsListBox* pOwner) = 0;
};

//////////////////////////////////////////////////////////////////////
// IXTSkin is a stand alone interface class.  It is used to create a skin
// for rendering UI elements.
//
interface IXTSkin
{
    // This member function is used to get a proxy for drawing a flat edit
	// control. Implementation can return NULL to indicate default control
	// behavior shall be used. Returns a proxy for drawing a flat edit control.
	//
    virtual IXTFlatEditProxy* GetFlatEditProxy() = 0;

    // This member function is used to get a proxy for drawing a flat combo
	// box. Implementation can return NULL to indicate default control behavior
	// shall be used. Returns a proxy for drawing a flat combo box. 
	//
    virtual IXTFlatComboProxy* GetFlatComboProxy() = 0;

    // This member function is used to get a proxy for drawing a toolbar
	// button. Implementation can return NULL to indicate default control
	// behaviour shall be used. Returns a proxy for drawing a toolbar button.
	//
    virtual IXTToolbarProxy* GetToolbarProxy() = 0;

    // This member function is used to get a proxy for drawing a commands list box
	// control. Implementation can return NULL to indicate default control
	// behavior shall be used. Returns a proxy for drawing a commands list box.
	//
    virtual IXTCommandsListBoxProxy* GetCommandsListBoxProxy() = 0;

    // This member function notifies that a toolbar is currently being
	// dragged or resized.
	//
    virtual void NotifyToolbarTracking(
        // Flag designating current operation state. TRUE if tracking
		// toolbar is pending, or FALSE otherwise.
        BOOL bTracking
        ) = 0;

    // This member function is a singleton type instantiation. Returns an
	// IXTSkin pointer.
	//
    static _XT_EXT_CLASS IXTSkin* GetInstance();
};


#endif // !defined(__XTSKINS_H__)
