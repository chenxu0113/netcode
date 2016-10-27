//:Ignore
// XTReBar.h interface for the CXTReBar class.
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

#if !defined(__XTREBAR_H__)
#define __XTREBAR_H__

// import base class

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

// class forwards
class CXTReBarCtrl;
class CXTNewCustomBarRequest;

//////////////////////////////////////////////////////////////////////
// CXTReBar is a CXTControlBar derived class.  It is used to create a CXTReBar
// object. A CXTReBar object is a control bar that provides layout, persistence,
// and state information for rebar controls.
//
// A rebar object can contain a variety of child windows, usually other
// controls, including edit boxes, toolbars, and list boxes.  A rebar object
// can display its child windows over a specified bitmap.  Your application
// can automatically resize the rebar, or the user can manually resize
// the rebar by clicking or dragging its gripper bar. 
//
// [b]Rebar Control[/b]
//
// A rebar object behaves similarly to a toolbar object.  A rebar uses
// the click-and-drag mechanism to resize its bands.  A rebar control can
// contain one or more bands, with each band having any combination of
// a gripper bar, a bitmap, a text label, and a child window.  However,
// bands cannot contain more than one child window.
//
// CXTReBar uses the CXTReBarCtrl class to provide its implementation.
// You can access the rebar control through GetReBarCtrl to take advantage
// of the control's customization options.  For more information about
// rebar controls, see CXTReBarCtrl.  For more information about using
// rebar controls, see Using CXTReBarCtrl in the XTreme Toolkit online
// help.
//
// [b]Warning[/b]    Rebar and rebar control objects do not support MFC
// control bar docking. If CRebar::EnableDocking is called, your application
// will assert.
//
class _XT_EXT_CLASS CXTReBar : public CXTControlBar
{
	DECLARE_DYNAMIC(CXTReBar)

public:

    // Constructs a CXTReBar object.
	//
	CXTReBar();

	// Call this member function to create a rebar. Returns nonzero if successful,
	// otherwise returns zero.
	//
	BOOL Create(
		// Pointer to the CWnd object whose Windows window is the parent of the
		// status bar.  This is normally your frame window.
		CWnd* pParentWnd,
		// The rebar control style.  By default, RBS_BANDBORDERS, which displays
		// narrow lines to separate adjacent bands within the rebar control.
		// See Rebar Control Styles in the Platform SDK for a list of styles.
		DWORD dwCtrlStyle = RBS_BANDBORDERS,
		// The rebar window styles.
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP,
		// The rebar's child-window ID.
		UINT nID = AFX_IDW_REBAR);

	// This member function allows direct access to the underlying common
	// control.  Call this member function to take advantage of the functionality
	// of the Windows rebar common control in customizing your rebar.  When
	// you call GetReBarCtrl, it returns a reference object to the CXTReBarCtrl
	// object so you can use either set of member functions.
	//
	// For more information about using CXTReBarCtrl to customize your rebar,
	// see Using CXTReBarCtrl in the XTreme Toolkit online help.
	//
	// [b]Example[/b]
	// 
	//[pre]CXTReBarCtrl& refReBarCtrl = m_wndReBar.GetReBarCtrl();[/pre]
	//
	//[pre]UINT nBandCount = refReBarCtrl.GetBandCount();
	//CString msg;
	//msg.Format("Band Count is: %u", nBandCount);
	//AfxMessageBox(msg);[/pre]
	// 
	// Returns a reference to a CXTReBarCtrl object.
	//
	CXTReBarCtrl& GetReBarCtrl() const;

	// Call this member function to add a band to the rebar.
	//
	// [b]Example[/b]
	// 
	//[pre]// Define a pointer to a CRebar in your class definition,
	//// such as: CReBar* m_pReBar; More often, however, you
	//// would probably specify an instance in your class
	//// definition, such as: CReBar m_ReBar;
	//m_pReBar = new CReBar();
	//m_pReBar->Create(this);
	//m_wndDlgBar.Create(this, IDR_MAINFRAME, CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR);
	//m_pReBar->AddBar(&m_wndDlgBar);[/pre]
	// 
	// Returns nonzero if successful, otherwise returns zero.
	//
	BOOL AddBar(
		// A pointer to a CWnd object that is the child window to be inserted
		// into the rebar.  The referenced object must have a WS_CHILD.
		CWnd* pBar,
		// A pointer to a string containing the text to appear on the rebar.
		// NULL by default.  The text contained in 'pszText' is not part of the
		// child window. It is on the rebar itself.
		LPCTSTR pszText = NULL,
		// A pointer to a CBitmap object to be displayed on the rebar background.
		// NULL by default.
		CBitmap* pbmp = NULL,
		// A DWORD containing the style to apply to the rebar.  See the 'fStyle'
		// function description in the Win32 structure REBARBANDINFO for a complete
		// list of band styles.
		DWORD dwStyle = RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP);

