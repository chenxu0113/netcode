// SaveYmDataThread.cpp : 实现文件
//

#include "stdafx.h"
#include "TSSampleDataSvr.h"
#include "SaveYmDataThread.h"
#include "TSSampleDataSvrDlg.h"

// CSaveYmDataThread
extern CLog *g_log;
IMPLEMENT_DYNCREATE(CSaveYmDataThread, CWinThread)

CSaveYmDataThread::CSaveYmDataThread()
{
	if(NULL==g_log)
	{
		g_log = new CLog(_T("\\TSSampleDataSvr"));
	}
}

CSaveYmDataThread::~CSaveYmDataThread()
{
}

BOOL CSaveYmDataThread::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	CTSSampleDataSvrDlg* pDlg = (CTSSampleDataSvrDlg*)AfxGetApp()->m_pMainWnd;
	BOOL beDone = FALSE;
	while(1)
	{
		SYSTEMTIME sys_time;
		GetLocalTime(&sys_time);
		if (sys_time.wHour==10&&beDone==FALSE&&pDlg->m_pCRedisRecvSample->beInited)
		{
			//GetDeviceId();
			ReguTrace(Config,"Check YmData begin!");
			DeviceRecordDefMap::iterator iterDevMap=pDlg->m_pCRedisRecvSample->m_DeviceRecordDefMap.begin();   
			for(;iterDevMap!=pDlg->m_pCRedisRecvSample->m_DeviceRecordDefMap.end();++iterDevMap)  
			{
				int iDevId = 0;
				int iStationId = 0;
				int iDevNo = 0;  //前置设备号
				iDevId = iterDevMap->second;
				CString ikey = _T("");
				ikey = iterDevMap->first;
				USES_CONVERSION;
				char* chArr = W2A(ikey);
				sscanf_s((const char *)chArr, "%d+%d",&iStationId, &iDevNo);
				SimpleYmDef YmDef;
				BYTE LowRtu = iStationId%256;
				BYTE HighRtu = iStationId/256;
				CString key = _T("");
				key.Format(_T("%d+%d"), iDevId, 7);
				YmConfigDefMap::iterator itym;	
				itym = pDlg->m_pCRedisRecvSample->m_YmConfigDefMap.find(key);
				if (itym == pDlg->m_pCRedisRecvSample->m_YmConfigDefMap.end())
				{
					continue;
				}
				YmDef = itym->second;
				int CallNum = 0;
				if (YmDef.nYmRaw==0)
				{
					continue;
				}
				CheckPaymentRecord(iDevId,YmDef.nYmRaw,CallNum);
				if (CallNum>0)
				{
					ProtocolCallItem *pItem = new ProtocolCallItem;
					if (NULL==pItem)
					{
						continue;
					}
					memset(pItem,0,sizeof(ProtocolCallItem));
					pItem->m_nType = 0xE1;//MSW 150828,加注释，0xE1-召唤充值记录的标志
					pItem->m_nDeviceNo = YmDef.nDeviceNo;    //设备表第一列 Id
					pItem->m_nStationNum = iStationId;

					SocketMsgPrepayRecord iPrepayRecord ;
					memset(&iPrepayRecord,0,sizeof(SocketMsgPrepayRecord));
					iPrepayRecord.btRtuNo = LowRtu;     //==m_nStationNum
					iPrepayRecord.btEndNo = 0xff;
					iPrepayRecord.btForeNo = HighRtu;
					iPrepayRecord.nDevNo = iDevNo;    //设备表第二列 deviceNo
					iPrepayRecord.wdYjNum = CallNum;
					memcpy(pItem->m_nBuffer,&iPrepayRecord,sizeof(SocketMsgPrepayRecord)-sizeof(SocketPrepayRecordUnit));

					ReguTrace(Config,"招充值记录DevId:%d!",iDevId);
					EnterCriticalSection(&pDlg->m_pCCSendP2pTask->m_csRecordList);
					pDlg->m_pCCSendP2pTask->m_RecordList.AddTail(pItem);
					LeaveCriticalSection(&pDlg->m_pCCSendP2pTask->m_csRecordList);
					SetEvent(pDlg->m_pCCSendP2pTask->m_ProcessDataEvent);
					Sleep(1000);
				}
			}
			ReguTrace(Config,"Check YmData end!");
			//for (int i=0;i<m_DevIdCArray.GetSize();i++)
			//{
			//	SimpleYmDef YmDef;
			//	int iDevId = m_DevIdCArray[i].iDevId;
			//	BYTE LowRtu = m_DevIdCArray[i].iStationId%256;
			//	BYTE HighRtu = m_DevIdCArray[i].iStationId/256;
			//	CString key = _T("");
			//	key.Format(_T("%d+%d"), iDevId, 7);
			//	YmConfigDefMap::iterator itym;	
			//	itym = pDlg->m_pCRedisRecvSample->m_YmConfigDefMap.find(key);
			//	if (itym == pDlg->m_pCRedisRecvSample->m_YmConfigDefMap.end())
			//	{
			//		continue;
			//	}
			//	YmDef = itym->second;
			//	int CallNum = 0;
			//	CheckPaymentRecord(iDevId,YmDef.nYmRaw,CallNum);
			//	if (CallNum>0)
			//	{
			//		ProtocolCallItem *pItem = new ProtocolCallItem;
			//		if (NULL==pItem)
			//		{
			//			continue;
			//		}
			//		memset(pItem,0,sizeof(ProtocolCallItem));
			//		pItem->m_nType = 0xE1;//MSW 150828,加注释，0xE1-召唤充值记录的标志
			//		pItem->m_nDeviceNo = YmDef.nDeviceNo;    //设备表第一列 Id
			//		pItem->m_nStationNum = m_DevIdCArray[i].iStationId;

			//		SocketMsgPrepayRecord iPrepayRecord ;
			//		memset(&iPrepayRecord,0,sizeof(SocketMsgPrepayRecord));
			//		iPrepayRecord.btRtuNo = LowRtu;     //==m_nStationNum
			//		iPrepayRecord.btEndNo = 0xff;
			//		iPrepayRecord.btForeNo = HighRtu;
			//		iPrepayRecord.nDevNo = m_DevIdCArray[i].iDevNum;    //设备表第二列 deviceNo
			//		iPrepayRecord.wdYjNum = CallNum;
			//		memcpy(pItem->m_nBuffer,&iPrepayRecord,sizeof(SocketMsgPrepayRecord)-sizeof(SocketPrepayRecordUnit));

			//		EnterCriticalSection(&pDlg->m_pCCSendP2pTask->m_csRecordList);
			//		pDlg->m_pCCSendP2pTask->m_RecordList.AddTail(pItem);
			//		LeaveCriticalSection(&pDlg->m_pCCSendP2pTask->m_csRecordList);
			//	}
			//}
			/*if (pDlg->m_pCCSendP2pTask->m_RecordList.GetSize()>0)
			{
				SetEvent(pDlg->m_pCCSendP2pTask->m_ProcessDataEvent);
			}*/
			beDone = TRUE;
		}
		if (0)//((sys_time.wHour==1||sys_time.wHour==10||sys_time.wHour==18)&&beDone==FALSE&&pDlg->m_pCRedisRecvSample->beInited)
		{
			ReguTrace(Config,"save YmData begin!");
			HANDLE hPipe = OpenRealDataPipe();
			
			CString UpdateSql = _T("");
			int icount = 0;
			YmConfigDefMap::iterator itym=pDlg->m_pCRedisRecvSample->m_YmConfigDefMap.begin();   
			for(;itym!=pDlg->m_pCRedisRecvSample->m_YmConfigDefMap.end();++itym)  
			{
				SimpleYmDef YmDef;
				YmDef = itym->second;
				CString TempSql = _T("");
				TempSql.Format(_T("UPDATE TB_PULSE SET RAWVALUE=%lld,YMVALUE=%f WHERE ID=%d ;"),YmDef.nYmRaw,YmDef.dYmValue,YmDef.nYmIndex);
				UpdateSql += TempSql;
				icount++;
				if (icount%100!=0 &&itym!=pDlg->m_pCRedisRecvSample->m_YmConfigDefMap.end())
				{
					continue;
				}

				BYTE* pRead = NULL;
				CString strSql = UpdateSql;
				pRead = (BYTE*)GetMessage_RecordOfSql_Ext(hPipe, strSql);
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
			beDone = TRUE;
			ReguTrace(Config,"save YmData end!");
		}
		if(sys_time.wHour==2||sys_time.wHour==11||sys_time.wHour==16||sys_time.wHour==19) 
		{
			beDone = FALSE;
		}
		Sleep(20000);
	}
	return TRUE;
}

BOOL CSaveYmDataThread::GetDeviceId()
{
	m_DevIdCArray.RemoveAll();
	int iFdCount = 3;	//	查询的字段数
	int iMaxRsCount = -1;	// 限制查询得到的最大记录个数，-1指不限制


	// 向数据管道发送SQL语句
	BYTE* pRead = NULL;

	HANDLE hPipe =OpenRealDataPipe();

	if (hPipe == NULL) 
	{
		MYERROR(_T("GetDeviceId hPipe null"));
		return FALSE;
	}

	int tempId = 0;
	while (1)
	{
		CString strQuery;
		strQuery.Format(_T("select top 5000 ID, DEVICENUM, STATIONID from TB_DEVICE where ID > %d order by ID;"),tempId);
		pRead = (BYTE*)GetMessage_RecordOfSql(hPipe,strQuery);

		if (NULL == pRead)	// 未查询到记录
		{
			CloseHandle(hPipe);
			MYERROR(_T("GetDeviceId pRead null"));
			return FALSE;
		}
		// 解析返回的结果
		NetMessageHead *pNetMessageHead = (NetMessageHead*)pRead;
		if (pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB) {
			//	MYWARN(_T("返回报文异常，%s"),bufToHexString(pRead,pNetMessageHead->Length).c_str());
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			MYERROR(_T("GetDeviceId pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB null"));
			return FALSE;
		}	

		if (pNetMessageHead->Length < 1)	// 空报文，无查询结果
		{
			//MYWARN(_T("接收到空回复报文，无法解析查询结果"));
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			MYERROR(_T("GetDeviceId pNetMessageHead->Length < 1 null"));
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
			MYERROR(_T("GetDeviceId nFieldNum != iFdCount null"));
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
			MYERROR(_T("GetDeviceId tValue == NULL"));
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			return FALSE;
		}

		// 将查询结果写入到缓存
		for (int i=0;i<(int)dwRecordNum;i++)
		{
			DevStaionInfo iDevInfo;
			memset(&iDevInfo,0,sizeof(DevStaionInfo));
			// iDevId
			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, pHead->DataLen);
			int iDevId = (int)*((DWORD *)tValue);
			pByte += pHead->DataLen;
			iDevInfo.iDevId = iDevId;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+1)->DataLen);
			iDevInfo.iDevNum =  (int)*((DWORD *)tValue);
			pByte += (pHead+1)->DataLen;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+2)->DataLen);
			iDevInfo.iStationId =  (int)*((DWORD *)tValue);
			pByte += (pHead+2)->DataLen;

			m_DevIdCArray.Add(iDevInfo);
			if (iDevId>tempId)
			{
				tempId = iDevId;
			}
		}
		delete []tValue;
		tValue = NULL;
		if (dwRecordNum<5000)
		{
			break;
		}
	}

	int num = m_DevIdCArray.GetSize();

	delete[] pRead;
	pRead = NULL;
	CloseHandle(hPipe);

	return TRUE;
}
BOOL CSaveYmDataThread::CheckPaymentRecord(int iDevId,int MaxBuyNum,int& CallNum)
{
	BYTE* pRead = NULL;
	int iFdCount = 1;	//	查询的字段数
	int iMaxRsCount = -1;	// 限制查询得到的最大记录个数，-1指不限制

	/*HANDLE hPipe =OpenRealDataPipe();
	if (hPipe == NULL) 
	{
		MYERROR(_T("CheckPaymentRecord hPipe null"));
		return FALSE;
	}*/


	CString strQuery;
	strQuery.Format(_T("select [COUNT] from TE_PAYMENT_PAYRECORD where DEVICEID=%d and [COUNT]>0 and [COUNT]<= %d order by [COUNT] desc;"),iDevId,MaxBuyNum);
	pRead = (BYTE*)GetMessage_RecordOfSql(strQuery);

	if (NULL == pRead)	// 未查询到记录
	{
		//CloseHandle(hPipe);
		MYERROR(_T("CheckPaymentRecord pRead null"));
		return FALSE;
	}
	// 解析返回的结果
	NetMessageHead *pNetMessageHead = (NetMessageHead*)pRead;
	if (pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB) {
		delete[] pRead;
		pRead = NULL;
		//CloseHandle(hPipe);
		CallNum = (MaxBuyNum<10)?MaxBuyNum:10;
		//MYERROR(_T("CheckPaymentRecord pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB null"));
		return FALSE;
	}	

	if (pNetMessageHead->Length < 1)	// 空报文，无查询结果
	{
		//MYWARN(_T("接收到空回复报文，无法解析查询结果"));
		delete[] pRead;
		pRead = NULL;
		//CloseHandle(hPipe);
		MYERROR(_T("CheckPaymentRecord pNetMessageHead->Length < 1 null"));
		CallNum = (MaxBuyNum<10)?MaxBuyNum:10;
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
		MYERROR(_T("CheckPaymentRecord nFieldNum != iFdCount null"));
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
		CallNum = (MaxBuyNum<10)?MaxBuyNum:10;
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


	// 将查询结果写入到缓存
	if (dwRecordNum==MaxBuyNum)
	{
		delete[] pRead;
		pRead = NULL;
		//CloseHandle(hPipe);
		return TRUE;
	}
	if ((MaxBuyNum - dwRecordNum)>10)
	{
		delete[] pRead;
		pRead = NULL;
		//CloseHandle(hPipe);
		return TRUE;
	}
	TCHAR *tValue = new TCHAR[1024+1];
	if (tValue == NULL)
	{
		delete[] pRead;
		pRead = NULL;
		//CloseHandle(hPipe);
		return FALSE;
	}

	int ChargeCnt[10] = {0};
	for (int i=0;i<(int)dwRecordNum&&i<10;i++)
	{
		ZeroMemory(tValue,1024+1);
		memcpy(tValue, pByte, pHead->DataLen);
		int iCount = (int)*((DWORD *)tValue);
		pByte += pHead->DataLen;
		if ((MaxBuyNum-iCount)<0)
		{
			continue;
		}
		ChargeCnt[MaxBuyNum-iCount] = iCount;
	}
	CallNum = 0;
	for (int i=0;i<MaxBuyNum&&i<10;i++)
	{
		if (ChargeCnt[i]==0)
		{
			CallNum = i + 1;
		}
	}
	delete []tValue;
	tValue = NULL;
	delete[] pRead;
	pRead = NULL;
	//CloseHandle(hPipe);

	return TRUE;
}
int CSaveYmDataThread::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSaveYmDataThread, CWinThread)
END_MESSAGE_MAP()


// CSaveYmDataThread 消息处理程序
