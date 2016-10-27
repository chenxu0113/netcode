#pragma once
#include "afxwin.h"


// CDlgAlarm �Ի���

class CDlgAlarm : public CDialog
{
	DECLARE_DYNAMIC(CDlgAlarm)

public:
	CDlgAlarm(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgAlarm();
	void AddToRtAlarm(NetworkDIDOSocket* ndd);

	int m_nTimer;
	CRITICAL_SECTION m_csRtAlarm;
	CList<NetworkDIDOSocket*, NetworkDIDOSocket*> m_listRtAlarm;
// �Ի�������
	enum { IDD = IDD_DLG_ALARM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	/*CBCGPListBox*/CListBox m_ListboxRtAlarm;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
