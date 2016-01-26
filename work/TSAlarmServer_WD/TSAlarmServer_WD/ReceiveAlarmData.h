#pragma once
#include "HiredisIntf.h"
#include "SoftBus.h"
#include "RedisBus.h"


// CReceiveAlarmData

class CReceiveAlarmData : public CWinThread
{
	DECLARE_DYNCREATE(CReceiveAlarmData)

public:
	CReceiveAlarmData();           // 动态创建所使用的受保护的构造函数
	virtual ~CReceiveAlarmData();
	BOOL InitCfgInfo();
	BOOL ConnectRedisServer();
	BOOL TChar2Char(const TCHAR* pTchar, char* pChar);
	BOOL GetDataFromRedis();
	void CreateUuid(TCHAR* csID);
	BOOL initAlarmType();
	void AddAlarm2DB(AlarmItemDef ALARMDef);
	void ChangeAlarmStatus(AlarmItemDef ALARMDef);
	//void Send2SMSRecorde();
	//BOOL LoadSMSTemplate();
	BOOL GetDIIndexByStationId(int StationId);
	BOOL Update2RTDB();
	void Insert2SMSRECORD(SMSConfigDef iSMSCofig,CString TemplateContent);
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
public:
	CSoftBus *m_softbus; 
	BOOL m_bHasRegister;
	CString RedisIPAddress;
	int RedisPort;
	map<int,AlarmTypeDef> m_mapAlarmType;  //告警类型
	CMap<CString,LPCTSTR,DevInfo,DevInfo&> m_DevInfoMap;
	CArray<YxConfigDef,YxConfigDef&> m_YxValueInfoCArray ;
protected:
	DECLARE_MESSAGE_MAP()
};


