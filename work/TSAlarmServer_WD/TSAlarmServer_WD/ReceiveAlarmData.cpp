// ReceiveAlarmData.cpp : 实现文件
//

#include "stdafx.h"
#include "TSAlarmServer_WD.h"
#include "ReceiveAlarmData.h"
#include "TSAlarmServer_WDDlg.h"

// CReceiveAlarmData
CLog *g_log=NULL;

IMPLEMENT_DYNCREATE(CReceiveAlarmData, CWinThread)

CReceiveAlarmData::CReceiveAlarmData()
{
	m_bHasRegister = FALSE;
	m_softbus = NULL;
	m_softbus = new CRedisBus;
	if (NULL == m_softbus)
	{
		return;
	}
	if(NULL==g_log)
	{
		g_log = new CLog(_T("\\TSAlarmServer"));
	}
}

CReceiveAlarmData::~CReceiveAlarmData()
{
	if (m_softbus!=NULL)
	{
		delete m_softbus;
		m_softbus = NULL;
	}
}

BOOL CReceiveAlarmData::InitCfgInfo()
{
	TCHAR szDirectory[MAX_PATH];
	memset(szDirectory,0,sizeof(TCHAR)*MAX_PATH);
	DWORD dwRet = 0;
	dwRet = GetModuleFileName(NULL,szDirectory,MAX_PATH);
	if (!dwRet)
	{
		int iRet = 0;
		iRet = GetLastError();
		return FALSE;
	}
	CString strPath = szDirectory;
	int iIndex = strPath.ReverseFind('\\');
	if (iIndex != 0)
	{
		strPath = strPath .Left(iIndex);
	}
	else
	{
		return FALSE;
	}
	CString strCountPath = _T("");
	strCountPath += strPath + _T("\\parameter\\emscfg.ini");
	RedisPort = GetPrivateProfileInt(_T("REDISCONFIG"),_T("RedisPort"),6379,strCountPath);
	if(RedisPort <= 0)
	{
		RedisPort = 16379;
	}
	CString strIPAddress = _T("");
	GetPrivateProfileString(_T("REDISCONFIG"),_T("RedisIPAddress"),_T("127.0.0.1"),strIPAddress.GetBuffer(32),32,strCountPath);
	strIPAddress.ReleaseBuffer();
	int index = strIPAddress.Find(_T(";"));
	if (index>=0)
	{
		strIPAddress = strIPAddress.Left(index);
	}
	int len = strIPAddress.GetLength();
	if (strIPAddress.GetLength()<7)
	{
		strIPAddress = _T("127.0.0.1");
	}
	RedisIPAddress = strIPAddress;
	return TRUE;
}

BOOL CReceiveAlarmData::TChar2Char(const TCHAR* pTchar, char* pChar)
{
	int iASCIILen = 0;
	iASCIILen = WideCharToMultiByte(CP_ACP, 0, pTchar, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, pTchar, -1, pChar, iASCIILen, NULL, NULL);
	return TRUE;
}

