//:Ignore
// XTPopupTearOffWnd.h : header file
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

#if !defined(__XTPOPUPTEAROFFWND_H__)
#define __XTPOPUPTEAROFFWND_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//:Ignore
class CXTToolBar;
class CXTWndHook;
class CXTToolbarTearOff;
//:End Ignore

/////////////////////////////////////////////////////////////////////////////
// CXTPopupTearOffWnd is a CWnd derived class window .  It is used to display a
// "tear-off" popup window to be activated when the user presses the down arrow on
// a toolbar
//
class _XT_EXT_CLASS CXTPopupTearOffWnd : public CWnd
{
public:
    // Constructs a CXTPopupTearOffWnd object.
    //
	CXTPopupTearOffWnd(
		// Command ID that is currently active for the popup window.
		int iCmdID);

	// Destroys a CXTPopupTearOffWnd object, handles cleanup and de-allocation.
    //
	virtual ~CXTPopupTearOffWnd();

// Attributes
protected:
	
	// Pointer to the parent toolbar.
	CXTToolBar* m_pWndParent;
	// Exclusion rectangle for drawing adjacent borders.
	CRect m_rcExclude;
	// Command ID of the button that created this window.
	int m_iCmdID;
	// Child window
	CWnd* m_pChild;	
	// Message hook.
	CXTWndHook* m_pHook;	
	// If window is tear-off
	BOOL m_bTearOff;

private:	
	//:Ignore	
	BOOL m_bPressed;
	BOOL m_bHighlighted;
	CXTControlBar* m_pControlBar;
	//:End Ignore
	
// Operations
public:

	// Call this member function to create a CXTToolBarPopupWnd object.  
	//
	virtual BOOL Create(
		// Points to a valid parent toolbar.
		CXTToolBar* pParent,
		// Points to a valid child controlbar.		
		CXTControlBar* pControlBar);

	//:Ignore
	//{{AFX_VIRTUAL(CXTPopupTearOffWnd)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
	//:End Ignore

protected:
	//:Ignore	
	BOOL PtInTearOffRect(CPoint point);
	virtual void OnHookedCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnStartDrag(CPoint point);
	BOOL _Create(CXTToolBar* pParent, CWnd* pChild, CXTControlBar* pControlBar);
	virtual CWnd* CloneChild(CXTControlBar* pControlBar) = 0;
	
	//:End Ignore
			
protected:
	//:Ignore
	//{{AFX_MSG(CXTPopupTearOffWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnNcPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) ;
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	//}}AFX_MSG
	
	class CXTPopupTearOffWndWndHook;
	friend class CXTPopupTearOffWndWndHook;
	//:End Ignore
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CXTPopupColorTearOff is a CXTPopupTearOffWnd derived class window .  It is used to display a
// "tear-off" popup window with Color Selector as a child
//
class _XT_EXT_CLASS CXTPopupColorTearOff : public CXTPopupTearOffWnd
{
public:
    // Constructs a CXTPopupToolbarTearOff object.
    //
	CXTPopupColorTearOff(
		// Command ID that is currently active for the popup window.
		int iCmdID);
	
protected:
	//:Ignore
	CWnd* CloneChild(CXTControlBar* pControlBar);

	//{{AFX_MSG(CXTPopupColorTearOff)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnSelEndOK(WPARAM wParam, LPARAM lParam);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	//}}AFX_MSG
	//:End Ignore
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CXTPopupToolbarTearOff is a CXTPopupTearOffWnd derived class window .  It is used to display a
// "tear-off" popup window with Toolbar as a child
//
class _XT_EXT_CLASS CXTPopupToolbarTearOff : public CXTPopupTearOffWnd
{
public:
    // Constructs a CXTPopupToolbarTearOff object.
    //
	CXTPopupToolbarTearOff(
		// Command ID that is currently active for the popup window.
		int& iCmdID);

	// Call this member function to create a CXTPopupToolbarTearOff object.  Returns
	// TRUE if successful, otherwise returns FALSE.
	//
	virtual BOOL Create(
		// Points to a valid parent toolbar.
		CXTToolBar* pParent,
		// Points to a valid child toolbar.
		CXTToolBar* pControlBar,
		// Number of columns to display when the toolbar is displayed.
		int iNumCols=5);

//:Ignore
private:	
	int& m_iCmd;
	void OnHookedCommand(WPARAM wParam, LPARAM lParam);
	CWnd* CloneChild(CXTControlBar* pControlBar);
//:End Ignore
	
protected:
	//:Ignore
	//{{AFX_MSG(CXTPopupToolbarTearOff)
	//}}AFX_MSG
	//:End Ignore
	
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //__XTPOPUPTEAROFFWND_H__





















