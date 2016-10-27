//:Ignore
// XTNewToolbarDlg.h interface for the CXTNewToolbarDlg class.
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

#if !defined(__XTNEWTOOLBARDLG_H__)
#define __XTNEWTOOLBARDLG_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTNewToolbarDlg is a multiple inheritance class derived from CXTDialogState
// and CDialog. CXTNewToolbarDlg is used to create the customize dialog
// that is used during toolbar customization.
//
class _XT_EXT_CLASS CXTNewToolbarDlg : CXTDialogState, public CDialog
{

public:

    // Constructs a CXTNewToolbarDlg object.
    //
    CXTNewToolbarDlg(
        // Points to the top-level frame window.
        CFrameWnd* pWndParent=NULL);

    //:Ignore
    //{{AFX_DATA(CXTNewToolbarDlg)
    enum { IDD = XT_IDD_NEWTOOLBAR };
    CXTEdit   m_editToolbar;
    CString m_strToolbar;
    //}}AFX_DATA
    //:End Ignore

	// A NULL terminated string that represents the toolbar name.
	CString m_strExistingName;
    // ID for a newly created toolbar.
	int m_nNewID;
    // Pointer to the top-level frame window.
	CFrameWnd* m_pFrameWnd;

    // This member function is called by the dialog to determine the next
	// available suggested name to be displayed in the new toolbar dialog.
    //
    void SetSuggestedName();

    //:Ignore
    //{{AFX_VIRTUAL(CXTNewToolbarDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    //{{AFX_MSG(CXTNewToolbarDlg)
    virtual BOOL OnInitDialog();
    virtual void OnOK();
    //}}AFX_MSG
    //:End Ignore
    DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTNEWTOOLBARDLG_H__)
