// WorkspaceBar.cpp : implementation of the CWorkspaceBar class
//

#include "stdafx.h"
#include "DVSShow.h"
#include "WorkspaceBar.h"
#include "DlgVideo1.h"
#include "Markup.h"
#include "VideoOutPutView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int nBorderSize = 1;
extern CDVSShowApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar

BEGIN_MESSAGE_MAP(CWorkspaceBar, CBCGPDockingControlBar)
	//{{AFX_MSG_MAP(CWorkspaceBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_NOTIFY(TVN_SELCHANGED, IDC_WSB_TREE, OnSelchangedTree)

	ON_NOTIFY(NM_RCLICK, IDC_WSB_TREE, OnRClickTree)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar construction/destruction

CWorkspaceBar::CWorkspaceBar()
{
	m_bIsRBtnDown = FALSE;
}

CWorkspaceBar::~CWorkspaceBar()
{
	m_imgList.DeleteImageList();

}

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar message handlers

int CWorkspaceBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CBCGPDockingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rectDummy;
	rectDummy.SetRectEmpty ();

	// Create tree windows.
	// TODO: create your own tab windows here:
	const DWORD dwViewStyle =	WS_CHILD | WS_VISIBLE | TVS_HASLINES | 
								TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;
	if (!m_wndTree.Create (dwViewStyle, rectDummy, this, IDC_WSB_TREE))
	{
		TRACE0("Failed to create workspace view\n");
		return -1;      // fail to create
	}

	CBitmap bmp;
	bmp.LoadBitmap(IDB_IL_TREE);
	m_imgList.Create(16, 16, ILC_COLOR32|ILC_MASK, 0, 2);
	m_imgList.Add(&bmp, RGB(0, 0, 0));
	m_wndTree.SetImageList(&m_imgList, TVSIL_NORMAL);
	
	BOOL bFlag = FALSE;
	bFlag = InitTree();

	CFont font;
	VERIFY(font.CreateFont(
		13,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		DEFAULT_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("����")));                 // lpszFacename

	
	CClientDC dc(this);  
	dc.SetBkMode(TRANSPARENT); 
	dc.SetTextColor(RGB(128, 0, 0));
	font.Detach();
	font.DeleteObject();

	return 0;
}

void CWorkspaceBar::OnSize(UINT nType, int cx, int cy) 
{
	CBCGPDockingControlBar::OnSize(nType, cx, cy);

	m_wndTree.SetWindowPos (NULL, nBorderSize, nBorderSize, 
		cx - 2 * nBorderSize, cy - 2 * nBorderSize,
		SWP_NOACTIVATE | SWP_NOZORDER);
}

void CWorkspaceBar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rectTree;
	m_wndTree.GetWindowRect (rectTree);
	ScreenToClient (rectTree);

	rectTree.InflateRect (nBorderSize, nBorderSize);
	dc.Draw3dRect (rectTree,	::GetSysColor (COLOR_3DSHADOW), 
								::GetSysColor (COLOR_3DSHADOW));
}

void CWorkspaceBar::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult)
{

}

void CWorkspaceBar::OnRClickTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	
	*pResult = 0;
}


