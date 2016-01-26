#pragma once



// CAlarmTask
enum DataType
{
	SUMCOUNT = 1,   //当日用电量
	MEANELEC		//平均用电量
};
struct DeviceInfoData
{
	int iDevId;
	TCHAR DevName[65];
	int iStationId;
	int iPlazaId;
	TCHAR PlazaName[128];
	int Occupied;
	int iBrandId;
	TCHAR BrandName[128];
	TCHAR BrandCode[128];
	int iContractId;
	int iContractType;
	TIMESTAMP_STRUCT startTime;
	TIMESTAMP_STRUCT endTime;
};

struct DevDayValue
{
	int iDevId;
	float DayValue;
	float last7Value;
};

struct DevSampleConfig
{
	int iDevId;
	TCHAR TableName[50];
	TCHAR ColumnName[50];
};
class CAlarmTask : public CWinThread
{
	DECLARE_DYNCREATE(CAlarmTask)

public:
	CAlarmTask();           // 动态创建所使用的受保护的构造函数
	virtual ~CAlarmTask();
	BOOL InitDevInfo();
	BOOL LoadDayValue(CString strTime,DataType iDataType);
	void ProcessData();
	void AddAlarm2DB(AlarmItemDef ALARMDef);
	void CreateUuid(TCHAR* csID);
	BOOL InitCfgInfo();
	void CreateAlarmDef(DeviceInfoData iDeviceInfo,int AlarmType,AlarmItemDef &ALARMDef);
	BOOL LoadSMSTemplate();
	BOOL LoadSMSConfig();
	void Insert2SMSRECORD(SMSConfigDef iSMSCofig,CString TemplateContent);
	void BackupData();
	BOOL GetNoDevContract();
	BOOL GetOverDraft();
	BOOL GetSampleDayConfig();
	void CheckBillAlarm();
	BOOL LoadStationTable();
	void CheckDevStateByStationId();
	void UpdateAlarmStatus();
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
public:
	//CMap<int,int,DeviceInfoData,DeviceInfoData&> m_DeviceInfoMap;
	vector<DeviceInfoData> m_vecDeviceInfo;
	CMap<int,int,DeviceInfoData,DeviceInfoData&> m_DeviceInfoMap;
	CMap<int,int,DevDayValue,DevDayValue&> m_DevDayValueMap;
	CMap<CString,LPCTSTR,SMSConfigDef,SMSConfigDef&> m_SMSConfigMap;
	CMap<int,int,CString,LPCTSTR> m_SMSTemplateMap;
	CArray<DevSampleConfig,DevSampleConfig&> m_DevSampleCfgArray;
	CMap<int,int,vector<int>,vector<int>> m_BrandDevMap;
	CArray<short,short&> m_StationArray;
	int m_MinValue;
	int m_MaxValue;
	float m_iMagnifyNum;
	float m_iReduceNum;
	int m_iValidDays;
protected:
	DECLARE_MESSAGE_MAP()
};