BOOL CReceiveAlarmData::ConnectRedisServer()
{
	int nCount = 0;
	char szIP[WORDDOC_NAMESTRING_LEN];
	memset(szIP, 0, WORDDOC_NAMESTRING_LEN);
	if (!TChar2Char(RedisIPAddress,szIP))
	{
		return FALSE;
	}
	while(1)
	{
		if (m_softbus->RegisterSoftBus(szIP, RedisPort) == REDIS_OK)
		{
			break;
		}
		else
		{
			m_softbus->UnRegisterSoftBus();
			m_softbus->RegisterSoftBus(szIP, RedisPort);
			nCount++;
			Sleep(3000);
			if (nCount>=5)
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL CReceiveAlarmData::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	if(!ConnectRedisServer())
	{
		ReguTrace(Config,"ConnectRedisServer err!");
		return FALSE;
	}
	ReguTrace(Config,"ConnectRedisServer end!");
	if (!initAlarmType())
	{
		ReguTrace(Config,"initAlarmType err!");
		return FALSE;
	}
	ReguTrace(Config,"initAlarmType end!");
	while(1)
	{
		BOOL bRet = FALSE;
		bRet = GetDataFromRedis();
		if (!bRet)
		{
			Sleep(1000);
		}
		Sleep(5);
	}
	return TRUE;
}

BOOL CReceiveAlarmData::GetDataFromRedis()
{
	unsigned char * buffer = NULL;
	int length = 0;
	int ret = REDIS_NODATA;
	ret = m_softbus->RecvMessageFromMsmq_Ext("AlarmInfoData",&buffer); 
	//ret = m_softbus->RecvMessageFromMsmq("SampleMessage",buffer,&length);
	if(REDIS_ERR == ret)
	{
		ReguTrace(Config,"ReconnectRedisServer!");
		m_softbus->UnRegisterSoftBus();
		ConnectRedisServer();
		if (buffer!=NULL)
		{
			delete [] buffer;
			buffer = NULL;
		}
		return FALSE;
	}
	else if (REDIS_OK == ret)
	{
		if (NULL == buffer)
			return FALSE;
		BYTE * pBuf = (BYTE*)buffer;
		AlarmMsgHead* pHead = (AlarmMsgHead*)pBuf;
		if (pHead->MsgBegin==0x7e&&pHead->MsgEnd==0x7e)  //0x7e为告警消息标记
		{
			ReguTrace(Config,"收到一条告警信息type=%d!",pHead->dAlarmType);
			BYTE* pMsg = pBuf + sizeof(AlarmMsgHead);
			AlarmInfo *pAlarmInfo = (AlarmInfo*)pMsg;
			if (pHead->dAlarmType!=pAlarmInfo->AlarmType)
			{
				ReguTrace(Config,"告警类型不匹配!");
				if (buffer!=NULL)
				{
					delete [] buffer;
					buffer = NULL;
				}
				return FALSE;
			}
			
			map<int,AlarmTypeDef>::iterator iterFinder = m_mapAlarmType.find(pHead->dAlarmType);
			if(iterFinder != m_mapAlarmType.end())
			{
				AlarmTypeDef iAlarmType = iterFinder->second;
				CString strAlarmTypeName = _T("");
				AlarmItemDef ALARMDef;
				memset(&ALARMDef, 0, sizeof(AlarmItemDef));
				CreateUuid(ALARMDef.alarmID);
				ALARMDef.alarmType = pAlarmInfo->AlarmType;
				ALARMDef.alarmLevel = iAlarmType.m_iAlarmLevel;
				ALARMDef.alarmObjID = pAlarmInfo->ObjectId;
				if (ALARMDef.alarmType==ALARMTYPE_DEV_UNCOMMUN)  //电表通讯中断
				{
					ALARMDef.stationID = pAlarmInfo->reserve;
				}
				if (ALARMDef.alarmType==ALARMTYPE_STATION_UNCOMMUN
					&&pAlarmInfo->Status==ALARMSTATUS_NEWALARM)  //集中器通讯中断
				{
					m_YxValueInfoCArray.RemoveAll();
					GetDIIndexByStationId(ALARMDef.alarmObjID);
					Update2RTDB();
					m_YxValueInfoCArray.RemoveAll();
				}
				memcpy(ALARMDef.alarmTypeName,iAlarmType.m_cstrName,sizeof(ALARMDef.alarmTypeName));
				//memcpy(ALARMDef.alarmObjName,0,sizeof(ALARMDef.alarmObjName));

				ALARMDef.alarmObjType = pAlarmInfo->ObjectType;
				if (ALARMDef.alarmObjType==31)
				{
					ALARMDef.devID = ALARMDef.alarmObjID;
				}
				CTime curTime = CTime::GetCurrentTime();
				ALARMDef.contentTime = curTime.GetTime();
				curTime = CTime(curTime.GetYear(),curTime.GetMonth(),curTime.GetDay(),0,0,0);
				ALARMDef.alarmTime = pAlarmInfo->AlarmTime.GetTime();

				ALARMDef.status = pAlarmInfo->Status;

				ALARMDef.projectID = 1;    //工程号
				ALARMDef.systemID = 1;    //子系统号
				ALARMDef.rtAlarm = 0;    //不记录实时告警
				memset(ALARMDef.stationName, 0, sizeof(ALARMDef.stationName));
				memset(ALARMDef.projectName, 0, sizeof(ALARMDef.projectName));
				memcpy(ALARMDef.alarmSource,_T("应用服务告警"),sizeof(ALARMDef.alarmSource));
				ALARMDef.groupID1 = iAlarmType.m_iGroupId1;
				ALARMDef.groupID2 = iAlarmType.m_iGroupId2;
				memcpy(ALARMDef.alarmContent,pAlarmInfo->Contents,sizeof(ALARMDef.alarmContent));
				memcpy(ALARMDef.reserve5,pAlarmInfo->Suggest,sizeof(ALARMDef.reserve5));

				if (ALARMDef.status==ALARMSTATUS_NEWALARM)
				{
					AddAlarm2DB(ALARMDef);
					//Send2SMSRecorde();
				} 
				else if(ALARMDef.status==ALARMSTATUS_REVERSE)
				{
					ChangeAlarmStatus(ALARMDef);
				}
				//插入到短信表
				if(ALARMDef.alarmType==8006)
				{
					return TRUE;
				}
				CTSAlarmServer_WDDlg* pDlg = (CTSAlarmServer_WDDlg*)AfxGetApp()->m_pMainWnd;
				DeviceInfoData iDeviceInfo;
				memset(&iDeviceInfo,0,sizeof(DeviceInfoData));
				pDlg->m_pAlarmTask->m_DeviceInfoMap.Lookup(ALARMDef.devID,iDeviceInfo);
				CString iKey = _T("");
				iKey.Format(_T("%d_%d"),iDeviceInfo.iPlazaId,ALARMDef.alarmType);
				SMSConfigDef iSMSCofig;
				memset(&iSMSCofig,0,sizeof(SMSConfigDef));
				if(pDlg->m_pAlarmTask->m_SMSConfigMap.Lookup(iKey,iSMSCofig))
				{
					if (iSMSCofig.IsShield==0)
					{
						CString TemplateContent = _T("");
						pDlg->m_pAlarmTask->m_SMSTemplateMap.Lookup(iSMSCofig.TemplateNum,TemplateContent);
						CString Owner = _T("");
						CString AlarmObject = _T("");
						CString strAlarmType = _T("");
						Owner.Format(_T("%s"),iSMSCofig.PersonName);
						AlarmObject.Format(_T("%s%s品牌下的%s"),iDeviceInfo.PlazaName,iDeviceInfo.BrandName,iDeviceInfo.DevName);
						strAlarmType.Format(_T("%s"),strAlarmTypeName);
						TemplateContent.Replace(_T("[owner]"),Owner);
						TemplateContent.Replace(_T("[object]"),AlarmObject);
						TemplateContent.Replace(_T("[alarmtype]"),strAlarmType);
						Insert2SMSRECORD(iSMSCofig,TemplateContent);
					}
				}
			}
			
		}
		else
		{
			ReguTrace(ERR,"REDIS_DATA ERR!");
			if (buffer!=NULL)
			{
				delete [] buffer;
				buffer = NULL;
			}
			return FALSE;
		}
		
		
	}
	else if (REDIS_NODATA == ret)
	{
		OutputDebugString(_T("REDIS_NODATA \n"));
		return FALSE;
	}

	if (buffer!=NULL)
	{
		delete [] buffer;
		buffer = NULL;
	}
	return TRUE;
}


#include <Rpcdce.h>
#pragma comment(lib,"Rpcrt4.lib")
void CReceiveAlarmData::CreateUuid(TCHAR* csID)
{
	UUID uuidRoute;
	RPC_STATUS st;

	st = UuidCreate(&uuidRoute);
	if (st != RPC_S_OK)
	{
		return;
	}
	TCHAR* pszUuid;     
	UuidToString(&uuidRoute, (RPC_WSTR*)&pszUuid);
	//stringcpy(csID, pszUuid);
	int len = sizeof(csID);
	memcpy(csID,pszUuid,sizeof(TCHAR)*37);
	RpcStringFree((RPC_WSTR*)&pszUuid);
}

BOOL CReceiveAlarmData::GetDIIndexByStationId(int StationId)
{
	HANDLE hPipe = NULL;
	YxConfigDef YxConfig;
	NetMessageHead* pNetMessageHead = NULL;
	MessageQueryRecord mqr;
	hPipe = OpenRealDataPipe();
	if (hPipe == NULL) {
		return FALSE;
	}
	mqr.BaseNo = BASE_NO_RTDB;
	mqr.TableNo = TABLE_NO_DI;
	mqr.FilterFieldNo = 3;//0;
	mqr.FilterType = FILTER_TYPE_EQU; 
	mqr.StartValue = StationId;
	mqr.EndValue = mqr.StartValue;
	pNetMessageHead = (NetMessageHead*)GetMessage_RecordOfTable(hPipe,&mqr);
	if (pNetMessageHead==NULL) {
		CloseHandle(hPipe);
		hPipe = NULL;
		return FALSE;
	}	

	if (pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB) {
		delete[] (BYTE *)pNetMessageHead; 
		pNetMessageHead = NULL;
		CloseHandle(hPipe);
		hPipe = NULL;
		return FALSE;
	}

	BYTE* pBuf = (BYTE*)pNetMessageHead;
	WORD* pWord = (WORD*)&pBuf[sizeof(NetMessageHead)];
	int nFieldNum = (int)*pWord;
	pWord++;
	MessageRetRecordHead* pRetRecordHead = (MessageRetRecordHead*)pWord;
	pBuf = (BYTE*)pWord;
	DWORD* pDword = (DWORD*)&pBuf[sizeof(MessageRetRecordHead)*nFieldNum];

	int dwRecordLen = *pDword; pDword++;
	int dwRecordNum = *pDword; pDword++;
	pBuf = (BYTE*)pDword;

	int nNum = dwRecordNum;
	int nOffset, i=0;
	while (nNum) {
		nOffset=0;
		memset(&YxConfig, 0, sizeof(YxConfigDef));
		memcpy(&YxConfig.nYxIndex,pBuf,pRetRecordHead->DataLen);
		nOffset+=pRetRecordHead->DataLen;
		//memcpy(&YxConfig.nProjectNo,pBuf+nOffset,(pRetRecordHead+1)->DataLen);
		nOffset+=(pRetRecordHead+1)->DataLen;
		//memcpy(&YxConfig.nStationNo,pBuf+nOffset,(pRetRecordHead+2)->DataLen);
		nOffset+=(pRetRecordHead+2)->DataLen;
		//memcpy(&YxConfig.nDeviceNo,pBuf+nOffset,(pRetRecordHead+3)->DataLen);
		nOffset+=(pRetRecordHead+3)->DataLen;
		memcpy(&YxConfig.nYxNum,pBuf+nOffset,(pRetRecordHead+4)->DataLen);
		nOffset+=(pRetRecordHead+4)->DataLen;
		//memcpy(YxConfig.cYxName,pBuf+nOffset,(pRetRecordHead+5)->DataLen);
		//nOffset+=(pRetRecordHead+5)->DataLen;
		//memcpy(YxConfig.cDescription,pBuf+nOffset,(pRetRecordHead+6)->DataLen);
		//nOffset+=(pRetRecordHead+6)->DataLen;
		//memcpy(&YxConfig.bYxRaw,pBuf+nOffset,(pRetRecordHead+7)->DataLen);
		//nOffset+=(pRetRecordHead+7)->DataLen;
		//memcpy(&YxConfig.bYxType,pBuf+nOffset,(pRetRecordHead+8)->DataLen);
		//nOffset+=(pRetRecordHead+8)->DataLen;
		//memcpy(&YxConfig.bYxValue,pBuf+nOffset,(pRetRecordHead+9)->DataLen);
		//nOffset+=(pRetRecordHead+9)->DataLen;

		if (YxConfig.nYxNum==0)
		{
			YxConfig.bYxRaw = 0;
			YxConfig.bYxValue = 0;
			m_YxValueInfoCArray.Add(YxConfig);
		}
		
		pBuf+=dwRecordLen;
		nNum--;
	}
	if(hPipe != NULL)			{CloseHandle(hPipe); hPipe = NULL;}
	if(pNetMessageHead != NULL){delete [](BYTE *)pNetMessageHead; pNetMessageHead=NULL;}

	return TRUE;
}

BOOL CReceiveAlarmData::initAlarmType()
{
	m_mapAlarmType.clear();
	int iFdCount = 5;	//	查询的字段数
	int iMaxRsCount = -1;	// 限制查询得到的最大记录个数，-1指不限制


	// 向数据管道发送SQL语句
	BYTE* pRead = NULL;

	HANDLE hPipe =OpenRealDataPipe();

	if (hPipe == NULL) 
	{
		MYERROR(_T("InitDevInfo hPipe null"));
		return FALSE;
	}
	CString strQuery = _T("select GROUPID1,GROUPID2,TYPE,NAME,ALARMLEVEL from TB_ALARMTYPE");
	pRead = (BYTE*)GetMessage_RecordOfSql(hPipe,strQuery);

	if (NULL == pRead)	// 未查询到记录
	{
		CloseHandle(hPipe);
		MYERROR(_T("InitDevInfo pRead null"));
		return FALSE;
	}
	// 解析返回的结果
	NetMessageHead *pNetMessageHead = (NetMessageHead*)pRead;
	if (pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB) {
		//	MYWARN(_T("返回报文异常，%s"),bufToHexString(pRead,pNetMessageHead->Length).c_str());
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("InitDevInfo pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB null"));
		return FALSE;
	}	

	if (pNetMessageHead->Length < 1)	// 空报文，无查询结果
	{
		//MYWARN(_T("接收到空回复报文，无法解析查询结果"));
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("InitDevInfo pNetMessageHead->Length < 1 null"));
		return FALSE;
	}

	// 获取查询的字段数
	WORD* pWord = (WORD*)&pRead[sizeof(NetMessageHead)];
	int nFieldNum = (int)*pWord;
	if (nFieldNum != iFdCount)	// 查询字段数量，不相等，曾查询结果异常退出
	{
		//	MYWARN(_T("查询字段数量，不相等，曾查询结果异常退出"));
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("InitDevInfo nFieldNum != iFdCount null"));
		return FALSE;
	}
	pWord++;

	// 记录集长度
	MessageRetRecordHead* pHead = (MessageRetRecordHead*)pWord;
	BYTE *pByte = (BYTE*)&pHead[nFieldNum];
	DWORD dwRecordLen = *((DWORD*)pByte);
	if (dwRecordLen < 1)
	{
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		return FALSE;
	}
	pByte += sizeof(DWORD);

	// 记录数
	DWORD dwRecordNum = *((int*)pByte);
	if ((iMaxRsCount > 0) && (dwRecordNum != iMaxRsCount))
	{
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		return FALSE;
	}
	pByte += sizeof(DWORD);

	// 将查询结果写入到缓存
	LZGRECORDSET rs;
	for (int i=0;i<(int)dwRecordNum;i++)
	{
		AlarmTypeDef alarmType;

		rs.clear();
		memcpy(rs.buf, pByte, pHead->DataLen);
		alarmType.m_iGroupId1 = rs.iRs;
		pByte += pHead->DataLen;
		rs.clear();

		//系统ID
		memcpy(rs.buf, pByte, (pHead+1)->DataLen);
		alarmType.m_iGroupId2 = rs.iRs;
		pByte += (pHead+1)->DataLen;
		rs.clear();

		//TYPE
		memcpy(rs.buf, pByte, (pHead+2)->DataLen);
		alarmType.m_iAlarmType = rs.iRs;
		pByte += (pHead+2)->DataLen;
		rs.clear();

		//告警Name
		memcpy(rs.buf, pByte, (pHead+3)->DataLen);
		TCHAR chName[33];
		memcpy(chName,rs.cBuf,sizeof(TCHAR)*33);
		memcpy(alarmType.m_cstrName,chName,sizeof(TCHAR)*33);
		pByte += (pHead+3)->DataLen;
		rs.clear();

		//告警等级
		memcpy(rs.buf, pByte, (pHead+4)->DataLen);
		alarmType.m_iAlarmLevel = rs.iRs;
		pByte += (pHead+4)->DataLen;
		rs.clear();
		m_mapAlarmType.insert(make_pair(alarmType.m_iAlarmType,alarmType));	
		
	}
	if(hPipe != NULL)
	{
		CloseHandle(hPipe); 
		hPipe = NULL;
	}
	return TRUE;
}	

void CReceiveAlarmData::AddAlarm2DB(AlarmItemDef ALARMDef)
{
	HANDLE hPipe = OpenRealDataPipe();
	int ReConnTime = 0;
	while (hPipe == NULL)
	{
		if (ReConnTime>=3)
		{
			ReguTrace(SQLERRO,"OpenRealDataPipe ERRO hPipe==null!");
			return ;
		}
		ReConnTime++;
		Sleep(1000);
		hPipe = OpenRealDataPipe();

	}
	BYTE* pRead = NULL;

	CString strSql;
	CTime dt1 = CTime(ALARMDef.alarmTime);
	CTime dt2 = CTime(ALARMDef.contentTime);

	CString ctrAlarmTime = _T("");
	ctrAlarmTime.Format(_T("%d-%d-%d %d:%d:%d"),dt1.GetYear(),
		dt1.GetMonth(),
		dt1.GetDay(),
		dt1.GetHour(),
		dt1.GetMinute(),
		dt1.GetSecond());
	CString strAlarmID(ALARMDef.alarmID);
	strAlarmID.Replace(_T("'"), _T("''"));
	CString strAlarmTypeName(ALARMDef.alarmTypeName);
	strAlarmTypeName.Replace(_T("'"), _T("''"));
	CString strAlarmSource(ALARMDef.alarmSource);
	strAlarmSource.Replace(_T("'"), _T("''"));
	CString strAlarmObjName(ALARMDef.alarmObjName);
	strAlarmObjName.Replace(_T("'"), _T("''"));
	CString strDevName(ALARMDef.devName);
	strDevName.Replace(_T("'"), _T("''"));
	CString strStationName(ALARMDef.stationName);
	strStationName.Replace(_T("'"), _T("''"));
	CString strAlarmContent(ALARMDef.alarmContent);
	strAlarmContent.Replace(_T("'"), _T("''"));
	CString strPrjName(ALARMDef.projectName);
	strPrjName.Replace(_T("'"), _T("''"));
	CString strReserve5(ALARMDef.reserve5);
	strReserve5.Replace(_T("'"), _T("''"));
	CString strReserve6(ALARMDef.reserve6);
	strReserve6.Replace(_T("'"), _T("''"));

	
	strSql.Format(_T("insert into TE_ALARM_INDEX (ID,GROUP1,GROUP2,ALARMTYPE,ALARMTYPENAME,ALARMLEVEL,ALARMSOURCE,ALARMOBJTYPE,PROJECTID,\
					 SYSTEMID,ALARMOBJID,DEVID,STATIONID,ALARMOBJNAME,DEVNAME,STATIONNAME,ALARMCONTENT,PROJECTNAME,RTALARM,STATUS,ALARMTIME,CONTENTTIME,\
					 RESERVE1,RESERVE2,RESERVE3,RESERVE4,RESERVE5,RESERVE6) values ('%s',%d,%d,%d,'%s',%d,'%s',%d,%d,%d,%d,%d,%d,'%s','%s','%s','%s','%s',\
					 %d,%d,'%d-%d-%d %d:%d:%d','%d-%d-%d %d:%d:%d',%d,%d,%f,%f,'%s','%s') "),
					 strAlarmID,
					 ALARMDef.groupID1,
					 ALARMDef.groupID2,
					 ALARMDef.alarmType,
					 strAlarmTypeName,
					 ALARMDef.alarmLevel,
					 strAlarmSource,
					 ALARMDef.alarmObjType,
					 ALARMDef.projectID,
					 ALARMDef.systemID,
					 ALARMDef.alarmObjID,
					 ALARMDef.devID,
					 ALARMDef.stationID,
					 strAlarmObjName,
					 strDevName,
					 strStationName,
					 strAlarmContent,
					 strPrjName,
					 ALARMDef.rtAlarm,
					 ALARMDef.status,
					 dt1.GetYear(),
					 dt1.GetMonth(),
					 dt1.GetDay(),
					 dt1.GetHour(),
					 dt1.GetMinute(),
					 dt1.GetSecond(),
					 dt2.GetYear(),
					 dt2.GetMonth(),
					 dt2.GetDay(),
					 dt2.GetHour(),
					 dt2.GetMinute(),
					 dt2.GetSecond(),
					 ALARMDef.reserve1,
					 ALARMDef.reserve2,
					 ALARMDef.reserve3,
					 ALARMDef.reserve4,
					 strReserve5,
					 strReserve6);
	//*****************************
	CString strSqlFinal = _T("");
	/*strSqlFinal.Format(C_SQL_INSERT_ALARMINDEX,ALARMDef.alarmType,ALARMDef.alarmObjID,ALARMDef.alarmObjType,strSql,strAlarmContent,ctrAlarmTime,
		ALARMDef.alarmType,ALARMDef.alarmObjID,ALARMDef.alarmObjType);*/
	ReguTrace(SQL,"告警记录入库SQL:<%s>",strSql);
	pRead = (BYTE*)GetMessage_RecordOfSql_Ext(hPipe, strSql);
	if (pRead)
	{
		delete[] pRead;
		pRead = NULL;
	}
	if (hPipe != NULL)
	{
		CloseHandle(hPipe);
		hPipe = NULL;
	}

}


void CReceiveAlarmData::ChangeAlarmStatus(AlarmItemDef ALARMDef)
{
	HANDLE hPipe = OpenRealDataPipe();
	int ReConnTime = 0;
	while (hPipe == NULL)
	{
		if (ReConnTime>=3)
		{
			ReguTrace(SQLERRO,"OpenRealDataPipe ERRO hPipe==null!");
			return ;
		}
		ReConnTime++;
		Sleep(1000);
		hPipe = OpenRealDataPipe();

	}
	CTime dt2 = CTime(ALARMDef.alarmTime);

	CString ctrContentimeTime = _T("");
	ctrContentimeTime.Format(_T("%d-%d-%d %d:%d:%d"),dt2.GetYear(),
		dt2.GetMonth(),
		dt2.GetDay(),
		dt2.GetHour(),
		dt2.GetMinute(),
		dt2.GetSecond());
	CString strAlarmContent(ALARMDef.alarmContent);
	strAlarmContent.Replace(_T("'"), _T("''"));
	BYTE* pRead = NULL;
	CString strPreSql = _T("");
	strPreSql.Format(_T("if exists (select id from TE_ALARM_INDEX where ALARMTYPE=%d and ALARMOBJID=%d and ALARMOBJTYPE=%d and STATUS=0) "),
		ALARMDef.alarmType,
		ALARMDef.alarmObjID,
		ALARMDef.alarmObjType);
	CString strUpdateSql = _T("");
	strUpdateSql.Format(_T("update TE_ALARM_INDEX set STATUS=2,ALARMCONTENT='%s', CONTENTTIME='%s' where ALARMTYPE=%d and ALARMOBJID=%d and ALARMOBJTYPE=%d and STATUS=0"),
		strAlarmContent,
		ctrContentimeTime,
		ALARMDef.alarmType,
		ALARMDef.alarmObjID,
		ALARMDef.alarmObjType);
	ReguTrace(SQL,"告警状态更新SQL:<%s>",strUpdateSql);
	pRead = (BYTE*)GetMessage_RecordOfSql_Ext(hPipe, strUpdateSql);
	if (pRead)
	{
		delete[] pRead;
		pRead = NULL;
	}
	if (hPipe != NULL)
	{
		CloseHandle(hPipe);
		hPipe = NULL;
	}
}

BOOL CReceiveAlarmData::Update2RTDB()
{
	HANDLE hPipe=NULL;	 
	hPipe = OpenRealDataPipe();
	if (hPipe==NULL)
		return FALSE;
	for (int i=0; i<m_YxValueInfoCArray.GetSize();i++)
	{
		YxConfigDef iYxConfig;
		iYxConfig = m_YxValueInfoCArray[i];
		BYTE *pData;
		int nRecordLen = sizeof(YxConfigDef);

		DWORD dwBufLen = nRecordLen + sizeof(UpdataRecordStruct) + sizeof(NetMessageHead);
		BYTE* pWrite = new BYTE[dwBufLen];
		if (NULL==pWrite)
		{
			ReguTrace(ERRO,"YxValue pWrite==null!");
			if (hPipe != NULL)
			{
				CloseHandle(hPipe);
				hPipe = NULL;
			}
			return FALSE;
		}
		NetMessageHead* pMessageHead = (NetMessageHead*)pWrite;										  
		pMessageHead->MessageType =	NET_MESSAGE_UPDATARECORD;						 
		pMessageHead->Length = nRecordLen + sizeof(UpdataRecordStruct) - 1;							 
		UpdataRecordStruct* pUpdataStruct = (UpdataRecordStruct*)&pWrite[sizeof(NetMessageHead)];

		pUpdataStruct->TableNo = TABLE_NO_DI;	
		memset(&(pUpdataStruct->ColFlag[0]),0,256);

		pUpdataStruct->ColFlag[7] = 1;
		pUpdataStruct->ColFlag[9] = 1;

		pUpdataStruct->UpdataType = UPDATARECORD_TYPE_UPDATA; 
		pUpdataStruct->RecordLen = nRecordLen;

		BYTE *pData1;
		pData = new BYTE[nRecordLen];
		if (NULL==pData)
		{
			ReguTrace(ERRO,"YxValue pData==null!");
			if (hPipe != NULL)
			{
				CloseHandle(hPipe);
				hPipe = NULL;
			}
			return FALSE;
		}
		memset(pData,0,nRecordLen);
		int nOffset = 0;
		pData1 = pData;

		memcpy(pData,&iYxConfig.nYxIndex,sizeof(iYxConfig.nYxIndex));
		nOffset+=sizeof(iYxConfig.nYxIndex);

		memcpy((pData+nOffset),&iYxConfig.nProjectNo,sizeof(iYxConfig.nProjectNo));
		nOffset+=sizeof(iYxConfig.nProjectNo);

		memcpy((pData+nOffset),&iYxConfig.nStationNo,sizeof(iYxConfig.nStationNo));
		nOffset+=sizeof(iYxConfig.nStationNo);

		memcpy((pData+nOffset),&iYxConfig.nDeviceNo,sizeof(iYxConfig.nDeviceNo));
		nOffset+=sizeof(iYxConfig.nDeviceNo);

		memcpy((pData+nOffset),&iYxConfig.nYxNum,sizeof(iYxConfig.nYxNum));
		nOffset+=sizeof(iYxConfig.nYxNum);

		memcpy((pData+nOffset),iYxConfig.cYxName,sizeof(iYxConfig.cYxName));
		nOffset+=sizeof(iYxConfig.cYxName);

		memcpy((pData+nOffset),iYxConfig.cDescription,sizeof(iYxConfig.cDescription));
		nOffset+=sizeof(iYxConfig.cDescription);

		memcpy((pData+nOffset),&iYxConfig.bYxRaw,sizeof(iYxConfig.bYxRaw));
		nOffset+=sizeof(iYxConfig.bYxRaw);

		memcpy((pData+nOffset),&iYxConfig.bYxType,sizeof(iYxConfig.bYxType));
		nOffset+=sizeof(iYxConfig.bYxType);

		memcpy((pData+nOffset),&iYxConfig.bYxValue,sizeof(iYxConfig.bYxValue));
		nOffset+=sizeof(iYxConfig.bYxValue);

		memcpy((pData+nOffset),&iYxConfig.bIdentifier,sizeof(iYxConfig.bIdentifier));
		nOffset+=sizeof(iYxConfig.bIdentifier);

		memcpy((pData+nOffset),&iYxConfig.nCountExchange,sizeof(iYxConfig.nCountExchange));
		nOffset+=sizeof(iYxConfig.nCountExchange);

		memcpy((pData+nOffset),&iYxConfig.nCountTrip,sizeof(iYxConfig.nCountTrip));
		nOffset+=sizeof(iYxConfig.nCountTrip);

		memcpy((pData+nOffset),&iYxConfig.nAlarmType,sizeof(iYxConfig.nAlarmType));
		nOffset+=sizeof(iYxConfig.nAlarmType);

		memcpy((pData+nOffset),&iYxConfig.nAlarmLevel,sizeof(iYxConfig.nAlarmLevel));
		nOffset+=sizeof(iYxConfig.nAlarmLevel);

		memcpy((pData+nOffset),&iYxConfig.nYxProp,sizeof(iYxConfig.nYxProp));
		nOffset+=sizeof(iYxConfig.nYxProp);	

		memmove(&pUpdataStruct->RecordData,pData1,nRecordLen);

		BYTE* pRead;
		if (hPipe)
			pRead = (BYTE*)GetNetMessage(hPipe, pWrite,dwBufLen-1);	

		if (pWrite!=NULL)
		{
			delete[] pWrite;
		}

		if (pRead==NULL) 
		{
			delete[] pData1;
			continue;
		}
		pMessageHead = (NetMessageHead*)pRead;
		MessageAck* pAck = (MessageAck*)&pRead[sizeof(NetMessageHead)];
		if (pMessageHead->MessageType==NET_MESSAGE_ACK && pAck->wdAckType==NET_MESSAGE_ACK_OK) 
		{
			delete[] pRead;
			delete[] pData1;
			continue;
		}
		delete[] pRead;
		delete[] pData1;
	}
	if (hPipe != NULL)
	{
		CloseHandle(hPipe);
		hPipe = NULL;
	}
	return TRUE;

}

