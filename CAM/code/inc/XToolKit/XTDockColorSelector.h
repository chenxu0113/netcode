//:Ignore
// XTDockColorSelector.h : header file
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

#if !defined(__XTDOCKCOLORSELECTOR_H__)
#define __XTDOCKCOLORSELECTOR_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTDockColorSelector is a CXTDockWindow derived class.  It is used to create
// sizing / docking windows with color selector as a child.
//
class _XT_EXT_CLASS CXTDockColorSelector : public CXTDockWindow  
{
	DECLARE_DYNAMIC(CXTDockColorSelector)
		
public:
    // Constructs a CXTColorPopup object.
    //
	CXTDockColorSelector();

    // Destroys a CXTColorPopup object, handles cleanup and de-allocation.
    //
	virtual ~CXTDockColorSelector();

    // This member function creates a CXTDockColorSelector.
    // Returns nonzero if successful, otherwise returns zero.
    //
    BOOL Create(
        // Pointer to the window that is the control bar’s parent.
        CWnd* pParentWnd,
        // The control bar’s window ID.
        UINT nID,
        // Points to a null-terminated character string that 
        // represents the control bar name. Used as text for 
        // the caption.
        LPCTSTR lpszCaption=NULL); 

    // Call this member function to get Color Selector control.
    //
	CXTColorSelectorCtrl* GetColorSelector();

protected:

	//:Ignore
	//{{AFX_MSG(CDockWinEx1)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnButtonClose();
	afx_msg void OnUpdateButtonClose(CCmdUI* pCmdUI);
	afx_msg LRESULT OnSelEndOK(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnQueryVisualize(WPARAM, LPARAM);
	//}}AFX_MSG
	//:End Ignore
	
	DECLARE_MESSAGE_MAP()
		
private:
	
//:Ignore
	CXTColorSelectorCtrl m_wndColors;
//:End Ignore
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE 	CXTColorSelectorCtrl*  CXTDockColorSelector::GetColorSelector() {
	return &m_wndColors;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //__XTDOCKCOLORSELECTOR_H__