	// Call this member function to add a band to the rebar.
	//
	// [b]Example[/b]
	// 
	//[pre]// Define a pointer to a CRebar in your class definition,
	//// such as: CReBar* m_pReBar; More often, however, you
	//// would probably specify an instance in your class
	//// definition, such as: CReBar m_ReBar;
	//m_pReBar = new CReBar();
	//m_pReBar->Create(this);
	//m_wndDlgBar.Create(this, IDR_MAINFRAME, CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR);
	//m_pReBar->AddBar(&m_wndDlgBar);[/pre]
	// 
	// Returns nonzero if successful, otherwise returns zero.
	//
	BOOL AddBar(
		// A pointer to a CWnd object that is the child window to be inserted
		// into the rebar.  The referenced object must have a WS_CHILD.
		CWnd* pBar,
		// An RGB value that represents the foreground color of the rebar.
		COLORREF clrFore,
		// An RGB value that represents the background color of the rebar.
		COLORREF clrBack,
		// A pointer to a string containing the text to appear on the rebar.
		// NULL by default.  The text contained in 'pszText' is not part of the
		// child window. It is on the rebar itself.
		LPCTSTR pszText = NULL,
		// A DWORD containing the style to apply to the rebar.  See the 'fStyle'
		// function description in the Win32 structure REBARBANDINFO for a complete
		// list of band styles.
		DWORD dwStyle = RBBS_GRIPPERALWAYS);

	// Call this member function to add a band to the rebar.
	//
	// [b]Example[/b]
	// 
	//[pre]// Define a pointer to a CRebar in your class definition,
	//// such as: CReBar* m_pReBar; More often, however, you
	//// would probably specify an instance in your class
	//// definition, such as: CReBar m_ReBar;
	//m_pReBar = new CReBar();
	//m_pReBar->Create(this);
	//m_wndDlgBar.Create(this, IDR_MAINFRAME, CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR);
	//m_pReBar->AddBar(&m_wndDlgBar);[/pre]
	// 
	// Returns nonzero if successful, otherwise returns zero.
	//
	BOOL AddBar(
		// A pointer to a CWnd object that is the child window to be inserted
		// into the rebar.  The referenced object must have a WS_CHILD.
		CWnd* pBar,
        // Points to a REBARBANDINFO struct that contains information about
		// the band added to a rebar control.
		REBARBANDINFO* pRBBI);

	// Call this function to store information about the rebar owned by the
	// frame window. This information can be read from the initialization
	// file using LoadState. Information stored includes visibility, 
	// horizontal and vertical orientation, band size, and control bar position.
	//
	void SaveState(
		// Name of a section in the initialization file or a key in the Windows 
		// registry where state information is stored.
		LPCTSTR lpszSection);

	// Call this function to restore the settings of the rebar owned by the
	// frame window. This information is written to the initialization file
	// using SaveState. Information restored includes visibility, 
	// horizontal and vertical orientation, band size, and control bar position.
	//
	void LoadState(
		// Name of a section in the initialization file or a key in the Windows 
		// registry where state information is stored.
		LPCTSTR lpszSection);

	//:Ignore
	//{{AFX_VIRTUAL(CXTReBar)
	public:
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget,BOOL bDisableIfNoHndler);
	virtual int OnToolHitTest(CPoint point,TOOLINFO* pTI) const;
	virtual CSize CalcFixedLayout(BOOL bStretch,BOOL bHorz);
	virtual CSize CalcDynamicLayout(int nLength,DWORD nMode);
	protected:
	virtual LRESULT WindowProc(UINT message,WPARAM wParam,LPARAM lParam);
	//}}AFX_VIRTUAL
	//:End Ignore

	//:Ignore
    int m_iBandCount;
    //{{AFX_MSG(CXTReBar)
	afx_msg BOOL OnNcCreate(LPCREATESTRUCT);
	afx_msg void OnPaint();
	afx_msg void OnHeightChange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNcPaint();
	afx_msg void OnNcCalcSize(BOOL, NCCALCSIZE_PARAMS*);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnShowBand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRecalcParent();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnChildSize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnIdealSizeChanged(WPARAM wParam, LPARAM lParam);
	
	void OnBarDestroyed(CControlBar* pBar);
	void SaveCustomBars(LPCTSTR pszSection);
	void LoadCustomBars(LPCTSTR pszSection);
	void CreateBar(CXTNewCustomBarRequest* pRequest);
public: 
	void PlaceNewBar(CFrameWnd* pFrame, CControlBar* pBar);
protected:
    //:End Ignore
	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTReBarCtrl& CXTReBar::GetReBarCtrl() const {
	return *(CXTReBarCtrl*)this;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTREBAR_H__)
