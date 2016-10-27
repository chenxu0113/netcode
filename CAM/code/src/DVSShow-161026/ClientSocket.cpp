// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ClientSocket.h"
#include "DVSShow.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CDVSShowApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket()
{
	m_sDestIp = "";
	m_uPort = 502;
	memset(m_pRevBuffer, 0, LENGTH_1024);
}

CClientSocket::CClientSocket(char* szIp, UINT uPort)
{
	m_sDestIp = "";
	m_uPort = 502;
	memset(m_pRevBuffer, 0, LENGTH_1024);

	m_sDestIp = szIp;
	m_uPort = uPort;
}


CClientSocket::~CClientSocket()
{
	
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CClientSocket, CSocket)
	//{{AFX_MSG_MAP(CClientSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CClientSocket member functions

void CClientSocket::OnReceive(int nErrorCode) 
{	
	CSocket::OnReceive(nErrorCode);

	if (nErrorCode==WSAENETDOWN) 
	{
		return;
	}	

	int nCount = 0;
	nCount = Receive(m_pRevBuffer, LENGTH_1024);
	AsyncSelect(FD_READ);
	if (nCount < 0)
	{
		return;
	}

	int n1 = 0, n2 = 0, n3 = 0, n4 = 0;
	BYTE *pBuf = new BYTE[nCount + LENGTH_4];

	memset(pBuf, 0, nCount + LENGTH_4);
	sscanf_s(m_sDestIp, _T("%d.%d.%d.%d"), &n1, &n2, &n3, &n4);

	pBuf[0] = (BYTE)n1;
	pBuf[1] = (BYTE)n2;
	pBuf[2] = (BYTE)n3;
	pBuf[3] = (BYTE)n4;
	memmove(pBuf + LENGTH_4, m_pRevBuffer, nCount);

	if (theApp.m_pSocketThread)
	{
		theApp.m_pSocketThread->OnReceNewSocketMsg(pBuf);
	}
}


BOOL CClientSocket::CreateSocket()
{
	if (!Create())
		return FALSE;
	
	int nLength = LENGTH_1024;

	AsyncSelect(FD_READ|FD_WRITE|FD_CLOSE|FD_CONNECT|FD_OOB);
	SetSockOpt(SO_DONTLINGER|SO_RCVBUF, (const char*)&nLength, sizeof(nLength), SOL_SOCKET);

	return Connect(m_sDestIp, m_uPort);
}

void CClientSocket::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);
	
}
