#if !defined(AFX_CLIENTSOCKET_H__0CF4A3BB_9954_47F2_B7D1_EE8DC9C70A70__INCLUDED_)
#define AFX_CLIENTSOCKET_H__0CF4A3BB_9954_47F2_B7D1_EE8DC9C70A70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClientSocket.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CClientSocket command target

class CClientSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CClientSocket();
	virtual ~CClientSocket();

	BYTE* m_pRevBuffer;

	BOOL ProcessRev(BYTE* pBuf, int nLen);
	void ClearBuf();
	
// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CClientSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTSOCKET_H__0CF4A3BB_9954_47F2_B7D1_EE8DC9C70A70__INCLUDED_)