// ��ʼ�����ṹ-�ݹ鷽ʽ
BOOL CWorkspaceBar::InitTree(HTREEITEM hParent/* =TVI_ROOT */)
{
	AfxGetApp()->DoWaitCursor(1);

	m_wndTree.DeleteAllItems();
	LONG lStyle = GetWindowLong(m_wndTree.m_hWnd,GWL_STYLE);
	SetWindowLong(m_wndTree.m_hWnd,GWL_STYLE,
				lStyle | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS);

	CMainFrame* pFrame = (CMainFrame*)theApp.GetMainFrame();
	m_wndTree.SetImageList(&pFrame->m_ilTreeImage,TVSIL_NORMAL);                 //��ͼ����������Ϳؼ�����   
	
	HTREEITEM hRootItem = NULL;

	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////
	CMarkup xml;
	DEV_INFO devInfo;

	devInfo.nPort=0;
	devInfo.lLoginID=0;
	devInfo.lID=0;

	if(xml.Load(GET_MODULE_FILE_INFO.strPath + "UserInfo.xml"))
	{
		while(xml.FindChildElem("ip"))
		{
			//read the information from XML
			CString strIP="",strUserName="",strPsw="",strDevName="";
			UINT nPort;
			long byChanNum=0,lID=0;

			UINT bSerialID,nSerPort;
			CString szSerIP="",szSerialInfo="";			
			xml.IntoElem();		

			xml.FindChildElem("ip2");
			strIP=xml.GetChildData();
			xml.FindChildElem("DEVICENAME");
			strDevName=xml.GetChildData();
			xml.FindChildElem("username");
			strUserName=xml.GetChildData();
			xml.FindChildElem("port");
			nPort=atoi(xml.GetChildData());
			xml.FindChildElem("pwd");
			strPsw=xml.GetChildData();
			xml.FindChildElem("byChanNum");
			byChanNum=atoi(xml.GetChildData());
			xml.FindChildElem("lID");
			lID=atoi(xml.GetChildData());

			xml.FindChildElem("bSerialID");
			bSerialID=atoi(xml.GetChildData());
			xml.FindChildElem("szSerIP");
			szSerIP=xml.GetChildData();
			xml.FindChildElem("nSerPort");
			nSerPort=atoi(xml.GetChildData());
			xml.FindChildElem("szSerialInfo");
			szSerialInfo=xml.GetChildData();//����ddns��¼
			xml.FindChildElem("LoginType");
			int LoginType=atoi(xml.GetChildData());
			devInfo.devctype=LoginType;

			xml.OutOfElem();

			devInfo.nTotalChannel =byChanNum;
			devInfo.nPort = nPort;

			devInfo.bSerialID=bSerialID;		
			devInfo.nSerPort=nSerPort;
			strcpy(devInfo.szSerIP,szSerIP.GetBuffer(0));		
			strcpy(devInfo.szSerialInfo, szSerialInfo.GetBuffer(0));//����ddns��¼		
			strcpy(devInfo.szDevName, strDevName);
			strcpy(devInfo.szUserName, strUserName);
			strcpy(devInfo.szPsw, strPsw);
			strcpy(devInfo.szIpaddress, strIP);
			DEV_INFO *pDev = new DEV_INFO;
			memset( pDev, 0, sizeof(DEV_INFO) );
			memcpy( pDev, &devInfo, sizeof(DEV_INFO) );
			pDev->lID = (long)pDev;
			m_devMap[pDev->lID] = pDev;
			HTREEITEM hAddItem = m_wndTree.InsertItem(strDevName);
//			m_wndTree.SetItemData(hAddItem, (DWORD)pDev);

			VIDEOPARAM* video = new VIDEOPARAM;
			video->lChannel = 0;
			strcpy(video->devinfo.szIpaddress, devInfo.szIpaddress);
			strcpy(video->devinfo.szPsw, devInfo.szPsw);
			strcpy(video->devinfo.szUserName, devInfo.szUserName);
			strcpy(video->devinfo.szDevName, devInfo.szDevName);
			video->devinfo.nPort = devInfo.nPort;
			video->nCameraNo = 1;	//��Ҫ����ʵ���޸�??????
			video->devinfo = *pDev;

			CString sKey = "";
			sKey.Format("%s,%d", video->devinfo.szIpaddress, video->lChannel);
			theApp.m_mapVideoParam.SetAt(sKey, video);

			CString strName("");
			for ( int i = 0; i < byChanNum; i ++)
			{
				strName.Format("CAM %d", i+1);
				HTREEITEM item = m_wndTree.InsertItem(strName, 0, 0, hAddItem);
//				m_wndTree.SetItemData(item, (DWORD)i);
				m_wndTree.SetItemData(item, (DWORD)video);
			}
		}
	}

	m_wndTree.Expand(hRootItem, TVE_EXPAND);
	m_wndTree.Invalidate();
	AfxGetApp()->DoWaitCursor(-1);

	return TRUE;

}


BOOL CWorkspaceBar::PreTranslateMessage(MSG* pMsg) 
{
	//���˫��ʱ������Ƶ
	if(pMsg->message == WM_LBUTTONDBLCLK)
	{
		CPoint point;
		if(!GetCursorPos(&point))
			return FALSE;

		m_wndTree.ScreenToClient(&point);
		UINT hitFlags = 0;
		HTREEITEM hSelectItem = m_wndTree.HitTest(point, &hitFlags);

		m_wndTree.ClientToScreen(&point);

		if(hSelectItem == NULL)
			return FALSE;
		
		CMainFrame* pFrame = theApp.GetMainFrame();
		CDlgVideo1* pVideo1 = (CDlgVideo1*)theApp.GetDlgVideo1();
		HTREEITEM parentItem = m_wndTree.GetParentItem(hSelectItem);
		if (!parentItem)//������Ǹ���㣬ֱ�ӷ��أ�ֻ��˫����������ʱ���Ŵ�ʵʱ��Ƶ
			return CBCGPDockingControlBar::PreTranslateMessage(pMsg);
		
		AfxGetApp()->DoWaitCursor(1);
		pFrame->m_bIsCameraNode = TRUE;
		HTREEITEM hRootItem = m_wndTree.GetRootItem();
		if (hSelectItem == hRootItem)
		{
			pFrame->m_bIsCameraNode = FALSE;
			AfxGetApp()->DoWaitCursor(-1);
			return FALSE;
		}
		//login��˫��������������
		VIDEOPARAM* video = (VIDEOPARAM*)m_wndTree.GetItemData(hSelectItem);
		if (!theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_bStartVideo)
		{
			pVideo1->StopRTVideoEx(pVideo1->m_nSubWndIndex);
		}
		pVideo1->StartRTVideoEx(video, -1, TRUE, STYLE_FLASH);
		AfxGetApp()->DoWaitCursor(-1);
			
		return TRUE;
	}
	else if (pMsg->message == WM_RBUTTONUP || pMsg->message == WM_RBUTTONDOWN)
	{
		CPoint point;
		if(!GetCursorPos(&point))
			return TRUE;

		m_wndTree.ScreenToClient(&point);
		UINT hitFlags = 0;
		HTREEITEM hSelectItem = m_wndTree.HitTest(point, &hitFlags);

		m_wndTree.ClientToScreen(&point);

		if(hSelectItem == NULL)
			return TRUE;

		m_wndTree.SelectItem(hSelectItem);
		return TRUE;
	}

	return CBCGPDockingControlBar::PreTranslateMessage(pMsg);
}



