//:Ignore
// XTColorSelectorCtrl.h : interface for the CXTColorSelectorCtrl class.
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

#if !defined(__XTCOLORCTRL_H__)
#define __XTCOLORCTRL_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// XT_COLOR_CELL is a stand alone helper structure class.  It is used by
// the CXTColorSelectorCtrl class.
//
struct XT_COLOR_CELL
{
    // Size of the color cell.
	CRect rect;
    // Command ID of the color cell.
	UINT nID;
	// Index of the color cell.
	UINT nIndex;
	// 'true' if the cell is checked.
	bool bChecked;
	// Windows style for the cell.
	DWORD dwStyle;
	// Tooltip text displayed for the color cell.
	TCHAR szText[256];
	// An RGB value that represents the color of the cell.
	COLORREF clr;
};

//:Ignore
// CList definition for the color cell array.
typedef CList<XT_COLOR_CELL*,XT_COLOR_CELL*> CColorCellArray;

// Array of listener windows to send notifications to.
typedef CArray<HWND, HWND> CListenerArray;
//:End Ignore

/////////////////////////////////////////////////////////////////////////////
// CXTColorSelectorCtrl is a CWnd derived class. It is used to create a CXTColorSelectorCtrl 
// control that will allow a user to select colors.
//
class _XT_EXT_CLASS CXTColorSelectorCtrl : public CWnd
{

public:
	// Constructs a CXTColorSelectorCtrl object.
    //
	CXTColorSelectorCtrl();

    // Destroys a CXTColorSelectorCtrl object, handles cleanup and de-allocation.
    //
    virtual ~CXTColorSelectorCtrl();

protected:
	
	// Array of user defined colors.
	static CUIntArray m_arUserDefColors;
	// Array of listener windows to be sent notifications.
	CListenerArray m_listeners;
	// Tooltip control.
	CToolTipCtrl m_tooltip;
	// Array of color items.
	CColorCellArray m_arCells;
	// Last known cursor position.
	CPoint m_point;
	// cx and cy size for a color picker button.
	CSize m_sizeButton;
	// Rect for the popup window.
	CRect m_rcWnd;
	// Color popup window style.
	DWORD m_dwPopup;
	// Points to the parent window for the popup window.
	CWnd* m_pParentWnd;
	// Number of rows in the color popup window.
	int m_nRows;
	// Number of columns in the color popup window.
	int m_nCols;
	// Index of the last button on the popup window.
	int m_nLastIndex;
	// Number of buttons in this window.
	int m_nBtnCount;
	// Currently selected index.
	int m_nCurSel;
	// An RGB value that represents the currently selected color.
	COLORREF m_clrColor;
	// An RGB value that represents the default color for the popup window.
	COLORREF m_clrDefault;
	// TRUE if the color dialog is open.
	BOOL m_bColorDlg;
	// Pressed button.
	int m_nPressed;
	// Control borders
	CRect m_rcBorders;

public:

	// This member function will return the index of the currently selected
	// color and will initialize 'pColorCell' struct. Returns the zero (0) based
	// index of the currently selected button.
    //
    int GetCurSel(
		// Receives a pointer to the currently selected button.
        XT_COLOR_CELL* pColorCell);
	
	// This member function will select a button based upon its index.
    //
    void SetCurSel(
		// An integer value that represents the zero (0) based
        // index of the button to be selected.
        int nIndex);

	// This member function handles the creation of the color popup window.
    // Returns TRUE if successful, otherwise returns FALSE.
    //
	BOOL Create(
		// A reference to a CRect object that represents the
        // size of the color popup window.
        CRect rect,
        // Points to the parent window for the color popup.
        CWnd* pParentWnd,
		// Style for the popup window. Styles can be one or 
        // more of the following:
        //[ul]
        //[li][b]CPS_XT_NOFILL[/b] - The color picker will display a No Fill
	    //       button rather than the default Automatic Color button.[/li]
        //[li][b]CPS_XT_EXTENDED[/b] - The color picker will display 40
		//       extended colors rather than the default 16 colors.[/li]
        //[li][b]CPS_XT_MORECOLORS[/b] - The color picker will display
		//       a More Colors button which will display a CXTColorDialog.[/li]
        //[li][b]CPS_XT_SHOW3DSELECTION[/b] - Displays the color selection
		//       box with a 3D raised border in CXTColorDialog.[/li]
        //[li][b]CPS_XT_SHOWHEXVALUE[/b] - Displays the hex equivalent
		//       of the selected color.[/li]
        //[/ul]
        DWORD dwPopup,
		// An RGB value that represents the currently selected color for the
		// popup window.
        COLORREF clrColor,
		// Specifies the default color for the color popup.  If the
        // current style includes CPS_NOFILL this parameter is
        // ignored.
        COLORREF clrDefault=CLR_DEFAULT);

	// This member function adds a window to send color picker notifications
	// to.
	//
	void AddListener(
		// A handle to the listener window.  Messages will be sent to it.
		HWND hwndListener);

	// This member function remove a window from notification list
	//
	void RemoveListener(
		// A handle to the listener window to remove		
		HWND hwndListener);

	// This member function is called to select a color cell.
    //
    void SelectColorCell(
		// Points to an XT_COLOR_CELL object.
        XT_COLOR_CELL* pColorCell);
	
	// This member function is called to select a color cell.
    //
    void SelectColor(
		// Color of selected cell.
        COLORREF clr);

	// This member function is called to return an XT_COLOR_CELL struct
	// from the color cell array. Returns an XT_COLOR_CELL object.
    //
    XT_COLOR_CELL* GetCellFromIndex(
		// Index into the color cell array.
        int iIndex);

    // This member function is called to set the size of the borders
    // for the control.
    //      
	void SetBorders(        
		// Specifies the left position.
        int cxLeft = 0,
        // Specifies the top.
        int cyTop = 0,
        // Specifies the right.
        int cxRight = 0,
        // Specifies the bottom.
        int cyBottom = 0);
	
protected:

    // This member function is called to draw the color selector.
    //
    void DrawColorSelector(
		// A CDC pointer that represents the current device context.
        CDC *pDC);

    // This member function is called to draw the cell specified by 'pColorCell'.
    //
    void DrawColorCell(
		// An XT_COLOR_CELL object.
        XT_COLOR_CELL* pColorCell,
		// A CDC pointer that represents the current device context.
        CDC* pDC,
		// TRUE to highlight the color cell.
        BOOL bHilite,
		// TRUE to press the color cell.		
		BOOL bPressed);
    
	// This member function will finish the selection process for the color
    // box or button in the color popup window.
    //
    virtual void EndSelection(
		// Current index of the selected color box or button
        // in the color popup window.
        int nCurSel);

    //:Ignore
    //{{AFX_VIRTUAL(CXTColorSelectorCtrl)
    public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    protected:
    //}}AFX_VIRTUAL
	void _EndSelection(int nCurSel, LPARAM callerParam);
	//:End Ignore
	
protected:
	//:Ignore
	//{{AFX_MSG(CXTColorSelectorCtrl)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseLeave();
	afx_msg UINT OnNcHitTest(CPoint point);
	//}}AFX_MSG

	friend class CXTPopupColorTearOff;
	//:End Ignore
	
	DECLARE_MESSAGE_MAP()

};

//:Ignore
/////////////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTColorSelectorCtrl::SetBorders(int cxLeft, int cyTop, int cxRight, int cyBottom) {
	m_rcBorders.left = cxLeft; m_rcBorders.top = cyTop; m_rcBorders.right = cxRight; m_rcBorders.bottom = cyBottom;	
}

//////////////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //__XTCOLORCTRL_H__
