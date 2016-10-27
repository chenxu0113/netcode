//:Ignore
// XTCaptionPopupWnd.h : interface for the CXTCaptionPopupWnd class.
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

#if !defined(__XTCAPTIONPOPUPWND_H__)
#define __XTCAPTIONPOPUPWND_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTCaptionPopupWnd is a CWnd derived class.  It is used by the CXTCaption
// class to display a popup child window similar to Outlook&trade;.
//
class _XT_EXT_CLASS CXTCaptionPopupWnd : public CWnd
{
	DECLARE_DYNAMIC(CXTCaptionPopupWnd)

public:

    // Constructs a CXTCaptionPopupWnd object.
    //
	CXTCaptionPopupWnd();

	// Destroys a CXTCaptionPopupWnd object, handles cleanup and de-allocation.
    //
    virtual ~CXTCaptionPopupWnd();

protected:

	// A CWnd pointer that represents the parent window.
	CWnd* m_pParentWnd;
	// A CWnd pointer that represents the child displayed in the popup.
	CWnd* m_pChildWnd;
	// A CWnd pointer that represents the parent of the child window.
	CWnd* m_pChildParent;
	// Caption that is displayed when the window is active.
	CXTCaption m_Caption;
	// Close button associated with the caption.
	CXTButton m_CaptionButton;
	// Handle to a normal button icon.
	CXTIconHandle m_hIconOn;
	// Handle to a pressed button icon.
	CXTIconHandle m_hIconOff;

public:
	
    // This member function is called to create and display the popup
    // window.  Returns TRUE if successful, otherwise returns FALSE.
    //
	virtual BOOL Create(
		// A CRect reference that represents the size of the popup window.
        const CRect& rect,
		// A CWnd pointer that represents the popup window.
        CWnd* pParentWnd,
		// A CWnd pointer that represents the child to be displayed when the
		// window is activated.
        CWnd* pChildWnd);

	//:Ignore
    //{{AFX_VIRTUAL(CXTCaptionPopupWnd)
	//}}AFX_VIRTUAL
    //:End Ignore

protected:

	//:Ignore
    //{{AFX_MSG(CXTCaptionPopupWnd)
	afx_msg void OnDestroy();
	afx_msg void OnCaptButton();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
    //:End Ignore
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTCAPTIONPOPUPWND_H__)
