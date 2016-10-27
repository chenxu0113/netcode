//:Ignore
// XTTipOfTheDay.h interface for the CXTTipOfTheDay class.
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

#if !defined(__XTTIPOFTHEDAY_H__)
#define __XTTIPOFTHEDAY_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTTipOfTheDay is a multiple inheritance class derived from CXTDialogState
// and CDialog. CXTTipOfTheDay is used to create a Visual Studio&trade; style
// Tip Of The Day dialog.  To use, place a file called "tips.txt" in the
// same directory as your application exe.  Place each tip on its own line.
//
class _XT_EXT_CLASS CXTTipOfTheDay : CXTDialogState, public CDialog
{
    DECLARE_DYNAMIC(CXTTipOfTheDay)

public:

    // Constructs a CXTTipOfTheDay object.
    //
    CXTTipOfTheDay(
        // A NULL terminated string that represents the path and file name
		// of where the tips text file is located.  By default, the file name
		// is set to "tips.txt".
        LPCTSTR lpszTipFile=NULL,
        // Points to the parent window for the Tip Of The Day Dialog.
        CWnd* pParent = NULL);

    // Destroys a CXTTipOfTheDay object, handles cleanup and de-allocation.
    //
    virtual ~CXTTipOfTheDay();

protected:

    // Default font used for "Did you know..." text.
	CFont m_fontTitle;
	// Default font used for tips.
	CFont m_fontTip;
	// User defined font for "Did you know..." text.
	CFont* m_pFontTitle;
	// User defined font for tips.
	CFont* m_pFontTip;
	// Represents the "Did you know" text.
	CString m_strTipTitle;
	// Represents the Tip Of The Day text.
	CString m_strTipText;
	// Represents the file name and path for the tips file.
	CString m_strTipFile;
	// A pointer to the open file stream.
	FILE* m_pStream;
	// Size of the total display area.
	CRect m_rcBorder;
	// Size of the shadowed rect displayed to the left of the tip.
	CRect m_rcShadow;
	// Size of the background area the tips are displayed on.
	CRect m_rcHilite;
	// Size of the display area for tip text.
	CRect m_rcTipText;

public:

    // This member function will set the fonts to be used by the "Did you
	// know..." and the Tip Of The Day text.
    //
    virtual void SetDefaultFonts(
        // Points to a CFont object that represents the new
        // font to be used for the "Did you know..." text.
        CFont* pFontTitle,
        // Points to a CFont object that represents the new
        // font to be used for the Tip Of The Day text.
        CFont* pFontTip);

    // This member function will retrieve the next string to be displayed
	// as the Tip Of The Day.
    //
    virtual void GetNextTipString(
        // A CString reference that represents the next
        // Tip Of The Day text that is to be displayed.
        CString& strNext);

    // This member function will set the text that is to be displayed
    // in place of the "Did you know..." string.
    //
    virtual void SetDefaultTitle(
        // Represents a NULL terminated string that is
        // the string to be displayed in place of the
        // "Did you know..." text.
        LPCTSTR lpszTitle);

    // This member function will set the path to where the tips file is
	// located.
    //
    virtual void SetTipsFilePath(
        // A NULL terminated string that represents the full
        // path to where the tips text file is located.
        LPCTSTR lpszTipFile);

    //:Ignore
    //{{AFX_DATA(CXTTipOfTheDay)
    enum { IDD = XT_IDD_TIPOFTHEDAY };
    CButton m_ok;
    CButton m_showTips;
    CButton m_btnNextTip;
    CStatic m_staticBorder;
    BOOL    m_bStartup;
    //}}AFX_DATA
    //:End Ignore

    //:Ignore
    //{{AFX_VIRTUAL(CXTTipOfTheDay)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    //:End Ignore

protected:

    //:Ignore
    //{{AFX_MSG(CXTTipOfTheDay)
    afx_msg void OnPaint();
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    afx_msg void OnDaytipNext();
    //}}AFX_MSG
    //:End Ignore
    DECLARE_MESSAGE_MAP()
};

//:Ignore
//////////////////////////////////////////////////////////////////////

AFX_INLINE void CXTTipOfTheDay::SetDefaultTitle(LPCTSTR lpszTitle) {
    m_strTipTitle = lpszTitle;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTTIPOFTHEDAY_H__)
