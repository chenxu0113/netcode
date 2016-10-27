// DVSShowView.cpp : implementation of the CDVSShowView class
//

#include "stdafx.h"
#include "DVSShow.h"

#include "DVSShowDoc.h"
#include "DVSShowView.h"
#include "MainFrm.h"
#include "DlgVideo1.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDVSShowView

IMPLEMENT_DYNCREATE(CDVSShowView, CView)

BEGIN_MESSAGE_MAP(CDVSShowView, CView)
	//{{AFX_MSG_MAP(CDVSShowView)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDVSShowView construction/destruction

CDVSShowView::CDVSShowView()
{
	// TODO: add construction code here

}

CDVSShowView::~CDVSShowView()
{
}

BOOL CDVSShowView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDVSShowView drawing

void CDVSShowView::OnDraw(CDC* pDC)
{
	CDVSShowDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CDVSShowView printing

BOOL CDVSShowView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDVSShowView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDVSShowView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDVSShowView diagnostics

#ifdef _DEBUG
void CDVSShowView::AssertValid() const
{
	CView::AssertValid();
}

void CDVSShowView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDVSShowDoc* CDVSShowView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDVSShowDoc)));
	return (CDVSShowDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDVSShowView message handlers
