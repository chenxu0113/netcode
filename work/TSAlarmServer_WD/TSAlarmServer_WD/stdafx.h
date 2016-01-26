
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

#define WM_ICON_NOTIFY WM_USER+102

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

#include "soapH.h"
#include "NETMESSAGE.H"
#include "wrkdllsock.h"
#include "wdwrkdll.h"
#include "PublicStruct.h"
#include "SOCKETDEF.H"
#include "Log.h"
#include "MyLogInc.h"
#include <map>
#include <vector>
#include "TypedefEx.h"
#include "AClient.h"
#include "LCD.h"

#pragma comment(lib,"LCD.lib")

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

const CString C_SQL_QUERY_DEVINFO = _T("select top 5000 TB_DEVICE.ID,TB_DEVICE.CNAME, STATIONID,pl.ID as plazaid,pl.CNAME as plazaname, \
									   TB_PAYMENT_DEVICE_EX.OCCUPIED,pa.ID as brandid,pa.CNAME as brandname,pa.CODE as brandcode, \
									   pa.ID as contractid,pa.ContractType,pa.PACTBEGINDATE,pa.PACTENDDATE \
									   from TB_DEVICE \
									   left join TB_PAYMENT_DEVICE_EX on TB_PAYMENT_DEVICE_EX.DEVICEID=TB_DEVICE.ID \
									   left JOIN TL_PAYMENT_ROOM ON TB_PAYMENT_DEVICE_EX.DEVICEID =TL_PAYMENT_ROOM.DEVICEID \
									   left JOIN TB_PAYMENT_ROOM rm on rm.ID=TL_PAYMENT_ROOM.ROOMID \
									   left JOIN TB_PAYMENT_ROOM pl on TB_PAYMENT_DEVICE_EX.PLAZAID=pl.ID \
									   left JOIN TB_PAYMENT_ACCOUNT pa ON  rm.ACCOUNTID = pa.ID where TB_DEVICE.ID >=%d order by TB_DEVICE.ID;");

const CString C_SQL_INSERT_ALARMINDEX = _T("if not exists (select ID FROM TE_ALARM_INDEX WHERE ALARMTYPE=%d and ALARMOBJID=%d and ALARMOBJTYPE=%d and STATUS=0) BEGIN %s END \
										   else begin update TE_ALARM_INDEX set alarmcontent='%s', contenttime='%s' where ALARMTYPE=%d and ALARMOBJID=%d and ALARMOBJTYPE=%d end; ");

const CString  C_SQL_QUERY_MESINFO =_T("select top 10000 TE_SMSRECORD.ID,CTX,PERSONTELNUM,NOTEDESCRIPTION FROM TE_SMSRECORD \
									   LEFT JOIN TE_PAYMENT_NOSMS ON TE_PAYMENT_NOSMS.PHONE=TE_SMSRECORD.PERSONTELNUM WHERE RESULT=0;");


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

struct AlarmTypeDef
{
	int m_iAlarmType;
	int m_iGroupId1;   
	int m_iGroupId2;   
	int m_iAlarmLevel;
	TCHAR m_cstrName[128];
};

struct DevInfo
{
	int DevId;
	TCHAR cname[65];
};

struct SMSConfigDef
{
	int iPlazaId;
	int type;
	int TemplateNum;
	BYTE IsShield;
	TCHAR PersonName[65];
	TCHAR PhoneNum[65];
};

#define ALARMSTATUS_NEWALARM	0   //���ڸ澯
#define ALARMSTATUS_REVERSE		2	//�ָ��澯
//ҵ��澯
#define ALARMTYPE_RUSH				8001
#define ALARMTYPE_CONTRACTNODEV		8002
#define ALARMTYPE_CONTRACTEND		8003
#define ALARMTYPE_BRAND_LOW			8004
#define ALARMTYPE_BUSSINESS_HIGH	8005
#define ALARMTYPE_BILLOVERTIME		8007				

//ϵͳ�澯

//�豸�澯
#define ALARMTYPE_DEV_OVERDRAFT		9001
#define ALARMTYPE_DEV_UNCOMMUN		9003
#define ALARMTYPE_STATION_UNCOMMUN	9004
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


