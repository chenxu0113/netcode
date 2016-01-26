// RedisRecvSample.cpp : 实现文件
//
#pragma once
#include "stdafx.h"
#include "RedisRecvSample.h"
#include "TSSampleDataSvr.h"
#include "TSSampleDataSvrDlg.h"

// CRedisRecvSample
CLog *g_log=NULL;
CLog *DataTime_log=NULL;
IMPLEMENT_DYNCREATE(CRedisRecvSample, CWinThread)

CRedisRecvSample::CRedisRecvSample()
{
	beInited = FALSE;
	m_bAutoDelete = TRUE;
	MaxStationNum = 0;
	m_bHasRegister = FALSE;
	m_softbus = NULL;
	m_softbus = new CRedisBus;
	BillNum = 0;
	m_strPath.Empty();
	CTime TempTime = CTime::GetCurrentTime();
	m_DayTime = CTime(TempTime.GetYear(),TempTime.GetMonth(),TempTime.GetDay(),0,0,0);
	if (NULL == m_softbus)
	{
		return;
	}
	if(NULL==g_log)
	{
		g_log = new CLog(_T("\\TSSampleDataSvr"));
	}
	InitCfgInfo();
	InitializeCriticalSection(&m_csYmConfigDefMap);
	InitializeCriticalSection(&m_csBillNum);
	InitializeCriticalSection(&m_csDayTime);
	InitializeCriticalSection(&m_csDevStateMap);
	//InitializeCriticalSection(&m_csUpdateRTDB);

}

BOOL CRedisRecvSample::InitCfgInfo()
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

CRedisRecvSample::~CRedisRecvSample()
{
	if (m_softbus!=NULL)
	{
		delete m_softbus;
		m_softbus = NULL;
	}
}

BOOL CRedisRecvSample::ConnectRedisServer()
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

BOOL CRedisRecvSample::InitInstance()
{
	ReguTrace(Config,"LoadAllForeignTable begin!");
	if (!LoadAllForeignTable())
	{
		exit(0);
		//return FALSE;
	}
	ReguTrace(Config,"LoadAllForeignTable end!");
	if (!GetDevInfo())
	{
		ReguTrace(Config,"GetDevInfo ERR!");
		exit(0);
		//return FALSE;
	}
	ReguTrace(Config,"GetDevInfo end!num=%d",m_DeviceInfoMap.GetSize());
	//if(!ConnectRedisServer())
	//{
	//	ReguTrace(Config,"ConnectRedisServer err!");
	//	exit(0);
	//	//return FALSE;
	//}
	//ReguTrace(Config,"ConnectRedisServer end!");
	//如果NTS-9000\data_sql\2015-11-23文件夹不存在，则创建
	if(m_strPath.IsEmpty()) 
	{
		TCHAR szDirectory[MAX_PATH];
		GetModuleFileName(GetModuleHandle(NULL),szDirectory,MAX_PATH);
		CString strPath = szDirectory;	
		int nIndex = strPath.ReverseFind('\\');
		if (nIndex != 0)
		{
			strPath = strPath.Left(nIndex);
		}
		else
		{
		}
		m_strPath = strPath;
	}

	SYSTEMTIME tt;
	//CString sFileName = _T("");
	GetLocalTime(&tt);
	//sFileName.Format(_T("%04d-%02d-%02d"), tt.wYear, tt.wMonth, tt.wDay);
	CString sPath = _T("\\data_sql") ;

	CFileFind fFind;
	if (!fFind.FindFile(m_strPath + sPath))
	{
		if (!CreateDirectory(m_strPath + sPath, NULL))
		{
			ReguTrace(ERRO,"创建文件夹出错,path=%s!",m_strPath+sPath);
		}
	}

	beInited = TRUE;
	BOOL bFlag = TRUE;
	while(!theApp.bExitFlag)
	{
		SYSTEMTIME sys_time;
		GetLocalTime(&sys_time);
		if ((sys_time.wHour==22&&sys_time.wMinute==0&&bFlag)
			||(sys_time.wHour==7&&sys_time.wMinute==0&&bFlag))
		{
			if (!GetDevInfo())
			{
				ReguTrace(Config,"GetDevInfo ERR!");
				continue;
			}
			ReguTrace(Config,"更新DevInfo成功!num=%d",m_DeviceInfoMap.GetSize());
			bFlag = FALSE;
		}
		if (sys_time.wHour==0&&sys_time.wMinute==0)
		{
			bFlag = TRUE;
		}
		Sleep(10000);
	}
	/*while(0)
	{
		BOOL bRet = FALSE;
		bRet = GetDataFromRedis();
		if (!bRet)
		{
			Sleep(1000);
		}
		SYSTEMTIME sys_time;
		GetLocalTime(&sys_time);
		if ((sys_time.wHour==22&&sys_time.wMinute==0&&bFlag)
			||(sys_time.wHour==7&&sys_time.wMinute==0&&bFlag))
		{
			if (!GetDevInfo())
			{
				ReguTrace(Config,"GetDevInfo ERR!");
				continue;
			}
			ReguTrace(Config,"更新DevInfo成功!");
			bFlag = FALSE;
		}
		if (sys_time.wHour==0&&sys_time.wMinute==0)
		{
			bFlag = TRUE;
		}
		Sleep(1);
	}*/
	ReguTrace(ERRO,"跳出while循环!");
	return TRUE;
}

BOOL  CRedisRecvSample::GetDevInfo()
{
	m_DeviceInfoMap.RemoveAll();
	int iFdCount = 8;	//	查询的字段数
	int iMaxRsCount = -1;	// 限制查询得到的最大记录个数，-1指不限制


	// 向数据管道发送SQL语句
	BYTE* pRead = NULL;

	HANDLE hPipe =OpenRealDataPipe();

	if (hPipe == NULL) 
	{
		MYERROR(_T("GetPlazaId hPipe null"));
		return FALSE;
	}

	int tempId = 0;
	while (1)
	{
		CString strQuery;
		//strQuery.Format(_T("select top(10000) DEVICEID,b.ID,b.CNAME from TB_PAYMENT_DEVICE_EX a left join TB_PAYMENT_ROOM b on(a.PLAZAID=b.ID) where deviceid > %d order by deviceid; "),tempId);
		strQuery.Format(_T("select top 5000 TB_DEVICE.ID,TB_DEVICE.DEVTYPE,pl.ID as plazaid,pl.CNAME as plazaname,TB_PAYMENT_ACCOUNT.ID as brandid,TB_PAYMENT_ACCOUNT.CNAME as brandname,rm.cname as roomname,TB_PAYMENT_ACCOUNT.NICKNAME \
						   from TB_DEVICE left join TB_PAYMENT_DEVICE_EX on TB_PAYMENT_DEVICE_EX.DEVICEID=TB_DEVICE.ID left JOIN TL_PAYMENT_ROOM ON TB_PAYMENT_DEVICE_EX.DEVICEID =TL_PAYMENT_ROOM.DEVICEID  left JOIN \
						   TB_PAYMENT_ROOM rm on rm.ID=TL_PAYMENT_ROOM.ROOMID left JOIN TB_PAYMENT_ROOM pl on TB_PAYMENT_DEVICE_EX.PLAZAID=pl.ID  left JOIN TB_PAYMENT_ACCOUNT ON  rm.ACCOUNTID = TB_PAYMENT_ACCOUNT.ID \
						   where TB_DEVICE.ID > %d order by TB_DEVICE.ID;"),tempId);
		pRead = (BYTE*)GetMessage_RecordOfSql(hPipe,strQuery);

		if (NULL == pRead)	// 未查询到记录
		{
			CloseHandle(hPipe);
			MYERROR(_T("GetPlazaId pRead null"));
			return FALSE;
		}
		// 解析返回的结果
		NetMessageHead *pNetMessageHead = (NetMessageHead*)pRead;
		if (pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB) {
			//	MYWARN(_T("返回报文异常，%s"),bufToHexString(pRead,pNetMessageHead->Length).c_str());
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			MYERROR(_T("GetPlazaId pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB null"));
			return FALSE;
		}	

		if (pNetMessageHead->Length < 1)	// 空报文，无查询结果
		{
			//MYWARN(_T("接收到空回复报文，无法解析查询结果"));
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			MYERROR(_T("GetPlazaId pNetMessageHead->Length < 1 null"));
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
			MYERROR(_T("GetPlazaId nFieldNum != iFdCount null"));
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

		TCHAR *tValue = new TCHAR[1024+1];
		if (tValue == NULL)
		{
			//MYERROR(_T("分配内存错误，错误码：%d"),GetLastError());
			MYERROR(_T("GetPlazaId tValue == NULL"));
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			return FALSE;
		}

		// 将查询结果写入到缓存
		for (int i=0;i<(int)dwRecordNum;i++)
		{
			DeviceInfo iDeviceInfo;
			memset(&iDeviceInfo,0,sizeof(DeviceInfo));

			// iDevId
			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, pHead->DataLen);
			iDeviceInfo.iDevId = (int)*((DWORD *)tValue);
			pByte += pHead->DataLen;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+1)->DataLen);
			iDeviceInfo.iDevType = (int)*((DWORD *)tValue);
			pByte += (pHead+1)->DataLen;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+2)->DataLen);
			iDeviceInfo.iPlazaId = (int)*(DWORD*)tValue;
			pByte += (pHead+2)->DataLen;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+3)->DataLen);
			memcpy(iDeviceInfo.PlazaName, tValue, sizeof(TCHAR)*128);
			pByte += (pHead+3)->DataLen;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+4)->DataLen);
			iDeviceInfo.iBrandId = (int)*(DWORD*)tValue;
			pByte += (pHead+4)->DataLen;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+5)->DataLen);
			CString csBrand = _T("");
			csBrand.Format(_T("%s"),tValue);
			csBrand.Replace(_T("'"),_T("\""));
			memcpy(iDeviceInfo.BrandName, csBrand.GetBuffer(), sizeof(TCHAR)*csBrand.GetLength());
			pByte += (pHead+5)->DataLen;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+6)->DataLen);
			memcpy(iDeviceInfo.RoomName, tValue, sizeof(TCHAR)*128);
			pByte += (pHead+6)->DataLen;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+7)->DataLen);
			memcpy(iDeviceInfo.NickName, tValue, sizeof(TCHAR)*600);
			pByte += (pHead+7)->DataLen;

			if (iDeviceInfo.iDevId>tempId)
			{
				tempId = iDeviceInfo.iDevId;
			}
			m_DeviceInfoMap.SetAt(iDeviceInfo.iDevId,iDeviceInfo);
		}
		delete []tValue;
		tValue = NULL;
		if (dwRecordNum<5000)
		{
			break;
		}
	}

	int num = m_DeviceInfoMap.GetSize();

	delete[] pRead;
	pRead = NULL;
	CloseHandle(hPipe);

	return TRUE;
}

BOOL CRedisRecvSample::GetDataFromRedis()
{
	/*unsigned char buffer[1024];
	memset(buffer,0,1024);*/
	CTSSampleDataSvrDlg* pDlg = (CTSSampleDataSvrDlg*)AfxGetApp()->m_pMainWnd;
	unsigned char * buffer = NULL;
	int length = 0;
	int ret = REDIS_NODATA;
	ret = m_softbus->RecvMessageFromMsmq_Ext("SampleMessage",&buffer); 
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
		//ReguTrace(Config,"收到初始报文:%s",bufToHexString(buffer,100).c_str());
		CString recvstr = _T("");
		if (NULL == buffer)
		{
			ReguTrace(Config,"buffer==NULL!");
			return FALSE;
		}
		BYTE * pBuf = (BYTE*)buffer;
		int index = 0;
		//srand(time(0));  //用时间作种子，使每次随即的数字不一样
		index = rand()%10;
		EnterCriticalSection(&pDlg->m_pCProcessThread[index]->m_csRedisDataList);
		pDlg->m_pCProcessThread[index]->m_RedisDataList.AddTail(pBuf);
		LeaveCriticalSection(&pDlg->m_pCProcessThread[index]->m_csRedisDataList);
		ReguTrace(Config,"发送给线程%d:%s",index,bufToHexString(buffer,100).c_str());
		SetEvent(pDlg->m_pCProcessThread[index]->m_ProcessDataEvent);
/*
		SocketMsgHead* pHead = (SocketMsgHead*)(pBuf+ sizeof(NetMessageHead));
		int nn = pHead->btType;
		int len = pHead->wdLen;
		int mm = pHead->dwSyncCode;
		int vv = sizeof(NetMessageHead)+sizeof(SocketMsgHead);
		BYTE* pMsg = pBuf + sizeof(NetMessageHead)+sizeof(SocketMsgHead);
		switch(pHead->btType)
		{
		case SOCKET_MSG_GROUP_YXDATA:
			ReguTrace(Config,"收到YX报文:%s",bufToHexString(pMsg,len).c_str());
			//OnSocketMsgGroupYx((SocketMsgGroupYxEx*)pMsg);
			break;
		case SOCKET_MSG_GROUP_YMDATA_EX_DATE:
			ReguTrace(Config,"收到YM报文:%s",bufToHexString(pMsg,len).c_str());
			//StartProcessData((SocketMsgGroupYmDataEx_DATE*)pMsg);
			break;
		case SOCKET_MSG_PREPAYRECORD:
			ReguTrace(Config,"收到PAYRECORD报文:%s",bufToHexString(pMsg,len).c_str());
			//OnCallPrePayRecord((SocketMsgPrepayRecord*)pMsg);
			break;
		default:
			break;

		}
		*/
		
	}
	else if (REDIS_NODATA == ret)
	{
		OutputDebugString(_T("REDIS_NODATA \n"));
		if (buffer!=NULL)
		{
			delete [] buffer;
			buffer = NULL;
		}
		return FALSE;
	}

	/*if (buffer!=NULL)
	{
		delete [] buffer;
		buffer = NULL;
	}*/
	return TRUE;
}

//void CRedisRecvSample::OnCallPrePayRecord(SocketMsgPrepayRecord* pMsgPrepayRecordData)
//{
//	HANDLE hPipe = NULL;
//	BYTE* pRead = NULL;
//	short nRtuNo = pMsgPrepayRecordData->btRtuNo;
//
//	nRtuNo = pMsgPrepayRecordData->btForeNo * 256 + pMsgPrepayRecordData->btRtuNo;
//	/*if (pMsgPrepayRecordData->btForeNo < ADDR_SOURCEMACHINE && pMsgPrepayRecordData->btForeNo >= 0)
//	{
//		nRtuNo = pMsgPrepayRecordData->btForeNo * 256 + pMsgPrepayRecordData->btRtuNo;
//	}*/
//	int nDevNo = pMsgPrepayRecordData->nDevNo;  //前置设备号
//	int nDevId = 0;
//	GetDevId(nRtuNo,pMsgPrepayRecordData->nDevNo,nDevId);
//	if(nDevId==0)
//	{
//		return;
//	}
//	SocketPrepayRecordUnit *pRecordUnit = NULL;
//	pRecordUnit = pMsgPrepayRecordData->prUnit;
//	int icount = 0;
//	CString UpdateSql = _T(""); 
//	for (WORD i=0; i<pMsgPrepayRecordData->wdYjNum; i++,pRecordUnit++)
//	{
//		if (NULL==hPipe)
//		{
//			hPipe = OpenServerPipe();
//		}
//		int iyear = 0, imonth = 0, iday = 0, ihour = 0, iminite = 0, isecond = 0;
//		iyear = pRecordUnit->nPercDate[0];
//		imonth = pRecordUnit->nPercDate[1];
//		iday = pRecordUnit->nPercDate[2];
//		ihour = pRecordUnit->nPercDate[3];
//		iminite = pRecordUnit->nPercDate[4];
//		CString timestr = _T("");
//		if (iyear==0||iyear>99||imonth<=0||imonth>12||iday<=0||iday>31)
//		{
//			timestr = _T("1999-01-01 00:00:00");
//		}
//		else
//		{
//			timestr.Format(_T("20%02d-%02d-%02d %02d:%02d:00"),iyear,imonth,iday,ihour,iminite);
//		}
//		int nTotalBuyCount = 0; //上一次充值后的总购电次数
//		nTotalBuyCount = pRecordUnit->nTotalPercNum;
//		float nBuyFee = 0;  //本次充值金额
//		nBuyFee = pRecordUnit->nPercFee*0.01;
//		float nBefReminAmount = 0; //本次充值前剩余金额
//		float nAftReminAmount = 0; //本次充值后剩余金额
//		float nTotalFee = 0;   //总购电金额
//		nBefReminAmount = pRecordUnit->nBefPercFee*0.01;
//		nAftReminAmount = pRecordUnit->nAftPercFee*0.01;
//		nTotalFee = pRecordUnit->nTotalPercFee*0.01;
//	
//		CString TempSql = _T("");
//		CString BillSerial = _T("");
//		CTime TempTime = CTime::GetCurrentTime();
//		if (TempTime.GetDay()!=m_DayTime.GetDay()||TempTime.GetMonth()!=m_DayTime.GetMonth()||TempTime.GetYear()!=m_DayTime.GetYear())
//		{
//			BillNum = 0;
//			m_DayTime = CTime(TempTime.GetYear(),TempTime.GetMonth(),TempTime.GetDay(),0,0,0);
//		}
//		BillNum++;
//		BillSerial.Format(_T("YJ20%02d%02d%02d%02d%02d%02d%04d"),iyear,imonth,iday,ihour,iminite,isecond,BillNum);
//		TempSql.Format(C_SQL_Insert_PaymentRecord,nDevId,nTotalBuyCount,timestr,nDevId,nBuyFee,2,BillSerial,2,nBefReminAmount,nAftReminAmount,timestr,nTotalBuyCount);
//		UpdateSql += TempSql;
//		icount++;
//		if (icount%SQL_COUNT_ONCE!=0 && i!=pMsgPrepayRecordData->wdYjNum - 1)
//		{
//			continue;
//		}
//		CString strSqlFinal = UpdateSql;
//		pRead = (BYTE*)GetMessage_RecordOfSql_Ext(hPipe, strSqlFinal);
//		if (pRead)
//		{
//			delete[] pRead;
//			pRead = NULL;
//		}
//		UpdateSql.Empty();
//
//	}
//	if (hPipe != NULL)
//	{
//		CloseHandle(hPipe);
//		hPipe = NULL;
//	}
//}
//
//void CRedisRecvSample::OnSocketMsgGroupYx(SocketMsgGroupYxEx* pMsgGroupYxData)
//{
//	HANDLE hPipe = NULL;
//	BYTE* pRead = NULL;
//	short nRtuNo = pMsgGroupYxData->btRtuNo;
//
//	nRtuNo = pMsgGroupYxData->btForeNo * 256 + pMsgGroupYxData->btRtuNo;
//	/*if (pMsgGroupYxData->btForeNo < ADDR_SOURCEMACHINE && pMsgGroupYxData->btForeNo >= 0)
//	{
//		nRtuNo = pMsgGroupYxData->btForeNo * 256 + pMsgGroupYxData->btRtuNo;
//	}*/
//	SocketGroupYxUnit *YxUnit = NULL;
//	YxUnit = pMsgGroupYxData->YxUnit;
//	int icount = 0;
//	CString UpdateSql = _T("");
//	m_YxValueInfoCArray.RemoveAll();
//    for (WORD i=0; i<pMsgGroupYxData->wdYxNum; i++,YxUnit++)
//    {
//		int DevId = 0;
//		GetDevId(nRtuNo,YxUnit->DevId,DevId);
//		if (DevId==0)
//		{
//			continue;
//		}
//		YxConfigDef iYxConfig;
//		memset(&iYxConfig,0,sizeof(YxConfigDef));
//		iYxConfig.nYxIndex = 100*2000*nRtuNo+YxUnit->DevId*100+YxUnit->Id;
//		iYxConfig.bYxValue = YxUnit->Value;
//		iYxConfig.bYxRaw = YxUnit->Value;
//		m_YxValueInfoCArray.Add(iYxConfig);
//		//if (NULL==hPipe)
//		//{
//		//	hPipe = OpenServerPipe();
//		//}
//		//CString TempSql = _T("");
//		//TempSql.Format(_T("UPDATE TB_DI SET YXVALUE=%d WHERE DEVICENUM=%d AND NUM=%d; "),YxUnit->Value,DevId,YxUnit->Id);  //TB_DI表中的DEVICENUM为设备表第一列
//		//UpdateSql += TempSql;
//		//icount++;
//		//if (icount%SQL_COUNT_ONCE!=0 && i!=pMsgGroupYxData->wdYxNum - 1)
//		//{
//		//	continue;
//		//}
//		//CString strSqlFinal = UpdateSql;
//		//pRead = (BYTE*)GetMessage_RecordOfSql_Ext(hPipe, strSqlFinal);
//		//if (pRead)
//		//{
//		//	delete[] pRead;
//		//	pRead = NULL;
//		//}
//		//UpdateSql.Empty();
//
//    }
//	Update2RTDB();
//	if (hPipe != NULL)
//	{
//		CloseHandle(hPipe);
//		hPipe = NULL;
//	}
//}

