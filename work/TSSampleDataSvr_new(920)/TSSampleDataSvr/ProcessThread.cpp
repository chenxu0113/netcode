// ProcessThread.cpp : 实现文件
//
#pragma once
#include "stdafx.h"
#include "TSSampleDataSvr.h"
#include "ProcessThread.h"
#include "TSSampleDataSvrDlg.h"

// CProcessThread
extern CLog *g_log;
extern CLog *DataTime_log;
IMPLEMENT_DYNCREATE(CProcessThread, CWinThread)

CProcessThread::CProcessThread()
{
	m_Index = 0;
	m_ProcessDataEvent = NULL;
	m_hExitEvent = NULL;
	m_hExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_ProcessDataEvent = CreateEvent(NULL,TRUE, FALSE, NULL);
	InitializeCriticalSection(&m_csRedisDataList);
	m_softbus = NULL;
	m_softbus = new CRedisBus;
	if (NULL == m_softbus)
	{
		return;
	}
	if(NULL==g_log)
	{
		g_log = new CLog(_T("\\TSSampleDataSvr"));
	}
	if (NULL==DataTime_log)
	{
		DataTime_log = new CLog(_T("\\SampleDataCheck"));
	}
}

CProcessThread::~CProcessThread()
{
	if (m_softbus!=NULL)
	{
		delete m_softbus;
		m_softbus = NULL;
	}
}

BOOL CProcessThread::ConnectRedisServer()
{
	CTSSampleDataSvrDlg* pDlg = (CTSSampleDataSvrDlg*)AfxGetApp()->m_pMainWnd;
	int nCount = 0;
	char szIP[WORDDOC_NAMESTRING_LEN];
	memset(szIP, 0, WORDDOC_NAMESTRING_LEN);
	if (!pDlg->m_pCRedisRecvSample->TChar2Char(pDlg->m_pCRedisRecvSample->RedisIPAddress,szIP))
	{
		return FALSE;
	}
	while(1)
	{
		if (m_softbus->RegisterSoftBus(szIP, pDlg->m_pCRedisRecvSample->RedisPort) == REDIS_OK)
		{
			break;
		}
		else
		{
			m_softbus->UnRegisterSoftBus();
			m_softbus->RegisterSoftBus(szIP, pDlg->m_pCRedisRecvSample->RedisPort);
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

BOOL CProcessThread::GetDataFromRedis()
{
	/*unsigned char buffer[1024];
	memset(buffer,0,1024);*/
	unsigned char * buffer = NULL;
	int ret = REDIS_NODATA;
	ret = m_softbus->RecvMessageFromMsmq_Ext("SampleMessage",&buffer); 
	//ret = m_softbus->RecvMessageFromMsmq("SampleMessage",buffer,&length);
	if(REDIS_ERR == ret)
	{
		ReguTrace(Config,"线程%d:ReconnectRedisServer!",m_Index);
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
		CString recvstr = _T("");
		if (NULL == buffer)
		{
			ReguTrace(Config,"线程%d:buffer==NULL!",m_Index);
			return FALSE;
		}
		BYTE * pBuf = (BYTE*)buffer;

		SocketMsgHead* pHead = (SocketMsgHead*)(pBuf+ sizeof(NetMessageHead));
		int nn = pHead->btType;
		int len = pHead->wdLen;
		int mm = pHead->dwSyncCode;
		int vv = sizeof(NetMessageHead)+sizeof(SocketMsgHead);
		BYTE* pMsg = pBuf + sizeof(NetMessageHead)+sizeof(SocketMsgHead);
		switch(pHead->btType)
		{
		case SOCKET_MSG_GROUP_YXDATA:
			ReguTrace(Config,"线程%d收到YX报文:%s",m_Index,bufToHexString(pMsg,len).c_str());
			OnSocketMsgGroupYx((SocketMsgGroupYxEx*)pMsg);
			break;
		case SOCKET_MSG_GROUP_YMDATA_EX_DATE:
			ReguTrace(Config,"线程%d收到YM报文:%s",m_Index,bufToHexString(pMsg,len).c_str());
			StartProcessData((SocketMsgGroupYmDataEx_DATE*)pMsg);
			break;
		case SOCKET_MSG_PREPAYRECORD:
			ReguTrace(Config,"线程%d收到PAYRECORD报文:%s",m_Index,bufToHexString(pMsg,len).c_str());
			OnCallPrePayRecord((SocketMsgPrepayRecord*)pMsg);
			break;
		default:
			break;
		}
		
		
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

	if (buffer!=NULL)
	{
		delete [] buffer;
		buffer = NULL;
	}
	return TRUE;
}

BOOL CProcessThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	CTSSampleDataSvrDlg* pDlg = (CTSSampleDataSvrDlg*)AfxGetApp()->m_pMainWnd;
	if(!ConnectRedisServer())
	{
		ReguTrace(Config,"线程%d:ConnectRedisServer err!",m_Index);
		//return FALSE;
	}
	ReguTrace(Config,"线程%d:ConnectRedisServer 成功!",m_Index);
	while(!theApp.bExitFlag)
	{
		if (!pDlg->m_pCRedisRecvSample->beInited)
		{
			Sleep(200);
			continue;
		}
		BOOL bRet = FALSE;
		bRet = GetDataFromRedis();
		if (!bRet)
		{
			Sleep(1000);
		}
	}

	//while(0)
	//{
	//	HANDLE hEvents[2];
	//	hEvents[0] = m_hExitEvent; 
	//	ResetEvent(hEvents[0]);
	//	hEvents[1] = m_ProcessDataEvent;
	//	ResetEvent(hEvents[1]);
	//	DWORD dwWait = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);
	//	switch (dwWait)
	//	{
	//	case WAIT_TIMEOUT:
	//		break;
	//	case WAIT_OBJECT_0:	  
	//		break;
	//	case WAIT_OBJECT_0+1:   
	//		ResetEvent(hEvents[1]);
	//		POSITION pos = NULL;

	//		/*EnterCriticalSection(&m_csRedisDataList);
	//		pos = m_RedisDataList.GetHeadPosition();
	//		LeaveCriticalSection(&m_csRedisDataList);

	//		while (pos) 
	//		{		
	//			pos1 = pos;

	//			EnterCriticalSection(&m_csRedisDataList);
	//			BYTE* pBuf = (BYTE*)m_RedisDataList.GetNext(pos);	
	//			m_listReceMsg.RemoveAt(pos1);
	//			LeaveCriticalSection(&m_csRedisDataList); 
	//			//这时GetNext后若pos为空，在处理过程中m_RedisDataList刚好有数据插入后，
	//			//循环终止，新的数据将不会接收到
	//			...
	//		}*/
	//		while(1)
	//		{
	//			EnterCriticalSection(&m_csRedisDataList);
	//			pos = m_RedisDataList.GetHeadPosition();
	//			if (!pos)
	//			{
	//				LeaveCriticalSection(&m_csRedisDataList);
	//				break;
	//			}
	//			POSITION pos1 = pos;
	//			BYTE *pBuf = (BYTE*)m_RedisDataList.GetNext(pos);
	//			m_RedisDataList.RemoveAt(pos1);
	//			int nNum = m_RedisDataList.GetSize();
	//			LeaveCriticalSection(&m_csRedisDataList);
	//			//ReguTrace(Config,"收到88报文:%s",bufToHexString(pBuf,100).c_str());
	//			SocketMsgHead* pHead = (SocketMsgHead*)(pBuf+ sizeof(NetMessageHead));
	//			int nn = pHead->btType;
	//			int len = pHead->wdLen;
	//			int mm = pHead->dwSyncCode;
	//			int vv = sizeof(NetMessageHead)+sizeof(SocketMsgHead);
	//			BYTE* pMsg = pBuf + sizeof(NetMessageHead)+sizeof(SocketMsgHead);
	//			switch(pHead->btType)
	//			{
	//			case SOCKET_MSG_GROUP_YXDATA:
	//				ReguTrace(Config,"线程%d收到YX报文(剩余%d条待处理):%s",m_Index,nNum,bufToHexString(pMsg,len).c_str());
	//				OnSocketMsgGroupYx((SocketMsgGroupYxEx*)pMsg);
	//				break;
	//			case SOCKET_MSG_GROUP_YMDATA_EX_DATE:
	//				ReguTrace(Config,"线程%d收到YM报文(剩余%d条待处理):%s",m_Index,nNum,bufToHexString(pMsg,len).c_str());
	//				StartProcessData((SocketMsgGroupYmDataEx_DATE*)pMsg);
	//				break;
	//			case SOCKET_MSG_PREPAYRECORD:
	//				ReguTrace(Config,"线程%d收到PAYRECORD报文(剩余%d条待处理):%s",m_Index,nNum,bufToHexString(pMsg,len).c_str());
	//				OnCallPrePayRecord((SocketMsgPrepayRecord*)pMsg);
	//				break;
	//			default:
	//				break;
	//			}
	//			
	//			if (pBuf!=NULL)
	//			{
	//				delete [] pBuf;
	//			}
	//			/*EnterCriticalSection(&m_csRedisDataList);
	//			m_RedisDataList.RemoveAt(pos1);
	//			LeaveCriticalSection(&m_csRedisDataList);*/
	//		}
	//		
	//	}
	//
	//}

	return TRUE;
}

int CProcessThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BOOL CProcessThread::QueryExamineRecorde(int iDevId, int ChargeCnt,float fFee, CTime ChargeTime, CString &SerialId)
{
	int iFdCount = 1;	//	查询的字段数
	int iMaxRsCount = -1;	// 限制查询得到的最大记录个数，-1指不限制
	CTime MinTime,MaxTime;
	MinTime = ChargeTime - CTimeSpan(0,1,0,0);
	MaxTime = ChargeTime + CTimeSpan(0,1,0,0);
	CString csMinTime = _T(""),csMaxTime = _T("");
	csMinTime.Format(_T("%d-%d-%d %d:%d:%d"),MinTime.GetYear(),MinTime.GetMonth(),MinTime.GetDay(),MinTime.GetHour(),MinTime.GetMinute(),MinTime.GetSecond());
	csMaxTime.Format(_T("%d-%d-%d %d:%d:%d"),MaxTime.GetYear(),MaxTime.GetMonth(),MaxTime.GetDay(),MaxTime.GetHour(),MaxTime.GetMinute(),MaxTime.GetSecond());
	// 向数据管道发送SQL语句
	BYTE* pRead = NULL;

	/*HANDLE hPipe =OpenRealDataPipe();

	if (hPipe == NULL) 
	{
		MYERROR(_T("QueryExamineRecorde hPipe null"));
		return FALSE;
	}*/

	
	CString strQuery;
	strQuery.Format(_T("select DetailsId from TB_EMS_EXAMINEACTION where StayObj= '%d' and (ISOK=0 or ISOK=4)and FUNCNAME='ns__cmdChargeWD' \
					   and ChargeCnt=%d and abs([Param]-%02f)<0.1 and UPDATETIME>='%s' and UPDATETIME<='%s';"),iDevId,ChargeCnt,fFee,csMinTime,csMaxTime);
	pRead = (BYTE*)GetMessage_RecordOfSql(strQuery);

	if (NULL == pRead)	// 未查询到记录
	{
		//CloseHandle(hPipe);
		MYERROR(_T("QueryExamineRecorde pRead null"));
		return FALSE;
	}
	// 解析返回的结果
	NetMessageHead *pNetMessageHead = (NetMessageHead*)pRead;
	if (pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB) {
		//	MYWARN(_T("返回报文异常，%s"),bufToHexString(pRead,pNetMessageHead->Length).c_str());
		delete[] pRead;
		pRead = NULL;
		//CloseHandle(hPipe);
		MYERROR(_T("QueryExamineRecorde pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB null"));
		return FALSE;
	}	

	if (pNetMessageHead->Length < 1)	// 空报文，无查询结果
	{
		//MYWARN(_T("接收到空回复报文，无法解析查询结果"));
		delete[] pRead;
		pRead = NULL;
		//CloseHandle(hPipe);
		MYERROR(_T("QueryExamineRecorde pNetMessageHead->Length < 1 null"));
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
		//CloseHandle(hPipe);
		MYERROR(_T("QueryExamineRecorde nFieldNum != iFdCount null"));
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
		//CloseHandle(hPipe);
		return FALSE;
	}
	pByte += sizeof(DWORD);

	// 记录数
	DWORD dwRecordNum = *((int*)pByte);
	if ((iMaxRsCount > 0) && (dwRecordNum != iMaxRsCount))
	{
		delete[] pRead;
		pRead = NULL;
		//CloseHandle(hPipe);
		return FALSE;
	}
	pByte += sizeof(DWORD);

	TCHAR *tValue = new TCHAR[1024+1];
	if (tValue == NULL)
	{
		//MYERROR(_T("分配内存错误，错误码：%d"),GetLastError());
		MYERROR(_T("QueryExamineRecorde tValue == NULL"));
		delete[] pRead;
		pRead = NULL;
		//CloseHandle(hPipe);
		return FALSE;
	}

	// 将查询结果写入到缓存
	if (dwRecordNum!=1)
	{
		delete[] pRead;
		pRead = NULL;
		delete []tValue;
		tValue = NULL;
		//CloseHandle(hPipe);
		return FALSE;
	}
	ZeroMemory(tValue,1024+1);
	memcpy(tValue, pByte, pHead->DataLen);
	TCHAR tSerialId[1024];
	memset(tSerialId,0,sizeof(TCHAR)*1024);
	memcpy(tSerialId,tValue,sizeof(TCHAR)*1024);
	SerialId.Format(_T("%s"),tSerialId);

	delete []tValue;
	tValue = NULL;


	delete[] pRead;
	pRead = NULL;
	//CloseHandle(hPipe);

	return TRUE;
}
void CProcessThread::OnCallPrePayRecord(SocketMsgPrepayRecord* pMsgPrepayRecordData)
{
	CTSSampleDataSvrDlg* pDlg = (CTSSampleDataSvrDlg*)AfxGetApp()->m_pMainWnd;
	HANDLE hPipe = NULL;
	BYTE* pRead = NULL;
	short nRtuNo = pMsgPrepayRecordData->btRtuNo;

	nRtuNo = pMsgPrepayRecordData->btForeNo * 256 + pMsgPrepayRecordData->btRtuNo;
	/*if (pMsgPrepayRecordData->btForeNo < ADDR_SOURCEMACHINE && pMsgPrepayRecordData->btForeNo >= 0)
	{
		nRtuNo = pMsgPrepayRecordData->btForeNo * 256 + pMsgPrepayRecordData->btRtuNo;
	}*/
	int nDevNo = pMsgPrepayRecordData->nDevNo;  //前置设备号
	int nDevId = 0;
	pDlg->m_pCRedisRecvSample->GetDevId(nRtuNo,pMsgPrepayRecordData->nDevNo,nDevId);
	if(nDevId==0)
	{
		return;
	}
	SocketPrepayRecordUnit *pRecordUnit = NULL;
	pRecordUnit = pMsgPrepayRecordData->prUnit;
	int icount = 0;
	CString UpdateSql = _T(""); 
	for (WORD i=0; i<pMsgPrepayRecordData->wdYjNum; i++,pRecordUnit++)
	{
		if (NULL==hPipe)
		{
			hPipe = OpenRealDataPipe();
		}
		int iyear = 0, imonth = 0, iday = 0, ihour = 0, iminite = 0, isecond = 0;
		iyear = pRecordUnit->nPercDate[0];
		imonth = pRecordUnit->nPercDate[1];
		iday = pRecordUnit->nPercDate[2];
		ihour = pRecordUnit->nPercDate[3];
		iminite = pRecordUnit->nPercDate[4];
		CString timestr = _T("");
		if (iyear==0||iyear>99||imonth<=0||imonth>12||iday<=0||iday>31)
		{
			timestr = _T("1999-01-01 00:00:00");
			continue;
		}
		else
		{
			timestr.Format(_T("20%02d-%02d-%02d %02d:%02d:00"),iyear,imonth,iday,ihour,iminite);
		}
		int nTotalBuyCount = 0; //上一次充值后的总购电次数
		nTotalBuyCount = pRecordUnit->nTotalPercNum;
		float nBuyFee = 0;  //本次充值金额
		nBuyFee = pRecordUnit->nPercFee*0.01;
		float nBefReminAmount = 0; //本次充值前剩余金额
		float nAftReminAmount = 0; //本次充值后剩余金额
		float nTotalFee = 0;   //总购电金额

		int nBeforeFee = (int)pRecordUnit->nBefPercFee; //GUOJ ADD 151022 
		int nAfterFee = (int)pRecordUnit->nAftPercFee;  //GUOJ ADD 151022 
		nBefReminAmount = nBeforeFee*0.01;
		nAftReminAmount = nAfterFee*0.01;
		nTotalFee = pRecordUnit->nTotalPercFee*0.01;

		if (abs((nAftReminAmount - nBefReminAmount) - nBuyFee) > 1)
		{
			nBefReminAmount = nAftReminAmount - nBuyFee;
			ReguTrace(PrepayRecord,"线程%d:充值前金额为计算得到!充值前=%f,充值后=%f,充值金额=%f",m_Index,nBeforeFee*0.01,nAftReminAmount,nBuyFee);
		}

		CString BillSerial = _T("");
		CString TempSql = _T("");
		int iPlazaId = 0;
		DeviceInfo iDeviceInfo;
		memset(&iDeviceInfo,0,sizeof(DeviceInfo));
		if(!pDlg->m_pCRedisRecvSample->m_DeviceInfoMap.Lookup(nDevId,iDeviceInfo))
		{
			ReguTrace(PrepayRecord,"线程%d:未找到该设备的广场信息DevId=%d",m_Index,nDevId);
		}
		CTime TempTime = CTime::GetCurrentTime();
		CString OperaterTime = _T("");
		OperaterTime.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"),TempTime.GetYear(),TempTime.GetMonth(),
			TempTime.GetDay(),TempTime.GetHour(),TempTime.GetMinute(),TempTime.GetSecond());
		//******************GUOJ ADD 151114 匹配代办*********************
		CTime ChargeTime = CTime(iyear+2000,imonth,iday,ihour,iminite,0);
		if (QueryExamineRecorde(nDevId,nTotalBuyCount,nBuyFee,ChargeTime,BillSerial))
		{
			int RecordType = 0;
			if (BillSerial.Left(2)==_T("ZN"))
			{
				RecordType = 1;
			}
			else if (BillSerial.Left(2)==_T("YJ"))
			{
				RecordType = 2;
			}
			TempSql.Format(_T("IF NOT EXISTS (SELECT ID FROM TE_PAYMENT_PAYRECORD WHERE DEVICEID=%d AND COUNT=%d) BEGIN INSERT INTO TE_PAYMENT_PAYRECORD \
							  (OPERATIONTIME,ACCOUNTCODE,ROOMNAME,DEVICEID,PAYCOUNT,PAPTAKER,BILLSERIAL,RECHARGETYPE,BEFORERECHARGE,AFTERERECHARGE,RECHARGETIME,COUNT,UPMARK,BRANDID,PLAZAID,BUNKCODE) VALUES \
							  ('%s','%s','%s',%d,%02f,%d,'%s',%d,%02f,%02f,'%s',%d,0,%d,%d,'%s') END; "),nDevId,nTotalBuyCount,OperaterTime,iDeviceInfo.BrandName,iDeviceInfo.PlazaName,
							  nDevId,nBuyFee,RecordType,BillSerial,RecordType,nBefReminAmount,nAftReminAmount,timestr,nTotalBuyCount,iDeviceInfo.iBrandId,iDeviceInfo.iPlazaId,iDeviceInfo.NickName);
			CString Sql_Examine = _T("");
			Sql_Examine.Format(_T("UPDATE TB_EMS_EXAMINEACTION SET ISOK = 1 WHERE DetailsId='%s'; "),BillSerial);
			UpdateSql += TempSql;
			UpdateSql += Sql_Examine;
			icount++;
			continue;                 //匹配了代办无需往下走
		}
		
		//******************GUOJ ADD 151114 end**************************
		
		if (TempTime.GetDay()!=pDlg->m_pCRedisRecvSample->m_DayTime.GetDay()||TempTime.GetMonth()!=pDlg->m_pCRedisRecvSample->m_DayTime.GetMonth()||TempTime.GetYear()!=pDlg->m_pCRedisRecvSample->m_DayTime.GetYear())
		{
			EnterCriticalSection(&pDlg->m_pCRedisRecvSample->m_csBillNum);
			pDlg->m_pCRedisRecvSample->BillNum = 0;
			LeaveCriticalSection(&pDlg->m_pCRedisRecvSample->m_csBillNum);

			EnterCriticalSection(&pDlg->m_pCRedisRecvSample->m_csDayTime);
			pDlg->m_pCRedisRecvSample->m_DayTime = CTime(TempTime.GetYear(),TempTime.GetMonth(),TempTime.GetDay(),0,0,0);
			LeaveCriticalSection(&pDlg->m_pCRedisRecvSample->m_csDayTime);
		}
		EnterCriticalSection(&pDlg->m_pCRedisRecvSample->m_csBillNum);
		pDlg->m_pCRedisRecvSample->BillNum++;
		int BillNum = pDlg->m_pCRedisRecvSample->BillNum;
		LeaveCriticalSection(&pDlg->m_pCRedisRecvSample->m_csBillNum);
		BillSerial.Format(_T("YJ20%02d%02d%02d%02d%02d%02d%04d"),iyear,imonth,iday,ihour,iminite,isecond,BillNum);
		TempSql.Format(C_SQL_Insert_PaymentRecord,nDevId,nTotalBuyCount,OperaterTime,iDeviceInfo.BrandName,iDeviceInfo.PlazaName,
			nDevId,nBuyFee,2,BillSerial,2,nBefReminAmount,nAftReminAmount,timestr,nTotalBuyCount,iDeviceInfo.iBrandId,iDeviceInfo.iPlazaId,iDeviceInfo.NickName);
		UpdateSql += TempSql;
		icount++;
		if (icount%SQL_COUNT_ONCE!=0 && i!=pMsgPrepayRecordData->wdYjNum - 1)
		{
			continue;
		}
		CString strSqlFinal = UpdateSql;
		ReguTrace(PrepayRecord,"线程%d:充值记录入库:%s",m_Index,strSqlFinal);
		pRead = (BYTE*)GetMessage_RecordOfSql_Ext(hPipe, strSqlFinal);
		if (pRead)
		{
			delete[] pRead;
			pRead = NULL;
		}
		UpdateSql.Empty();


	}
	if (!UpdateSql.IsEmpty())
	{
		ReguTrace(PrepayRecord,"线程%d:充值记录入库::%s",m_Index,UpdateSql);
		pRead = (BYTE*)GetMessage_RecordOfSql_Ext(hPipe, UpdateSql);
		if (pRead)
		{
			delete[] pRead;
			pRead = NULL;
		}
		UpdateSql.Empty();
	}
	if (hPipe != NULL)
	{
		CloseHandle(hPipe);
		hPipe = NULL;
	}
}

void CProcessThread::OnSocketMsgGroupYx(SocketMsgGroupYxEx* pMsgGroupYxData)
{
	CTSSampleDataSvrDlg* pDlg = (CTSSampleDataSvrDlg*)AfxGetApp()->m_pMainWnd;
	HANDLE hPipe = NULL;
	BYTE* pRead = NULL;
	short nRtuNo = pMsgGroupYxData->btRtuNo;

	nRtuNo = pMsgGroupYxData->btForeNo * 256 + pMsgGroupYxData->btRtuNo;
	/*if (pMsgGroupYxData->btForeNo < ADDR_SOURCEMACHINE && pMsgGroupYxData->btForeNo >= 0)
	{
		nRtuNo = pMsgGroupYxData->btForeNo * 256 + pMsgGroupYxData->btRtuNo;
	}*/
	SocketGroupYxUnit *YxUnit = NULL;
	YxUnit = pMsgGroupYxData->YxUnit;
	int icount = 0;
	m_YxValueInfoCArray.RemoveAll();
	m_YmValueInfoCArray.RemoveAll();
	CString OutputStr = _T("");
	OutputStr.Format(_T("解析遥信:厂站号%d,"),nRtuNo);
	CTime CurTime = CTime::GetCurrentTime();
    for (WORD i=0; i<pMsgGroupYxData->wdYxNum; i++,YxUnit++)
    {
		int DevId = 0;
		pDlg->m_pCRedisRecvSample->GetDevId(nRtuNo,YxUnit->DevId,DevId);
		if (DevId==0)
		{
			continue;
		}
		/*YmYxData*pYmYxData = new YmYxData;
		YxConfigDef*pYxDefData = new YxConfigDef;
		memset(pYxDefData,0,sizeof(YxConfigDef));
		pYxDefData->nYxIndex = 100*2000*nRtuNo+YxUnit->DevId*100+YxUnit->Id;
		pYxDefData->bYxValue = YxUnit->Value;
		pYxDefData->bYxRaw = YxUnit->Value;
		pYmYxData->iType = 2;
		pYmYxData->data = pYxDefData;
		EnterCriticalSection(&pDlg->m_pCUpdateRTDBThread->m_csYxYmDataList);
		pDlg->m_pCUpdateRTDBThread->m_YxYmDataList.AddTail(pYmYxData);
		LeaveCriticalSection(&pDlg->m_pCUpdateRTDBThread->m_csYxYmDataList);*/

		YxConfigDef iYxConfig;
		memset(&iYxConfig,0,sizeof(YxConfigDef));
		iYxConfig.nYxIndex = 100*2000*nRtuNo+YxUnit->DevId*100+YxUnit->Id;
		iYxConfig.bYxValue = YxUnit->Value;
		iYxConfig.bYxRaw = YxUnit->Value;
		//m_YxValueInfoCArray.Add(iYxConfig);  //GUOJ DEL 151218
		CString temstr = _T("");
		temstr.Format(_T("YxIndex:%d_value:%d&&"),iYxConfig.nYxIndex,YxUnit->Value);
		OutputStr+=temstr;

		//GUOJ ADD 151030
		BYTE lastState = 0;
		if (pDlg->m_pCRedisRecvSample->m_DevStateMap.Lookup(iYxConfig.nYxIndex,lastState)) //GUOJ MOD 151218
		{
			if (iYxConfig.bYxValue!=lastState)
			{
				if (YxUnit->Id==0)
				{
					AlarmInfo *pAlarmInfo = new AlarmInfo;
					if(NULL==pAlarmInfo)
					{
						m_YxValueInfoCArray.Add(iYxConfig);
						EnterCriticalSection(&pDlg->m_pCRedisRecvSample->m_csDevStateMap);
						pDlg->m_pCRedisRecvSample->m_DevStateMap.SetAt(iYxConfig.nYxIndex,iYxConfig.bYxValue); //GUOJ MOD 151218
						LeaveCriticalSection(&pDlg->m_pCRedisRecvSample->m_csDevStateMap);
						continue;
					}
					memset(pAlarmInfo,0,sizeof(AlarmInfo));
					pAlarmInfo->AlarmType = 9003;	//9003为电表通讯中断
					pAlarmInfo->ObjectId = DevId;
					pAlarmInfo->ObjectType = 31;
					pAlarmInfo->reserve = nRtuNo;
					pAlarmInfo->AlarmTime = CurTime;
					if (iYxConfig.bYxValue==0)	//通讯中断 产生告警
					{
						pAlarmInfo->Status = 0;
						CString strContents = _T("设备与集中器通讯中断，产生告警!");
						memcpy(pAlarmInfo->Contents,strContents.GetBuffer(),sizeof(pAlarmInfo->Contents));
					}
					else if (iYxConfig.bYxValue==1)	//通讯恢复 恢复告警
					{
						pAlarmInfo->Status = 2;
						CString strContents = _T("设备与集中器通讯恢复，恢复告警!");
						memcpy(pAlarmInfo->Contents,strContents.GetBuffer(),sizeof(pAlarmInfo->Contents));
					}

					EnterCriticalSection(&pDlg->m_pCCSendP2pTask->m_csAlarmInfoList);
					pDlg->m_pCCSendP2pTask->m_AlarmInfoList.AddTail(pAlarmInfo);
					LeaveCriticalSection(&pDlg->m_pCCSendP2pTask->m_csAlarmInfoList);
				}
				
				m_YxValueInfoCArray.Add(iYxConfig);
				EnterCriticalSection(&pDlg->m_pCRedisRecvSample->m_csDevStateMap);
				pDlg->m_pCRedisRecvSample->m_DevStateMap.SetAt(iYxConfig.nYxIndex,iYxConfig.bYxValue); //GUOJ MOD 151218
				LeaveCriticalSection(&pDlg->m_pCRedisRecvSample->m_csDevStateMap);

				
			} 
		}
		else
		{
			ReguTrace(Debug,"线程%d:未找到该点位:%d",m_Index,iYxConfig.nYxIndex);
		}
		//GUOJ ADD 151030 end

		//if (NULL==hPipe)
		//{
		//	hPipe = OpenServerPipe();
		//}
		//CString TempSql = _T("");
		//TempSql.Format(_T("UPDATE TB_DI SET YXVALUE=%d WHERE DEVICENUM=%d AND NUM=%d; "),YxUnit->Value,DevId,YxUnit->Id);  //TB_DI表中的DEVICENUM为设备表第一列
		//UpdateSql += TempSql;
		//icount++;
		//if (icount%SQL_COUNT_ONCE!=0 && i!=pMsgGroupYxData->wdYxNum - 1)
		//{
		//	continue;
		//}
		//CString strSqlFinal = UpdateSql;
		//pRead = (BYTE*)GetMessage_RecordOfSql_Ext(hPipe, strSqlFinal);
		//if (pRead)
		//{
		//	delete[] pRead;
		//	pRead = NULL;
		//}
		//UpdateSql.Empty();

    }
	ReguTrace(YXDATA,"线程%d:%s",m_Index,OutputStr);
	if (pDlg->m_pCCSendP2pTask->m_AlarmInfoList.GetSize()>0)
	{
		ReguTrace(YXDATA,"线程%d:触发告警",m_Index);
		SetEvent(pDlg->m_pCCSendP2pTask->m_AlarmEvent);
	}
	//SetEvent(pDlg->m_pCUpdateRTDBThread->m_ProcessDataEvent);
	if (m_YxValueInfoCArray.GetSize()>0)
	{
		ReguTrace(Debug,"线程%d:Update2RTDB 开始!更新点位个数%d。",m_Index,m_YxValueInfoCArray.GetSize());
		Update2RTDB();
		ReguTrace(Debug,"线程%d:Update2RTDB 结束!",m_Index);
	}
	if (hPipe != NULL)
	{
		CloseHandle(hPipe);
		hPipe = NULL;
	}
}


void CProcessThread::StartProcessData(SocketMsgGroupYmDataEx_DATE *pSocketMsgYmData)
{
	short nRtuNo = pSocketMsgYmData->btRtuNo;
	int SampleTableNo = TABLE_NO_SAMPLEHOUR;
	int iyear = 0, imonth = 0, iday = 0, ihour = 0, iminite = 0, isecond = 0;
	iyear = pSocketMsgYmData->bYear;
	imonth = pSocketMsgYmData->bMonth;
	iday = pSocketMsgYmData->bDay;
	ihour = pSocketMsgYmData->bHour;
	iminite = pSocketMsgYmData->bMinite;
	CString timestr = _T("");
	if (imonth>12||imonth<=0||iday>31||iday<=0||ihour>=24||ihour<0||iminite>=60||iminite<0)
	{
		ReguTrace(ERRO,"Time ERRO! iyear=%d,imonth=%d,iday=%d,ihour=%d,iminite=%d",iyear,imonth,iday,ihour,iminite);
		return;
	}
	CTime dataTime = CTime(2000+iyear,imonth,iday,ihour,iminite,0);
	if(iminite>0&&iminite<=30)
	{
		iminite = 0;
		dataTime = CTime(2000+iyear,imonth,iday,ihour,iminite,0);
	}
	else if (iminite>30&&iminite<=59)
	{
		dataTime = dataTime + CTimeSpan(0,0,60-iminite,0);
	}
	//timestr.Format(_T("20%02d-%02d-%02d %02d:%02d:00"),iyear,imonth,iday,ihour,iminite);
	timestr.Format(_T("%04d-%02d-%02d %02d:%02d:00"),dataTime.GetYear(),
		dataTime.GetMonth(),dataTime.GetDay(),dataTime.GetHour(),dataTime.GetMinute());
	nRtuNo = pSocketMsgYmData->btForeNo * 256 + pSocketMsgYmData->btRtuNo;
	/*if (pSocketMsgYmData->btForeNo < ADDR_SOURCEMACHINE && pSocketMsgYmData->btForeNo >= 0)
	{
		nRtuNo = pSocketMsgYmData->btForeNo * 256 + pSocketMsgYmData->btRtuNo;
	}*/
	int YmNum = pSocketMsgYmData->wdYmNum;
	TIMESTAMP_STRUCT iTimeStamp;
	memset(&iTimeStamp,0,sizeof(TIMESTAMP_STRUCT));
	iTimeStamp.year = dataTime.GetYear();
	iTimeStamp.month = dataTime.GetMonth();
	iTimeStamp.day = dataTime.GetDay();
	iTimeStamp.hour = dataTime.GetHour();
	iTimeStamp.minute = dataTime.GetMinute();
	unsigned char* pbYmData = NULL;
	int nLengthYmStruct = 0;

	SocketGroupYmUnitEx* pSocketYmDataEx = pSocketMsgYmData->YmUnitEx;
	if (NULL == pSocketYmDataEx)	
	{
		return;
	}
	else	//带标志位的脉冲量
	{
		pbYmData = (unsigned char*)pSocketYmDataEx;
		nLengthYmStruct = sizeof(SocketGroupYmUnitEx);
	}
	int nLengthWord = sizeof(WORD);
	int nLengthDWord = sizeof(DWORD);
	int MinSampleNo = 0;
	int MaxSampleNo = 0;
	int MinSampleNoDay = 0;
	int MaxSampleNoDay = 0;
	int icount = 0;
	int icountDay = 0;
	m_SampleValueInfoMap.clear();
	m_SampleValueInfoMapDay.clear();
	m_YmValueInfoCArray.RemoveAll();
	m_YxValueInfoCArray.RemoveAll();
	BOOL DataFlag[9] = {};
	memset(DataFlag,0,sizeof(DataFlag));
	CTime CurTime = CTime::GetCurrentTime();
	CTSSampleDataSvrDlg* pDlg = (CTSSampleDataSvrDlg*)AfxGetApp()->m_pMainWnd;
	for (int i=0; i<YmNum; i++,pbYmData += nLengthYmStruct) 
	{
		unsigned char cFlag = DATA_FLAG_INVALID;        
		cFlag = *(pbYmData + sizeof(SocketGroupYmUnit));   //标志位

		PowFileInfo powInfo;
		memset(&powInfo, 0, sizeof(PowFileInfo));
		powInfo.nDevID = *((WORD*)pbYmData);                  //前置设备号
		powInfo.nYmNum = *((WORD*)(pbYmData + nLengthWord));   //点位号
		powInfo.nYmVal = (DWORD) *((DWORD*)(pbYmData + nLengthWord*2));  //原始值
		powInfo.cFlag = cFlag;
		if (powInfo.nYmNum>0&&powInfo.nYmNum<9)
		{
			DataFlag[powInfo.nYmNum] = TRUE;
		}
		
		//YmConfigDef YmDef;
		SimpleYmDef YmDef;
		int iDevId = 0 ;
		if(!pDlg->m_pCRedisRecvSample->GetYmDefByPowInfo(nRtuNo,&powInfo,YmDef,iDevId))
		{
			ReguTrace(ERRO,"线程%d:GetYmDef FAILED! StationId=%d,DevNum=%d,Num=%d.",m_Index,nRtuNo,powInfo.nDevID,powInfo.nYmNum);
			continue;
		}		
		CTime iLastDataTime = CTime(YmDef.DataTime.year,YmDef.DataTime.month,YmDef.DataTime.day,YmDef.DataTime.hour,YmDef.DataTime.minute,YmDef.DataTime.second);
		CTimeSpan TimeSpan = dataTime - iLastDataTime;
		DeviceInfo iDevInfo;
		memset(&iDevInfo,0,sizeof(DeviceInfo));
		pDlg->m_pCRedisRecvSample->m_DeviceInfoMap.Lookup(iDevId,iDevInfo);
		if (iDevInfo.iDevType==52)	//国网的表
		{
			if(TimeSpan.GetHours()>9)
			{
				CString ff = _T("");
				ff.Format(_T("%04d-%02d-%02d %02d:%02d:00"),iLastDataTime.GetYear(),iLastDataTime.GetMonth(),
					iLastDataTime.GetDay(),iLastDataTime.GetHour(),iLastDataTime.GetMinute());
				ReguTrace_ext(DataTime_log,ERRO,"设备id:%d(%d),点位号:%d,上次更新时间%s,本次更新时间%s",
					iDevId,iDevInfo.iDevType,powInfo.nYmNum,ff,timestr);
			}
		}
		else if (iDevInfo.iDevType==9||iDevInfo.iDevType==10)	//天溯仪表
		{
			if(TimeSpan.GetHours()>4)
			{
				CString ff = _T("");
				ff.Format(_T("%04d-%02d-%02d %02d:%02d:00"),iLastDataTime.GetYear(),iLastDataTime.GetMonth(),
					iLastDataTime.GetDay(),iLastDataTime.GetHour(),iLastDataTime.GetMinute());
				ReguTrace_ext(DataTime_log,ERRO,"设备id:%d(%d),点位号:%d,上次更新时间%s,本次更新时间%s",
					iDevId,iDevInfo.iDevType,powInfo.nYmNum,ff,timestr);
			}
		}
		if (powInfo.nYmNum==2&&powInfo.cFlag==1)	//GUOJ ADD 151117 更新DI的通信状态为1
		{
			/*YmYxData*pYmYxData = new YmYxData;
			YxConfigDef*pYxDefData = new YxConfigDef;*/
			YxConfigDef iYxConfig;
			memset(&iYxConfig,0,sizeof(YxConfigDef));
			iYxConfig.nYxIndex = 100*2000*nRtuNo + powInfo.nDevID*100 + 0;
			iYxConfig.bYxValue = 1;
			iYxConfig.bYxRaw = 1;
			/*pYmYxData->iType = 2;
			pYmYxData->data = pYxDefData;
			EnterCriticalSection(&pDlg->m_pCUpdateRTDBThread->m_csYxYmDataList);
			pDlg->m_pCUpdateRTDBThread->m_YxYmDataList.AddTail(pYmYxData);
			LeaveCriticalSection(&pDlg->m_pCUpdateRTDBThread->m_csYxYmDataList);*/

			m_YxValueInfoCArray.Add(iYxConfig); 
			EnterCriticalSection(&pDlg->m_pCRedisRecvSample->m_csDevStateMap);
			pDlg->m_pCRedisRecvSample->m_DevStateMap.SetAt(iYxConfig.nYxIndex,iYxConfig.bYxValue); 
			LeaveCriticalSection(&pDlg->m_pCRedisRecvSample->m_csDevStateMap);
			BYTE lastState = 0;
			//if (pDlg->m_pCRedisRecvSample->m_DevStateMap.Lookup(iYxConfig.nYxIndex,lastState)) //GUOJ MOD 151218
			//{
				//if (iYxConfig.bYxValue!=lastState)
				//{
					//AlarmInfo *pAlarmInfo = new AlarmInfo;
					//memset(pAlarmInfo,0,sizeof(AlarmInfo));
					//pAlarmInfo->AlarmType = 9003;	//9003为电表通讯中断
					//pAlarmInfo->ObjectId = iDevId;
					//pAlarmInfo->ObjectType = 31;
					//pAlarmInfo->reserve = nRtuNo;
					//pAlarmInfo->AlarmTime = CurTime;
					//if (iYxConfig.bYxValue==0)	//通讯中断 产生告警
					//{
					//	pAlarmInfo->Status = 0;
					//	CString strContents = _T("设备与集中器通讯中断，产生告警!");
					//	memcpy(pAlarmInfo->Contents,strContents,sizeof(pAlarmInfo->Contents));
					//}
					//else if (iYxConfig.bYxValue==1)	//通讯恢复 恢复告警
					//{
					//	pAlarmInfo->Status = 2;
					//	CString strContents = _T("设备与集中器通讯恢复，恢复告警!");
					//	memcpy(pAlarmInfo->Contents,strContents,sizeof(pAlarmInfo->Contents));
					//}
					//m_YxValueInfoCArray.Add(iYxConfig);  //GUOJ ADD 151218
					//EnterCriticalSection(&pDlg->m_pCRedisRecvSample->m_csDevStateMap);
					//pDlg->m_pCRedisRecvSample->m_DevStateMap.SetAt(iYxConfig.nYxIndex,iYxConfig.bYxValue);  //GUOJ MOD 151218
					//LeaveCriticalSection(&pDlg->m_pCRedisRecvSample->m_csDevStateMap);

					/*EnterCriticalSection(&pDlg->m_pCCSendP2pTask->m_csAlarmInfoList);
					pDlg->m_pCCSendP2pTask->m_AlarmInfoList.AddTail(pAlarmInfo);
					LeaveCriticalSection(&pDlg->m_pCCSendP2pTask->m_csAlarmInfoList);*/
				//} 
			//}
			
		}  //GUOJ ADD 151117

		CString strKey = _T("");
		strKey.Format(_T("%d+%d+%d"),SampleTableNo, TABLE_NO_PULSE, YmDef.nYmIndex);
		SampleDayNo::iterator iSampleInfo;	
		iSampleInfo = pDlg->m_pCRedisRecvSample->m_MapSampleNo.find(strKey);
		if (iSampleInfo == pDlg->m_pCRedisRecvSample->m_MapSampleNo.end())
		{
			ReguTrace(ERRO,"线程%d:m_MapSampleNo 无此配置 strKey='%s'!",m_Index,strKey);
			continue;
		}
		icount++;
		int SampleNo = iSampleInfo->second;
		if (icount==1)
		{
			MinSampleNo = SampleNo;
			MaxSampleNo = SampleNo;
		}
		else 
		{
			if (MinSampleNo>SampleNo)
			{
				MinSampleNo = SampleNo;
			}
			if (MaxSampleNo<SampleNo)
			{
				MaxSampleNo = SampleNo;
			}	
		}
		double dbYm = 0.0;
		dbYm = powInfo.nYmVal * YmDef.dYmQuotiety;

		SampleValueInfo iSampleValue;
		iSampleValue.SampleNo = SampleNo;
		iSampleValue.dbValue = dbYm;
		iSampleValue.cFlag = cFlag;
		m_SampleValueInfoMap[SampleNo] = iSampleValue;

		//把购电次数加入列表  用于检查是否招充值记录
		if (powInfo.nYmNum==7&&powInfo.cFlag==1)
		{
			if (YmDef.nYmRaw<powInfo.nYmVal)  //购电次数大于实时库中的购电次数
			{
				ProtocolCallItem *pItem = new ProtocolCallItem;
				if (NULL==pItem)
				{
					continue;
				}
				memset(pItem,0,sizeof(ProtocolCallItem));
				pItem->m_nType = 0xE1;//MSW 150828,加注释，0xE1-召唤充值记录的标志
				pItem->m_nDeviceNo = YmDef.nDeviceNo;    //设备表第一列 Id
				pItem->m_nStationNum = nRtuNo;

				SocketMsgPrepayRecord iPrepayRecord ;
				memset(&iPrepayRecord,0,sizeof(SocketMsgPrepayRecord));
				iPrepayRecord.btRtuNo = pSocketMsgYmData->btRtuNo;     //==m_nStationNum
				iPrepayRecord.btEndNo = 0xff;
				iPrepayRecord.btForeNo = pSocketMsgYmData->btForeNo;
				iPrepayRecord.nDevNo = powInfo.nDevID;    //设备表第二列 deviceNo
				iPrepayRecord.wdYjNum = powInfo.nYmVal - YmDef.nYmRaw;
				memcpy(pItem->m_nBuffer,&iPrepayRecord,sizeof(SocketMsgPrepayRecord)-sizeof(SocketPrepayRecordUnit));

				EnterCriticalSection(&pDlg->m_pCCSendP2pTask->m_csRecordList);
				pDlg->m_pCCSendP2pTask->m_RecordList.AddTail(pItem);
				LeaveCriticalSection(&pDlg->m_pCCSendP2pTask->m_csRecordList);
			}
		}
		

		if (cFlag==1)  //更新至m_YmConfigDefMap里
		{
			YmDef.nYmRaw = powInfo.nYmVal;
			YmDef.dYmValue = powInfo.nYmVal*YmDef.dYmQuotiety;
			YmDef.DataTime = iTimeStamp;
			YmYxData*pYmYxData = new YmYxData;
			YmConfigDef*pYmDefData = new YmConfigDef;

			//YmConfigDef iYmConfig;
			memset(pYmDefData,0,sizeof(YmConfigDef));
			pYmDefData->nYmIndex = YmDef.nYmIndex;
			pYmDefData->nDeviceNo = YmDef.nDeviceNo;
			pYmDefData->nYmNum = YmDef.nYmNum;
			pYmDefData->dYmQuotiety = YmDef.dYmQuotiety;
			pYmDefData->nYmRaw = YmDef.nYmRaw;
			pYmDefData->dYmValue = YmDef.dYmValue;
			//m_YmValueInfoCArray.Add(iYmConfig);
			pYmYxData->iType = 1;
			pYmYxData->data = pYmDefData;
			EnterCriticalSection(&pDlg->m_pCUpdateRTDBThread->m_csYxYmDataList);
			pDlg->m_pCUpdateRTDBThread->m_YxYmDataList.AddTail(pYmYxData);
			LeaveCriticalSection(&pDlg->m_pCUpdateRTDBThread->m_csYxYmDataList);
			CString tempKey = _T("");
			tempKey.Format(_T("%d+%d"), YmDef.nDeviceNo, YmDef.nYmNum);	//key为“设备ID+遥脉序号”
			EnterCriticalSection(&pDlg->m_pCRedisRecvSample->m_csYmConfigDefMap);
			pDlg->m_pCRedisRecvSample->m_YmConfigDefMap[tempKey] = YmDef;
			LeaveCriticalSection(&pDlg->m_pCRedisRecvSample->m_csYmConfigDefMap);
		}
		if (ihour==0&&iminite==0)
		{
			strKey = _T("");
			strKey.Format(_T("%d+%d+%d"),TABLE_NO_SAMPLEDAY, TABLE_NO_PULSE, YmDef.nYmIndex);
			SampleDayNo::iterator iSampleInfo;	
			iSampleInfo = pDlg->m_pCRedisRecvSample->m_MapSampleNo.find(strKey);
			if (iSampleInfo == pDlg->m_pCRedisRecvSample->m_MapSampleNo.end())
			{
				ReguTrace(ERRO,"线程%d:m_MapSampleNo 无此配置 strKey='%s'!",m_Index,strKey);
				continue;
			}
			icountDay++;
			int SampleNoDay = iSampleInfo->second;
			if (icountDay==1)
			{
				MinSampleNoDay = SampleNoDay;
				MaxSampleNoDay = SampleNoDay;
			}
			else 
			{
				if (MinSampleNoDay>SampleNoDay)
				{
					MinSampleNoDay = SampleNoDay;
				}
				if (MaxSampleNoDay<SampleNoDay)
				{
					MaxSampleNoDay = SampleNoDay;
				}	
			}
			SampleValueInfo iSampleValueDay;
			iSampleValueDay.SampleNo = SampleNoDay;
			iSampleValueDay.dbValue = dbYm;
			iSampleValueDay.cFlag = cFlag;
			m_SampleValueInfoMapDay[SampleNoDay] = iSampleValueDay;
		}
	}

	if (pDlg->m_pCCSendP2pTask->m_RecordList.GetSize()>0)
	{
		SetEvent(pDlg->m_pCCSendP2pTask->m_ProcessDataEvent);
	}
	SetEvent(pDlg->m_pCUpdateRTDBThread->m_ProcessDataEvent);
	if (m_YxValueInfoCArray.GetSize()>0)
	{
		//ReguTrace(Debug,"线程%d:Update2RTDB 开始!",m_Index);
		Update2RTDB();
		//ReguTrace(Debug,"线程%d:Update2RTDB 结束!",m_Index);
	}
	if (m_SampleValueInfoMap.size()>0)
	{
		if (!(DataFlag[2]==TRUE&&DataFlag[3]==TRUE&&DataFlag[4]==TRUE&&DataFlag[5]==TRUE&&DataFlag[6]==TRUE))
		{
			SaveSampleData2DB(MinSampleNo,MaxSampleNo,timestr,0);
		}
		
	}
	if (m_SampleValueInfoMapDay.size()>0)
	{
		SaveSampleData2DB(MinSampleNoDay,MaxSampleNoDay,timestr,1);
	}
}

void CProcessThread::SaveSampleData2DB(int MinSampleNo, int MaxSampleNo,CString StrTimeId,int itype)
{
	CTSSampleDataSvrDlg* pDlg = (CTSSampleDataSvrDlg*)AfxGetApp()->m_pMainWnd;
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
	SYSTEMTIME tt;
	CString sFileName = _T("");

	GetLocalTime(&tt);
	sFileName.Format(_T("%04d-%02d-%02d"), tt.wYear, tt.wMonth, tt.wDay);
	CString sPath = pDlg->m_pCRedisRecvSample->m_strPath + _T("\\data_sql\\") + sFileName;

	CFileFind fFind;
	if (!fFind.FindFile(sPath))
	{
		ReguTrace(ERRO,"线程%d:未找到该路径,path=%s!",m_Index,sPath);
		if (!CreateDirectory(sPath, NULL))
		{
			ReguTrace(ERRO,"线程%d:创建文件夹出错,path=%s!",m_Index,sPath);
			return ;
		}
	}
	sFileName.Format(_T("%d_%04d-%02d-%02d %02d-%02d-%02d-%03d"), m_Index,tt.wYear, tt.wMonth, tt.wDay, tt.wHour, tt.wMinute, tt.wSecond, tt.wMilliseconds);
	sPath += _T("\\SampleData_") + sFileName + _T(".txt");

	

	int TableMinNo = MinSampleNo/SAMPLE_FIELDNUM ;
	int TableMaxNo = MaxSampleNo/SAMPLE_FIELDNUM ;
	if(MinSampleNo%SAMPLE_FIELDNUM==0&&TableMinNo!=0)		//GUOJ ADD 151228 防止最小采样号为128的倍数的时候 这个点是存在										
	{										//上一张表的最后一个字段的，即V128，所以要减1
		TableMinNo = TableMinNo - 1;
	}
	CString PreTableName = _T("TS_H1_%03d");
	CString PreTableExtName = _T("TS_H1_EXT_%03d");
	if(itype==1)
	{
		PreTableName = _T("TS_DAY_%03d");
		PreTableExtName = _T("TS_DAY_EXT_%03d");
	}
	CString strnSql = _T("");
	int nCount = 0;
	for (int i = TableMinNo; i<=TableMaxNo;i++)
	{
		CString tableName = _T("");
		tableName.Format(PreTableName,i);
		CString tableNameExt = _T("");
		tableNameExt.Format(PreTableExtName,i);
		CString strSql = _T("");
		CString strSql_Ext = _T("");
		CString ColonmNames = _T("TIMEID");
		CString ColonNames_Ext = _T("TIMEID");
		CString strVals = _T("");
		CString strValsflag = _T("");
		CString UpdateVals = _T("");
		CString UpdateVals_Ext = _T("");
		for (int j = 1;j<=SAMPLE_FIELDNUM;j++)
		{
			int sampleNo = i*SAMPLE_FIELDNUM + j;
			if (itype==0)  //小时采样
			{
				SampleValueInfoMap::iterator its;	
				its = m_SampleValueInfoMap.find(sampleNo);
				if (its != m_SampleValueInfoMap.end())
				{
					SampleValueInfo iSampleValue = its->second;
					TCHAR Text[5];
					TCHAR TextExt[10];
					TCHAR ValueText[40];
					TCHAR ValueTextflag[40];
					TCHAR UpdateText[40];
					TCHAR UpdateTextExt[40];
					_stprintf(Text,_T(",V%03d"), j);
					ColonmNames += Text;
					_stprintf(TextExt,_T(",V%03d_FLAG"), j);
					ColonNames_Ext += TextExt;
					_stprintf(ValueText,_T(",%2f"),iSampleValue.dbValue);
					strVals += ValueText;
					_stprintf(ValueTextflag,_T(",%d"),iSampleValue.cFlag);
					strValsflag += ValueTextflag;
					_stprintf(UpdateText,_T("V%03d=%2f,"),j,iSampleValue.dbValue);
					UpdateVals += UpdateText;
					_stprintf(UpdateTextExt,_T("V%03d_FLAG=%d,"),j,iSampleValue.cFlag);
					UpdateVals_Ext += UpdateTextExt;
				}

			}
			else if (itype==1)  //天采样
			{
				SampleValueInfoMap::iterator its;	
				its = m_SampleValueInfoMapDay.find(sampleNo);
				if (its != m_SampleValueInfoMapDay.end())
				{
					/*SampleValueInfo iSampleValue = its->second;
					TCHAR Text[5];
					TCHAR ValueText[40];
					TCHAR UpdateText[40];
					_stprintf(Text,_T(",V%03d"), j);
					ColonmNames += Text;
					_stprintf(ValueText,_T(",%2f"),iSampleValue.dbValue);
					strVals += ValueText;
					_stprintf(UpdateText,_T("V%03d=%2f,"),j,iSampleValue.dbValue);
					UpdateVals += UpdateText;*/
					SampleValueInfo iSampleValue = its->second;
					TCHAR Text[5];
					TCHAR TextExt[10];
					TCHAR ValueText[40];
					TCHAR ValueTextflag[40];
					TCHAR UpdateText[40];
					TCHAR UpdateTextExt[40];
					_stprintf(Text,_T(",V%03d"), j);
					ColonmNames += Text;
					_stprintf(TextExt,_T(",V%03d_FLAG"), j);
					ColonNames_Ext += TextExt;
					_stprintf(ValueText,_T(",%2f"),iSampleValue.dbValue);
					strVals += ValueText;
					_stprintf(ValueTextflag,_T(",%d"),iSampleValue.cFlag);
					strValsflag += ValueTextflag;
					_stprintf(UpdateText,_T("V%03d=%2f,"),j,iSampleValue.dbValue);
					UpdateVals += UpdateText;
					_stprintf(UpdateTextExt,_T("V%03d_FLAG=%d,"),j,iSampleValue.cFlag);
					UpdateVals_Ext += UpdateTextExt;
				}
			}

		}
		if (UpdateVals.IsEmpty())
		{
			if (!strnSql.IsEmpty())
			{
				CString strSqlFinal1 = strnSql;
				ReguTrace(SQL,"线程%d:SQL:<%s>",m_Index,strSqlFinal1);
				CStdioFile fLog; 
				if(fLog.Open(sPath, CFile::modeCreate|CFile::shareDenyNone | CFile::modeNoTruncate | CFile::modeReadWrite))
				{
					fLog.SeekToEnd();
					fLog.WriteString(strSqlFinal1);
					fLog.Close();
				}
				else
				{
					ReguTrace(SQL,"线程%d:SQL:写文件失败",m_Index);
					BYTE* pRead = NULL;
					pRead = (BYTE*)GetMessage_RecordOfSql_Ext(hPipe, strSqlFinal1);
					if (pRead)
					{
						delete[] pRead;
						pRead = NULL;
					}
				}
				/*BYTE* pRead = NULL;
				pRead = (BYTE*)GetMessage_RecordOfSql_Ext(hPipe, strSqlFinal1);
				if (pRead)
				{
					delete[] pRead;
					pRead = NULL;
				}*/
				strnSql.Empty();
			}
			continue;
		}
		UpdateVals = UpdateVals.Left(UpdateVals.GetLength()-1);
		strSql.Format(C_SQL_Insert_SampleTable,tableName,StrTimeId,tableName,UpdateVals,StrTimeId,tableName,ColonmNames,StrTimeId,strVals);
		strSql_Ext.Format(C_SQL_Insert_SampleTable,tableNameExt,StrTimeId,tableNameExt,UpdateVals_Ext,StrTimeId,tableNameExt,ColonNames_Ext,StrTimeId,strValsflag);
		//strSql.Format(_T("if insert into %s (%s) values ('%s'%s);"),tableName,ColonmNames,StrTimeId,strVals);
		//strnSql += (strSql + strSql_Ext);
		strnSql += strSql;
		nCount++;
		if (nCount%SQL_COUNT_ONCE!=0 && i!=TableMaxNo)
		{
			continue;
		}
		CString strSqlFinal = strnSql;
		ReguTrace(SQL,"线程%d:SQL:<%s>",m_Index,strSqlFinal);
		CStdioFile fLog; 
		if(fLog.Open(sPath, CFile::modeCreate|CFile::shareDenyNone | CFile::modeNoTruncate | CFile::modeReadWrite))
		{
			fLog.SeekToEnd();
			fLog.WriteString(strSqlFinal);
			fLog.Close();
		}
		else
		{
			ReguTrace(SQL,"线程%d:SQL:写文件失败",m_Index);
			BYTE* pRead = NULL;
			pRead = (BYTE*)GetMessage_RecordOfSql_Ext(hPipe, strSqlFinal);
			if (pRead)
			{
				delete[] pRead;
				pRead = NULL;
			}
		}
		/*BYTE* pRead = NULL;
		pRead = (BYTE*)GetMessage_RecordOfSql_Ext(hPipe, strSqlFinal);
		if (pRead)
		{
			delete []pRead;
			pRead = NULL;
		}*/
		strnSql.Empty();
	}
	if (hPipe != NULL)
	{
		CloseHandle(hPipe);
		hPipe = NULL;
	}
}

//更新至实时库
BOOL CProcessThread::Update2RTDB()
{
	CTSSampleDataSvrDlg* pDlg = (CTSSampleDataSvrDlg*)AfxGetApp()->m_pMainWnd;
	HANDLE hPipe=NULL;	 
	hPipe = OpenRealDataPipe();
	if (hPipe==NULL)
		return FALSE;
	for (int i=0; i<m_YmValueInfoCArray.GetSize();i++)
	{
		YmConfigDef iYmDef;
		iYmDef = m_YmValueInfoCArray[i];
		if (iYmDef.nYmNum!=7)
		{
			continue;
		}
		BYTE *pData;
		int nRecordLen = sizeof(YmConfigDef);
		DWORD dwBufLen = nRecordLen + sizeof(UpdataRecordStruct) + sizeof(NetMessageHead);
		BYTE* pWrite = new BYTE[dwBufLen];
		if (NULL==pWrite)
		{
			ReguTrace(ERRO,"YmValue pWrite==null!");
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

		pUpdataStruct->TableNo = TABLE_NO_PULSE;	
		memset(&(pUpdataStruct->ColFlag[0]),0,256);
		//GUOJ DEL 151117	防止把pulse表其他字段都更新为空值
		pUpdataStruct->ColFlag[7] = 1;
		pUpdataStruct->ColFlag[9] = 1;
		/*for (int i=0; i<11; i++) {
			pUpdataStruct->ColFlag[i] = 1;
		}*/

		pUpdataStruct->UpdataType = UPDATARECORD_TYPE_UPDATA; 
		pUpdataStruct->RecordLen = nRecordLen;

		BYTE *pData1;
		pData = new BYTE[nRecordLen];
		if (NULL==pData)
		{
			ReguTrace(ERRO,"YmValue pData==null!");
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

		memcpy(pData,&iYmDef.nYmIndex,sizeof(iYmDef.nYmIndex));
		nOffset+=sizeof(iYmDef.nYmIndex);

		memcpy((pData+nOffset),&iYmDef.nProjectNo,sizeof(iYmDef.nProjectNo));
		nOffset+=sizeof(iYmDef.nProjectNo);

		memcpy((pData+nOffset),&iYmDef.nStationNo,sizeof(iYmDef.nStationNo));
		nOffset+=sizeof(iYmDef.nStationNo);

		memcpy((pData+nOffset),&iYmDef.nDeviceNo,sizeof(iYmDef.nDeviceNo));
		nOffset+=sizeof(iYmDef.nDeviceNo);

		memcpy((pData+nOffset),&iYmDef.nYmNum,sizeof(iYmDef.nYmNum));
		nOffset+=sizeof(iYmDef.nYmNum);

		memcpy((pData+nOffset),iYmDef.cYmName,sizeof(iYmDef.cYmName));
		nOffset+=sizeof(iYmDef.cYmName);

		memcpy((pData+nOffset),iYmDef.cDescription,sizeof(iYmDef.cDescription));
		nOffset+=sizeof(iYmDef.cDescription);

		memcpy((pData+nOffset),&iYmDef.nYmRaw,sizeof(iYmDef.nYmRaw));
		nOffset+=sizeof(iYmDef.nYmRaw);

		memcpy((pData+nOffset),&iYmDef.dYmQuotiety,sizeof(iYmDef.dYmQuotiety));
		nOffset+=sizeof(iYmDef.dYmQuotiety);

		memcpy((pData+nOffset),&iYmDef.dYmValue,sizeof(iYmDef.dYmValue));
		nOffset+=sizeof(iYmDef.dYmValue);

		memcpy((pData+nOffset),&iYmDef.bIdentifier,sizeof(iYmDef.bIdentifier));
		nOffset+=sizeof(iYmDef.bIdentifier);

		memmove(&pUpdataStruct->RecordData,pData1,nRecordLen);

		BYTE* pRead;
		if (hPipe)
			//EnterCriticalSection(&pDlg->m_pCRedisRecvSample->m_csUpdateRTDB);
			pRead = (BYTE*)GetNetMessage(hPipe, pWrite,dwBufLen-1);	
			//LeaveCriticalSection(&pDlg->m_pCRedisRecvSample->m_csUpdateRTDB);

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
		if (pMessageHead->MessageType!=NET_MESSAGE_ACK || pAck->wdAckType!=NET_MESSAGE_ACK_OK) 
		{
			delete[] pRead;	
			delete[] pData1;
			continue;
		}
		delete[] pRead;
		delete[] pData1;

	}
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

BEGIN_MESSAGE_MAP(CProcessThread, CWinThread)
END_MESSAGE_MAP()


// CProcessThread 消息处理程序
