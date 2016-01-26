#pragma once

#include "HiredisIntf.h"
#include "SoftBus.h"
#include "RedisBus.h"
// CRedisRecvSample

struct DeviceInfo
{
	int iDevId;
	int iDevType;
	int iPlazaId;
	TCHAR PlazaName[128];
	int iBrandId;
	TCHAR BrandName[128];
	TCHAR RoomName[128];
	TCHAR NickName[600];
};
class CRedisRecvSample : public CWinThread
{
	DECLARE_DYNCREATE(CRedisRecvSample)

public:
	CRedisRecvSample();           // 动态创建所使用的受保护的构造函数
	virtual ~CRedisRecvSample();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	BOOL CharToTchar (const char * _char, TCHAR * tchar) ;
	BOOL TChar2Char(const TCHAR* pTchar, char* pChar);
	//void StartProcessData(SocketMsgGroupYmDataEx_DATE *pSocketMsgYmData);
	BOOL SetTimeId(CString strTime);
	BOOL InitCfgInfo();
	BOOL LoadAllForeignTable();
	BOOL LoadStationTable();
	BOOL LoadDeviceTable();
	BOOL LoadYmTable();
	BOOL LoadSampleExtTable(WORD SampleTableNo);
	BOOL GetYmDefByPowInfo(short nRtu, const PowFileInfo *pPowInfo, SimpleYmDef &ymDef, int &iDevId);
	BOOL GetDeviceInfoByCollectorInfo(int nCollectorID, int nCollector3YNum, int &nDevID, int &nDev3YNum);
	//void SaveSampleData2DB(int MinSampleNo, int MaxSampleNo,CString StrTimeId,int itype); GUOJ DEL 150909
	BOOL GetDataFromRedis();
	BOOL ConnectRedisServer();
	//void OnSocketMsgGroupYx(SocketMsgGroupYxEx* pMsgGroupYxData); GUOJ DEL 150909
	BOOL GetDevId(short nRtu, int DeviceNo, int &DevId);
	//void OnCallPrePayRecord(SocketMsgPrepayRecord* pMsgPrepayRecordData); GUOJ DEL 150909
	//BOOL Update2RTDB(); GUOJ DEL 150909
	BOOL  GetDevInfo();
	BOOL  LoadYxTable();
	BOOL LoadSampleTable(WORD SampleTableNo);
	BOOL LoadSampleTableFromRTDB(WORD SampleTableNo);
public:
	CSoftBus *m_softbus; 
	BOOL m_bHasRegister;
	CString RedisIPAddress;
	int RedisPort;
	StationDefMap m_StationDefMap;			//厂站表
	DeviceRecordDefMap m_DeviceRecordDefMap;	//设备表
	DeviceRecordDefMap2 m_DeviceRecordDefMap2;	//设备表2
	YmConfigDefMap m_YmConfigDefMap;		//遥脉表
	CollectorMappingMap m_CollectorMappingMap;  //集中器向虚设备的映射map   
	SampleDayNo m_MapSampleNo;
	//GUOJ DEL 150909
	//SampleValueInfoMap m_SampleValueInfoMap;
	//SampleValueInfoMap m_SampleValueInfoMapDay;
	//CArray<YmConfigDef,YmConfigDef&> m_YmValueInfoCArray ;
	//CArray<YxConfigDef,YxConfigDef&> m_YxValueInfoCArray ;
	//GUOJ DEL 150909 END
	CRITICAL_SECTION m_csYmConfigDefMap;
	CRITICAL_SECTION m_csBillNum;
	CRITICAL_SECTION m_csDayTime;
	CRITICAL_SECTION m_csDevStateMap;
	//CRITICAL_SECTION m_csUpdateRTDB;
	int MaxStationNum;
	int BillNum;
	CTime m_DayTime;
	CMap<int,int,DeviceInfo,DeviceInfo&> m_DeviceInfoMap;
	CMap<int,int,BYTE,BYTE&> m_DevStateMap;
	BOOL beInited;
	CString m_strPath;
protected:
	DECLARE_MESSAGE_MAP()
};

