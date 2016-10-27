//:Ignore
// XTColorPageCustom.h : header file
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

#if !defined(__XTCOLORCUSTOM_H__)
#define __XTCOLORCUSTOM_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// XT_HAS_FOCUS is an enumeration used by CXTColorBase for determining 
// which color window has focus.
//
typedef	enum XT_HAS_FOCUS
{
	// Neither color window has focus.
	none,
	// Color wheel window has focus.
    wnd,
	// Lumination window has focus.
    lum 
};

//////////////////////////////////////////////////////////////////////
// CXTColorBase is a CStatic derived base class.  It is used to create
// custom color selection windows.
//
class _XT_EXT_CLASS CXTColorBase : public CStatic
{
public:

    // Constructs a CXTColorBase object.
    //
	CXTColorBase();

	// Destroys a CXTColorBase object, handles cleanup and de-allocation.
    //
    virtual ~CXTColorBase();

protected:

	// Background device context.
	CDC m_dcPicker;
	// An RGB value that represents the color selection.
	COLORREF m_clrColor;
	// Determines which color has focus.
	static XT_HAS_FOCUS m_eHasFocus;

protected:

	// 'true' for initialization.
	bool m_bInitControl;

public:

	// Current lumination value.
	double m_nLum;
	// Current saturation value.
	double m_nSat;
	// Current hue value.
	double m_nHue;
	// Current mouse postion relative to the device context.
	CPoint m_ptMousePos;

	// Call this member function to set the color for the selection window.
    //
	virtual void SetColor(
		// An RGB value that represents the color.
        COLORREF clr,
		// 'true' to update the cursor position.
        bool bUpdate=true);

	// Call this member function to return the current color selection.
	// Returns an RGB value that indicates the color selection.
    //
	virtual COLORREF GetColor();

	// Call this member function to update the cursor position.
    //
	virtual void UpdateCursorPos(
		// Current cursor location relative to the device context.
        CPoint point,
		// TRUE to notify the parent of change.
        BOOL bNotify=TRUE);

	// This member function is called to convert an RGB color value to
	// an HSL value.
    //
	static void RGBtoHSL(
		// An RGB value that represents the value to convert to HSL.
        COLORREF color,
		// Represents the color hue.
        double *h,
		// Represents the color saturation.
        double *s,
		// Represents the color lumination.
        double *l );

	// This member function is called to convert an HLS value to an RGB
	// color value.
	// Returns a COLORREF value.
    //
	static COLORREF HLStoRGB(
		// Represents the color hue.
        double h,
		// Represents the color lumination.
        double l,
		// Represents the color saturation.
        double s );

protected:

