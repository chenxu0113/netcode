#pragma once



// CUpDateRTDB

class CUpDateRTDB : public CWinThread
{
	DECLARE_DYNCREATE(CUpDateRTDB)

public:
	CUpDateRTDB();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CUpDateRTDB();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void Update2DB(YmYxData *pYmYxData);
public:
	CPtrList m_YxYmDataList;
	HANDLE m_ProcessDataEvent,m_hExitEvent;  
	CRITICAL_SECTION m_csYxYmDataList;

protected:
	DECLARE_MESSAGE_MAP()
};


