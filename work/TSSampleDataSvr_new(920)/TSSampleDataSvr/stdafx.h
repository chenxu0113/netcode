
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��

#include "NetMessage.h"
#include "wrkdllsock.h"
#include "wdwrkdll.h"
#include "PublicStruct.h"
#include "SOCKETDEF.H"
#include "Log.h"
#include "MyLogInc.h"
#include <map>

typedef enum{
	REGU_HIREDIS,
	REGU_FDADATA,
	REGU_FDACMD,
	REGU_YK,
}dbg_type_t;

#define  ReguTrace(type,fmt,...)     do{\
	CString s1,s2;\
	TCHAR* t=_T("Unknow");\
	s1.Format(_T(fmt),__VA_ARGS__) ;\
	s2.Format(_T("[%s][%20s:%-4d]"),_T(#type),_T(__FUNCTION__),__LINE__);\
	g_log->Log(s2+s1);\
	}while(0)

#define  ReguTrace_ext(logger,type,fmt,...)     do{\
	CString s1,s2;\
	TCHAR* t=_T("Unknow");\
	s1.Format(_T(fmt),__VA_ARGS__) ;\
	s2.Format(_T("[%s][%20s:%-4d]"),_T(#type),_T(__FUNCTION__),__LINE__);\
	(logger)->Log(s2+s1);\
	}while(0)

#pragma pack(1)
typedef struct _PowFileInfo		//�ļ��е�����ĵ������Ϣ
{
	WORD nDevID;		//�豸ID
	WORD nYmNum;		//ң�����
	DWORD nYmVal;		//ң��ֵ
	unsigned char cFlag;	//��־λ
}PowFileInfo;

typedef struct _SampleValueInfo
{
	int SampleNo;
	double dbValue;
	unsigned char cFlag;
}SampleValueInfo;

typedef struct _YmValueInfo
{
	int YmIndex;
	int OrigValue;
	float YmValue;
}YmValueInfo;

struct   ProtocolCallItem
{
	int  m_nType ;
	//	byte m_nStationNum;
	unsigned short m_nStationNum;//MSW 150615 MOD
	//	WORD m_nDeviceNo;
	int m_nDeviceNo;//�豸ID���豸�������
	byte m_nBuffer[2060] ;
};


typedef struct _AlarmMsgHead
{
	DWORD MsgBegin;
	DWORD dAlarmType;
	DWORD MsgEnd;
}AlarmMsgHead;

typedef struct _AlarmInfo
{
	int ObjectId;			//�澯����Id
	int ObjectType;			//��������	31-�豸 33-������
	int AlarmType;			//�澯����
	int reserve;			//�����ֶ� ����������Ϊ31ʱ ��Ϊ��վ��
	CTime AlarmTime;		//�澯ʱ��
	int Status;				//�澯״̬
	TCHAR Contents[256];	//�澯����
	TCHAR Suggest[256];		//���������ʾ
}AlarmInfo;
//GUOJ ADD 151116
struct	SimpleYmDef
{
	int			nYmIndex;
	int			nDeviceNo;
	int			nYmNum;
	__int64		nYmRaw;
	float		dYmQuotiety;
	double		dYmValue;
	TIMESTAMP_STRUCT DataTime;
};

struct YmYxData 
{
	BYTE iType;	//1--Ym,2-->Yx
	void* data;
};
#pragma pack()

typedef std::map<CString, SimpleYmDef>	YmConfigDefMap;		//���м�����CString��"�豸ID+ң�����"�������
typedef std::map<int, int>	StationDefMap;		//���м�Ϊ"Rtu��"����StationDef�е�nPortNo
typedef std::map<CString, int>	DeviceRecordDefMap;		//���м�����CString��"��վID+ǰ���豸��"�������
typedef std::map<int, DeviceRecordDef>	DeviceRecordDefMap2;		//���м�Ϊ�豸ID
typedef std::map<CString, int> SampleDayNo;
typedef std::map<int,SampleValueInfo> SampleValueInfoMap;
typedef std::map<__int64, __int64>	CollectorMappingMap;	//�����������豸��ӳ��map�����м��ĸ�4λΪ������ID����4λΪ�������µ����ţ�
//ֵ�ĸ�4λΪ���豸ID����4λΪ���豸�µ�����    //ZHOUN 140909 ADD
const CString C_SQL_Insert_SampleTable = _T("if exists (select RECNUM FROM %s WHERE TIMEID='%s') BEGIN UPDATE %s SET %s WHERE TIMEID='%s' END ELSE BEGIN INSERT INTO %s (%s) VALUES ('%s'%s) END ; ");
const CString C_SQL_Insert_PaymentRecord = _T("IF NOT EXISTS (SELECT ID FROM TE_PAYMENT_PAYRECORD WHERE DEVICEID=%d AND COUNT=%d ) BEGIN INSERT INTO TE_PAYMENT_PAYRECORD \
											  (OPERATIONTIME,ACCOUNTCODE,ROOMNAME,DEVICEID,PAYCOUNT,PAPTAKER,BILLSERIAL,RECHARGETYPE,BEFORERECHARGE,AFTERERECHARGE,RECHARGETIME,COUNT,UPMARK,BRANDID,PLAZAID,BUNKCODE) VALUES ('%s','%s','%s',%d,%02f,%d,'%s',%d,%02f,%02f,'%s',%d,0,%d,%d,'%s') END; ");
#define SQL_COUNT_ONCE	10

#define THREAD_NUM	10
#define WM_ICON_NOTIFY WM_USER+102

#define SIZEOFSTATION   1024
#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


