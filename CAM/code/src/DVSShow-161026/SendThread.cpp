// SendThread.cpp : implementation file
//

#include "stdafx.h"
#include "SendThread.h"
#include "DVSShow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CDVSShowApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CSendThread

IMPLEMENT_DYNCREATE(CSendThread, CWinThread)

CSendThread::CSendThread()
{
	m_pWdSocket = NULL;
}

CSendThread::~CSendThread()
{
	theApp.CaptureNetworkDidoSocket();
	for (int i=0; i<theApp.m_mapNetworkDidoSocket.GetCount(); i++)
	{
		if (m_pClientSocket[i])
		{
			m_pClientSocket[i]->Close();
			delete m_pClientSocket[i];
			m_pClientSocket[i] = NULL;
		}
	}
	theApp.ReleaseNetworkDidoSocket();
}

BOOL CSendThread::InitInstance()
{
	if (!InitSocket())
 	{
 		return TRUE;
 	}

	return TRUE;
}

int CSendThread::ExitInstance()
{
	
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSendThread, CWinThread)
	//{{AFX_MSG_MAP(CSendThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CSendThread::InitSocket()
{
	int nLength = LENGTH_1024;
	POSITION pos = theApp.m_mapNetworkDidoSocket.GetStartPosition();

	while(pos)
	{
		BOOL bRet = FALSE;
		CString sIp = "";
		NetworkDIDOSocket nds;
		
		memset(&nds, 0, sizeof(NetworkDIDOSocket));
		theApp.m_mapNetworkDidoSocket.GetNextAssoc(pos, sIp, nds);
		m_pClientSocket[nds.nIndex] = NULL;
		m_pClientSocket[nds.nIndex] = new CClientSocket(nds.ndc.szIP, nds.ndc.nPort);

		bRet = m_pClientSocket[nds.nIndex]->CreateSocket();
		if (!bRet)
		{
			continue;
		}
		nds.bInitSuccess = bRet;
		
		CString sIP(nds.ndc.szIP);
		theApp.m_mapNetworkDidoSocket.SetAt(sIP, nds);
	}

	return TRUE;
}

void CSendThread::AskDiStatus()
{
	if (NULL == this)
		return;

	POSITION pos = theApp.m_mapNetworkDidoSocket.GetStartPosition();

 	while(pos)
 	{
		BYTE cSndBuf[LENGTH_12];
		CString sIp = "";
		NetworkDIDOSocket nds;

		memset(&nds, 0, sizeof(NetworkDIDOSocket));
		theApp.m_mapNetworkDidoSocket.GetNextAssoc(pos, sIp, nds);

		if (!nds.bInitSuccess)
		{
			continue;
		}

 		memset(cSndBuf, 0, LENGTH_12);
 		cSndBuf[0] = PROTOCOL_NONE;
 		cSndBuf[1] = PROTOCOL_EVENT_DEAL;
 		cSndBuf[2] = PROTOCOL_PROTOCAL;
 		cSndBuf[3] = PROTOCOL_PROTOCAL;
 		cSndBuf[4] = PROTOCOL_NONE;
 		cSndBuf[5] = 0x06;//6-11¹²6¸ö×Ö½Ú
 		cSndBuf[6] = PROTOCOL_UNIT;
 		cSndBuf[7] = PROTOCOL_FUNC_R;
 		cSndBuf[8] = PROTOCOL_NONE;
 		cSndBuf[9] = PROTOCOL_ADDRESS;
 		cSndBuf[10] = PROTOCOL_NONE;
 		cSndBuf[11] = nds.ndc.cMaxNum;

		SendSocketMsg(nds.nIndex, cSndBuf, LENGTH_12);
		
		Sleep(50);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSendThread message handlers

void CSendThread::SendSocketMsg(int nIndex, BYTE *pMsg, WORD len)
{
	if ((nIndex < 0) || (pMsg == NULL) || (len <= 0))
		return;

	int nSend = 0;
	if(m_pClientSocket[nIndex])
	{
		nSend = m_pClientSocket[nIndex]->Send(pMsg, len);
	}
	int d = 0;
}