BOOL CRedisRecvSample::GetDevId(short nRtu, int DeviceNo, int &DevId)
{
	//通过Rtu号找到厂站ID
	/*StationDefMap::iterator its;
	its = m_StationDefMap.find(nRtu);
	if (its == m_StationDefMap.end())
	{
		return FALSE;
	}*/  //GUOJ DEL 151022

	//通过厂站ID和前置设备号找到设备ID
	CString key = _T("");
	key.Format(_T("%d+%d"), nRtu, DeviceNo);
	DeviceRecordDefMap::iterator itd;
	itd = m_DeviceRecordDefMap.find(key);
	if (itd == m_DeviceRecordDefMap.end())
	{
		return FALSE;
	}

	DevId = itd->second;
	//如是集中器模式，则需进行设备映射  //ZHOUN 140909 ADD
	//int nDevID = 0, nDev3YNum = 0;
	/*int nDev3YNum = 0;
	if (GetDeviceInfoByCollectorInfo(itd->second.nID, YxUnit->Id, DevId, nDev3YNum))
	{
		DeviceRecordDefMap2::iterator itd2;
		itd2 = m_DeviceRecordDefMap2.find(DevId);
		if (itd2 == m_DeviceRecordDefMap2.end())
		{
			DevId = itd2->second.nID;
		}
	}
	else
	{
		DevId = itd->second.nID;
		nDev3YNum = YxUnit->Id;
	}*/

}

BOOL CRedisRecvSample::GetYmDefByPowInfo(short nRtu, const PowFileInfo *pPowInfo, SimpleYmDef &ymDef, int &iDevId)
{
	memset(&ymDef, 0, sizeof(SimpleYmDef));
	CString key = _T("");

	//通过Rtu号找到厂站ID
	/*StationDefMap::iterator its;
	its = m_StationDefMap.find(nRtu);
	if (its == m_StationDefMap.end())
	{
		return FALSE;
	}*/

	//通过厂站ID和前置设备号找到设备ID
	//key.Format(_T("%d+%d"), its->second.nStationNum, pPowInfo->nDevID);
	key.Format(_T("%d+%d"), nRtu, pPowInfo->nDevID);  //GUOJ MOD 151022
	DeviceRecordDefMap::iterator itd;
	itd = m_DeviceRecordDefMap.find(key);
	if (itd == m_DeviceRecordDefMap.end())
	{
		return FALSE;
	}

	////如是集中器模式，则需进行设备映射  //ZHOUN 140909 ADD
	int nDevID = 0, nDev3YNum = 0;
	//if (GetDeviceInfoByCollectorInfo(itd->second.nID, pPowInfo->nYmNum, nDevID, nDev3YNum))
	//{
	//	DeviceRecordDefMap2::iterator itd2;
	//	itd2 = m_DeviceRecordDefMap2.find(nDevID);
	//	if (itd2 == m_DeviceRecordDefMap2.end())
	//	{
	//		nDevID = itd2->second.nID;
	//	}
	//}
	//else
	//{
	//	nDevID = itd->second.nID;
	//	nDev3YNum = pPowInfo->nYmNum;
	//}//

	nDevID = itd->second;
	nDev3YNum = pPowInfo->nYmNum;
	iDevId = nDevID;
	//通过设备ID和遥脉序号找到对应的遥脉记录
	key.Format(_T("%d+%d"), nDevID, nDev3YNum);
	YmConfigDefMap::iterator itym;	
	itym = m_YmConfigDefMap.find(key);
	if (itym == m_YmConfigDefMap.end())
	{
		return FALSE;
	}

	ymDef = itym->second;
	return TRUE;
}

BOOL CRedisRecvSample::GetDeviceInfoByCollectorInfo(int nCollectorID, int nCollector3YNum, int &nDevID, int &nDev3YNum)
{
	CollectorMappingMap::iterator itcm;
	__int64 key = 0, val = 0;
	key |= (__int64)nCollectorID << 32;
	key |= (__int64)nCollector3YNum;
	itcm = m_CollectorMappingMap.find(key);
	if (itcm == m_CollectorMappingMap.end())
	{
		return FALSE;
	}

	val = itcm->second;
	nDevID = ((unsigned __int64)val >> 32) & 0xFFFF;        //转换成无符号int64以免引发右移带出符号位的隐患
	nDev3YNum = val & 0xFFFF;

	return TRUE;
}

BOOL CRedisRecvSample::LoadAllForeignTable()
{
	if (!LoadStationTable())
	{
		ReguTrace(Config,"LoadStationTable err!");
		return FALSE;
	}
	if (!LoadDeviceTable())
	{
		ReguTrace(Config,"LoadDeviceTable err!");
		return FALSE;
	}
	if (!LoadYmTable())
	{
		ReguTrace(Config,"LoadYmTable err!");
		return FALSE;
	}
	m_MapSampleNo.clear();
	ReguTrace(Config,"LoadSampleExtTable begin!");

	if (!LoadSampleTableFromRTDB(TABLE_NO_SAMPLEHOUR))
	{
		ReguTrace(Config,"LoadSampleTable HOUR err!");
		return FALSE;
	}
	if (!LoadSampleTableFromRTDB(TABLE_NO_SAMPLEDAY))
	{
		ReguTrace(Config,"LoadSampleTable DAY err!");
		return FALSE;
	}
	/*if (!LoadSampleExtTable(TABLE_NO_SAMPLEHOUR_EXT))
	{
		ReguTrace(Config,"LoadSampleExtTable HOUR err!");
		return FALSE;
	}
	if (!LoadSampleExtTable(TABLE_NO_SAMPLEDAY_EXT))
	{
		ReguTrace(Config,"LoadSampleExtTable DAY err!");
		return FALSE;
	}*/
	ReguTrace(Config,"LoadSampleExtTable end! num=%d",m_MapSampleNo.size());
	if (!LoadYxTable())
	{
		ReguTrace(Config,"LoadYxTable err!");
		return FALSE;
	}
	ReguTrace(Config,"LoadYxTable end! num=%d",m_DevStateMap.GetSize());
	m_StationDefMap.clear();     //把厂站配置清空，减少内存。
	return TRUE;
}

BOOL CRedisRecvSample::LoadSampleExtTable(WORD SampleTableNo)
{
	CString strKey = _T("");
	int iFdCount = 3;	//	查询的字段数
	CString TableName = _T("TB_SAMPLEHOUR_EXT");
	if(SampleTableNo==TABLE_NO_SAMPLEDAY_EXT)
	{
		TableName = _T("TB_SAMPLEDAY_EXT");
	}
	else if (SampleTableNo==TABLE_NO_SAMPLEHOUR_EXT)
	{
		TableName = _T("TB_SAMPLEHOUR_EXT");
	}

	HANDLE hPipe =OpenRealDataPipe();

	if (hPipe == NULL) 
	{
		//MYERROR(_T("LoadSampleExtTable hPipe null"));
		return FALSE;
	}
	int tempSampleId = 0;
	while(1)
	{
		CString sql = _T("");
		sql.Format(_T("SELECT TOP 5000 SAMPLENO,TABLENO,RECORDNO FROM %s where SAMPLENO>'%d' order by SAMPLENO"),TableName,tempSampleId);
		//sql.Format(_T("SELECT TOP 1 SAMPLENO,TABLENO,RECORDNO FROM %s where SAMPLENO>'%d' order by SAMPLENO"),TableName,tempSampleId);
		BYTE* pRead = (BYTE*)GetMessage_RecordOfSql(hPipe,sql);
		if (NULL == pRead)	// 未查询到记录
		{
			MYERROR(_T("查询异常，系统后台没有响应，错误码：%d"),GetLastError());
			if (hPipe)
			{
				CloseHandle(hPipe);
				hPipe = NULL;
			}
			return FALSE;
		}
		// 解析返回的结果
		NetMessageHead *pNetMessageHead = (NetMessageHead*)pRead;
		if (pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB) {
			//MYWARN(_T("返回报文异常，%s"),bufToHexString(pRead,pNetMessageHead->Length).c_str());
			delete[] pRead;
			pRead = NULL;

			if (hPipe)
			{
				CloseHandle(hPipe);
				hPipe = NULL;
			}
			return FALSE;
		}

		if (pNetMessageHead->Length < 1)	// 空报文，无查询结果
		{
			//MYWARN(_T("空报文，无查询结果"));
			delete[] pRead;
			pRead = NULL;
			if (hPipe)
			{
				CloseHandle(hPipe);
				hPipe = NULL;
			}

			return FALSE;
		}

		// 获取查询的字段数
		WORD* pWord = (WORD*)&pRead[sizeof(NetMessageHead)];
		int nFieldNum = (int)*pWord;
		if (nFieldNum != iFdCount)   //需要根据查询出的字段数修改
		{
			//MYWARN(_T("查询结果报文中返回的字段数不正确[nFieldNum=%d]"),nFieldNum);
			delete[] pRead;
			pRead = NULL;
			if (hPipe)
			{
				CloseHandle(hPipe);
				hPipe = NULL;
			}
			return FALSE;
		}
		pWord++;

		// 记录集长度
		MessageRetRecordHead* pHead = (MessageRetRecordHead*)pWord;
		BYTE *pByte = (BYTE*)&pHead[nFieldNum];
		DWORD dwRecordLen = *((DWORD*)pByte);
		if (dwRecordLen < 0)
		{
			//MYWARN(_T("查询结果报文中返回的记录集长度不正确[dwRecordLen=%d]"),dwRecordLen);
			delete[] pRead;
			pRead = NULL;
			if (hPipe)
			{
				CloseHandle(hPipe);
				hPipe = NULL;
			}
			return FALSE;
		}
		pByte += sizeof(DWORD);

		// 记录数
		DWORD dwRecordNum = *((int*)pByte);
		if (dwRecordNum < 0)
		{
			delete[] pRead;
			pRead = NULL;
			if (hPipe)
			{
				CloseHandle(hPipe);
				hPipe = NULL;
			}
			break;
		}
		pByte += sizeof(DWORD);


		TCHAR *tValue = new TCHAR[1024+1];
		if (tValue == NULL)
		{
			//MYERROR(_T("分配内存错误，错误码：%d"),GetLastError());
			//MYERROR(_T("GetPlazaId tValue == NULL"));
			delete[] pRead;
			pRead = NULL;
			if (hPipe)
			{
				CloseHandle(hPipe);
				hPipe = NULL;
			}
			return FALSE;
		}

		// 将查询结果写入到缓存
		for (int i=0;i<(int)dwRecordNum;i++)
		{
			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, pHead->DataLen);
			int SampleNo = (int)*((DWORD *)tValue);
			pByte += pHead->DataLen;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+1)->DataLen);
			int nTableNo = (int)*(DWORD*)tValue;
			pByte += (pHead+1)->DataLen;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+2)->DataLen);
			int n3YIndex = (int)*(DWORD*)tValue;
			pByte += (pHead+2)->DataLen;

			strKey.Format(_T("%d+%d+%d"), SampleTableNo, nTableNo, n3YIndex);	//key为“采样表号+三遥表号+三遥序号”
			m_MapSampleNo[strKey] = SampleNo;
			if (SampleNo>tempSampleId)
			{
				tempSampleId = SampleNo;
			}
		}
		delete []tValue;
		tValue = NULL;
		delete[] pRead;
		pRead = NULL;
		if (dwRecordNum<5000)
		{
			break;
		}
	}

	if (hPipe)
	{
		CloseHandle(hPipe);
		hPipe = NULL;
	}

	return TRUE;
}

