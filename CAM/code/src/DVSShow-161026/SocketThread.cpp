// SocketThread.cpp : implementation file
//

#include "stdafx.h"
#include "SocketThread.h"
#include "ClientSocket.h"
#include "DVSShow.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CDVSShowApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CSocketThread

IMPLEMENT_DYNCREATE(CSocketThread, CWinThread)

CSocketThread::CSocketThread()
{
	m_hSocketEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	InitializeCriticalSection(&m_csReceMsg);
}

CSocketThread::~CSocketThread()
{
	CloseHandle(m_hSocketEvent);
	
	DeleteCriticalSection(&m_csReceMsg);
}

BOOL CSocketThread::InitInstance()
{
	HANDLE hEvents[2];
	hEvents[0] = m_hSocketEvent;
	hEvents[1] = theApp.m_hExit;

	while(1)
	{
		DWORD dwWait = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);		

		switch (dwWait) 
		{
		case WAIT_OBJECT_0:
			{
				ResetEvent(m_hSocketEvent);
				ReceMsgProcess();
			}
			break;
		case WAIT_OBJECT_0 + 1:
			return TRUE;
			
		default:
			break;
		}
	}
	
	return TRUE;
}


void CSocketThread::ReceMsgProcess()
{
	EnterCriticalSection(&m_csReceMsg);

	POSITION pos = m_listReceMsg.GetHeadPosition();
	
	while (pos)
	{
		POSITION pos1 = pos;
		BYTE* pBuf = (BYTE*)m_listReceMsg.GetNext(pos);	
		m_listReceMsg.RemoveAt(pos1);

		//////////////////////////////////////////////////////////////////////////
		CString sIP = "";
		NetworkDIDOSocket nds;
		SYSTEMTIME st;
		
		GetLocalTime(&st);
		memset(&nds, 0, sizeof(NetworkDIDOSocket));
		sIP.Format("%d.%d.%d.%d", pBuf[0], pBuf[1], pBuf[2], pBuf[3]);

		theApp.CaptureNetworkDidoSocket();
		theApp.m_mapNetworkDidoSocket.Lookup(sIP, nds);

		nds.st = st;

		WORD nValue = PROTOCOL_NONE;
		BOOL bRead = TRUE;
		if (pBuf[7 + LENGTH_4] == PROTOCOL_FUNC_W)	//写状态功能码，7代表第8位，无实际值返回
		{
			bRead = FALSE;
		//	WORD* pTemp = (WORD*)&pBuf[10 + LENGTH_4];	//第11+4个字节为value
		//	nValue = *pTemp;
		}
		else if (pBuf[7 + LENGTH_4] == PROTOCOL_FUNC_R)	//读状态功能码，7代表第8位
			nValue = pBuf[9 + LENGTH_4];	//第9+4个字节为value

		for (int i=0; i<nds.ndc.cMaxNum; i++)
		{
			unsigned char bValue = PROTOCOL_NONE;
			if (i == 0)
			{
				bValue = nValue & 0x01;//设备上送的值
			}
			else
			{
				bValue = (nValue >> i) & 0x01;//设备上送的值
			}

			//与告警值相等，产生告警，并推出视频

			//设备上送的值与初始值不相等，并且等于告警值，产生告警
			if (bValue != nds.ndc.cNum[i] && bRead)//因为写只返回成功与否，但不返回实际状态值
			{
				nds.ndc.cNum[i] = bValue;
				Add2RtAlarm(&nds, i);	//产生实时告警
			}
			//设备上送的值等于告警值，推出视频，并开启录像，只要告警不取消，会一直弹出视频
			if (nds.ndc.cNum[i] == nds.ndc.cAlarmValue && bRead)
			{
				StartRtVideo(&nds, i);
			}

		}
		theApp.m_mapNetworkDidoSocket.SetAt(sIP, nds);
		theApp.ReleaseNetworkDidoSocket();
		//////////////////////////////////////////////////////////////////////////
		
		if(pBuf)
		{
			delete []pBuf;
			pBuf=NULL;
		}
	}
	LeaveCriticalSection(&m_csReceMsg);
}

BOOL CSocketThread::StartRtVideo(const NetworkDIDOSocket* nds, int nNum)
{
	if (NULL == nds || nNum < 0)
		return FALSE;

	CDlgVideo1* pVideo1 = (CDlgVideo1*)theApp.GetDlgVideo1();//chenxu ??????
	//if (!theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_bStartVideo)//已经有视频存在，不再推出
	//{
	//	return FALSE;
	//}

	CString sIp = "";
	LinkInfoDef lid;

	AfxGetApp()->DoWaitCursor(1);
	//通过传入的网络DI/DO模块的IP、点号信息，找到摄像机与网络模块对应关系表中的摄像机IP、channel，从而获取摄像机VIDEOPARAM
	POSITION pos = theApp.m_mapLinkInfoDef.GetStartPosition();
	BOOL bFind = FALSE;
	while(pos)
	{
		sIp = "";
		memset(&lid, 0, sizeof(LinkInfoDef));
		theApp.m_mapLinkInfoDef.GetNextAssoc(pos, sIp, lid);
		if((strcmp(lid.cNetworkSocketIP, nds->ndc.szIP) == 0) && (lid.bNetworkNum == nNum))
		{
			bFind = TRUE;
			break;
		}

	}
	if (bFind == FALSE)
	{
		return FALSE;
	}
	CString sKey = "";
	VIDEOPARAM* video = NULL;

	sKey.Format("%s,%d", lid.cCamIP, lid.bChannel);
	if (!theApp.m_mapVideoParam.Lookup(sKey, video))
	{
		AfxGetApp()->DoWaitCursor(-1);
		return FALSE;
	}
	if (video->lPlayWnd != -1)
	{
		if (!theApp.m_dlgOutput[video->lPlayWnd].m_bStartVideo)//已经有视频存在，不再推出
		{
			//开启临时录像
			if (theApp.m_bnStartTempRec)
			{
				theApp.m_dlgOutput[video->lPlayWnd].StartLocalRecord();
			}
			theApp.m_dlgOutput[video->lPlayWnd].m_nAlarmFlashCount = ALARM_FLASH_TIME;
			return FALSE;
		}
	}
	pVideo1->StartRTVideoEx(video, -1, FALSE, STYLE_FLASH);

	AfxGetApp()->DoWaitCursor(-1);

	//显示告警对话框
	CMainFrame* pFrame = theApp.GetMainFrame();
	pFrame->OnViewAlarm();

	return TRUE;
}

BOOL CSocketThread::Add2RtAlarm(const NetworkDIDOSocket* nds, int nIndex)
{
	if (nIndex < 0 || nds == NULL)
		return FALSE;
	if (theApp.m_pDlgAlarm)
	{
		NetworkDIDOSocket* ndd = new NetworkDIDOSocket;
		if(ndd)
		{
			SYSTEMTIME st;

			GetLocalTime(&st);
			memcpy(ndd, nds, sizeof(NetworkDIDOSocket));
			ndd->cAlarmNum = nIndex;
			ndd->st = st;
			theApp.m_pDlgAlarm->AddToRtAlarm(ndd);
		}
	}

	return TRUE;
}

void CSocketThread::OnReceNewSocketMsg(BYTE* pReadMsg)
{
	EnterCriticalSection(&m_csReceMsg);
	m_listReceMsg.AddTail(pReadMsg);
	LeaveCriticalSection(&m_csReceMsg);

	SetEvent(m_hSocketEvent);
}

int CSocketThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSocketThread, CWinThread)
	//{{AFX_MSG_MAP(CSocketThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSocketThread message handlers

