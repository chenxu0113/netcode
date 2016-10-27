// SendThread.cpp : implementation file
//

#include "stdafx.h"
#include "SendThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSendThread

IMPLEMENT_DYNCREATE(CSendThread, CWinThread)

CSendThread::CSendThread()
{
	m_pWdSocket = new CClientSocket;

}

CSendThread::~CSendThread()
{

	if(m_pWdSocket)
	{
		m_pWdSocket->Close();
		delete m_pWdSocket;
		m_pWdSocket = NULL;
	}

}

BOOL CSendThread::InitInstance()
{
	BOOL bRet = FALSE;
	
	bRet = m_pWdSocket->Create();
	if (!bRet)
	{
		return TRUE;
	}
	m_pWdSocket->AsyncSelect(FD_READ|FD_WRITE|FD_CLOSE|FD_CONNECT|FD_OOB);
	int nLength = 1024;
	m_pWdSocket->SetSockOpt(SO_DONTLINGER|SO_RCVBUF, (const char*)&nLength, sizeof(nLength), SOL_SOCKET);

	bRet = m_pWdSocket->Connect("10.0.0.10", 5022);
	if (!bRet)
	{
		return TRUE;
	}
//	m_pWdSocket->AsyncSelect(FD_READ|FD_WRITE|FD_OOB|FD_CONNECT|FD_ACCEPT|FD_CLOSE);
	
	
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




/////////////////////////////////////////////////////////////////////////////
// CSendThread message handlers


void CSendThread::SendSocketMsg(BYTE *pMsg,WORD len)
{
	int nSend = 0;
	if(m_pWdSocket)
	{
		nSend = m_pWdSocket->Send(pMsg, len);	
	}
	int d = 0;
}