BOOL CRedisRecvSample::LoadSampleTable(WORD SampleTableNo)
{
	CString strKey = _T("");
	int iFdCount = 2;	//	查询的字段数
	CString TableName = _T("TB_SAMPLEHOUR");
	if(SampleTableNo==TABLE_NO_SAMPLEDAY)
	{
		TableName = _T("TB_SAMPLEDAY");
	}
	else if (SampleTableNo==TABLE_NO_SAMPLEHOUR)
	{
		TableName = _T("TB_SAMPLEHOUR");
	}

	HANDLE hPipe =OpenRealDataPipe();

	if (hPipe == NULL) 
	{
		//MYERROR(_T("LoadSampleExtTable hPipe null"));
		return FALSE;
	}
	int tempSampleId = 0;
	while(1)
	{
		CString sql = _T("");
		sql.Format(_T("SELECT TOP 5000 ID,DATASOURCE FROM %s where ID>'%d' order by ID"),TableName,tempSampleId);
		//sql.Format(_T("SELECT TOP 1 SAMPLENO,TABLENO,RECORDNO FROM %s where SAMPLENO>'%d' order by SAMPLENO"),TableName,tempSampleId);
		BYTE* pRead = (BYTE*)GetMessage_RecordOfSql(hPipe,sql);
		if (NULL == pRead)	// 未查询到记录
		{
			MYERROR(_T("查询异常，系统后台没有响应，错误码：%d"),GetLastError());
			if (hPipe)
			{
				CloseHandle(hPipe);
				hPipe = NULL;
			}
			return FALSE;
		}
		// 解析返回的结果
		NetMessageHead *pNetMessageHead = (NetMessageHead*)pRead;
		if (pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB) {
			//MYWARN(_T("返回报文异常，%s"),bufToHexString(pRead,pNetMessageHead->Length).c_str());
			delete[] pRead;
			pRead = NULL;

			if (hPipe)
			{
				CloseHandle(hPipe);
				hPipe = NULL;
			}
			return FALSE;
		}

		if (pNetMessageHead->Length < 1)	// 空报文，无查询结果
		{
			//MYWARN(_T("空报文，无查询结果"));
			delete[] pRead;
			pRead = NULL;
			if (hPipe)
			{
				CloseHandle(hPipe);
				hPipe = NULL;
			}

			return FALSE;
		}

		// 获取查询的字段数
		WORD* pWord = (WORD*)&pRead[sizeof(NetMessageHead)];
		int nFieldNum = (int)*pWord;
		if (nFieldNum != iFdCount)   //需要根据查询出的字段数修改
		{
			//MYWARN(_T("查询结果报文中返回的字段数不正确[nFieldNum=%d]"),nFieldNum);
			delete[] pRead;
			pRead = NULL;
			if (hPipe)
			{
				CloseHandle(hPipe);
				hPipe = NULL;
			}
			return FALSE;
		}
		pWord++;

		// 记录集长度
		MessageRetRecordHead* pHead = (MessageRetRecordHead*)pWord;
		BYTE *pByte = (BYTE*)&pHead[nFieldNum];
		DWORD dwRecordLen = *((DWORD*)pByte);
		if (dwRecordLen < 0)
		{
			//MYWARN(_T("查询结果报文中返回的记录集长度不正确[dwRecordLen=%d]"),dwRecordLen);
			delete[] pRead;
			pRead = NULL;
			if (hPipe)
			{
				CloseHandle(hPipe);
				hPipe = NULL;
			}
			return FALSE;
		}
		pByte += sizeof(DWORD);

		// 记录数
		DWORD dwRecordNum = *((int*)pByte);
		if (dwRecordNum < 0)
		{
			delete[] pRead;
			pRead = NULL;
			if (hPipe)
			{
				CloseHandle(hPipe);
				hPipe = NULL;
			}
			break;
		}
		pByte += sizeof(DWORD);


		TCHAR *tValue = new TCHAR[1024+1];
		if (tValue == NULL)
		{
			//MYERROR(_T("分配内存错误，错误码：%d"),GetLastError());
			//MYERROR(_T("GetPlazaId tValue == NULL"));
			delete[] pRead;
			pRead = NULL;
			if (hPipe)
			{
				CloseHandle(hPipe);
				hPipe = NULL;
			}
			return FALSE;
		}

		// 将查询结果写入到缓存
		for (int i=0;i<(int)dwRecordNum;i++)
		{
			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, pHead->DataLen);
			int SampleNo = (int)*((DWORD *)tValue);
			pByte += pHead->DataLen;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+1)->DataLen);
			IndexDef *pIndex =  (IndexDef*)tValue;
			pByte += (pHead+1)->DataLen;


			strKey.Format(_T("%d+%d+%d"), SampleTableNo, TABLE_NO_PULSE, pIndex->RecordNo);	//key为“采样表号+三遥表号+三遥序号”
			m_MapSampleNo[strKey] = SampleNo;
			if (SampleNo>tempSampleId)
			{
				tempSampleId = SampleNo;
			}
		}
		delete []tValue;
		tValue = NULL;
		delete[] pRead;
		pRead = NULL;
		if (dwRecordNum<5000)
		{
			break;
		}
	}

	if (hPipe)
	{
		CloseHandle(hPipe);
		hPipe = NULL;
	}

	return TRUE;
}

BOOL CRedisRecvSample::LoadStationTable()
{
	m_StationDefMap.clear();
	HANDLE hPipe = NULL;
	StationDef pStaDef;
	NetMessageHead* pNetMessageHead = NULL;
	MessageQueryRecord mqr;

	//读取厂站信息表
	mqr.BaseNo = BASE_NO_RTDB;
	mqr.TableNo = TABLE_NO_STATION;
	mqr.FilterFieldNo = 0;
	mqr.FilterType = FILTER_TYPE_EQU; 
	mqr.StartValue = 0;
	mqr.EndValue = mqr.StartValue;


	hPipe = OpenRealDataPipe();
	if (hPipe == NULL) {
		return FALSE;
	}
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
		memset(&pStaDef, 0, sizeof(StationDef));
		memcpy(&pStaDef.nStationNum, pBuf, pRetRecordHead->DataLen);
		nOffset += pRetRecordHead->DataLen;

		if (pStaDef.nStationNum>MaxStationNum)
		{
			MaxStationNum = pStaDef.nStationNum;
		}
		memcpy(pStaDef.cStationName, (pBuf+nOffset), (pRetRecordHead+1)->DataLen);
		nOffset += (pRetRecordHead+1)->DataLen;

		memcpy(&pStaDef.nProjectID, (pBuf+nOffset), (pRetRecordHead+2)->DataLen);
		nOffset += (pRetRecordHead+2)->DataLen;

		memcpy(&pStaDef.nStationID, (pBuf+nOffset), (pRetRecordHead+3)->DataLen);
		nOffset += (pRetRecordHead+3)->DataLen;

		memcpy(&pStaDef.nPortNo, (pBuf+nOffset), (pRetRecordHead+4)->DataLen);
		nOffset += (pRetRecordHead+4)->DataLen;

		memcpy(&pStaDef.pFaultSignal, (pBuf+nOffset), (pRetRecordHead+5)->DataLen);
		nOffset += (pRetRecordHead+5)->DataLen;

		memcpy(&pStaDef.bRtuStatus, (pBuf+nOffset), (pRetRecordHead+6)->DataLen);
		nOffset += (pRetRecordHead+6)->DataLen;

		pStaDef.dt.year = ((TIMESTAMP_STRUCT*)(pBuf+nOffset))->year;
		pStaDef.dt.month = ((TIMESTAMP_STRUCT*)(pBuf+nOffset))->month;
		pStaDef.dt.day = ((TIMESTAMP_STRUCT*)(pBuf+nOffset))->day;
		pStaDef.dt.hour = ((TIMESTAMP_STRUCT*)(pBuf+nOffset))->hour;
		pStaDef.dt.minute = ((TIMESTAMP_STRUCT*)(pBuf+nOffset))->minute;
		pStaDef.dt.second = ((TIMESTAMP_STRUCT*)(pBuf+nOffset))->second;
		nOffset += (pRetRecordHead+7)->DataLen;

		memcpy(&pStaDef.nTimeRunningDay, (pBuf+nOffset), (pRetRecordHead+8)->DataLen);
		nOffset += (pRetRecordHead+8)->DataLen;

		memcpy(&pStaDef.nTimeOutTageDay, (pBuf+nOffset), (pRetRecordHead+9)->DataLen);
		nOffset += (pRetRecordHead+9)->DataLen;

		memcpy(&pStaDef.bCountFaultSignal, (pBuf+nOffset), (pRetRecordHead+10)->DataLen);
		nOffset += (pRetRecordHead+10)->DataLen;

		memcpy(&pStaDef.nSafetyDays, (pBuf+nOffset), (pRetRecordHead+11)->DataLen);

		m_StationDefMap[pStaDef.nPortNo] = pStaDef.nStationID;  //GUOJ MOD 151020
		pBuf+=dwRecordLen;
		nNum--;
	}

	//清理
	if(hPipe != NULL)			{CloseHandle(hPipe); hPipe = NULL;}
	if(pNetMessageHead != NULL){delete [](BYTE *)pNetMessageHead; pNetMessageHead=NULL;}

	return TRUE;
}

