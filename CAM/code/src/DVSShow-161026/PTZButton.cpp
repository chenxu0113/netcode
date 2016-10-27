
#include "stdafx.h"
#include "DVSShow.h"
#include "PTZButton.h"
#include "DlgOutput.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int g_iPtzBtnIndex = 0;
extern CDVSShowApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPTZButton
/*********************************************************
  Function:	CPTZButton
  Desc:		Constructor
  Input:	
  Output:	
  Return:	
**********************************************************/
CPTZButton::CPTZButton()
: m_lPlayHandle(-1)
{
	m_iSubBtnIndex = g_iPtzBtnIndex++;
}

/*********************************************************
  Function:	~CPTZButton
  Desc:		destructor
  Input:	
  Output:	
  Return:	
**********************************************************/
CPTZButton::~CPTZButton()
{
}


/*********************************************************
Function:	BEGIN_MESSAGE_MAP
Desc:		the map between control and function
Input:	
Output:	
Return:	
**********************************************************/
BEGIN_MESSAGE_MAP(CPTZButton, CBCGPButton)
	//{{AFX_MSG_MAP(CPTZButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPTZButton message handlers
/*********************************************************
  Function:	OnLButtonDown
  Desc:		按下鼠标左键的响应函数,处理对应的云台操作
  Input:	nFlags,功能键状态;point,鼠标位置
  Output:	
  Return:	
**********************************************************/
void CPTZButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bRet = FALSE;
	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();
	if (NULL == pVideo1)
		return;

	int iDevIndex =  pVideo1->m_nSubWndIndex;
	int iChanIndex = pVideo1->m_nCurChanNum;

	m_lPlayHandle = theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_lPlayHandle;

	DLL_DVR_PTZControl(&theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_TSCamDef, m_iSubBtnIndex, (BYTE)theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].nSpeed, FALSE);

	CBCGPButton::OnLButtonDown(nFlags, point);
}

/*********************************************************
  Function:	OnLButtonUp
  Desc:		鼠标左键弹起的响应函数,处理对应的云台操作
  Input:	nFlags,功能键状态;point,鼠标位置
  Output:	
  Return:	
**********************************************************/
void CPTZButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	BOOL bRet = FALSE;
	CDlgVideo1* pVideo1 = theApp.GetDlgVideo1();
	if (NULL == pVideo1)
		return;

	int iDevIndex = pVideo1->m_nSubWndIndex;
	int iChanIndex = pVideo1->m_nCurChanNum;
 
	m_lPlayHandle = theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_lPlayHandle;

	DLL_DVR_PTZControl(&theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_TSCamDef, m_iSubBtnIndex, (BYTE)theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].nSpeed, TRUE);

	CBCGPButton::OnLButtonUp(nFlags, point);
}
