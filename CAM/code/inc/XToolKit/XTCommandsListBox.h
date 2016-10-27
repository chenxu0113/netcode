//:Ignore
// XTCommandsListBox.h : interface for the CXTCommandsListBox class.
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

#if !defined(__XTCOMMANDSLISTBOX_H__)
#define __XTCOMMANDSLISTBOX_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTCustomGroupItemInfo;

//////////////////////////////////////////////////////////////////////
// CXTCommandsListBox is a CListBox derived class. It is used during toolbar
// customization to display a list of currently available commands.
//
class _XT_EXT_CLASS CXTCommandsListBox : public CListBox
{

public:
	// This object site API.
	//
	interface ISite
	{
		// This member function gets a command icon. Returns an true if found, false otherwise
		//
		virtual bool GetCommandIcon(
			// Command ID whose icon is queried for.
			UINT nCmdID,
			// destination to store command icon, valid if function returns true
			HICON& hIcon, 
			// destination to store disabled command icon, can be NULL, valid if function returns true
			HICON& hDisabledIcon,
			// destination to store hot command icon, can be NULL, valid if function returns true.
			HICON& hHotIcon) = 0;

		// This member function gets command ID by its position.  Returns a UINT
		// object.
		//
		virtual UINT GetItemCommand(
			// Position (item data) of the item in the list box.
			int pos) = 0;

		// This member function performs a drag and drop of an item at the position
		// provided.
		//
		virtual void DragNDrop(
			// Position (item data) of the item in the list box.
			int pos) = 0;
	};

private:
	ISite* const m_site;
	CSize m_sizeIcon;

public:


	ISite* GetSite() const {
		return m_site;
	}
	CSize& GetIconSize() {
		return m_sizeIcon;
	}

    // Constructs a CXTCommandsListBox object.
    //
    CXTCommandsListBox(ISite* site);

    // Destroys a CXTCommandsListBox object, handles cleanup and de-allocation.
    //
    virtual ~CXTCommandsListBox();

public:

	//{{AFX_VIRTUAL(CXTCommandsListBox)
    public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    //}}AFX_VIRTUAL

protected:
	
    //{{AFX_MSG(CXTCommandsListBox)
    afx_msg void OnPaint();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTCOMMANDSLISTBOX_H__)
//:End Ignore
