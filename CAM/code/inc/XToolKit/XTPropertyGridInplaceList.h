//:Ignore
// XTPropertyGridInplaceList.h interface for the CXTPropertyGridInplaceList class.
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

#if !defined(__XTPROPERTYGRIDINPLACELIST_H__)
#define __XTPROPERTYGRIDINPLACELIST_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// class forwards.
class CXTPropertyGridView;
class CXTPropertyGridItem;

//////////////////////////////////////////////////////////////////////
// CXTPropertyGridInplaceList is a CListBox derived class. 
// It is an internal class used by Property Grid control
//
class CXTPropertyGridInplaceList : public CListBox
{
	DECLARE_DYNAMIC(CXTPropertyGridInplaceList)

public:

	CXTPropertyGridInplaceList();
	virtual ~CXTPropertyGridInplaceList();

	virtual void Create(CXTPropertyGridItem* pItem, CRect rect);
	virtual void DestroyItem();

private:

	CXTPropertyGridItem* m_pItem;

protected:

    //{{AFX_VIRTUAL(CXTPropertyGridInplaceList)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
    //}}AFX_VIRTUAL

	virtual void Cancel(void);
	virtual void Apply(void);

    //{{AFX_MSG(CXTPropertyGridInplaceList)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
    //}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // #if !defined(__XTPROPERTYGRIDINPLACELIST_H__)

