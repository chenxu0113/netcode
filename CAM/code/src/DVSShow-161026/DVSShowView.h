// DVSShowView.h : interface of the CDVSShowView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DVSSHOWVIEW_H__CDF06939_1428_4948_9B82_510750EE1AE5__INCLUDED_)
#define AFX_DVSSHOWVIEW_H__CDF06939_1428_4948_9B82_510750EE1AE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DVSShowDoc.h"

class CDVSShowView : public CView
{
protected: // create from serialization only
	CDVSShowView();
	DECLARE_DYNCREATE(CDVSShowView)

// Attributes
public:
	CDVSShowDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDVSShowView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDVSShowView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDVSShowView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DVSShowView.cpp
inline CDVSShowDoc* CDVSShowView::GetDocument()
   { return (CDVSShowDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DVSSHOWVIEW_H__CDF06939_1428_4948_9B82_510750EE1AE5__INCLUDED_)
