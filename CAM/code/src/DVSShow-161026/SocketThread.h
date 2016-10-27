#if !defined(AFX_SOCKETTHREAD_H__EF5C4F17_13E0_40CE_8A54_0299B86B5778__INCLUDED_)
#define AFX_SOCKETTHREAD_H__EF5C4F17_13E0_40CE_8A54_0299B86B5778__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SocketThread.dcxxxdh : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSocketThread thread
class CSocketThread : public CWinThread
{
	DECLARE_DYNCREATE(CSocketThread)
public:
	CSocketThread();           // protected constructor used by dynamic creation
	CRITICAL_SECTION m_csReceMsg;
	CPtrList m_listReceMsg;
	HANDLE m_hSocketEvent;


// Attributes
public:
	BOOL StartRtVideo(const NetworkDIDOSocket* nds, int nNum);
	BOOL Add2RtAlarm(const NetworkDIDOSocket* nds, int nIndex);
	void OnReceNewSocketMsg(BYTE* pReadMsg);
	void ReceMsgProcess();
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocketThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSocketThread();

	// Generated message map functions
	//{{AFX_MSG(CSocketThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETTHREAD_H__EF5C4F17_13E0_40CE_8A54_0299B86B5778__INCLUDED_)
