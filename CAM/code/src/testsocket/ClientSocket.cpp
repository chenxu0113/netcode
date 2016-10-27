// ClientSocket.cpp : implementation file
//

#include "stdafx.h"
#include "ClientSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CClientSocket

CClientSocket::CClientSocket()
{
	m_pRevBuffer = NULL;
	m_pRevBuffer = new BYTE[1024];
}


CClientSocket::~CClientSocket()
{
	if (m_pRevBuffer)
	{
		delete[] m_pRevBuffer;
	}
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

   CString  NetSource;
   UINT portno = 0;

   int nLen = 0, nCount = 0, i = 0;

   nCount = Receive(m_pRevBuffer, 1024);
   int nValue = m_pRevBuffer[9];
   int n1 = nValue & 0x01;
   int n2 = (nValue >> 1) & 0x01;
   int n3 = (nValue >> 2) & 0x01;
   int n4 = (nValue >> 3) & 0x01;
   AsyncSelect(FD_READ);
   BYTE *pBuf = m_pRevBuffer;

   if (nCount==0 || nLen==SOCKET_ERROR) 
   {	   
	   return ;			
   }
}



void CClientSocket::ClearBuf()
{

}


void CClientSocket::OnClose(int nErrorCode)
{
	CSocket::OnClose(nErrorCode);
	
}