	//:Ignore
    //{{AFX_VIRTUAL(CXTColorBase)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
    //:End Ignore

protected:
	//:Ignore
    //{{AFX_MSG(CXTColorBase)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg LRESULT OnInitControl(WPARAM wParam, LPARAM lParam);
    //:End Ignore

	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE COLORREF CXTColorBase::GetColor() {
	return m_clrColor;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTColorWnd is a CXTColorBase derived class.  It is used to create a
// color selection window.
//
class _XT_EXT_CLASS CXTColorWnd : public CXTColorBase
{
public:

    // Constructs a CXTColorWnd object.
	//
	CXTColorWnd();

	// Destroys a CXTColorWnd object, handles cleanup and de-allocation.
	//
    virtual ~CXTColorWnd();

public:

	// Call this member function to set the color for the selection window.
	//
	virtual void SetColor(
		// An RGB value that represents the color.
		COLORREF clr, 
		// 'true' to update cursor position.
		bool bUpdate=true);

	// Call this member function to update the cursor position.
	//
	virtual void UpdateCursorPos(
		// Current cursor location relative to the device context.
		CPoint point, 
		// TRUE to notify the parent of change.
		BOOL bNotify=TRUE);

	// This member function is called to update the cross hair cursor.
	//
	void DrawCrossHair(
		// A CDC pointer that represents the current device context.
		CDC* pDC);

	// This member function is called to set the hue for the color window.
	//
	void SetHue(
		// Represents the new hue value to set.
		double h);

	// This member function is called to set the saturation for the color
	// window.
	//
	void SetSaturation(
		// Represents the new saturation value to set.
		double s);

	//:Ignore
    //{{AFX_VIRTUAL(CXTColorWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
    //:End Ignore

protected:
	//:Ignore
    //{{AFX_MSG(CXTColorWnd)
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
    //:End Ignore

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////
// CXTColorLum is a CXTColorBase derived class.  It is used to create a
// color luminance selection bar.
//
class _XT_EXT_CLASS CXTColorLum : public CXTColorBase
{
public:

    // Constructs a CXTColorLum object.
	//
	CXTColorLum();

	// Destroys a CXTColorLum object, handles cleanup and de-allocation.
	//
    virtual ~CXTColorLum();

protected:

	// Current location of the slider.
	int m_nSliderPos; 

public:

	// Call this member function to set the color for the selection window.
	//
	virtual void SetColor(
		// An RGB value that represents the color.
		COLORREF clr,
		// 'true' to update the cursor position.
		bool bUpdate=true);

	// Call this member function to update the cursor position.
	//
	virtual void UpdateCursorPos(
		// Current cursor location relative to the device context.
		CPoint point, 
		// TRUE to notify the parent of change.
		BOOL bNotify=TRUE);

	// This member function is called to draw the indicator arrow.
	//
	void DrawSliderArrow(
		// A CDC pointer that represents the current device context.
		CDC* pDC);

	// This member function is called to draw the selection bar.
	//
	void DrawLuminanceBar(
		// A CDC pointer that represents the current device context.
		CDC* pDC);

	// This member function is called to set the luminance for the color window.
	//
	void SetLuminance(
		// Represents the new luminance value to set.
		double l);

	// This member function is called to get the display size of the luminance
	// bar.
	//
	void GetLumBarRect(
		// Address to a CRect object.
		CRect& rect);

	//:Ignore
    //{{AFX_VIRTUAL(CXTColorLum)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
    //:End Ignore

protected:
	//:Ignore
    //{{AFX_MSG(CXTColorLum)
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
    //:End Ignore

	DECLARE_MESSAGE_MAP()
};

// forwards
class CXTColorDialog;

//////////////////////////////////////////////////////////////////////
// CXTColorPageCustom is a multiple inheritance class derived from CXTDialogState
// and CPropertyPage.  It is used to create a CXTColorPageCustom dialog.
//
class _XT_EXT_CLASS CXTColorPageCustom : CXTDialogState, public CPropertyPage
{
	DECLARE_DYNCREATE(CXTColorPageCustom)

public:

    // Constructs a CXTColorPageCustom object.
	//
	CXTColorPageCustom(
		// A pointer to the parent sheet.
		CXTColorDialog* pParentSheet=NULL);

	// Destroys a CXTColorPageCustom object, handles cleanup and de-allocation.
	//
    virtual ~CXTColorPageCustom();

	//:Ignore
	//{{AFX_DATA(CXTColorPageCustom)
	enum { IDD = XT_IDD_COLORCUSTOM };
	CXTColorWnd	m_colorWnd;
	CXTColorLum	m_colorLum;
	CStatic	m_txtSat;
	CStatic	m_txtRed;
	CStatic	m_txtLum;
	CStatic	m_txtHue;
	CStatic	m_txtGreen;
	CStatic	m_txtBlue;
	CSpinButtonCtrl	m_spinSat;
	CSpinButtonCtrl	m_spinRed;
	CSpinButtonCtrl	m_spinLum;
	CSpinButtonCtrl	m_spinHue;
	CSpinButtonCtrl	m_spinGreen;
	CSpinButtonCtrl	m_spinBlue;
	CXTEdit	m_editHue;
	CXTEdit	m_editGreen;
	CXTEdit	m_editBlue;
	CXTEdit	m_editLum;
	CXTEdit	m_editRed;
	CXTEdit	m_editSat;
	int		m_nR;
	int		m_nB;
	int		m_nG;
	int		m_nH;
	int		m_nL;
	int		m_nS;
	//}}AFX_DATA
	//:End Ignore

protected:

	// A pointer to the parent sheet.
	CXTColorDialog* m_pParentSheet; 

public:

	// This member function is called to retrieve the HSL values of the RGB
	// specified by color.
	//
	void RGBtoHSL(
		// An RGB value that represents the color.
		COLORREF color, 
		// Address of the integer to receive the lumination value (0-255).
		int* lum, 
		// Address of the integer to receive the saturation value (0-255).
		int* sat, 
		// Address of the integer to receive the hue value (0-255).
		int* hue);

	//:Ignore
    //{{AFX_VIRTUAL(CXTColorPageCustom)
	public:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
    //:End Ignore

protected:

	//:Ignore
    //{{AFX_MSG(CXTColorPageCustom)
	afx_msg void OnChangeEdit();
	afx_msg void OnChangeEditLum();
	afx_msg void OnChangeEditHue();
	afx_msg void OnChangeEditSat();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	afx_msg LRESULT OnUpdateColor(WPARAM wParam, LPARAM lParam);
    //:End Ignore

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__XTCOLORCUSTOM_H__)