BOOL CRedisRecvSample::LoadDeviceTable()
{
	m_DeviceRecordDefMap.clear();
	//m_DeviceRecordDefMap2.clear();

	CString strKey = _T("");
	HANDLE hPipe = NULL;
	DeviceRecordDef DeviceRecord;
	NetMessageHead* pNetMessageHead = NULL;
	MessageQueryRecord mqr;
	hPipe = OpenRealDataPipe();
	if (hPipe == NULL) {
		return FALSE;
	}

	int iStationNum = 1;
	while(1)
	{
		
		if (iStationNum>MaxStationNum)
		{
			break;
		}
		map<int, int>::iterator it = m_StationDefMap.find(iStationNum);
		if (it==m_StationDefMap.end())
		{
			iStationNum++;
			continue;
		}
		//读取设备表
		mqr.BaseNo = BASE_NO_RTDB;
		mqr.TableNo = TABLE_NO_DEVICE;
		mqr.FilterFieldNo = 6;
		mqr.FilterType = FILTER_TYPE_EQU; 
		mqr.StartValue = iStationNum;
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
			memset(&DeviceRecord, 0, sizeof(DeviceRecordDef));
			memcpy(&DeviceRecord.nID,pBuf,pRetRecordHead->DataLen);
			nOffset+=pRetRecordHead->DataLen;
			memcpy(&DeviceRecord.nDeviceNo,(pBuf+nOffset),(pRetRecordHead+1)->DataLen);
			nOffset+=(pRetRecordHead+1)->DataLen;
			memcpy(DeviceRecord.cDeviceNo,(pBuf+nOffset),(pRetRecordHead+2)->DataLen);
			nOffset+=(pRetRecordHead+2)->DataLen;
			memcpy(DeviceRecord.cCName,(pBuf+nOffset),(pRetRecordHead+3)->DataLen);
			nOffset+=(pRetRecordHead+3)->DataLen;
			memcpy(&DeviceRecord.nProjectNo,(pBuf+nOffset),(pRetRecordHead+4)->DataLen);
			nOffset+=(pRetRecordHead+4)->DataLen;
			memcpy(&DeviceRecord.nStationNo,pBuf+nOffset,(pRetRecordHead+5)->DataLen);
			nOffset+=(pRetRecordHead+5)->DataLen;
			memcpy(&DeviceRecord.nParentDev,pBuf+nOffset,(pRetRecordHead+6)->DataLen);
			nOffset+=(pRetRecordHead+6)->DataLen;
			memcpy(&DeviceRecord.nChannelNo,pBuf+nOffset,(pRetRecordHead+7)->DataLen);
			nOffset+=(pRetRecordHead+7)->DataLen;
			memcpy(&DeviceRecord.nDeviceType,pBuf+nOffset,(pRetRecordHead+8)->DataLen);
			int i = 0;
			for (i=0; i<13; i++)
			{
				nOffset+=(pRetRecordHead+8+i)->DataLen;
			}
			memcpy(&DeviceRecord.nSysID,pBuf+nOffset,(pRetRecordHead+8+i)->DataLen);

			strKey.Format(_T("%d+%d"), DeviceRecord.nStationNo, DeviceRecord.nDeviceNo);	//key为“厂站ID+前置设备号”
			m_DeviceRecordDefMap[strKey] = DeviceRecord.nID;

			//m_DeviceRecordDefMap2[DeviceRecord.nID] = DeviceRecord;	//key为设备id

			pBuf+=dwRecordLen;
			nNum--;
		}

		iStationNum++;
	}
	
	//清理
	if(hPipe != NULL)			{CloseHandle(hPipe); hPipe = NULL;}
	if(pNetMessageHead != NULL){delete [](BYTE *)pNetMessageHead; pNetMessageHead=NULL;}

	return TRUE;
}

BOOL CRedisRecvSample::LoadYxTable()
{
	m_DevStateMap.RemoveAll();
	HANDLE hPipe = NULL;
	NetMessageHead* pNetMessageHead = NULL;
	MessageQueryRecord mqr;
	hPipe = OpenRealDataPipe();
	if (hPipe == NULL) {
		return FALSE;
	}
	//读取遥脉信息表
	int iStationNum = 1;
	while(1)
	{
		if (iStationNum>MaxStationNum)
		{
			break;
		}
		map<int, int>::iterator it = m_StationDefMap.find(iStationNum);
		if (it==m_StationDefMap.end())
		{
			iStationNum++;
			continue;
		}
		mqr.BaseNo = BASE_NO_RTDB;
		mqr.TableNo = TABLE_NO_DI;
		mqr.FilterFieldNo = 3;//0;
		mqr.FilterType = FILTER_TYPE_EQU; 
		mqr.StartValue = iStationNum;
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
			int nYxIndex = 0;
			int iDevId = 0;
			int iNum = 0;
			BYTE YxValue = 0;
			memcpy(&nYxIndex,pBuf,pRetRecordHead->DataLen);
			nOffset+=pRetRecordHead->DataLen;
			nOffset+=(pRetRecordHead+1)->DataLen;
			nOffset+=(pRetRecordHead+2)->DataLen;
			memcpy(&iDevId,pBuf+nOffset,(pRetRecordHead+3)->DataLen);
			nOffset+=(pRetRecordHead+3)->DataLen;
			memcpy(&iNum,pBuf+nOffset,(pRetRecordHead+4)->DataLen);
			//GUOJ DEL 151218
			/*if (iNum!=0)
			{
				nNum--;
				continue;
			}*/
			nOffset+=(pRetRecordHead+4)->DataLen;
			nOffset+=(pRetRecordHead+5)->DataLen;
			nOffset+=(pRetRecordHead+6)->DataLen;
			nOffset+=(pRetRecordHead+7)->DataLen;
			nOffset+=(pRetRecordHead+8)->DataLen;
			memcpy(&YxValue,pBuf+nOffset,(pRetRecordHead+9)->DataLen);
			nOffset+=(pRetRecordHead+9)->DataLen;

			m_DevStateMap.SetAt(nYxIndex,YxValue);  //GUOJ MOD 151218
			pBuf+=dwRecordLen;
			nNum--;
		}

		iStationNum++;
	}
	//清理
	if(hPipe != NULL)			{CloseHandle(hPipe); hPipe = NULL;}
	if(pNetMessageHead != NULL){delete [](BYTE *)pNetMessageHead; pNetMessageHead=NULL;}

	return TRUE;

	//int iFdCount = 2;	//	查询的字段数
	//int iMaxRsCount = -1;	// 限制查询得到的最大记录个数，-1指不限制


	//// 向数据管道发送SQL语句
	//BYTE* pRead = NULL;

	//HANDLE hPipe =OpenRealDataPipe();

	//if (hPipe == NULL) 
	//{
	//	MYERROR(_T("GetPlazaId hPipe null"));
	//	return FALSE;
	//}

	//int tempId = 0;
	//while (1)
	//{
	//	CString strQuery;
	//	strQuery.Format(_T("SELECT top 5000 DEVICENUM,YXVALUE FROM TB_DI WHERE NUM=0 AND DEVICENUM > %d order by DEVICENUM;"),tempId);
	//	pRead = (BYTE*)GetMessage_RecordOfSql(hPipe,strQuery);

	//	if (NULL == pRead)	// 未查询到记录
	//	{
	//		CloseHandle(hPipe);
	//		MYERROR(_T("GetPlazaId pRead null"));
	//		return FALSE;
	//	}
	//	// 解析返回的结果
	//	NetMessageHead *pNetMessageHead = (NetMessageHead*)pRead;
	//	if (pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB) {
	//		//	MYWARN(_T("返回报文异常，%s"),bufToHexString(pRead,pNetMessageHead->Length).c_str());
	//		delete[] pRead;
	//		pRead = NULL;
	//		CloseHandle(hPipe);
	//		MYERROR(_T("GetPlazaId pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB null"));
	//		return FALSE;
	//	}	

	//	if (pNetMessageHead->Length < 1)	// 空报文，无查询结果
	//	{
	//		//MYWARN(_T("接收到空回复报文，无法解析查询结果"));
	//		delete[] pRead;
	//		pRead = NULL;
	//		CloseHandle(hPipe);
	//		MYERROR(_T("GetPlazaId pNetMessageHead->Length < 1 null"));
	//		return FALSE;
	//	}

	//	// 获取查询的字段数
	//	WORD* pWord = (WORD*)&pRead[sizeof(NetMessageHead)];
	//	int nFieldNum = (int)*pWord;
	//	if (nFieldNum != iFdCount)	// 查询字段数量，不相等，曾查询结果异常退出
	//	{
	//		//	MYWARN(_T("查询字段数量，不相等，曾查询结果异常退出"));
	//		delete[] pRead;
	//		pRead = NULL;
	//		CloseHandle(hPipe);
	//		MYERROR(_T("GetPlazaId nFieldNum != iFdCount null"));
	//		return FALSE;
	//	}
	//	pWord++;

	//	// 记录集长度
	//	MessageRetRecordHead* pHead = (MessageRetRecordHead*)pWord;
	//	BYTE *pByte = (BYTE*)&pHead[nFieldNum];
	//	DWORD dwRecordLen = *((DWORD*)pByte);
	//	if (dwRecordLen < 1)
	//	{
	//		delete[] pRead;
	//		pRead = NULL;
	//		CloseHandle(hPipe);
	//		return FALSE;
	//	}
	//	pByte += sizeof(DWORD);

	//	// 记录数
	//	DWORD dwRecordNum = *((int*)pByte);
	//	if ((iMaxRsCount > 0) && (dwRecordNum != iMaxRsCount))
	//	{
	//		delete[] pRead;
	//		pRead = NULL;
	//		CloseHandle(hPipe);
	//		return FALSE;
	//	}
	//	pByte += sizeof(DWORD);

	//	TCHAR *tValue = new TCHAR[1024+1];
	//	if (tValue == NULL)
	//	{
	//		//MYERROR(_T("分配内存错误，错误码：%d"),GetLastError());
	//		MYERROR(_T("GetPlazaId tValue == NULL"));
	//		delete[] pRead;
	//		pRead = NULL;
	//		CloseHandle(hPipe);
	//		return FALSE;
	//	}

	//	// 将查询结果写入到缓存
	//	for (int i=0;i<(int)dwRecordNum;i++)
	//	{
	//		// iDevId
	//		int iDevId = 0;
	//		ZeroMemory(tValue,1024+1);
	//		memcpy(tValue, pByte, pHead->DataLen);
	//		iDevId = (int)*((DWORD *)tValue);
	//		pByte += pHead->DataLen;

	//		BYTE YxValue = 0;
	//		ZeroMemory(tValue,1024+1);
	//		memcpy(tValue, pByte, (pHead+1)->DataLen);
	//		YxValue = *(BYTE*)tValue;
	//		pByte += (pHead+1)->DataLen;

	//		if (iDevId>tempId)
	//		{
	//			tempId = iDevId;
	//		}
	//		m_DevStateMap.SetAt(iDevId,YxValue);
	//	}
	//	delete []tValue;
	//	tValue = NULL;
	//	if (dwRecordNum<5000)
	//	{
	//		break;
	//	}
	//}

	//delete[] pRead;
	//pRead = NULL;
	//CloseHandle(hPipe);

	//return TRUE;
}

