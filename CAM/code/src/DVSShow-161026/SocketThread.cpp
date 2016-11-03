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
		if (pBuf[7 + LENGTH_4] == PROTOCOL_FUNC_W)	//д״̬�����룬7�����8λ����ʵ��ֵ����
		{
			bRead = FALSE;
		//	WORD* pTemp = (WORD*)&pBuf[10 + LENGTH_4];	//��11+4���ֽ�Ϊvalue
		//	nValue = *pTemp;
		}
		else if (pBuf[7 + LENGTH_4] == PROTOCOL_FUNC_R)	//��״̬�����룬7�����8λ
			nValue = pBuf[9 + LENGTH_4];	//��9+4���ֽ�Ϊvalue

		for (int i=0; i<nds.ndc.cMaxNum; i++)
		{
			unsigned char bValue = PROTOCOL_NONE;
			if (i == 0)
			{
				bValue = nValue & 0x01;//�豸���͵�ֵ
			}
			else
			{
				bValue = (nValue >> i) & 0x01;//�豸���͵�ֵ
			}

			//��澯ֵ��ȣ������澯�����Ƴ���Ƶ

			//�豸���͵�ֵ���ʼֵ����ȣ����ҵ��ڸ澯ֵ�������澯
			if (bValue != nds.ndc.cNum[i] && bRead)//��Ϊдֻ���سɹ���񣬵�������ʵ��״ֵ̬
			{
				nds.ndc.cNum[i] = bValue;
				Add2RtAlarm(&nds, i);	//����ʵʱ�澯
			}
			//�豸���͵�ֵ���ڸ澯ֵ���Ƴ���Ƶ��������¼��ֻҪ�澯��ȡ������һֱ������Ƶ
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
	//if (!theApp.m_dlgOutput[pVideo1->m_nSubWndIndex].m_bStartVideo)//�Ѿ�����Ƶ���ڣ������Ƴ�
	//{
	//	return FALSE;
	//}

	CString sIp = "";
	LinkInfoDef lid;

	AfxGetApp()->DoWaitCursor(1);
	//ͨ�����������DI/DOģ���IP�������Ϣ���ҵ������������ģ���Ӧ��ϵ���е������IP��channel���Ӷ���ȡ�����VIDEOPARAM
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
		if (!theApp.m_dlgOutput[video->lPlayWnd].m_bStartVideo)//�Ѿ�����Ƶ���ڣ������Ƴ�
		{
			//������ʱ¼��
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

	//��ʾ�澯�Ի���
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

