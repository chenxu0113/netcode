// DVSShowDoc.cpp : implementation of the CDVSShowDoc class
//

#include "stdafx.h"
#include "DVSShow.h"

#include "DVSShowDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDVSShowDoc

IMPLEMENT_DYNCREATE(CDVSShowDoc, CDocument)

BEGIN_MESSAGE_MAP(CDVSShowDoc, CDocument)
	//{{AFX_MSG_MAP(CDVSShowDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDVSShowDoc construction/destruction

CDVSShowDoc::CDVSShowDoc()
{
	// TODO: add one-time construction code here

}

CDVSShowDoc::~CDVSShowDoc()
{
}

BOOL CDVSShowDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDVSShowDoc serialization

void CDVSShowDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDVSShowDoc diagnostics

#ifdef _DEBUG
void CDVSShowDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDVSShowDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDVSShowDoc commands
