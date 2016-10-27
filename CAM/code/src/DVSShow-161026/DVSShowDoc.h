// DVSShowDoc.h : interface of the CDVSShowDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DVSSHOWDOC_H__F486B27B_39E9_41F6_968A_0CB3D42777C3__INCLUDED_)
#define AFX_DVSSHOWDOC_H__F486B27B_39E9_41F6_968A_0CB3D42777C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDVSShowDoc : public CDocument
{
protected: // create from serialization only
	CDVSShowDoc();
	DECLARE_DYNCREATE(CDVSShowDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDVSShowDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDVSShowDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDVSShowDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DVSSHOWDOC_H__F486B27B_39E9_41F6_968A_0CB3D42777C3__INCLUDED_)
