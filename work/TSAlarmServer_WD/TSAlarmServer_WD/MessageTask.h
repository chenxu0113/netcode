#pragma once



// MessageTask
struct MesInfoData
{
	int Dataid;
	TCHAR fromSys[20];         //*��Ϣ��Դϵͳ���루�ô�����ͳһ��Ϣƽ̨ע��ϵͳ��Ϣʱ���ɣ�
	TCHAR target_Phone[12];          //*��ϢĿ������˵�RTXNO���߽������ֻ���
	TCHAR target_Ctx[12];
	TCHAR msTitle[20];         //������Ϣ�ı��⣬�����Ϊ""
	TCHAR msContent[500];     //*�������ݣ���Ϣ���ݲ��ܳ���500����
	TCHAR targetTime[20];      //Ŀ�귢��ʱ�䣬����Ϊ""�����Ϊ"",��Ĭ��Ϊ�������͸�ʽ"yyyy-MM-dd HH:mm:ss"
	TCHAR priority[1];         //�������ȼ�������Ϊ"",���Ϊ"",��Ĭ��Ϊ1
	BOOL  bFlag;
	BOOL  bCtxFlag;
	BOOL  bPhoneFlag;
};
class CMessageTask : public CWinThread
{
	DECLARE_DYNCREATE(CMessageTask)

public:
	CMessageTask();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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


