#pragma once

struct	DevStaionInfo
{
	int iDevId;
	int iDevNum;
	int	iStationId;
};

// CSaveYmDataThread

class CSaveYmDataThread : public CWinThread
{
	DECLARE_DYNCREATE(CSaveYmDataThread)

public:
	CSaveYmDataThread();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CSaveYmDataThread();

public:
	BOOL CheckPaymentRecord(int iDevId,int MaxBuyNum,int& CallNum);
	BOOL GetDeviceId();
	virtual BOOL InitInstance();
	virtual int ExitInstance();
public:
	CArray<DevStaionInfo,DevStaionInfo&> m_DevIdCArray ;
protected:
	DECLARE_MESSAGE_MAP()
};


