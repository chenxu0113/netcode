//:Ignore
// XTMiniDockFrameWnd.h interface for the CXTMiniDockFrameWnd class.
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

#if !defined(__XTMINIDOCKFRAMEWND_H__)
#define __XTMINIDOCKFRAMEWND_H__

//:Ignore
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
//:End Ignore

// forwards
class CXTDockBar;
class CXTControlBar;
class CXTDockContext;
class CXTFrameButton;
class CXTExpMenuWnd;

//////////////////////////////////////////////////////////////////////
// CXTMiniDockFrameWnd is a CMiniDockFrameWnd class.  It is used to
// handle docking for the CXTDockWindow class.
//
class _XT_EXT_CLASS CXTMiniDockFrameWnd : public CMiniDockFrameWnd
{
	DECLARE_DYNCREATE(CXTMiniDockFrameWnd)

public:

    // Constructs a CXTMiniDockFrameWnd object.
    //
	CXTMiniDockFrameWnd();

	// Destroys a CXTMiniDockFrameWnd object, handles cleanup and de-allocation.
    //
    virtual ~CXTMiniDockFrameWnd();

protected:

	//:Ignore
    enum APPEARANCE_STYLE
    {
        APPEARANCE_UNDEFINED,
        APPEARANCE_XPSTYLE,
        APPEARANCE_CLASSIC,        
    // appearance as most recently rendered
	} m_currentAppearance;  
    
	// prescribed current appearance
	static APPEARANCE_STYLE GetCurrentAppearance();  

	CPtrList m_arrButtons;

	CXTExpMenuWnd*  m_pPopupWnd;
	CXTFrameButton* m_pPressedBtn;
	CToolTipCtrl    m_toolTip;
	// Image list used by mini-frame buttons.
	CImageList      m_imageList;          
	// tells if initial update completed
	bool m_bInitCompleted;
	
    //{{AFX_VIRTUAL(CXTMiniDockFrameWnd)
	public:
	virtual BOOL Create(CWnd* pParent, DWORD dwBarStyle);
	//}}AFX_VIRTUAL
    virtual bool IsControlBar(CXTControlBar* pBar);
    virtual bool IsToolbar(CXTControlBar* pBar);
    virtual bool IsMenubar(CXTControlBar* pBar);
    virtual bool IsDockWindow(CXTControlBar* pBar);
    virtual CXTDockBar* GetXTDockBar() const;
    virtual CXTControlBar* GetXTControlBar() const;
    virtual CXTDockContext* GetXTDockContext(CXTControlBar* pBar) const;
	virtual void DrawXPFrameControl(CDC* pDC, CRect& r, CXTFrameButton* pFrameButton);
	virtual void ActivateToolTips(CPoint point, bool bIsCloseButton);
	virtual void CalcWindowRect(LPRECT lpClientRect,
						UINT nAdjustType = adjustBorder);
	//:End Ignore

	//:Ignore
    //{{AFX_MSG(CXTMiniDockFrameWnd)
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnNcPaint();
	afx_msg BOOL OnNcActivate(BOOL bActive);
    afx_msg void OnIdleUpdateCmdUI();
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnCustomize();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
    afx_msg LRESULT OnInitMiniFrame(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetText(WPARAM, LPARAM);
    afx_msg void OnPopupClosed();
    //:End Ignore
	DECLARE_MESSAGE_MAP()

	friend class CDockBar;
};

//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTMINIDOCKFRAMEWND_H__)

