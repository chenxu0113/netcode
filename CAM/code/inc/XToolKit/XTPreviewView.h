//:Ignore
// XTPreviewView.h interface for the CXTPreviewView class.
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

#if !defined(__XTPREVIEWVIEW_H__)
#define __XTPREVIEWVIEW_H__

//:Ignore
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//:End Ignore

//////////////////////////////////////////////////////////////////////
// CXTPreviewView is a CPreviewView derived class.  CXTPreviewView is used
// to create an alternate print preview dialog.  In derived classes, implement
// special window handling here. 
// 
//[pre]// must not create this on the frame.  Must outlive this function
//CPrintPreviewState* pState = new CPrintPreviewState;[/pre]
// 
// DoPrintPreview's return value does not necessarily indicate that
// Print preview succeeded or failed, but rather what actions are necessary
// at this point.  If DoPrintPreview returns TRUE, it means that
// OnEndPrintPreview will be (or has already been) called and the
// 'pState' structure will be or has been deleted.  If DoPrintPreview returns
// FALSE, it means that OnEndPrintPreview [b]WILL NOT[/b] be called, and that cleanup,
// including deleting 'pState', must be done here.
//
// To use, override CView::OnFilePrintPreview in your view class, and add 
// the following code:
// 
//[pre]if (!DoPrintPreview(XT_IDD_PREVIEW_TOOLBAR, this, RUNTIME_CLASS(CXTPreviewView), pState))
//{
//    // In derived classes, reverse special window handling here for
//    // Preview failure case
//    TRACE0("Error: DoPrintPreview failed.\n");
//
//    AfxMessageBox(AFX_IDP_COMMAND_FAILURE);
//
//    delete pState;      // preview failed to initialize, delete State now
//}[/pre]
//
class _XT_EXT_CLASS CXTPreviewView : public CPreviewView
{
    DECLARE_DYNCREATE(CXTPreviewView)

protected:
    
    // Constructs a CXTPreviewView object.
    //
    CXTPreviewView();

    // Destroys a CXTPreviewView object, handles cleanup and de-allocation.
    //
    virtual ~CXTPreviewView();

    // Print button.
	CXTButton m_btnPrint;
	// Next button.
	CXTButton m_btnNext;
	// Previous button.
	CXTButton m_btnPrevious;
	// One / two page toggle button.
	CXTButton m_btnNumPage;
	// Zoom-in button.
	CXTButton m_btnZoomIn;
	// Zoom-out button.
	CXTButton m_btnZoomOut;
	// Close button.
	CXTButton m_btnClose;
	// Image list for preview buttons.
	CImageList m_ImageList;

    // This member function will subclass a print preview button.
    //
    void SubclassPreviewButton(
        // Resource ID of the print preview button to subclass.
        UINT nID,
        // Index into the button imagelist.
        int nImage,
        // A reference to the CXTButton object to subclass.
        CXTButton& button);

    //:Ignore
    //{{AFX_VIRTUAL(CXTPreviewView)
    //}}AFX_VIRTUAL
    //:End Ignore

    //:Ignore
    //{{AFX_MSG(CXTPreviewView)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnUpdateNumPageChange(CCmdUI* pCmdUI);
    //}}AFX_MSG
	afx_msg LRESULT OnInitToolbar(WPARAM wParam, LPARAM lParam);
    //:End Ignore
    DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTPREVIEWVIEW_H__)