void CReceiveAlarmData::Insert2SMSRECORD(SMSConfigDef iSMSCofig,CString TemplateContent)
{
	HANDLE hPipe = OpenRealDataPipe();
	int ReConnTime = 0;
	while (hPipe == NULL)
	{
		if (ReConnTime>=3)
		{
			ReguTrace(SQLERRO,"OpenRealDataPipe ERRO hPipe==null!");
			return ;
		}
		ReConnTime++;
		Sleep(1000);
		hPipe = OpenRealDataPipe();

	}
	BYTE* pRead = NULL;

	CTime CurTime = CTime::GetCurrentTime();

	CString strSqlFinal = _T("");
	strSqlFinal.Format(_T("INSERT INTO TE_SMSRECORD VALUES(1,'%s','%s','%s','%d-%d-%d %d:%d:%d',1)"),
		iSMSCofig.PersonName,iSMSCofig.PhoneNum,TemplateContent,CurTime.GetYear(),CurTime.GetMonth(),
		CurTime.GetDay(),CurTime.GetHour(),CurTime.GetMinute(),CurTime.GetSecond());
	ReguTrace(SQL,"短信记录入库SQL:<%s>",strSqlFinal);
	pRead = (BYTE*)GetMessage_RecordOfSql_Ext(hPipe, strSqlFinal);
	if (pRead)
	{
		delete[] pRead;
		pRead = NULL;
	}
	if (hPipe != NULL)
	{
		CloseHandle(hPipe);
		hPipe = NULL;
	}

}

int CReceiveAlarmData::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CReceiveAlarmData, CWinThread)
END_MESSAGE_MAP()


// CReceiveAlarmData 消息处理程序