BOOL CRedisRecvSample::LoadSampleTableFromRTDB(WORD SampleTableNo)
{
	CString strKey = _T("");
	HANDLE hPipe = NULL;
	NetMessageHead* pNetMessageHead = NULL;
	MessageQueryRecord mqr;
	hPipe = OpenRealDataPipe();
	if (hPipe == NULL) {
		return FALSE;
	}
	//读取遥脉信息表
	int tempSampleId = 0;
	while(1)
	{
		mqr.BaseNo = BASE_NO_RTDB;
		mqr.TableNo = SampleTableNo;
		mqr.FilterFieldNo = 5;
		mqr.FilterType = FILTER_TYPE_EQU; 
		mqr.StartValue = tempSampleId;
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
		if(nNum<=0)
		{
			break;
		}
		while (nNum) {
			nOffset=0;
			int SampleNo = 0;
			memcpy(&SampleNo,pBuf,pRetRecordHead->DataLen);
			nOffset+=pRetRecordHead->DataLen;
			//memcpy(&YmConfig.nProjectNo,pBuf+nOffset,(pRetRecordHead+1)->DataLen);
			nOffset+=(pRetRecordHead+1)->DataLen;
			IndexDef iIndexDef;
			memcpy(&iIndexDef,pBuf+nOffset,(pRetRecordHead+2)->DataLen);
			nOffset+=(pRetRecordHead+2)->DataLen;
			nOffset+=(pRetRecordHead+3)->DataLen;
			nOffset+=(pRetRecordHead+4)->DataLen;
			nOffset+=(pRetRecordHead+5)->DataLen;
			

			strKey.Format(_T("%d+%d+%d"), SampleTableNo, TABLE_NO_PULSE, iIndexDef.RecordNo);	//key为“采样表号+三遥表号+三遥序号”
			m_MapSampleNo[strKey] = SampleNo;
			/*if (SampleNo>tempSampleId)
			{
				tempSampleId = SampleNo;
			}*/
			pBuf+=dwRecordLen;
			nNum--;
		}
		
		tempSampleId++;

	}
	//清理
	if(hPipe != NULL)			{CloseHandle(hPipe); hPipe = NULL;}
	if(pNetMessageHead != NULL){delete [](BYTE *)pNetMessageHead; pNetMessageHead=NULL;}

	return TRUE;
}
BOOL CRedisRecvSample::LoadYmTable()
{
	m_YmConfigDefMap.clear();

	CString strKey = _T("");
	HANDLE hPipe = NULL;
	//YmConfigDef YmConfig;
	SimpleYmDef SimpleYmConfig;
	NetMessageHead* pNetMessageHead = NULL;
	MessageQueryRecord mqr;
	hPipe = OpenRealDataPipe();
	if (hPipe == NULL) {
		return FALSE;
	}
	//读取遥脉信息表
	int iStationNum = 1;
	SYSTEMTIME sys_time;
	GetLocalTime(&sys_time);
	TIMESTAMP_STRUCT iTimeStamp;
	memset(&iTimeStamp,0,sizeof(TIMESTAMP_STRUCT));
	iTimeStamp.year = sys_time.wYear;
	iTimeStamp.month = sys_time.wMonth;
	iTimeStamp.day = sys_time.wDay;
	iTimeStamp.hour = sys_time.wHour;
	while(1)
	{
		if (iStationNum>MaxStationNum)
		{
			break;
		}
		map<int, int>::iterator it = m_StationDefMap.find(iStationNum);
		if (it==m_StationDefMap.end())
		{
			iStationNum++;
			continue;
		}
		mqr.BaseNo = BASE_NO_RTDB;
		mqr.TableNo = TABLE_NO_PULSE;
		mqr.FilterFieldNo = 3;//0;
		mqr.FilterType = FILTER_TYPE_EQU; 
		mqr.StartValue = iStationNum;
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
			memset(&SimpleYmConfig, 0, sizeof(SimpleYmDef));
			memcpy(&SimpleYmConfig.nYmIndex,pBuf,pRetRecordHead->DataLen);
			nOffset+=pRetRecordHead->DataLen;
			//memcpy(&YmConfig.nProjectNo,pBuf+nOffset,(pRetRecordHead+1)->DataLen);
			nOffset+=(pRetRecordHead+1)->DataLen;
			//memcpy(&YmConfig.nStationNo,pBuf+nOffset,(pRetRecordHead+2)->DataLen);
			nOffset+=(pRetRecordHead+2)->DataLen;
			memcpy(&SimpleYmConfig.nDeviceNo,pBuf+nOffset,(pRetRecordHead+3)->DataLen);
			nOffset+=(pRetRecordHead+3)->DataLen;
			memcpy(&SimpleYmConfig.nYmNum,pBuf+nOffset,(pRetRecordHead+4)->DataLen);
			nOffset+=(pRetRecordHead+4)->DataLen;
			//memcpy(YmConfig.cYmName,pBuf+nOffset,(pRetRecordHead+5)->DataLen);
			nOffset+=(pRetRecordHead+5)->DataLen;
			//memcpy(YmConfig.cDescription,pBuf+nOffset,(pRetRecordHead+6)->DataLen);
			nOffset+=(pRetRecordHead+6)->DataLen;
			memcpy(&SimpleYmConfig.nYmRaw,pBuf+nOffset,(pRetRecordHead+7)->DataLen);
			nOffset+=(pRetRecordHead+7)->DataLen;
			memcpy(&SimpleYmConfig.dYmQuotiety,pBuf+nOffset,(pRetRecordHead+8)->DataLen);
			nOffset+=(pRetRecordHead+8)->DataLen;
			memcpy(&SimpleYmConfig.dYmValue,pBuf+nOffset,(pRetRecordHead+9)->DataLen);
			nOffset+=(pRetRecordHead+9)->DataLen;
			//memcpy(&YmConfig.bIdentifier,pBuf+nOffset,(pRetRecordHead+10)->DataLen);
			
			SimpleYmConfig.DataTime = iTimeStamp;
			strKey.Format(_T("%d+%d"), SimpleYmConfig.nDeviceNo, SimpleYmConfig.nYmNum);	//key为“设备ID+遥脉序号”
			m_YmConfigDefMap[strKey] = SimpleYmConfig;
			pBuf+=dwRecordLen;
			nNum--;
		}

		iStationNum++;
	}
	//清理
	if(hPipe != NULL)			{CloseHandle(hPipe); hPipe = NULL;}
	if(pNetMessageHead != NULL){delete [](BYTE *)pNetMessageHead; pNetMessageHead=NULL;}

	return TRUE;
}

