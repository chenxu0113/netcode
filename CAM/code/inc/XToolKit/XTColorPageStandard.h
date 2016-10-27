//:Ignore
// XTColorPageStandard.h : header file
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

#if !defined(__XTCOLORSTANDARD_H__)
#define __XTCOLORSTANDARD_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// XT_COLORCELL is a stand alone helper structure class.  It is used by
// the CXTColorHex class.
//
struct XT_COLORCELL
{
	// Array that indicates which cell index is to the left, top, right and
	// bottom of the color cell.
	int direction[4];
	// TRUE if the color cell is a standard selection rectangle.
	BOOL bSmall;
	// Represents the current cursor position.
	CPoint* pPoint;
	// An RGB value.
	COLORREF clr;
};

//:Ignore
// Definition for the color cell array.
typedef CList<XT_COLORCELL*,XT_COLORCELL*>   CCXTColorCell;
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTColorHex is a CStatic derived class.  It displays a color selection
// grid used by CXTColorPageStandard.
//
class _XT_EXT_CLASS CXTColorHex : public CStatic
{
public:

    // Constructs a CXTColorHex object.
    //
	CXTColorHex();

    // Destroys a CXTColorHex object, handles cleanup and de-allocation.
    //
    virtual ~CXTColorHex();

protected:

	// 'true' for initialization.
	bool m_bInitControl;
	// TRUE when the left mouse button is pressed.
	BOOL m_bLBtnDown;
	// TRUE when a small color cell is selected.
	BOOL m_bSmallCell;
	// Holds the last known selection point.
	CPoint m_ptCurrent;
	// A COLORREF value that contains the RGB information for the current color.
	COLORREF m_clrColor;
	// Array of XT_COLORCELL structs that represent displayed color cells.
	CCXTColorCell m_arCells;
	// Background device context.
	CDC m_dcPicker;

public:

	// Call this member function to retrieve RGB information for the color
	// found at the location specified by point. Returns a COLORREF value.
    //
	COLORREF ColorFromPoint(
		// XY location of the color to get RGB information for.
        CPoint point);

	// Call this member function to get a pointer to the XT_COLORCELL struct
	// that is represented by 'clr'. Returns a pointer to an XT_COLORCELL struct.
    //
	XT_COLORCELL* GetColorCell(
		// An RGB value that represents the color.
        COLORREF clr);

	// Call this member function to select the color specified by 'iIndex'.
    //
	void SetSelectedColor(
		// Index into m_arCells list.
        int iIndex);

	// Call this member function to select the color specified by 'clr'.
    //
	void SetSelectedColor(
		// An RGB value that represents the color.
        COLORREF clr);

	// Call this member function to return the currently selected color.
 	// Returns an RGB color value that represents the selected color.
	//
	COLORREF GetSelectedColor() const;

protected:

	// This member function is called by the class to draw a single color
	// cell.
    //
	void DrawCell(
		// A CDC pointer that represents the current device context.
        CDC *pDC,
		// XY position of the starting point for the first pixel drawn.
        CPoint point,
		// An RGB value that represents the color of the cell to draw.
        COLORREF clr,
		// Index of the cell to be selected when VK_LEFT is pressed.
        int l,
		// Index of the cell to be selected when VK_UP is pressed.
        int u,
		// Index of the cell to be selected when VK_RIGHT is pressed.
        int r,
		// Index of the cell to be selected when VK_DOWN is pressed.
        int d);

	// This member function is called by the class to draw a single large
	// color cell.
    //
	void DrawLargeCell(
		// A CDC pointer that represents the current device context.
        CDC *pDC,
		// XY position of the starting point for the first pixel drawn.
        CPoint point,
		// An RGB value that represents the color of the cell to draw.
        COLORREF clr,
		// Index of the cell to be selected when VK_LEFT is pressed.
        int l,
		// Index of the cell to be selected when VK_UP is pressed.
        int u,
		// Index of the cell to be selected when VK_RIGHT is pressed.
        int r,
		// Index of the cell to be selected when VK_DOWN is pressed.
        int d);

	// This member function is called to draw the selection window.
    //
	void DrawColorSelector(
		// A CDC pointer that represents the current device context.
        CDC *pDC);
	
	// This member function is called to select a color cell if any are selected.
    //
	void DrawSelectCell(
		// A CDC pointer that represents the current device context.
        CDC *pDC);

	// This member function is called to select a large color cell if any
	// are selected.
    //
	void DrawLargeSelectCell(
		// A CDC pointer that represents the current device context.
        CDC *pDC);

	// This member function updates the color selection based on the XY coordinates
	// specified by 'point'.
    //
	void UpdateSelection(
		// Current location of the color to select.
        CPoint point);

	// This member function is called to select a cell when a new selection is made.
    //
	void SelectColorCell(
		// A CDC pointer that represents the current device context.
        CDC* pDC);

	//:Ignore
    //{{AFX_VIRTUAL(CXTColorHex)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
	virtual bool IsValidColor(COLORREF cr) const;
    //:End Ignore

protected:
	//:Ignore
    //{{AFX_MSG(CXTColorHex)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnInitControl(WPARAM wParam, LPARAM lParam);
    //:End Ignore

	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE COLORREF CXTColorHex::GetSelectedColor() const {
	return m_clrColor;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTColorPageStandard is a multiple inheritance class derived from CXTDialogState
// and CPropertyPage.  It is used to create a CXTColorPageStandard dialog.
//
class _XT_EXT_CLASS CXTColorPageStandard : CXTDialogState, public CPropertyPage
{
	DECLARE_DYNCREATE(CXTColorPageStandard)

public:

    // Constructs a CXTColorPageStandard object.
	//
	CXTColorPageStandard(
		// Points to the parent property sheet.
		CXTColorDialog* pParentSheet=NULL);

	// Destroys a CXTColorPageStandard object, handles cleanup and de-allocation.
	//
    virtual ~CXTColorPageStandard();

	// This member function is called to set the selected color for the page.
	//
	void SetColor(
		// An RGB value that represents the color.
		COLORREF clr);

protected:

	// Points to the parent property sheet.
	CXTColorDialog* m_pParentSheet; 

	//:Ignore
	//{{AFX_DATA(CXTColorPageStandard)
	enum { IDD = XT_IDD_COLORSTANDARD };
	CXTColorHex	m_colorHex;
	//}}AFX_DATA
    //:End Ignore
	
	//:Ignore
    //{{AFX_VIRTUAL(CXTColorPageStandard)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
    //:End Ignore

protected:

	//:Ignore
    //{{AFX_MSG(CXTColorPageStandard)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg LRESULT OnUpdateColor(WPARAM wParam, LPARAM lParam);
    //:End Ignore

	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTColorPageStandard::SetColor(COLORREF clr) {
	m_colorHex.SetSelectedColor(clr);
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__XTCOLORSTANDARD_H__)
