//:Ignore
// XTStatusBar.h : header file
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

#ifndef __XTSTATUSBAR_H__
#define __XTSTATUSBAR_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// XT_STATUSPANE is a stand alone structure class.  It is used to create
// an XT_STATUSPANE structure.
//
struct XT_STATUSPANE
{
	// IDC of indicator: 0 => normal text area.
	UINT nID;
	// Width of the string area, in pixels.  On both sides there is a 3 pixel
	// gap and a 1 pixel border, making a pane 6 pixels wider.
	int cxText;
	// Style flags (SBPS_*).
	UINT nStyle;
	// State flags (SBPF_*).
	UINT nFlags;
	// Text in the pane.
	CString strText;
};

//////////////////////////////////////////////////////////////////////
// XT_STATUSPANE_CTRL is a stand alone helper structure class.  It is used
// by the CXTStatusBar class.
//
struct XT_STATUSPANE_CTRL
{
	// A pointer to a valid CWnd object.
	CWnd* pWnd;
	// ID of the indicator pane.
	UINT nID;
	// TRUE if the control is to be deleted when destroyed.
	BOOL bAutoDelete;
};

//////////////////////////////////////////////////////////////////////
// CXTStatusBar is a CStatusBar derived class.  It allows you to easily
// insert controls into your status bar, and remove controls from your
// status bar.
//
class _XT_EXT_CLASS CXTStatusBar : public CStatusBar
{
	DECLARE_DYNAMIC(CXTStatusBar)

public:

    // Constructs a CXTStatusBar object.
    //
	CXTStatusBar();

	// Destroys a CXTStatusBar object, handles cleanup and de-allocation.
    //
    virtual ~CXTStatusBar();

// Attributes
protected:

	// Array of pane controls.
	CArray <XT_STATUSPANE_CTRL*,XT_STATUSPANE_CTRL*> m_arControls; 

// Operations
public:

	// This member function is called by the status bar to reposition pane
	// item controls.
    //
	void PositionControls();

	// Call this member function to retrieve information for the specified
	// indicator pane. Returns a pointer to an XT_STATUSPANE_CTRL struct.
    //
	XT_STATUSPANE_CTRL* GetPaneControl(
		// ID of the indicator pane.
        UINT nID);

	// Call this member function to add a control to an indicator pane. Returns
	// TRUE if successful, otherwise returns FALSE.
    //
	BOOL AddControl(
		// Points to a control window.
        CWnd* pWnd,
		// ID of the indicator pane.
        UINT nID,
		// TRUE if the control is to be deleted when destroyed.
        BOOL bAutoDelete=TRUE);

	// Call this member function to set the width for an indicator pane.
    //
	void SetPaneWidth(
		// Index of the indicator pane.
        int nIndex,
		// New width for the indicator pane.
        int cxText);

	// Call this member function to add an indicator pane to the status bar.
	// Returns TRUE if successful, otherwise returns FALSE.
    //
	BOOL AddIndicator(
		// ID of the indicator pane.
        UINT nID,
		// Index of the indicator pane.
        int nIndex);

	// Call this member function to remove an indicator pane from the status
	// bar. Returns TRUE if successful, otherwise returns FALSE.
    //
	BOOL RemoveIndicator(
		// ID of the indicator pane.
        UINT nID);

	// Call this member function to retrieve information for an indicator
	// pane. Returns TRUE if successful, otherwise returns FALSE.
    //
	BOOL GetPaneInfoEx(
		// Index of the indicator pane.
        int nIndex,
		// Address of an XT_STATUSPANE struct.
        XT_STATUSPANE* pSBP);

	// Call this member function to set the information for an indicator pane.
	// Returns TRUE if successful, otherwise returns FALSE.
    //
	BOOL SetPaneInfoEx(
		// Index of the indicator pane.
        int nIndex,
		// Address of an XT_STATUSPANE struct.
        XT_STATUSPANE* pSBP);

	// Call this member function to get the pane information for the given
	// index. Returns a pointer to an XT_STATUSPANE struct.
    //
	XT_STATUSPANE* GetPanePtr(
		// Index of the indicator pane.
        int nIndex) const;

	// Call this member function to return the number of panes that are created
	// for the status bar. Returns an integer value that represents the number
	// of panes for the status bar. 
	//
	int GetPaneCount() const;

	//:Ignore
    //{{AFX_VIRTUAL(CXTStatusBar)
	//}}AFX_VIRTUAL
    //:End Ignore

protected:

	//:Ignore
    //{{AFX_MSG(CXTStatusBar)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnIdleUpdateCmdUI (WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
    //:End Ignore
	
	DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE int CXTStatusBar::GetPaneCount() const {
	return m_nCount;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // __XTSTATUSBAR_H__

