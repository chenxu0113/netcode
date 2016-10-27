//:Ignore
// XTPropertyGridInplaceEdit.h interface for the CXTPropertyGridInplaceEdit class.
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

#if !defined(__XTPROPERTYGRIDINPLACEEDIT_H__)
#define __XTPROPERTYGRIDINPLACEEDIT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// class forwards.
class CXTPropertyGridView;
class CXTPropertyGridItem;

//////////////////////////////////////////////////////////////////////
// CXTPropertyGridInplaceEdit is a CEdit derived class. 
// It is an internal class used by Property Grid control
//
class CXTPropertyGridInplaceEdit : public CEdit
{
	DECLARE_DYNAMIC(CXTPropertyGridInplaceEdit)

public:
	CXTPropertyGridInplaceEdit();
	virtual ~CXTPropertyGridInplaceEdit();

	virtual void SetValue(CString strValue);
	virtual void HideWindow();
	virtual void Create(CXTPropertyGridItem* pItem, CRect rect);
	virtual void DestroyItem();

	CXTPropertyGridItem* GetItem();

private:

    CString              m_strValue;
    CBrush               m_brBack;
    CWnd*                m_pGrid;
    CXTPropertyGridItem* m_pItem;

protected:

    //{{AFX_VIRTUAL(CXTPropertyGridInplaceEdit)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL

	virtual BOOL SelectConstraint(int nDirection, BOOL bCircle);

    //{{AFX_MSG(CXTPropertyGridInplaceEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnEnKillfocus();
	afx_msg void OnEnSetfocus();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    //}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	friend class CXTPropertyGridItem;
};

AFX_INLINE CXTPropertyGridItem* CXTPropertyGridInplaceEdit::GetItem() {
	return m_pItem;
}

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTPROPERTYGRIDINPLACEEDIT_H__)
