#if !defined(AFX_SENDTHREAD_H__0AA6369A_6F38_44DC_860B_7DB7805FACD8__INCLUDED_)
#define AFX_SENDTHREAD_H__0AA6369A_6F38_44DC_860B_7DB7805FACD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SendThread.h : header file
//
#include "ClientSocket.h"

/////////////////////////////////////////////////////////////////////////////
// CSendThread thread
class CMySocketWnd;	// MSW 121211 ADD
class CSendThread : public CWinThread
{
	DECLARE_DYNCREATE(CSendThread)
public:
	CSendThread();           // protected constructor used by dynamic creation
	virtual ~CSendThread();

// Attributes
public:
	CClientSocket* m_pWdSocket;
	CClientSocket* m_pClientSocket[MAX_NETWORKDIIO];

// Operations
public:
	void AskDiStatus();
	BOOL InitSocket();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendThread)
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

public:
	void SendSocketMsg(int nIndex, BYTE *pMsg, WORD len);
// Implementation
protected:
	

	// Generated message map functions
	//{{AFX_MSG(CSendThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDTHREAD_H__0AA6369A_6F38_44DC_860B_7DB7805FACD8__INCLUDED_)
