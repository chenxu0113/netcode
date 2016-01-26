
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持

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
typedef struct _PowFileInfo		//文件中单个点的电度量信息
{
	WORD nDevID;		//设备ID
	WORD nYmNum;		//遥脉序号
	DWORD nYmVal;		//遥脉值
	unsigned char cFlag;	//标志位
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
	int m_nDeviceNo;//设备ID，设备表的主键
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
	int ObjectId;			//告警对象Id
	int ObjectType;			//对象类型	31-设备 33-集中器
	int AlarmType;			//告警类型
	int reserve;			//保留字段 当对象类型为31时 作为场站号
	CTime AlarmTime;		//告警时间
	int Status;				//告警状态
	TCHAR Contents[256];	//告警内容
	TCHAR Suggest[256];		//处理策略提示
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

typedef std::map<CString, SimpleYmDef>	YmConfigDefMap;		//其中键类型CString由"设备ID+遥脉序号"组合生成
typedef std::map<int, int>	StationDefMap;		//其中键为"Rtu号"，即StationDef中的nPortNo
typedef std::map<CString, int>	DeviceRecordDefMap;		//其中键类型CString由"厂站ID+前置设备号"组合生成
typedef std::map<int, DeviceRecordDef>	DeviceRecordDefMap2;		//其中键为设备ID
typedef std::map<CString, int> SampleDayNo;
typedef std::map<int,SampleValueInfo> SampleValueInfoMap;
typedef std::map<__int64, __int64>	CollectorMappingMap;	//集中器向虚设备的映射map，其中键的高4位为集中器ID，低4位为集中器下点的序号；
//值的高4位为虚设备ID，低4位为虚设备下点的序号    //ZHOUN 140909 ADD
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


