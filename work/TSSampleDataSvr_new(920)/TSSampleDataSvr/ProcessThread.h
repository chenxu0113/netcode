#pragma once
#include "HiredisIntf.h"
#include "SoftBus.h"
#include "RedisBus.h"


// CProcessThread

class CProcessThread : public CWinThread
{
	DECLARE_DYNCREATE(CProcessThread)

public:
	CProcessThread();           // 动态创建所使用的受保护的构造函数
	virtual ~CProcessThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void StartProcessData(SocketMsgGroupYmDataEx_DATE *pSocketMsgYmData);
	void OnSocketMsgGroupYx(SocketMsgGroupYxEx* pMsgGroupYxData);
	void OnCallPrePayRecord(SocketMsgPrepayRecord* pMsgPrepayRecordData);
	BOOL Update2RTDB();
	void SaveSampleData2DB(int MinSampleNo, int MaxSampleNo,CString StrTimeId,int itype);
	BOOL QueryExamineRecorde(int iDevId, int ChargeCnt,float fFee,CTime ChargeTime, CString &SerialId);
	BOOL GetDataFromRedis();
	BOOL ConnectRedisServer();
public:
	SampleValueInfoMap m_SampleValueInfoMap;
	SampleValueInfoMap m_SampleValueInfoMapDay;
	CArray<YmConfigDef,YmConfigDef&> m_YmValueInfoCArray ;
	CArray<YxConfigDef,YxConfigDef&> m_YxValueInfoCArray ;
	CPtrList m_RedisDataList;
	HANDLE m_ProcessDataEvent,m_hExitEvent;  
	CRITICAL_SECTION m_csRedisDataList;
	CSoftBus *m_softbus; 
	int m_Index;
	//CLog *m_log;
protected:
	DECLARE_MESSAGE_MAP()
};


