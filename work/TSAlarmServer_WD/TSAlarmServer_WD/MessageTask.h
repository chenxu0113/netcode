#pragma once



// MessageTask
struct MesInfoData
{
	int Dataid;
	TCHAR fromSys[20];         //*信息来源系统代码（该代码由统一信息平台注册系统信息时生成）
	TCHAR target_Phone[12];          //*信息目标接收人的RTXNO或者接受人手机号
	TCHAR target_Ctx[12];
	TCHAR msTitle[20];         //发送信息的标题，标题可为""
	TCHAR msContent[500];     //*发送内容，信息内容不能超过500个字
	TCHAR targetTime[20];      //目标发送时间，可以为""，如果为"",则默认为立即发送格式"yyyy-MM-dd HH:mm:ss"
	TCHAR priority[1];         //发送优先级，可以为"",如果为"",则默认为1
	BOOL  bFlag;
	BOOL  bCtxFlag;
	BOOL  bPhoneFlag;
};
class CMessageTask : public CWinThread
{
	DECLARE_DYNCREATE(CMessageTask)

public:
	CMessageTask();           // 动态创建所使用的受保护的构造函数
	virtual ~CMessageTask();
    BOOL InitMesInfo();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	BOOL SendMessageProcess(MesInfoData& iMessageInfo);
	void UpdateMsToDB();
	BOOL TChar2Char(const TCHAR* pTchar, char* pChar);
	vector<MesInfoData> m_vecMesInfo;
	//CMap<int,int,MesInfoData,MesInfoData&> m_MesInfoMap;
	//CMap<int,int,DevDayValue,DevDayValue&> m_DevDayValueMap;
	int m_MinValue;
	int m_MaxValue;
	float m_iMagnifyNum;
	float m_iReduceNum;
protected:
	DECLARE_MESSAGE_MAP()
};