BOOL CRedisRecvSample::SetTimeId(CString strTime)
{
	if (strTime==_T(""))
	{
		return FALSE;
	}
	USES_CONVERSION;
	char* chArr = W2A(strTime);
	int iyear =0;
	int imonth = 0;
	int iday = 0;
	int ihour = 0;
	int iminute = 0;
	int nTableNo = 0;
	sscanf((const char *)chArr, "%d-%d-%d %d:%d:00_%d",&iyear, &imonth, &iday,&ihour,&iminute,&nTableNo);
	if (iyear==0||imonth==0||iday==0||nTableNo==0)
	{
		return FALSE;
	}
	return TRUE;
}
//void CRedisRecvSample::StartProcessData(SocketMsgGroupYmDataEx_DATE *pSocketMsgYmData)
//{
//	short nRtuNo = pSocketMsgYmData->btRtuNo;
//	int SampleTableNo = TABLE_NO_SAMPLEHOUR_EXT;
//	int iyear = 0, imonth = 0, iday = 0, ihour = 0, iminite = 0, isecond = 0;
//	iyear = pSocketMsgYmData->bYear;
//	imonth = pSocketMsgYmData->bMonth;
//	iday = pSocketMsgYmData->bDay;
//	ihour = pSocketMsgYmData->bHour;
//	iminite = pSocketMsgYmData->bMinite;
//	CString timestr = _T("");
//	timestr.Format(_T("20%02d-%02d-%02d %02d:%02d:00"),iyear,imonth,iday,ihour,iminite);
//	nRtuNo = pSocketMsgYmData->btForeNo * 256 + pSocketMsgYmData->btRtuNo;
//	/*if (pSocketMsgYmData->btForeNo < ADDR_SOURCEMACHINE && pSocketMsgYmData->btForeNo >= 0)
//	{
//		nRtuNo = pSocketMsgYmData->btForeNo * 256 + pSocketMsgYmData->btRtuNo;
//	}*/
//	int YmNum = pSocketMsgYmData->wdYmNum;
//
//	unsigned char* pbYmData = NULL;
//	int nLengthYmStruct = 0;
//
//	SocketGroupYmUnitEx* pSocketYmDataEx = pSocketMsgYmData->YmUnitEx;
//	if (NULL == pSocketYmDataEx)	
//	{
//		return;
//	}
//	else	//带标志位的脉冲量
//	{
//		pbYmData = (unsigned char*)pSocketYmDataEx;
//		nLengthYmStruct = sizeof(SocketGroupYmUnitEx);
//	}
//	int nLengthWord = sizeof(WORD);
//	int nLengthDWord = sizeof(DWORD);
//	int MinSampleNo = 0;
//	int MaxSampleNo = 0;
//	int MinSampleNoDay = 0;
//	int MaxSampleNoDay = 0;
//	int icount = 0;
//	int icountDay = 0;
//	m_SampleValueInfoMap.clear();
//	m_SampleValueInfoMapDay.clear();
//	m_YmValueInfoCArray.RemoveAll();
//	CTSSampleDataSvrDlg* pDlg = (CTSSampleDataSvrDlg*)AfxGetApp()->m_pMainWnd;
//	for (int i=0; i<YmNum; i++,pbYmData += nLengthYmStruct) 
//	{
//		unsigned char cFlag = DATA_FLAG_INVALID;        
//		cFlag = *(pbYmData + sizeof(SocketGroupYmUnit));   //标志位
//
//		PowFileInfo powInfo;
//		memset(&powInfo, 0, sizeof(PowFileInfo));
//		powInfo.nDevID = *((WORD*)pbYmData);                  //前置设备号
//		powInfo.nYmNum = *((WORD*)(pbYmData + nLengthWord));   //点位号
//		powInfo.nYmVal = (DWORD) *((DWORD*)(pbYmData + nLengthWord*2));  //原始值
//		powInfo.cFlag = cFlag;
//		YmConfigDef YmDef;
//		int iDevId = 0 ;
//		GetYmDefByPowInfo(nRtuNo,&powInfo,YmDef,iDevId);
//		
//		
//		CString strKey = _T("");
//		strKey.Format(_T("%d+%d+%d"),SampleTableNo, TABLE_NO_PULSE, YmDef.nYmIndex);
//		SampleDayNo::iterator iSampleInfo;	
//		iSampleInfo = m_MapSampleNo.find(strKey);
//		if (iSampleInfo == m_MapSampleNo.end())
//		{
//			continue;
//		}
//		icount++;
//		int SampleNo = iSampleInfo->second;
//		if (icount==1)
//		{
//			MinSampleNo = SampleNo;
//			MaxSampleNo = SampleNo;
//		}
//		else 
//		{
//			if (MinSampleNo>SampleNo)
//			{
//				MinSampleNo = SampleNo;
//			}
//			if (MaxSampleNo<SampleNo)
//			{
//				MaxSampleNo = SampleNo;
//			}	
//		}
//		double dbYm = 0.0;
//		dbYm = powInfo.nYmVal * YmDef.dYmQuotiety;
//		SampleValueInfo iSampleValue;
//		iSampleValue.SampleNo = SampleNo;
//		iSampleValue.dbValue = dbYm;
//		iSampleValue.cFlag = cFlag;
//		m_SampleValueInfoMap[SampleNo] = iSampleValue;
//
//		//把购电次数加入列表  用于检查是否招充值记录
//		if (powInfo.nYmNum==7&&powInfo.cFlag==1)
//		{
//			if (YmDef.nYmRaw<powInfo.nYmVal)  //购电次数大于实时库中的购电次数
//			{
//				ProtocolCallItem *pItem = new ProtocolCallItem;
//				if (NULL==pItem)
//				{
//					continue;
//				}
//				memset(pItem,0,sizeof(ProtocolCallItem));
//				pItem->m_nType = 0xE1;//MSW 150828,加注释，0xE1-召唤充值记录的标志
//				pItem->m_nDeviceNo = YmDef.nDeviceNo;    //设备表第一列 Id
//				pItem->m_nStationNum = nRtuNo;
//
//				SocketMsgPrepayRecord iPrepayRecord ;
//				memset(&iPrepayRecord,0,sizeof(SocketMsgPrepayRecord));
//				iPrepayRecord.btRtuNo = pSocketMsgYmData->btRtuNo;     //==m_nStationNum
//				iPrepayRecord.btEndNo = 0xff;
//				iPrepayRecord.btForeNo = pSocketMsgYmData->btForeNo;
//				iPrepayRecord.nDevNo = powInfo.nDevID;    //设备表第二列 deviceNo
//				iPrepayRecord.wdYjNum = powInfo.nYmVal - YmDef.nYmRaw;
//				memcpy(pItem->m_nBuffer,&iPrepayRecord,sizeof(SocketMsgPrepayRecord)-sizeof(SocketPrepayRecordUnit));
//
//				EnterCriticalSection(&pDlg->m_pCCSendP2pTask->m_csRecordList);
//				pDlg->m_pCCSendP2pTask->m_RecordList.AddTail(pItem);
//				LeaveCriticalSection(&pDlg->m_pCCSendP2pTask->m_csRecordList);
//			}
//		}
//		if (cFlag==1)  //更新至m_YmConfigDefMap里
//		{
//			YmDef.nYmRaw = powInfo.nYmVal;
//			YmDef.dYmValue = powInfo.nYmVal*YmDef.dYmQuotiety;
//			m_YmValueInfoCArray.Add(YmDef);
//			CString tempKey = _T("");
//			tempKey.Format(_T("%d+%d"), YmDef.nDeviceNo, YmDef.nYmNum);	//key为“设备ID+遥脉序号”
//			m_YmConfigDefMap[tempKey] = YmDef;
//		}
//		if (ihour==0&&iminite==0)
//		{
//			strKey = _T("");
//			strKey.Format(_T("%d+%d+%d"),TABLE_NO_SAMPLEDAY_EXT, TABLE_NO_PULSE, YmDef.nYmIndex);
//			SampleDayNo::iterator iSampleInfo;	
//			iSampleInfo = m_MapSampleNo.find(strKey);
//			if (iSampleInfo == m_MapSampleNo.end())
//			{
//				continue;
//			}
//			icountDay++;
//			int SampleNoDay = iSampleInfo->second;
//			if (icountDay==1)
//			{
//				MinSampleNoDay = SampleNoDay;
//				MaxSampleNoDay = SampleNoDay;
//			}
//			else 
//			{
//				if (MinSampleNoDay>SampleNoDay)
//				{
//					MinSampleNoDay = SampleNoDay;
//				}
//				if (MaxSampleNoDay<SampleNoDay)
//				{
//					MaxSampleNoDay = SampleNoDay;
//				}	
//			}
//			SampleValueInfo iSampleValueDay;
//			iSampleValueDay.SampleNo = SampleNoDay;
//			iSampleValueDay.dbValue = dbYm;
//			iSampleValueDay.cFlag = cFlag;
//			m_SampleValueInfoMapDay[SampleNoDay] = iSampleValueDay;
//		}
//	}
//
//	if (pDlg->m_pCCSendP2pTask->m_RecordList.GetSize()>0)
//	{
//		SetEvent(pDlg->m_pCCSendP2pTask->m_ProcessDataEvent);
//	}
//	if (m_YmValueInfoCArray.GetSize()>0)
//	{
//		Update2RTDB();
//	}
//	if (m_SampleValueInfoMap.size()>0)
//	{
//		SaveSampleData2DB(MinSampleNo,MaxSampleNo,timestr,0);
//	}
//	if (m_SampleValueInfoMapDay.size()>0)
//	{
//		SaveSampleData2DB(MinSampleNoDay,MaxSampleNoDay,timestr,1);
//	}
//}
//
//void CRedisRecvSample::SaveSampleData2DB(int MinSampleNo, int MaxSampleNo,CString StrTimeId,int itype)
//{
//	HANDLE hPipe = OpenServerPipe();
//	if (hPipe == NULL)
//	{
//		return ;
//	}
//	BYTE* pRead = NULL;
//	int TableMinNo = MinSampleNo/SAMPLE_FIELDNUM ;
//	int TableMaxNo = MaxSampleNo/SAMPLE_FIELDNUM ;
//	CString PreTableName = _T("TS_H1_%03d");
//	CString PreTableExtName = _T("TS_H1_EXT_%03d");
//	if(itype==1)
//	{
//		PreTableName = _T("TS_DAY_%03d");
//		PreTableExtName = _T("TS_DAY_EXT_%03d");
//	}
//	CString strnSql = _T("");
//	int nCount = 0;
//	for (int i = TableMinNo; i<=TableMaxNo;i++)
//	{
//		CString tableName = _T("");
//		tableName.Format(PreTableName,i);
//		CString tableNameExt = _T("");
//		tableNameExt.Format(PreTableExtName,i);
//		CString strSql = _T("");
//		CString strSql_Ext = _T("");
//		CString ColonmNames = _T("TIMEID");
//		CString ColonNames_Ext = _T("TIMEID");
//		CString strVals = _T("");
//		CString strValsflag = _T("");
//		CString UpdateVals = _T("");
//		CString UpdateVals_Ext = _T("");
//		for (int j = 1;j<=SAMPLE_FIELDNUM;j++)
//		{
//			int sampleNo = i*SAMPLE_FIELDNUM + j;
//			if (itype==0)  //小时采样
//			{
//				SampleValueInfoMap::iterator its;	
//				its = m_SampleValueInfoMap.find(sampleNo);
//				if (its != m_SampleValueInfoMap.end())
//				{
//					SampleValueInfo iSampleValue = its->second;
//					TCHAR Text[5];
//					TCHAR TextExt[10];
//					TCHAR ValueText[40];
//					TCHAR ValueTextflag[40];
//					TCHAR UpdateText[40];
//					TCHAR UpdateTextExt[40];
//					_stprintf(Text,_T(",V%03d"), j);
//					ColonmNames += Text;
//					_stprintf(TextExt,_T(",V%03d_FLAG"), j);
//					ColonNames_Ext += TextExt;
//					_stprintf(ValueText,_T(",%2f"),iSampleValue.dbValue);
//					strVals += ValueText;
//					_stprintf(ValueTextflag,_T(",%d"),iSampleValue.cFlag);
//					strValsflag += ValueTextflag;
//					_stprintf(UpdateText,_T("V%03d=%2f,"),j,iSampleValue.dbValue);
//					UpdateVals += UpdateText;
//					_stprintf(UpdateTextExt,_T("V%03d_FLAG=%d,"),j,iSampleValue.cFlag);
//					UpdateVals_Ext += UpdateTextExt;
//				}
//
//			}
//			else if (itype==1)  //天采样
//			{
//				SampleValueInfoMap::iterator its;	
//				its = m_SampleValueInfoMapDay.find(sampleNo);
//				if (its != m_SampleValueInfoMapDay.end())
//				{
//					/*SampleValueInfo iSampleValue = its->second;
//					TCHAR Text[5];
//					TCHAR ValueText[40];
//					TCHAR UpdateText[40];
//					_stprintf(Text,_T(",V%03d"), j);
//					ColonmNames += Text;
//					_stprintf(ValueText,_T(",%2f"),iSampleValue.dbValue);
//					strVals += ValueText;
//					_stprintf(UpdateText,_T("V%03d=%2f,"),j,iSampleValue.dbValue);
//					UpdateVals += UpdateText;*/
//					SampleValueInfo iSampleValue = its->second;
//					TCHAR Text[5];
//					TCHAR TextExt[10];
//					TCHAR ValueText[40];
//					TCHAR ValueTextflag[40];
//					TCHAR UpdateText[40];
//					TCHAR UpdateTextExt[40];
//					_stprintf(Text,_T(",V%03d"), j);
//					ColonmNames += Text;
//					_stprintf(TextExt,_T(",V%03d_FLAG"), j);
//					ColonNames_Ext += TextExt;
//					_stprintf(ValueText,_T(",%2f"),iSampleValue.dbValue);
//					strVals += ValueText;
//					_stprintf(ValueTextflag,_T(",%d"),iSampleValue.cFlag);
//					strValsflag += ValueTextflag;
//					_stprintf(UpdateText,_T("V%03d=%2f,"),j,iSampleValue.dbValue);
//					UpdateVals += UpdateText;
//					_stprintf(UpdateTextExt,_T("V%03d_FLAG=%d,"),j,iSampleValue.cFlag);
//					UpdateVals_Ext += UpdateTextExt;
//				}
//			}
//
//		}
//		if (UpdateVals.IsEmpty())
//		{
//			continue;
//		}
//		UpdateVals = UpdateVals.Left(UpdateVals.GetLength()-1);
//		strSql.Format(C_SQL_Insert_SampleTable,tableName,StrTimeId,tableName,UpdateVals,StrTimeId,tableName,ColonmNames,StrTimeId,strVals);
//		strSql_Ext.Format(C_SQL_Insert_SampleTable,tableNameExt,StrTimeId,tableNameExt,UpdateVals_Ext,StrTimeId,tableNameExt,ColonNames_Ext,StrTimeId,strValsflag);
//		//strSql.Format(_T("if insert into %s (%s) values ('%s'%s);"),tableName,ColonmNames,StrTimeId,strVals);
//		//strnSql += (strSql + strSql_Ext);
//		strnSql += strSql;
//		nCount++;
//		if (nCount%SQL_COUNT_ONCE!=0 && i!=TableMaxNo)
//		{
//			continue;
//		}
//		CString strSqlFinal = strnSql;
//		ReguTrace(SQL,"SQL:<%s>",strSqlFinal);
//		pRead = (BYTE*)GetMessage_RecordOfSql_Ext(hPipe, strSqlFinal);
//		if (pRead)
//		{
//			delete[] pRead;
//			pRead = NULL;
//		}
//		strnSql.Empty();
//	}
//	if (hPipe != NULL)
//	{
//		CloseHandle(hPipe);
//		hPipe = NULL;
//	}
//}
//
////更新至实时库
//BOOL CRedisRecvSample::Update2RTDB()
//{
//	HANDLE hPipe=NULL;	 
//	hPipe = OpenServerPipe();
//	if (hPipe==NULL)
//		return FALSE;
//	for (int i=0; i<m_YmValueInfoCArray.GetSize();i++)
//	{
//		YmConfigDef iYmDef;
//		iYmDef = m_YmValueInfoCArray[i];
//		BYTE *pData;
//		int nRecordLen = sizeof(YmConfigDef);
//		DWORD dwBufLen = nRecordLen + sizeof(UpdataRecordStruct) + sizeof(NetMessageHead);
//		BYTE* pWrite = new BYTE[dwBufLen];
//		NetMessageHead* pMessageHead = (NetMessageHead*)pWrite;										  
//		pMessageHead->MessageType =	NET_MESSAGE_UPDATARECORD;						 
//		pMessageHead->Length = nRecordLen + sizeof(UpdataRecordStruct) - 1;							 
//		UpdataRecordStruct* pUpdataStruct = (UpdataRecordStruct*)&pWrite[sizeof(NetMessageHead)];
//
//		pUpdataStruct->TableNo = TABLE_NO_PULSE;	
//		memset(&(pUpdataStruct->ColFlag[0]),0,256);
//		for (int i=0; i<11; i++) {
//			pUpdataStruct->ColFlag[i] = 1;
//		}
//
//		pUpdataStruct->UpdataType = UPDATARECORD_TYPE_UPDATA; 
//		pUpdataStruct->RecordLen = nRecordLen;
//
//		BYTE *pData1;
//		pData = new BYTE[nRecordLen];
//		memset(pData,0,nRecordLen);
//		int nOffset = 0;
//		pData1 = pData;
//
//		memcpy(pData,&iYmDef.nYmIndex,sizeof(iYmDef.nYmIndex));
//		nOffset+=sizeof(iYmDef.nYmIndex);
//
//		memcpy((pData+nOffset),&iYmDef.nProjectNo,sizeof(iYmDef.nProjectNo));
//		nOffset+=sizeof(iYmDef.nProjectNo);
//
//		memcpy((pData+nOffset),&iYmDef.nStationNo,sizeof(iYmDef.nStationNo));
//		nOffset+=sizeof(iYmDef.nStationNo);
//
//		memcpy((pData+nOffset),&iYmDef.nDeviceNo,sizeof(iYmDef.nDeviceNo));
//		nOffset+=sizeof(iYmDef.nDeviceNo);
//
//		memcpy((pData+nOffset),&iYmDef.nYmNum,sizeof(iYmDef.nYmNum));
//		nOffset+=sizeof(iYmDef.nYmNum);
//
//		memcpy((pData+nOffset),iYmDef.cYmName,sizeof(iYmDef.cYmName));
//		nOffset+=sizeof(iYmDef.cYmName);
//
//		memcpy((pData+nOffset),iYmDef.cDescription,sizeof(iYmDef.cDescription));
//		nOffset+=sizeof(iYmDef.cDescription);
//
//		memcpy((pData+nOffset),&iYmDef.nYmRaw,sizeof(iYmDef.nYmRaw));
//		nOffset+=sizeof(iYmDef.nYmRaw);
//
//		memcpy((pData+nOffset),&iYmDef.dYmQuotiety,sizeof(iYmDef.dYmQuotiety));
//		nOffset+=sizeof(iYmDef.dYmQuotiety);
//
//		memcpy((pData+nOffset),&iYmDef.dYmValue,sizeof(iYmDef.dYmValue));
//		nOffset+=sizeof(iYmDef.dYmValue);
//
//		memcpy((pData+nOffset),&iYmDef.bIdentifier,sizeof(iYmDef.bIdentifier));
//		nOffset+=sizeof(iYmDef.bIdentifier);
//
//		memmove(&pUpdataStruct->RecordData,pData1,nRecordLen);
//
//		BYTE* pRead;
//		if (hPipe)
//			pRead = (BYTE*)GetNetMessage(hPipe, pWrite,dwBufLen-1);	
//
//		delete[] pWrite;  //pData 没有释放
//		if (pRead==NULL) 
//		{
//			continue;
//		}
//		pMessageHead = (NetMessageHead*)pRead;
//		MessageAck* pAck = (MessageAck*)&pRead[sizeof(NetMessageHead)];
//		if (pMessageHead->MessageType!=NET_MESSAGE_ACK || pAck->wdAckType!=NET_MESSAGE_ACK_OK) 
//		{
//			delete[] pRead;		
//			continue;
//		}
//		delete[] pRead;
//
//	}
//	for (int i=0; i<m_YxValueInfoCArray.GetSize();i++)
//	{
//		YxConfigDef iYxConfig;
//		iYxConfig = m_YxValueInfoCArray[i];
//		BYTE *pData;
//		int nRecordLen = sizeof(YxConfigDef);
//
//		DWORD dwBufLen = nRecordLen + sizeof(UpdataRecordStruct) + sizeof(NetMessageHead);
//		BYTE* pWrite = new BYTE[dwBufLen];
//		NetMessageHead* pMessageHead = (NetMessageHead*)pWrite;										  
//		pMessageHead->MessageType =	NET_MESSAGE_UPDATARECORD;						 
//		pMessageHead->Length = nRecordLen + sizeof(UpdataRecordStruct) - 1;							 
//		UpdataRecordStruct* pUpdataStruct = (UpdataRecordStruct*)&pWrite[sizeof(NetMessageHead)];
//
//		pUpdataStruct->TableNo = TABLE_NO_DI;	
//		memset(&(pUpdataStruct->ColFlag[0]),0,256);
//		
//		pUpdataStruct->ColFlag[7] = 1;
//		pUpdataStruct->ColFlag[9] = 1;
//
//		pUpdataStruct->UpdataType = UPDATARECORD_TYPE_UPDATA; 
//		pUpdataStruct->RecordLen = nRecordLen;
//
//		BYTE *pData1;
//		pData = new BYTE[nRecordLen];
//		memset(pData,0,nRecordLen);
//		int nOffset = 0;
//		pData1 = pData;
//
//		memcpy(pData,&iYxConfig.nYxIndex,sizeof(iYxConfig.nYxIndex));
//		nOffset+=sizeof(iYxConfig.nYxIndex);
//
//		memcpy((pData+nOffset),&iYxConfig.nProjectNo,sizeof(iYxConfig.nProjectNo));
//		nOffset+=sizeof(iYxConfig.nProjectNo);
//
//		memcpy((pData+nOffset),&iYxConfig.nStationNo,sizeof(iYxConfig.nStationNo));
//		nOffset+=sizeof(iYxConfig.nStationNo);
//
//		memcpy((pData+nOffset),&iYxConfig.nDeviceNo,sizeof(iYxConfig.nDeviceNo));
//		nOffset+=sizeof(iYxConfig.nDeviceNo);
//
//		memcpy((pData+nOffset),&iYxConfig.nYxNum,sizeof(iYxConfig.nYxNum));
//		nOffset+=sizeof(iYxConfig.nYxNum);
//
//		memcpy((pData+nOffset),iYxConfig.cYxName,sizeof(iYxConfig.cYxName));
//		nOffset+=sizeof(iYxConfig.cYxName);
//
//		memcpy((pData+nOffset),iYxConfig.cDescription,sizeof(iYxConfig.cDescription));
//		nOffset+=sizeof(iYxConfig.cDescription);
//
//		memcpy((pData+nOffset),&iYxConfig.bYxRaw,sizeof(iYxConfig.bYxRaw));
//		nOffset+=sizeof(iYxConfig.bYxRaw);
//
//		memcpy((pData+nOffset),&iYxConfig.bYxType,sizeof(iYxConfig.bYxType));
//		nOffset+=sizeof(iYxConfig.bYxType);
//
//		memcpy((pData+nOffset),&iYxConfig.bYxValue,sizeof(iYxConfig.bYxValue));
//		nOffset+=sizeof(iYxConfig.bYxValue);
//
//		memcpy((pData+nOffset),&iYxConfig.bIdentifier,sizeof(iYxConfig.bIdentifier));
//		nOffset+=sizeof(iYxConfig.bIdentifier);
//
//		memcpy((pData+nOffset),&iYxConfig.nCountExchange,sizeof(iYxConfig.nCountExchange));
//		nOffset+=sizeof(iYxConfig.nCountExchange);
//
//		memcpy((pData+nOffset),&iYxConfig.nCountTrip,sizeof(iYxConfig.nCountTrip));
//		nOffset+=sizeof(iYxConfig.nCountTrip);
//
//		memcpy((pData+nOffset),&iYxConfig.nAlarmType,sizeof(iYxConfig.nAlarmType));
//		nOffset+=sizeof(iYxConfig.nAlarmType);
//
//		memcpy((pData+nOffset),&iYxConfig.nAlarmLevel,sizeof(iYxConfig.nAlarmLevel));
//		nOffset+=sizeof(iYxConfig.nAlarmLevel);
//
//		memcpy((pData+nOffset),&iYxConfig.nYxProp,sizeof(iYxConfig.nYxProp));
//		nOffset+=sizeof(iYxConfig.nYxProp);	
//
//		memmove(&pUpdataStruct->RecordData,pData1,nRecordLen);
//
//		BYTE* pRead;
//		if (hPipe)
//			pRead = (BYTE*)GetNetMessage(hPipe, pWrite,dwBufLen-1);	
//
//		delete[] pWrite;	
//		if (pRead==NULL) 
//		{
//			continue;
//		}
//		pMessageHead = (NetMessageHead*)pRead;
//		MessageAck* pAck = (MessageAck*)&pRead[sizeof(NetMessageHead)];
//		if (pMessageHead->MessageType==NET_MESSAGE_ACK && pAck->wdAckType==NET_MESSAGE_ACK_OK) 
//		{
//			delete[] pRead;		
//			continue;
//		}
//		delete[] pRead;
//	}
//	if (hPipe != NULL)
//	{
//		CloseHandle(hPipe);
//		hPipe = NULL;
//	}
//	return TRUE;
//}

BOOL CRedisRecvSample::TChar2Char(const TCHAR* pTchar, char* pChar)
{
	int iASCIILen = 0;
	iASCIILen = WideCharToMultiByte(CP_ACP, 0, pTchar, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, pTchar, -1, pChar, iASCIILen, NULL, NULL);
	return TRUE;
}

BOOL CRedisRecvSample::CharToTchar (const char * _char, TCHAR * tchar) 
{ 
	int iLength ; 
	iLength = MultiByteToWideChar (CP_ACP, 0, _char, strlen (_char) + 1, NULL, 0) ; 
	MultiByteToWideChar (CP_ACP, 0, _char, strlen (_char) + 1, tchar, iLength) ; 
	return TRUE;
} 

int CRedisRecvSample::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CRedisRecvSample, CWinThread)
END_MESSAGE_MAP()


// CRedisRecvSample 消息处理程序
