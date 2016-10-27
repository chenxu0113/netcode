// VideoOutPutView.cpp : implementation file
//

#include "stdafx.h"
#include "VideoOutPutView.h"
#include "DVSShow.h"
#include "DlgVideo1.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "include/HCNetSDK.h"
#include "include/plaympeg4.h"

HWND g_hWnd = NULL;
void CALLBACK g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser);
void CALLBACK RemoteDisplayCBFun(long nPort,char * pBuf,long nSize,long nWidth,long nHeight,long nStamp,long nType,long nReserved);
extern CDlgOutput g_dlgOutput[MAX_VIDEO];
extern CDlgVideo1 *g_pDlgVideo1;
/////////////////////////////////////////////////////////////////////////////
// CVideoOutPutView

IMPLEMENT_DYNCREATE(CVideoOutPutView, CView)

CVideoOutPutView::CVideoOutPutView()
{
	DVR_Init();
}

CVideoOutPutView::~CVideoOutPutView()
{
}


BEGIN_MESSAGE_MAP(CVideoOutPutView, CView)
	//{{AFX_MSG_MAP(CVideoOutPutView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoOutPutView drawing

void CVideoOutPutView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CVideoOutPutView diagnostics

#ifdef _DEBUG
void CVideoOutPutView::AssertValid() const
{
	CView::AssertValid();
}

void CVideoOutPutView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVideoOutPutView message handlers
void CALLBACK RemoteDisplayCBFun(long nPort,char * pBuf,long nSize,long nWidth,long nHeight,long nStamp,long nType,long nReserved)
{
}
void CALLBACK g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser)
{
	BOOL iRet = FALSE;
	DWORD dwWinIndex = *((unsigned long *)pUser);
	if (dwWinIndex < 0 || dwWinIndex > MAX_VIDEO)			// MSW 100925 ADD
		return ;
	//get player port to protect the process safe
	LONG lPort = g_dlgOutput[dwWinIndex].m_lPort;//-1;//software decode play handle of software decode
	HANDLE hCardHandle = 0;//g_dlgOutput[dwWinIndex].m_hCardHandle;//hardware decode play handle of software decode

	switch (dwDataType)
	{
	case NET_DVR_SYSHEAD://coming the stream header, open stream
		///////////////////////////////////////////////////////////////////
		//soft decode

		PlayM4_GetPort(&lPort);
		//set the port of the out put window

		if (dwBufSize > 0)
		{
			//set as stream mode, real-time stream under preview
			PlayM4_SetStreamOpenMode(lPort, STREAME_REALTIME);
			//start player
			if (!PlayM4_OpenStream(lPort, pBuffer, dwBufSize, 1024*1024))
			{
				break;
			}
			//set soft decode display callback function to capture
			PlayM4_SetDisplayCallBack(lPort, RemoteDisplayCBFun);

			
			//start play, set play window
			if (!PlayM4_Play(lPort, g_hWnd))
			{
				break;
			}

			//set frame buffer number
			PlayM4_SetDisplayBuf(lPort, 0x0000000f);//g_dlgOutput[dwWinIndex].m_iNetDelayBufNum);
			
			//set display mode
			PlayM4_SetOverlayMode(lPort, FALSE, COLORREF(0));//play off screen
			g_dlgOutput[dwWinIndex].m_lPort	= lPort;
		}
		break;
	default:
		break;
	}

	return ;
}

// 初始化
BOOL CVideoOutPutView::DVR_Init()
{
	return NET_DVR_Init();
}

UINT CVideoOutPutView::DVR_StartVideo(VIDEOPARAM* pVideoParam, long& lPlayHandle, int nWndIndex)
{
	if (pVideoParam->lChannel < 1)	// 通道号错误
		return NET_DVR_CHANNEL_ERROR;

	CString str;
	str.Format(_T("%s"), pVideoParam->szUserName);
	if (!str.Compare(_T("")))	// 用户名为空或不存在
		return NET_DVR_USERNOTEXIST;

	g_hWnd = pVideoParam->hWnd;
	NET_DVR_CLIENTINFO struPlayInfo;
	memset(&struPlayInfo, 0, sizeof(NET_DVR_CLIENTINFO));
	struPlayInfo.hPlayWnd = g_hWnd;	
	struPlayInfo.lChannel = pVideoParam->lChannel;
	struPlayInfo.lLinkMode = pVideoParam->lLinkMode;
//	struPlayInfo.sMultiCastIP = pVideoParam->szMultiCastIP;// MSW 120905 DEL

//	int iSubWndIndex = 2;
	LONG lUserID = -1;
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;

	memset(&struDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));

	/*lUserID = NET_DVR_Login_V30(pVideoParam->szDVRIP, pVideoParam->wDVRPort,// MSW 120905 DEL
								pVideoParam->szUserName, pVideoParam->szPassword,
								&struDeviceInfo);*/

	if (lUserID < 0)	// 参数设置错误
	{
		return NET_DVR_GetLastError();
	}
	lPlayHandle = NET_DVR_RealPlay_V30(0, &struPlayInfo, g_RealDataCallBack_V30, &nWndIndex, TRUE);
//*/
	return NET_DVR_NOERROR;
}


UINT CVideoOutPutView::DVR_StopVideo(VIDEOPARAM* pVideoParam, long lPlayHandle)
{
	if (!NET_DVR_StopRealPlay(lPlayHandle))
		return NET_DVR_CHANNEL_ERROR;
//	if (!PlayM4_Stop(pVideoParam->wDVRPort))
	if (!PlayM4_Stop(lPlayHandle))
		return NET_DVR_CHANNEL_ERROR;
	if (!PlayM4_CloseStream(lPlayHandle))
		return NET_DVR_CHANNEL_ERROR;
	if (!PlayM4_FreePort(lPlayHandle))
		return NET_DVR_CHANNEL_ERROR;

	return NET_DVR_NOERROR;
}