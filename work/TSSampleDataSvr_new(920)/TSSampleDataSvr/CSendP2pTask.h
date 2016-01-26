#pragma once
#include "softbus.h"
#include "HiredisIntf.h"
#include "RedisBus.h"

// CCSendP2pTask

class CCSendP2pTask : public CWinThread
{
	DECLARE_DYNCREATE(CCSendP2pTask)

public:
	CCSendP2pTask();           // 动态创建所使用的受保护的构造函数
	virtual ~CCSendP2pTask();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	int GetTestBuffer(unsigned char ** buffer);
	BOOL ConnectRedisServer();
	CPtrList m_RecordList;
	HANDLE m_ProcessDataEvent,m_AlarmEvent,m_hExitEvent;  
	CRITICAL_SECTION m_csRecordList;
	CPtrList m_AlarmInfoList;
	CRITICAL_SECTION m_csAlarmInfoList;
public:
	CSoftBus *m_SoftBus;

protected:
	DECLARE_MESSAGE_MAP()
};


