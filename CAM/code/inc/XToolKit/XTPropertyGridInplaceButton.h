//:Ignore
// XTPropertyGridInplaceButton.h interface for the CXTPropertyGridInplaceButton class.
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

#if !defined(__XTPROPERTYGRIDINPLACEBUTTON_H__)
#define __XTPROPERTYGRIDINPLACEBUTTON_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// class forwards
class CXTPropertyGridView;
class CXTPropertyGridItem;

//////////////////////////////////////////////////////////////////////
// CXTPropertyGridInplaceButton is a CStatic derived class. 
// It is an internal class used by Property Grid control
//
class CXTPropertyGridInplaceButton : public CStatic
{
	DECLARE_DYNAMIC(CXTPropertyGridInplaceButton)

public:
	
	CXTPropertyGridInplaceButton();
	virtual ~CXTPropertyGridInplaceButton();

	virtual void HideWindow();
	virtual void Create(CXTPropertyGridItem* pItem, CRect rect);
	virtual void DestroyItem();

private:
	
    CWnd*                m_pGrid;
    CXTPropertyGridItem* m_pItem;
    BOOL                 m_bPressed;
    BOOL                 m_bOver;

protected:

    //{{AFX_VIRTUAL(CXTPropertyGridInplaceButton)
    //}}AFX_VIRTUAL

    //{{AFX_MSG(CXTPropertyGridInplaceButton)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
    //}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	friend class CXTPropertyGridItem;
};

//////////////////////////////////////////////////////////////////////
//:End Ignore

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif //#if !defined(__XTPROPERTYGRIDINPLACEBUTTON_H__)