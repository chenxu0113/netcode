// AlarmTask.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TSAlarmServer_WD.h"
#include "AlarmTask.h"


// CAlarmTask
extern CLog *g_log ;
IMPLEMENT_DYNCREATE(CAlarmTask, CWinThread)

CAlarmTask::CAlarmTask()
{
	if(NULL==g_log)
	{
		g_log = new CLog(_T("\\TSAlarmServer"));
	}
	m_MinValue = 0;
	m_MaxValue = 0;
	m_iReduceNum = 0;
	m_iMagnifyNum = 0;
	m_iValidDays = 30;
}

CAlarmTask::~CAlarmTask()
{
	
}

BOOL CAlarmTask::InitDevInfo()
{
	m_vecDeviceInfo.clear();
	m_DeviceInfoMap.RemoveAll();
	m_BrandDevMap.RemoveAll();
	int iFdCount = 13;	//	��ѯ���ֶ���
	int iMaxRsCount = -1;	// ���Ʋ�ѯ�õ�������¼������-1ָ������


	// �����ݹܵ�����SQL���
	BYTE* pRead = NULL;

	HANDLE hPipe =OpenRealDataPipe();

	if (hPipe == NULL) 
	{
		MYERROR(_T("InitDevInfo hPipe null"));
		return FALSE;
	}

	int tempId = 0;
	while (1)
	{
		CString strQuery;
		strQuery.Format(C_SQL_QUERY_DEVINFO,tempId);
		pRead = (BYTE*)GetMessage_RecordOfSql(hPipe,strQuery);

		if (NULL == pRead)	// δ��ѯ����¼
		{
			CloseHandle(hPipe);
			MYERROR(_T("InitDevInfo pRead null"));
			return FALSE;
		}
		// �������صĽ��
		NetMessageHead *pNetMessageHead = (NetMessageHead*)pRead;
		if (pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB) {
			//	MYWARN(_T("���ر����쳣��%s"),bufToHexString(pRead,pNetMessageHead->Length).c_str());
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			MYERROR(_T("InitDevInfo pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB null"));
			return FALSE;
		}	

		if (pNetMessageHead->Length < 1)	// �ձ��ģ��޲�ѯ���
		{
			//MYWARN(_T("���յ��ջظ����ģ��޷�������ѯ���"));
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			MYERROR(_T("InitDevInfo pNetMessageHead->Length < 1 null"));
			return FALSE;
		}

		// ��ȡ��ѯ���ֶ���
		WORD* pWord = (WORD*)&pRead[sizeof(NetMessageHead)];
		int nFieldNum = (int)*pWord;
		if (nFieldNum != iFdCount)	// ��ѯ�ֶ�����������ȣ�����ѯ����쳣�˳�
		{
			//	MYWARN(_T("��ѯ�ֶ�����������ȣ�����ѯ����쳣�˳�"));
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			MYERROR(_T("InitDevInfo nFieldNum != iFdCount null"));
			return FALSE;
		}
		pWord++;

		// ��¼������
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

		// ��¼��
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
			//MYERROR(_T("�����ڴ���󣬴����룺%d"),GetLastError());
			MYERROR(_T("InitDevInfo tValue == NULL"));
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			return FALSE;
		}

		// ����ѯ���д�뵽����
		LZGRECORDSET rs;
		for (int i=0;i<(int)dwRecordNum;i++)
		{
			DeviceInfoData iDeviceInfo;
			memset(&iDeviceInfo,0,sizeof(DeviceInfo));

			// iDevId
			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, pHead->DataLen);
			iDeviceInfo.iDevId = (int)*((DWORD *)tValue);
			pByte += pHead->DataLen;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+1)->DataLen);
			memcpy(iDeviceInfo.DevName, tValue, sizeof(TCHAR)*65);
			pByte += (pHead+1)->DataLen;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+2)->DataLen);
			iDeviceInfo.iStationId = (int)*((DWORD *)tValue);
			pByte += (pHead+2)->DataLen;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+3)->DataLen);
			iDeviceInfo.iPlazaId = (int)*(DWORD*)tValue;
			pByte += (pHead+3)->DataLen;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+4)->DataLen);
			memcpy(iDeviceInfo.PlazaName, tValue, sizeof(TCHAR)*128);
			pByte += (pHead+4)->DataLen;

			rs.clear();
			memcpy(rs.buf,pByte, (pHead+5)->DataLen);
			iDeviceInfo.Occupied = rs.iRs;
			pByte += (pHead+5)->DataLen;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+6)->DataLen);
			iDeviceInfo.iBrandId = (int)*(DWORD*)tValue;
			pByte += (pHead+6)->DataLen;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+7)->DataLen);
			CString csBrand = _T("");
			/*memcpy(csBrand.GetBuffer(), tValue, (pHead+7)->DataLen);
			csBrand.ReleaseBuffer();*/
			csBrand.Format(_T("%s"),tValue);
			csBrand.Replace(_T("'"),_T("\""));
			memcpy(iDeviceInfo.BrandName, csBrand.GetBuffer(), sizeof(TCHAR)*128);
			pByte += (pHead+7)->DataLen;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+8)->DataLen);
			memcpy(iDeviceInfo.BrandCode, tValue, sizeof(TCHAR)*128);
			pByte += (pHead+8)->DataLen;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+9)->DataLen);
			iDeviceInfo.iContractId = (int)*(DWORD*)tValue;
			pByte += (pHead+9)->DataLen;

			ZeroMemory(tValue,1024+1);
			memcpy(tValue, pByte, (pHead+10)->DataLen);
			iDeviceInfo.iContractType = (int)*(DWORD*)tValue;
			pByte += (pHead+10)->DataLen;

			rs.clear();
			memcpy(rs.buf,pByte, (pHead+11)->DataLen);
			iDeviceInfo.startTime = rs.timeRs;
			pByte += (pHead+11)->DataLen;

			rs.clear();
			memcpy(rs.buf,pByte, (pHead+12)->DataLen);
			iDeviceInfo.endTime = rs.timeRs;
			pByte += (pHead+12)->DataLen;

			if (iDeviceInfo.iDevId>tempId)
			{
				tempId = iDeviceInfo.iDevId;
			}
			//m_DeviceInfoMap.SetAt(iDeviceInfo.iDevId,iDeviceInfo);
			m_vecDeviceInfo.push_back(iDeviceInfo);
			m_DeviceInfoMap.SetAt(iDeviceInfo.iDevId,iDeviceInfo);
			if (iDeviceInfo.iBrandId!=0)
			{
				vector<int> vecDev;
				if (m_BrandDevMap.Lookup(iDeviceInfo.iBrandId,vecDev))
				{
					vecDev.push_back(iDeviceInfo.iDevId);
				}
				else
				{
					vecDev.push_back(iDeviceInfo.iDevId);
				}
				m_BrandDevMap.SetAt(iDeviceInfo.iBrandId,vecDev);
			}
		}
		delete []tValue;
		tValue = NULL;
		if (dwRecordNum<5000)
		{
			break;
		}
	}

	int num = m_vecDeviceInfo.capacity();

	delete[] pRead;
	pRead = NULL;
	CloseHandle(hPipe);

	return TRUE;
}

BOOL CAlarmTask::LoadStationTable()
{
	m_StationArray.RemoveAll();
	HANDLE hPipe = NULL;
	StationDef pStaDef;
	NetMessageHead* pNetMessageHead = NULL;
	MessageQueryRecord mqr;

	//��ȡ��վ��Ϣ��
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

		m_StationArray.Add(pStaDef.nPortNo);
		//m_StationDefMap[pStaDef.nPortNo] = pStaDef.nStationID;  //GUOJ MOD 151020
		pBuf+=dwRecordLen;
		nNum--;
	}

	//����
	if(hPipe != NULL)			{CloseHandle(hPipe); hPipe = NULL;}
	if(pNetMessageHead != NULL){delete [](BYTE *)pNetMessageHead; pNetMessageHead=NULL;}

	return TRUE;
}

BOOL CAlarmTask::InitInstance()
{
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	InitCfgInfo();
	
	if (!LoadSMSTemplate())
	{
		ReguTrace(Config,"LoadSMSTemplate err!");
		return FALSE;
	}
	if (!LoadSMSConfig())
	{
		ReguTrace(Config,"LoadSMSConfig err!");
		return FALSE;
	}
	BOOL beDone = FALSE;
	while(1)
	{
		SYSTEMTIME sys_time;
		GetLocalTime(&sys_time);
		if (sys_time.wHour>=5&&sys_time.wHour<=6&&beDone==FALSE)
		{			
			SYSLOG(LOG_DEBUG,L"׼����ʼ���豸��Ϣ");
			ReguTrace(Config,"׼����ʼ���豸��Ϣ��");
			if (!InitDevInfo())
			{
				ReguTrace(Config,"InitDevInfo err!");
				return FALSE;
			}
			GetNoDevContract();
			//����豸Ƿ�Ѹ澯
			ReguTrace(Config,"��ʼ���ҵ��澯��");
			if(!GetOverDraft())
			{
				ReguTrace(Config,"����豸Ƿ�Ѹ澯ʧ�ܣ�");
			}
			m_DevSampleCfgArray.RemoveAll();
			//��ȡ�ղ�������
			GetSampleDayConfig();
			//��������쳣�澯
			CheckBillAlarm();
			m_DevSampleCfgArray.RemoveAll();
			CString strTime = _T("");
			CString strlastTime = _T("");
			CTime ttCurTime = CTime(sys_time.wYear,sys_time.wMonth,sys_time.wDay,0,0,0);
			CTime ttLastTime = ttCurTime - CTimeSpan(1,0,0,0);
			CTime ttLastLastTime = ttCurTime - CTimeSpan(2,0,0,0);
			strTime.Format(_T("%04d-%02d-%02d 00:00:00"),ttLastTime.GetYear(),
				ttLastTime.GetMonth(),ttLastTime.GetDay());
			strlastTime.Format(_T("%04d-%02d-%02d 00:00:00"),ttLastLastTime.GetYear(),
				ttLastLastTime.GetMonth(),ttLastLastTime.GetDay());
			
			m_DevDayValueMap.RemoveAll();
			LoadDayValue(strTime,SUMCOUNT);
			LoadDayValue(strlastTime,MEANELEC);
			int num = m_DevDayValueMap.GetCount();
			ReguTrace(Config,"��ȡ�˵����ݽ���NUM=%d!",num);
			ProcessData();
			beDone = TRUE;
			
			
			//GetNoDevContract();
			
			LoadStationTable();
			CheckDevStateByStationId();
			ReguTrace(Config,"ҵ��澯��Ͻ�����");
			//if (sys_time.wDayOfWeek==2)  //����к�ͬ�ޱ��쳣
			//{
			//	
			//}
			UpdateAlarmStatus();
		}
		
		if (sys_time.wHour==0&&sys_time.wMinute==0)
		{
			beDone = FALSE;
			BackupData();  //���ݸ澯����
		}
		Sleep(20000);
		
	}
	return TRUE;
}

void CAlarmTask::CheckDevStateByStationId()
{
	BYTE* pRead = NULL;

	HANDLE hPipe =OpenRealDataPipe();

	if (hPipe == NULL) 
	{
		MYERROR(_T("CheckDevStateByStationId hPipe null"));
		return ;
	}
	for (int i=0;i<m_StationArray.GetSize();i++)
	{
		int iFdCount = 2;	     //	��ѯ���ֶ���
		int iMaxRsCount = -1;	// ���Ʋ�ѯ�õ�������¼������-1ָ������
		CString strQuery  = _T("");
		strQuery.Format(_T("select ID from TB_DI where STATIONNUM=%d and NUM=0 and YXVALUE=1;"),m_StationArray[i]);
		pRead = (BYTE*)GetMessage_RecordOfSql(hPipe,strQuery);

		if (NULL == pRead)	// δ��ѯ����¼
		{
			CloseHandle(hPipe);
			MYERROR(_T("CheckDevStateByStationId pRead null"));
			return ;
		}
		// �������صĽ��
		NetMessageHead *pNetMessageHead = (NetMessageHead*)pRead;
		if (pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB) {
			//	MYWARN(_T("���ر����쳣��%s"),bufToHexString(pRead,pNetMessageHead->Length).c_str());
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			MYERROR(_T("CheckDevStateByStationId pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB null"));
			return ;
		}	

		if (pNetMessageHead->Length < 1)	// �ձ��ģ��޲�ѯ���
		{
			//MYWARN(_T("���յ��ջظ����ģ��޷�������ѯ���"));
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			MYERROR(_T("CheckDevStateByStationId pNetMessageHead->Length < 1 null"));
			return ;
		}

		// ��ȡ��ѯ���ֶ���
		WORD* pWord = (WORD*)&pRead[sizeof(NetMessageHead)];
		int nFieldNum = (int)*pWord;
		if (nFieldNum != iFdCount)	// ��ѯ�ֶ�����������ȣ�����ѯ����쳣�˳�
		{
			//	MYWARN(_T("��ѯ�ֶ�����������ȣ�����ѯ����쳣�˳�"));
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			MYERROR(_T("CheckDevStateByStationId nFieldNum != iFdCount null"));
			return ;
		}
		pWord++;

		// ��¼������
		MessageRetRecordHead* pHead = (MessageRetRecordHead*)pWord;
		BYTE *pByte = (BYTE*)&pHead[nFieldNum];
		DWORD dwRecordLen = *((DWORD*)pByte);
		if (dwRecordLen < 1)
		{
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			return;
		}
		pByte += sizeof(DWORD);

		// ��¼��
		DWORD dwRecordNum = *((int*)pByte);
		if ((iMaxRsCount > 0) && (dwRecordNum != iMaxRsCount))
		{
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			return;
		}
		pByte += sizeof(DWORD);
		if (dwRecordNum>0)
		{
			continue;
		}
		//Ƿ�ѳ���15������澯
		AlarmItemDef ALARMDef;
		memset(&ALARMDef, 0, sizeof(AlarmItemDef));
		CreateUuid(ALARMDef.alarmID);
		ALARMDef.alarmType = 8008;
		ALARMDef.alarmLevel = 10;
		ALARMDef.alarmObjID = m_StationArray[i];
		ALARMDef.devID = 0;
		ALARMDef.stationID = m_StationArray[i];
		memcpy(ALARMDef.alarmTypeName,_T("�����������쳣"),sizeof(ALARMDef.alarmTypeName));
		//memcpy(ALARMDef.alarmObjName,iDeviceInfo.DevName,sizeof(ALARMDef.alarmObjName));

		ALARMDef.alarmObjType = 33; 
		CTime curTime = CTime::GetCurrentTime();
		ALARMDef.contentTime = curTime.GetTime();
		//curTime = CTime(curTime.GetYear(),curTime.GetMonth(),curTime.GetDay(),0,0,0);
		ALARMDef.alarmTime = curTime.GetTime();

		ALARMDef.status = 0;

		ALARMDef.projectID = 1;    //���̺�
		ALARMDef.systemID = 1;    //��ϵͳ��
		ALARMDef.rtAlarm = 0;    //����¼ʵʱ�澯
		memset(ALARMDef.stationName, 0, sizeof(ALARMDef.stationName));
		memset(ALARMDef.projectName, 0, sizeof(ALARMDef.projectName));
		//memset(ALARMDef.alarmSource, 0, sizeof(ALARMDef.alarmSource));
		//_tcsncpy_s(ALARMDef.alarmSource, _countof(ALARMDef.alarmSource), C_ALARMRESORCE, C_ALARMRESORCE.GetLength());
		memcpy(ALARMDef.alarmSource,_T("Ӧ�÷���澯"),sizeof(ALARMDef.alarmSource));
		ALARMDef.groupID1 = 1;
		ALARMDef.groupID2 = 2;
		CString strAlarmContent = _T("");
		strAlarmContent.Format(_T("������ID=%d�������豸ͨѶ�жϣ���ȷ��!"),
			m_StationArray[i]);
		memcpy(ALARMDef.alarmContent,strAlarmContent.GetBuffer(),sizeof(ALARMDef.alarmContent));
		AddAlarm2DB(ALARMDef);	
	}
	delete[] pRead;
	pRead = NULL;
	CloseHandle(hPipe);
}
BOOL CAlarmTask::InitCfgInfo()
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
	m_MinValue = GetPrivateProfileInt(_T("ALARMWD"),_T("MinValue"),1,strCountPath);
	if(m_MinValue <= 0)
	{
		m_MinValue = 1;
	}
	m_MaxValue = GetPrivateProfileInt(_T("ALARMWD"),_T("MaxValue"),5,strCountPath);
	if(m_MaxValue <= 0)
	{
		m_MaxValue = 5;
	}
	
	TCHAR chNumcfg[64]={0};
	GetPrivateProfileString(_T("UPRUSHCONFIG"),_T("MagnifyNum"),_T("1,0.2"),chNumcfg,64,strCountPath);
	CString csNumcfg = chNumcfg;
	int iPos = csNumcfg.Find(_T(","));
	CString csMagnify = csNumcfg.Left(iPos);
	m_iMagnifyNum = _wtof(csMagnify);
	CString csReduce = csNumcfg.Right(csNumcfg.GetLength()-iPos-1);
	m_iReduceNum = _wtof(csReduce);

	return TRUE;
}

#include <Rpcdce.h>
#pragma comment(lib,"Rpcrt4.lib")
void CAlarmTask::CreateUuid(TCHAR* csID)
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

void CAlarmTask::CheckBillAlarm()
{
	BYTE* pRead = NULL;

	HANDLE hPipe =OpenRealDataPipe();

	if (hPipe == NULL) 
	{
		MYERROR(_T("CheckBillAlarm hPipe null"));
		return ;
	}
	for (int i = 0;i<m_DevSampleCfgArray.GetSize();i++)
	{
		int iFdCount = 2;	     //	��ѯ���ֶ���
		int iMaxRsCount = -1;	// ���Ʋ�ѯ�õ�������¼������-1ָ������
		CString strQuery  = _T("");
		strQuery.Format(_T("select top(15) TIMEID, %s from %s where %s>0 order by TIMEID desc;"),
			m_DevSampleCfgArray[i].ColumnName,m_DevSampleCfgArray[i].TableName,m_DevSampleCfgArray[i].ColumnName);
		pRead = (BYTE*)GetMessage_RecordOfSql(hPipe,strQuery);

		if (NULL == pRead)	// δ��ѯ����¼
		{
			CloseHandle(hPipe);
			MYERROR(_T("CheckBillAlarm pRead null"));
			return ;
		}
		// �������صĽ��
		NetMessageHead *pNetMessageHead = (NetMessageHead*)pRead;
		if (pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB) {
			//	MYWARN(_T("���ر����쳣��%s"),bufToHexString(pRead,pNetMessageHead->Length).c_str());
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			MYERROR(_T("CheckBillAlarm pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB null"));
			return ;
		}	

		if (pNetMessageHead->Length < 1)	// �ձ��ģ��޲�ѯ���
		{
			//MYWARN(_T("���յ��ջظ����ģ��޷�������ѯ���"));
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			MYERROR(_T("CheckBillAlarm pNetMessageHead->Length < 1 null"));
			return ;
		}

		// ��ȡ��ѯ���ֶ���
		WORD* pWord = (WORD*)&pRead[sizeof(NetMessageHead)];
		int nFieldNum = (int)*pWord;
		if (nFieldNum != iFdCount)	// ��ѯ�ֶ�����������ȣ�����ѯ����쳣�˳�
		{
			//	MYWARN(_T("��ѯ�ֶ�����������ȣ�����ѯ����쳣�˳�"));
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			MYERROR(_T("CheckBillAlarm nFieldNum != iFdCount null"));
			return ;
		}
		pWord++;

		// ��¼������
		MessageRetRecordHead* pHead = (MessageRetRecordHead*)pWord;
		BYTE *pByte = (BYTE*)&pHead[nFieldNum];
		DWORD dwRecordLen = *((DWORD*)pByte);
		if (dwRecordLen < 1)
		{
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			return;
		}
		pByte += sizeof(DWORD);

		// ��¼��
		DWORD dwRecordNum = *((int*)pByte);
		if ((iMaxRsCount > 0) && (dwRecordNum != iMaxRsCount))
		{
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			return;
		}
		pByte += sizeof(DWORD);
		if (dwRecordNum<15)
		{
			continue;
		}
		//Ƿ�ѳ���15������澯
		int iDevId = m_DevSampleCfgArray[i].iDevId;
		AlarmItemDef ALARMDef;
		memset(&ALARMDef, 0, sizeof(AlarmItemDef));
		CreateUuid(ALARMDef.alarmID);
		DeviceInfoData iDeviceInfo;
		memset(&iDeviceInfo,0,sizeof(DeviceInfoData));
		m_DeviceInfoMap.Lookup(iDevId,iDeviceInfo);
		ALARMDef.alarmType = ALARMTYPE_BILLOVERTIME;
		ALARMDef.alarmLevel = 10;
		ALARMDef.alarmObjID = iDevId;
		ALARMDef.devID = iDevId;
		ALARMDef.stationID = iDeviceInfo.iStationId;
		memcpy(ALARMDef.alarmTypeName,_T("�����쳣�澯"),sizeof(ALARMDef.alarmTypeName));
		memcpy(ALARMDef.alarmObjName,iDeviceInfo.DevName,sizeof(ALARMDef.alarmObjName));

		ALARMDef.alarmObjType = 31; 
		CTime curTime = CTime::GetCurrentTime();
		ALARMDef.contentTime = curTime.GetTime();
		//curTime = CTime(curTime.GetYear(),curTime.GetMonth(),curTime.GetDay(),0,0,0);
		ALARMDef.alarmTime = curTime.GetTime();

		ALARMDef.status = 0;

		ALARMDef.projectID = 1;    //���̺�
		ALARMDef.systemID = 1;    //��ϵͳ��
		ALARMDef.rtAlarm = 0;    //����¼ʵʱ�澯
		memset(ALARMDef.stationName, 0, sizeof(ALARMDef.stationName));
		memset(ALARMDef.projectName, 0, sizeof(ALARMDef.projectName));
		//memset(ALARMDef.alarmSource, 0, sizeof(ALARMDef.alarmSource));
		//_tcsncpy_s(ALARMDef.alarmSource, _countof(ALARMDef.alarmSource), C_ALARMRESORCE, C_ALARMRESORCE.GetLength());
		memcpy(ALARMDef.alarmSource,_T("Ӧ�÷���澯"),sizeof(ALARMDef.alarmSource));
		ALARMDef.groupID1 = 1;
		ALARMDef.groupID2 = 2;
		CString strAlarmContent = _T("");
		strAlarmContent.Format(_T("�豸%s������Ƿ��15�죬��ȷ��!"),
			iDeviceInfo.DevName);
		memcpy(ALARMDef.alarmContent,strAlarmContent.GetBuffer(),sizeof(ALARMDef.alarmContent));
		AddAlarm2DB(ALARMDef);	
	}
	delete[] pRead;
	pRead = NULL;
	CloseHandle(hPipe);
}
BOOL CAlarmTask::GetSampleDayConfig()
{
	int iFdCount = 3;	//	��ѯ���ֶ���
	int iMaxRsCount = -1;	// ���Ʋ�ѯ�õ�������¼������-1ָ������


	// �����ݹܵ�����SQL���
	BYTE* pRead = NULL;

	HANDLE hPipe =OpenRealDataPipe();

	if (hPipe == NULL) 
	{
		MYERROR(_T("GetOverDraft hPipe null"));
		return FALSE;
	}

	CString strQuery  = _T("select DEVICEID,SAMPLETABLENAME,COLUMNNAME from TB_SAMPLEDAY_EXT where \
						   DEVICEID in(select DEVICENUM from TB_PULSE where num=1 and YMVALUE>0) and RECORDNO%10=1;");
	pRead = (BYTE*)GetMessage_RecordOfSql(hPipe,strQuery);

	if (NULL == pRead)	// δ��ѯ����¼
	{
		CloseHandle(hPipe);
		MYERROR(_T("GetOverDraft pRead null"));
		return FALSE;
	}
	// �������صĽ��
	NetMessageHead *pNetMessageHead = (NetMessageHead*)pRead;
	if (pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB) {
		//	MYWARN(_T("���ر����쳣��%s"),bufToHexString(pRead,pNetMessageHead->Length).c_str());
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("GetOverDraft pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB null"));
		return FALSE;
	}	

	if (pNetMessageHead->Length < 1)	// �ձ��ģ��޲�ѯ���
	{
		//MYWARN(_T("���յ��ջظ����ģ��޷�������ѯ���"));
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("GetOverDraft pNetMessageHead->Length < 1 null"));
		return FALSE;
	}

	// ��ȡ��ѯ���ֶ���
	WORD* pWord = (WORD*)&pRead[sizeof(NetMessageHead)];
	int nFieldNum = (int)*pWord;
	if (nFieldNum != iFdCount)	// ��ѯ�ֶ�����������ȣ�����ѯ����쳣�˳�
	{
		//	MYWARN(_T("��ѯ�ֶ�����������ȣ�����ѯ����쳣�˳�"));
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("GetOverDraft nFieldNum != iFdCount null"));
		return FALSE;
	}
	pWord++;

	// ��¼������
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

	// ��¼��
	DWORD dwRecordNum = *((int*)pByte);
	if ((iMaxRsCount > 0) && (dwRecordNum != iMaxRsCount))
	{
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		return FALSE;
	}
	pByte += sizeof(DWORD);

	// ����ѯ���д�뵽����
	LZGRECORDSET rs;
	for (int i=0;i<(int)dwRecordNum;i++)
	{
		DevSampleConfig iDevConfig;
		memset(&iDevConfig, 0, sizeof(DevSampleConfig));

		rs.clear();
		memcpy(rs.buf, pByte, pHead->DataLen);
		iDevConfig.iDevId = rs.iRs;
		pByte += pHead->DataLen;

		memcpy(rs.buf, pByte, (pHead+1)->DataLen);
		memcpy(iDevConfig.TableName, rs.cBuf, sizeof(TCHAR)*50);
		pByte += (pHead+1)->DataLen;
		rs.clear();

		memcpy(rs.buf, pByte, (pHead+2)->DataLen);
		memcpy(iDevConfig.ColumnName, rs.cBuf, sizeof(TCHAR)*50);
		pByte += (pHead+2)->DataLen;
		rs.clear();
		m_DevSampleCfgArray.Add(iDevConfig);
	}
	delete[] pRead;
	pRead = NULL;
	CloseHandle(hPipe);

	return TRUE;
}
BOOL CAlarmTask::GetOverDraft()
{
	int iFdCount = 3;	//	��ѯ���ֶ���
	int iMaxRsCount = -1;	// ���Ʋ�ѯ�õ�������¼������-1ָ������


	// �����ݹܵ�����SQL���
	BYTE* pRead = NULL;

	HANDLE hPipe =OpenRealDataPipe();

	if (hPipe == NULL) 
	{
		MYERROR(_T("GetOverDraft hPipe null"));
		return FALSE;
	}

	CString strQuery  = _T("select STATIONNUM,DEVICENUM,YMVALUE from TB_PULSE where num=1 and YMVALUE>0;");
	pRead = (BYTE*)GetMessage_RecordOfSql(hPipe,strQuery);

	if (NULL == pRead)	// δ��ѯ����¼
	{
		CloseHandle(hPipe);
		MYERROR(_T("GetOverDraft pRead null"));
		return FALSE;
	}
	// �������صĽ��
	NetMessageHead *pNetMessageHead = (NetMessageHead*)pRead;
	if (pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB) {
		//	MYWARN(_T("���ر����쳣��%s"),bufToHexString(pRead,pNetMessageHead->Length).c_str());
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("GetOverDraft pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB null"));
		return FALSE;
	}	

	if (pNetMessageHead->Length < 1)	// �ձ��ģ��޲�ѯ���
	{
		//MYWARN(_T("���յ��ջظ����ģ��޷�������ѯ���"));
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("GetOverDraft pNetMessageHead->Length < 1 null"));
		return FALSE;
	}

	// ��ȡ��ѯ���ֶ���
	WORD* pWord = (WORD*)&pRead[sizeof(NetMessageHead)];
	int nFieldNum = (int)*pWord;
	if (nFieldNum != iFdCount)	// ��ѯ�ֶ�����������ȣ�����ѯ����쳣�˳�
	{
		//	MYWARN(_T("��ѯ�ֶ�����������ȣ�����ѯ����쳣�˳�"));
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("GetOverDraft nFieldNum != iFdCount null"));
		return FALSE;
	}
	pWord++;

	// ��¼������
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

	// ��¼��
	DWORD dwRecordNum = *((int*)pByte);
	if ((iMaxRsCount > 0) && (dwRecordNum != iMaxRsCount))
	{
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		return FALSE;
	}
	pByte += sizeof(DWORD);

	// ����ѯ���д�뵽����
	LZGRECORDSET rs;
	for (int i=0;i<(int)dwRecordNum;i++)
	{
		AlarmItemDef ALARMDef;
		memset(&ALARMDef, 0, sizeof(AlarmItemDef));
		CreateUuid(ALARMDef.alarmID);

		rs.clear();
		int iStationId = 0;
		memcpy(rs.buf, pByte, pHead->DataLen);
		iStationId = rs.iRs;
		pByte += pHead->DataLen;

		int iDevId = 0;
		memcpy(rs.buf, pByte, (pHead+1)->DataLen);
		iDevId = rs.iRs;
		pByte += (pHead+1)->DataLen;
		rs.clear();

		double YmValue = 0;
		memcpy(rs.buf, pByte, (pHead+2)->DataLen);
		YmValue = rs.dfRs;
		pByte += (pHead+2)->DataLen;
		rs.clear();

		DeviceInfoData iDeviceInfo;
		memset(&iDeviceInfo,0,sizeof(DeviceInfoData));
		m_DeviceInfoMap.Lookup(iDevId,iDeviceInfo);
		ALARMDef.alarmType = ALARMTYPE_DEV_OVERDRAFT;
		ALARMDef.alarmLevel = 10;
		ALARMDef.alarmObjID = iDevId;
		ALARMDef.devID = iDevId;
		ALARMDef.stationID = iStationId;
		memcpy(ALARMDef.alarmTypeName,_T("���Ƿ�Ѹ澯"),sizeof(ALARMDef.alarmTypeName));
		memcpy(ALARMDef.alarmObjName,iDeviceInfo.DevName,sizeof(ALARMDef.alarmObjName));

		ALARMDef.alarmObjType = 31; 
		CTime curTime = CTime::GetCurrentTime();
		ALARMDef.contentTime = curTime.GetTime();
		//curTime = CTime(curTime.GetYear(),curTime.GetMonth(),curTime.GetDay(),0,0,0);
		ALARMDef.alarmTime = curTime.GetTime();

		ALARMDef.status = 0;

		ALARMDef.projectID = 1;    //���̺�
		ALARMDef.systemID = 1;    //��ϵͳ��
		ALARMDef.rtAlarm = 0;    //����¼ʵʱ�澯
		memset(ALARMDef.stationName, 0, sizeof(ALARMDef.stationName));
		memset(ALARMDef.projectName, 0, sizeof(ALARMDef.projectName));
		//memset(ALARMDef.alarmSource, 0, sizeof(ALARMDef.alarmSource));
		//_tcsncpy_s(ALARMDef.alarmSource, _countof(ALARMDef.alarmSource), C_ALARMRESORCE, C_ALARMRESORCE.GetLength());
		memcpy(ALARMDef.alarmSource,_T("Ӧ�÷���澯"),sizeof(ALARMDef.alarmSource));
		ALARMDef.groupID1 = 1;
		ALARMDef.groupID2 = 3;
		CString strAlarmContent = _T("");
		strAlarmContent.Format(_T("�豸%s��Ƿ�ѣ�Ƿ�ѽ��Ϊ%.2f,��ȷ��!"),
			iDeviceInfo.DevName,YmValue);
		memcpy(ALARMDef.alarmContent,strAlarmContent.GetBuffer(),sizeof(ALARMDef.alarmContent));
		//_tcsncpy_s(ALARMDef.alarmContent, _countof(ALARMDef.alarmContent), strAlarmContent, strAlarmContent.GetLength());
		AddAlarm2DB(ALARMDef);	
	}

	delete[] pRead;
	pRead = NULL;
	CloseHandle(hPipe);

	return TRUE;
}
BOOL CAlarmTask::GetNoDevContract()
{
	int iFdCount = 6;	//	��ѯ���ֶ���
	int iMaxRsCount = -1;	// ���Ʋ�ѯ�õ�������¼������-1ָ������


	// �����ݹܵ�����SQL���
	BYTE* pRead = NULL;

	HANDLE hPipe =OpenRealDataPipe();

	if (hPipe == NULL) 
	{
		MYERROR(_T("GetNoDevContract hPipe null"));
		return FALSE;
	}

	CString strQuery  = _T("select bpa.ID,bpa.CNAME,bpa.code,bpa.NICKNAME,bpa.PLAZAID,bpr1.CNAME from TB_PAYMENT_ACCOUNT bpa \
		left join TB_PAYMENT_ROOM bpr on bpa.ID = bpr.ACCOUNTID \
		left join TB_PAYMENT_ROOM bpr1 on bpr1.ID = bpa.PLAZAID \
		left join TL_PAYMENT_ROOM lpr on bpr.ID = lpr.ROOMID \
		left join TB_DEVICE bd on lpr.DEVICEID = bd.ID where ISNULL(bd.ID,-1) = -1 and bpa.CURRENTSETTLETYPE=1 \
		and (PACTENDDATE > GETDATE()) and ContractType != 0 ;");
	pRead = (BYTE*)GetMessage_RecordOfSql(hPipe,strQuery);

	if (NULL == pRead)	// δ��ѯ����¼
	{
		CloseHandle(hPipe);
		MYERROR(_T("GetNoDevContract pRead null"));
		return FALSE;
	}
	// �������صĽ��
	NetMessageHead *pNetMessageHead = (NetMessageHead*)pRead;
	if (pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB) {
		//	MYWARN(_T("���ر����쳣��%s"),bufToHexString(pRead,pNetMessageHead->Length).c_str());
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("GetNoDevContract pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB null"));
		return FALSE;
	}	

	if (pNetMessageHead->Length < 1)	// �ձ��ģ��޲�ѯ���
	{
		//MYWARN(_T("���յ��ջظ����ģ��޷�������ѯ���"));
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("GetNoDevContract pNetMessageHead->Length < 1 null"));
		return FALSE;
	}

	// ��ȡ��ѯ���ֶ���
	WORD* pWord = (WORD*)&pRead[sizeof(NetMessageHead)];
	int nFieldNum = (int)*pWord;
	if (nFieldNum != iFdCount)	// ��ѯ�ֶ�����������ȣ�����ѯ����쳣�˳�
	{
		//	MYWARN(_T("��ѯ�ֶ�����������ȣ�����ѯ����쳣�˳�"));
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("GetNoDevContract nFieldNum != iFdCount null"));
		return FALSE;
	}
	pWord++;

	// ��¼������
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

	// ��¼��
	DWORD dwRecordNum = *((int*)pByte);
	if ((iMaxRsCount > 0) && (dwRecordNum != iMaxRsCount))
	{
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		return FALSE;
	}
	pByte += sizeof(DWORD);

	// ����ѯ���д�뵽����
	LZGRECORDSET rs;
	for (int i=0;i<(int)dwRecordNum;i++)
	{
		AlarmItemDef ALARMDef;
		memset(&ALARMDef, 0, sizeof(AlarmItemDef));
		CreateUuid(ALARMDef.alarmID);

		rs.clear();
		int ContractId = 0;
		memcpy(rs.buf, pByte, pHead->DataLen);
		ContractId = rs.iRs;
		pByte += pHead->DataLen;

		TCHAR AccoutName[65];
		memset(AccoutName,0,sizeof(TCHAR)*65);
		memcpy(rs.buf, pByte, (pHead+1)->DataLen);
		memcpy(AccoutName, rs.cBuf, (pHead+1)->DataLen);
		pByte += (pHead+1)->DataLen;
		rs.clear();

		TCHAR AccountCode[17];
		memset(AccountCode,0,sizeof(TCHAR)*17);
		memcpy(rs.buf, pByte, (pHead+2)->DataLen);
		memcpy(AccountCode, rs.cBuf, (pHead+2)->DataLen);
		pByte += (pHead+2)->DataLen;
		rs.clear();

		TCHAR NickName[601];
		memset(NickName,0,sizeof(TCHAR)*601);
		memcpy(rs.buf, pByte, (pHead+3)->DataLen);
		memcpy(NickName, rs.cBuf, (pHead+3)->DataLen);
		pByte += (pHead+3)->DataLen;
		rs.clear();

		memcpy(rs.buf, pByte, (pHead+4)->DataLen);
		int iPlazaId = 0;
		iPlazaId = rs.iRs;
		pByte += (pHead+4)->DataLen;
		rs.clear();

		TCHAR PlazaName[301];
		memset(PlazaName,0,sizeof(TCHAR)*301);
		memcpy(rs.buf, pByte, (pHead+5)->DataLen);
		memcpy(PlazaName, rs.cBuf, (pHead+5)->DataLen);
		pByte += (pHead+5)->DataLen;
		rs.clear();

		ALARMDef.alarmType = ALARMTYPE_CONTRACTNODEV;
		ALARMDef.alarmLevel = 10;
		ALARMDef.alarmObjID = ContractId;
		//ALARMDef.stationID = iDeviceInfo.iStationId;
		memcpy(ALARMDef.alarmTypeName,_T("Ʒ��ȱʧ���"),sizeof(ALARMDef.alarmTypeName));
		memcpy(ALARMDef.alarmObjName,AccoutName,sizeof(AccoutName));

		ALARMDef.alarmObjType = 36;  //35 ����澯����ΪƷ�ƣ�36Ϊ��ͬ
		//ALARMDef.devID = iDeviceInfo.iDevId;
		CTime curTime = CTime::GetCurrentTime();
		ALARMDef.contentTime = curTime.GetTime();
		curTime = CTime(curTime.GetYear(),curTime.GetMonth(),curTime.GetDay(),0,0,0);
		ALARMDef.alarmTime = curTime.GetTime();

		ALARMDef.status = 0;

		ALARMDef.projectID = 1;    //���̺�
		ALARMDef.systemID = 1;    //��ϵͳ��
		ALARMDef.rtAlarm = 0;    //����¼ʵʱ�澯
		memset(ALARMDef.stationName, 0, sizeof(ALARMDef.stationName));
		memset(ALARMDef.projectName, 0, sizeof(ALARMDef.projectName));
		//memset(ALARMDef.alarmSource, 0, sizeof(ALARMDef.alarmSource));
		//_tcsncpy_s(ALARMDef.alarmSource, _countof(ALARMDef.alarmSource), C_ALARMRESORCE, C_ALARMRESORCE.GetLength());
		memcpy(ALARMDef.alarmSource,_T("Ӧ�÷���澯"),sizeof(ALARMDef.alarmSource));
		ALARMDef.groupID1 = 1;
		ALARMDef.groupID2 = 2;
		CString strAlarmContent = _T("");
		strAlarmContent.Format(_T("%s%sƷ���º�ͬ��Ϊ%d,û�������豸����ȷ��!"),
			PlazaName,AccoutName,ContractId);
		memcpy(ALARMDef.alarmContent,strAlarmContent.GetBuffer(),sizeof(ALARMDef.alarmContent));
		AddAlarm2DB(ALARMDef);		
	}

	delete[] pRead;
	pRead = NULL;
	CloseHandle(hPipe);

	return TRUE;
}
BOOL CAlarmTask::LoadSMSConfig()
{
	int iFdCount = 6;	//	��ѯ���ֶ���
	int iMaxRsCount = -1;	// ���Ʋ�ѯ�õ�������¼������-1ָ������


	// �����ݹܵ�����SQL���
	BYTE* pRead = NULL;

	HANDLE hPipe =OpenRealDataPipe();

	if (hPipe == NULL) 
	{
		MYERROR(_T("LoadSMSConfig hPipe null"));
		return FALSE;
	}

	CString strQuery  = _T("select PLAZAIDS,[TYPE],TEMPLATENUM,ISSHIELD,REALNAME,PHONE from TE_PAYMENT_NOSMS;");
	pRead = (BYTE*)GetMessage_RecordOfSql(hPipe,strQuery);

	if (NULL == pRead)	// δ��ѯ����¼
	{
		CloseHandle(hPipe);
		MYERROR(_T("LoadSMSConfig pRead null"));
		return FALSE;
	}
	// �������صĽ��
	NetMessageHead *pNetMessageHead = (NetMessageHead*)pRead;
	if (pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB) {
		//	MYWARN(_T("���ر����쳣��%s"),bufToHexString(pRead,pNetMessageHead->Length).c_str());
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("LoadSMSConfig pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB null"));
		return FALSE;
	}	

	if (pNetMessageHead->Length < 1)	// �ձ��ģ��޲�ѯ���
	{
		//MYWARN(_T("���յ��ջظ����ģ��޷�������ѯ���"));
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("LoadSMSConfig pNetMessageHead->Length < 1 null"));
		return FALSE;
	}

	// ��ȡ��ѯ���ֶ���
	WORD* pWord = (WORD*)&pRead[sizeof(NetMessageHead)];
	int nFieldNum = (int)*pWord;
	if (nFieldNum != iFdCount)	// ��ѯ�ֶ�����������ȣ�����ѯ����쳣�˳�
	{
		//	MYWARN(_T("��ѯ�ֶ�����������ȣ�����ѯ����쳣�˳�"));
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("LoadSMSConfig nFieldNum != iFdCount null"));
		return FALSE;
	}
	pWord++;

	// ��¼������
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

	// ��¼��
	DWORD dwRecordNum = *((int*)pByte);
	if ((iMaxRsCount > 0) && (dwRecordNum != iMaxRsCount))
	{
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		return FALSE;
	}
	pByte += sizeof(DWORD);

	TCHAR *tValue = new TCHAR[2048+1];
	if (tValue == NULL)
	{
		//MYERROR(_T("�����ڴ���󣬴����룺%d"),GetLastError());
		MYERROR(_T("GetPlazaId tValue == NULL"));
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		return FALSE;
	}

	// ����ѯ���д�뵽����
	LZGRECORDSET rs;
	for (int i=0;i<(int)dwRecordNum;i++)
	{
		rs.clear();
		CString csPlazaId = _T("");
		ZeroMemory(tValue,2048+1);
		memcpy(tValue, pByte, pHead->DataLen);
		
		csPlazaId.Format(_T("%s"),tValue);
		//iSMSCfg.iPlazaId = rs.iRs;
		pByte += pHead->DataLen;

		memcpy(rs.buf, pByte, (pHead+1)->DataLen);
		int iType = rs.iRs;
		pByte += (pHead+1)->DataLen;
		rs.clear();

		memcpy(rs.buf, pByte, (pHead+2)->DataLen);
		int TemplateNum = rs.iRs;
		pByte += (pHead+2)->DataLen;
		rs.clear();

		memcpy(rs.buf, pByte, (pHead+3)->DataLen);
		int IsShield = rs.iRs;
		pByte += (pHead+3)->DataLen;
		rs.clear();

		memcpy(rs.buf, pByte, (pHead+4)->DataLen);
		CString PersonName = _T("");
		PersonName.Format(_T("%s"),rs.cBuf);
		//memcpy(iSMSCfg.PersonName,rs.cBuf,sizeof(TCHAR)*65);
		pByte += (pHead+4)->DataLen;
		rs.clear();

		memcpy(rs.buf, pByte, (pHead+5)->DataLen);
		CString PhoneNum = _T("");
		PhoneNum.Format(_T("%s"),rs.cBuf);
		//memcpy(iSMSCfg.PhoneNum,rs.cBuf,sizeof(TCHAR)*65);
		pByte += (pHead+5)->DataLen;
		rs.clear();
		
		int iPos = csPlazaId.Find(_T(","));
		while(iPos!=-1)
		{
			CString StriPlazaId = csPlazaId.Left(iPos);
			csPlazaId = csPlazaId.Right(csPlazaId.GetLength() - iPos - 1);
			int iPlazaId = _wtoi(StriPlazaId);
			SMSConfigDef iSMSCfg;
			memset(&iSMSCfg,0,sizeof(SMSConfigDef));
			iSMSCfg.iPlazaId = iPlazaId;
			iSMSCfg.IsShield = IsShield;
			memcpy(iSMSCfg.PersonName,PersonName.GetBuffer(),sizeof(TCHAR)*PersonName.GetLength());
			memcpy(iSMSCfg.PhoneNum,PhoneNum.GetBuffer(),sizeof(TCHAR)*PhoneNum.GetLength());
			iSMSCfg.TemplateNum = TemplateNum;
			iSMSCfg.type = iType;
			CString ikey = _T("");
			ikey.Format(_T("%d_%d"),iSMSCfg.iPlazaId,iSMSCfg.type);
			m_SMSConfigMap.SetAt(ikey,iSMSCfg);
			iPos = csPlazaId.Find(_T(","));
		}

		int iPlazaId = _wtoi(csPlazaId);
		SMSConfigDef iSMSCfg;
		memset(&iSMSCfg,0,sizeof(SMSConfigDef));
		iSMSCfg.iPlazaId = iPlazaId;
		iSMSCfg.IsShield = IsShield;
		memcpy(iSMSCfg.PersonName,PersonName.GetBuffer(),sizeof(TCHAR)*PersonName.GetLength());
		memcpy(iSMSCfg.PhoneNum,PhoneNum.GetBuffer(),sizeof(TCHAR)*PhoneNum.GetLength());
		iSMSCfg.TemplateNum = TemplateNum;
		iSMSCfg.type = iType;
		CString ikey = _T("");
		ikey.Format(_T("%d_%d"),iSMSCfg.iPlazaId,iSMSCfg.type);
		m_SMSConfigMap.SetAt(ikey,iSMSCfg);
	}

	delete []tValue;
	tValue = NULL;
	delete[] pRead;
	pRead = NULL;
	CloseHandle(hPipe);

	return TRUE;
}
BOOL CAlarmTask::LoadSMSTemplate()
{
	int iFdCount = 2;	//	��ѯ���ֶ���
	int iMaxRsCount = -1;	// ���Ʋ�ѯ�õ�������¼������-1ָ������


	// �����ݹܵ�����SQL���
	BYTE* pRead = NULL;

	HANDLE hPipe =OpenRealDataPipe();

	if (hPipe == NULL) 
	{
		MYERROR(_T("LoadSMSTemplate hPipe null"));
		return FALSE;
	}

	CString strQuery  = _T("SELECT NUM,TEMPLATE FROM TB_PAYMENT_SMSTEMPLATE; ");
	pRead = (BYTE*)GetMessage_RecordOfSql(hPipe,strQuery);

	if (NULL == pRead)	// δ��ѯ����¼
	{
		CloseHandle(hPipe);
		MYERROR(_T("LoadSMSTemplate pRead null"));
		return FALSE;
	}
	// �������صĽ��
	NetMessageHead *pNetMessageHead = (NetMessageHead*)pRead;
	if (pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB) {
		//	MYWARN(_T("���ر����쳣��%s"),bufToHexString(pRead,pNetMessageHead->Length).c_str());
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("LoadSMSTemplate pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB null"));
		return FALSE;
	}	

	if (pNetMessageHead->Length < 1)	// �ձ��ģ��޲�ѯ���
	{
		//MYWARN(_T("���յ��ջظ����ģ��޷�������ѯ���"));
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("LoadSMSTemplate pNetMessageHead->Length < 1 null"));
		return FALSE;
	}

	// ��ȡ��ѯ���ֶ���
	WORD* pWord = (WORD*)&pRead[sizeof(NetMessageHead)];
	int nFieldNum = (int)*pWord;
	if (nFieldNum != iFdCount)	// ��ѯ�ֶ�����������ȣ�����ѯ����쳣�˳�
	{
		//	MYWARN(_T("��ѯ�ֶ�����������ȣ�����ѯ����쳣�˳�"));
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("LoadSMSTemplate nFieldNum != iFdCount null"));
		return FALSE;
	}
	pWord++;

	// ��¼������
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

	// ��¼��
	DWORD dwRecordNum = *((int*)pByte);
	if ((iMaxRsCount > 0) && (dwRecordNum != iMaxRsCount))
	{
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		return FALSE;
	}
	pByte += sizeof(DWORD);

	// ����ѯ���д�뵽����
	LZGRECORDSET rs;
	for (int i=0;i<(int)dwRecordNum;i++)
	{
		rs.clear();
		int TemplateNum = 0;
		memcpy(rs.buf, pByte, pHead->DataLen);
		TemplateNum = rs.iRs;
		pByte += pHead->DataLen;

		CString Template = _T("");
		memcpy(rs.buf, pByte, (pHead+1)->DataLen);
		Template.Format(_T("%s"),rs.cBuf);
		pByte += (pHead+1)->DataLen;
		rs.clear();

		m_SMSTemplateMap.SetAt(TemplateNum,Template);
	}

	delete[] pRead;
	pRead = NULL;
	CloseHandle(hPipe);

	return TRUE;
}

void CAlarmTask::ProcessData()
{
	CTime CurrTime = CTime::GetCurrentTime();
	CurrTime = CTime(CurrTime.GetYear(),CurrTime.GetMonth(),CurrTime.GetDay(),0,0,0);
	vector<DeviceInfoData>::iterator iterDeviceInfoB = m_vecDeviceInfo.begin();
	vector<DeviceInfoData>::iterator iterDeviceInfoE = m_vecDeviceInfo.end();
	for (; iterDeviceInfoB != iterDeviceInfoE; iterDeviceInfoB++)
	{
		DeviceInfoData& iDeviceInfo = *iterDeviceInfoB;
		DevDayValue iDevValue;
		BOOL bresult = m_DevDayValueMap.Lookup(iDeviceInfo.iDevId,iDevValue);
		if (!bresult)
		{
			ReguTrace(ERR,"δ�ҵ�DevId=%d���˵�",iDeviceInfo.iDevId);
			//continue;
		}

		//�ܺ�ͻ���쳣�ж�
		if(bresult&&iDevValue.DayValue>=1&&iDeviceInfo.Occupied==0)
		{
			if(iDevValue.DayValue>iDevValue.last7Value*m_iMagnifyNum)
			{
				AlarmItemDef ALARMDef;
				memset(&ALARMDef, 0, sizeof(AlarmItemDef));
				CreateAlarmDef(iDeviceInfo,ALARMTYPE_RUSH,ALARMDef);
				CString strAlarmContent = _T("");
				strAlarmContent.Format(_T("�豸%s�����ܺ�ͻ���澯,��ǰֵ%.2f,ƽ��ֵ%.2f��"),
					iDeviceInfo.DevName,iDevValue.DayValue,iDevValue.last7Value);
				memcpy(ALARMDef.alarmContent,strAlarmContent.GetBuffer(),sizeof(ALARMDef.alarmContent));
				AddAlarm2DB(ALARMDef);
			}
		}
		
		//��ͬ���ڻ��ұ�
		if (iDeviceInfo.iBrandId!=0&&iDeviceInfo.iContractId!=0)
		{
			CTime ContractEndLine = CTime(iDeviceInfo.endTime.year,iDeviceInfo.endTime.month,iDeviceInfo.endTime.day,0,0,0);

			if(CurrTime>ContractEndLine)
			{
				AlarmItemDef ALARMDef;
				memset(&ALARMDef, 0, sizeof(AlarmItemDef));
				CreateAlarmDef(iDeviceInfo,ALARMTYPE_CONTRACTEND,ALARMDef);
				CString strAlarmContent = _T("");
				strAlarmContent.Format(_T("�豸%s�������̺�ͬ�ѵ���,������%d-%d-%d 00:00:00��"),
					iDeviceInfo.DevName,iDeviceInfo.endTime.year,iDeviceInfo.endTime.month,iDeviceInfo.endTime.day);
				memcpy(ALARMDef.alarmContent,strAlarmContent.GetBuffer(),sizeof(ALARMDef.alarmContent));
				AddAlarm2DB(ALARMDef);
			}
		}
		

		//����Ʒ���õ���
		if(iDeviceInfo.iBrandId!=0&&bresult)
		{
			vector<int> vecDev;
			m_BrandDevMap.Lookup(iDeviceInfo.iBrandId,vecDev);
			float fBrandConsume = 0;
			vector<int>::iterator iterB = vecDev.begin();
			vector<int>::iterator iterE = vecDev.end();
			for (; iterB != iterE; iterB++)
			{
				int& iDevId = *iterB;
				DevDayValue iDevValue1;
				BOOL bresult1 = m_DevDayValueMap.Lookup(iDevId,iDevValue1);
				if (bresult1)
				{
					fBrandConsume += iDevValue1.DayValue;
				}
			}
			if (fBrandConsume<m_MinValue)
			{
				AlarmItemDef ALARMDef;
				memset(&ALARMDef, 0, sizeof(AlarmItemDef));
				CreateAlarmDef(iDeviceInfo,ALARMTYPE_BRAND_LOW,ALARMDef);
				CString strAlarmContent = _T("");
				strAlarmContent.Format(_T("%s������%s�õ���ƫС,ʵ���õ���%.2f,��׼ֵ%d��"),
					iDeviceInfo.PlazaName,iDeviceInfo.BrandName,fBrandConsume,m_MinValue);
				memcpy(ALARMDef.alarmContent,strAlarmContent.GetBuffer(),sizeof(ALARMDef.alarmContent));
				ALARMDef.alarmObjID = iDeviceInfo.iBrandId;
				ALARMDef.alarmObjType = 35;
				AddAlarm2DB(ALARMDef);
			}
		}

		//�����̹����õ�� ֻ����̹���δռ�õı��õ��
		if(iDeviceInfo.Occupied==0)    //(iDeviceInfo.iBrandId==0&&iDeviceInfo.iContractId==0)
		{
			if (iDevValue.DayValue>m_MaxValue)
			{
				AlarmItemDef ALARMDef;
				memset(&ALARMDef, 0, sizeof(AlarmItemDef));
				CreateAlarmDef(iDeviceInfo,ALARMTYPE_BUSSINESS_HIGH,ALARMDef);
				CString strAlarmContent = _T("");
				strAlarmContent.Format(_T("�豸%sδ��Ʒ���õ���ƫ��,ʵ��ֵ%.2f,��׼ֵ%d��"),
					iDeviceInfo.DevName,iDevValue.DayValue,m_MaxValue);
				memcpy(ALARMDef.alarmContent,strAlarmContent.GetBuffer(),sizeof(ALARMDef.alarmContent));
				AddAlarm2DB(ALARMDef);
			}
		}
	}
}

void CAlarmTask::CreateAlarmDef(DeviceInfoData iDeviceInfo,int AlarmType,AlarmItemDef &ALARMDef)
{
	CreateUuid(ALARMDef.alarmID);
	ALARMDef.alarmType = AlarmType;
	CString strAlarmTypeName = _T("");
	CString strAlarmContent = _T("");
	if (AlarmType==ALARMTYPE_RUSH)  //�ܺ�ͻ���쳣
	{
		strAlarmTypeName = _T("�ܺ�ͻ���澯");
	} 
	else if(AlarmType==ALARMTYPE_CONTRACTNODEV)  //�к�ͬ�ޱ�
	{
		strAlarmTypeName = _T("Ʒ��ȱʧ���");
	}
	else if(AlarmType==ALARMTYPE_CONTRACTEND)   //��ͬ���ڻ��ұ�
	{
		strAlarmTypeName = _T("Ʒ�Ƴ����澯");
	}
	else if(AlarmType==ALARMTYPE_BRAND_LOW)  //����Ʒ���õ���
	{
		strAlarmTypeName = _T("Ʒ���õ�澯");
	}
	else if(AlarmType==ALARMTYPE_BUSSINESS_HIGH)  //�����̹����õ��
	{
		strAlarmTypeName = _T("�̹��õ�澯");
	}
	else
	{
		return;
	}
	CString iKey = _T("");
	iKey.Format(_T("%d_%d"),iDeviceInfo.iPlazaId,AlarmType);
	SMSConfigDef iSMSCofig;
	memset(&iSMSCofig,0,sizeof(SMSConfigDef));
	if(m_SMSConfigMap.Lookup(iKey,iSMSCofig))
	{
		if (iSMSCofig.IsShield==0)
		{
			CString TemplateContent = _T("");
			m_SMSTemplateMap.Lookup(iSMSCofig.TemplateNum,TemplateContent);
			CString Owner = _T("");
			CString AlarmObject = _T("");
			CString strAlarmType = _T("");
			Owner.Format(_T("%s"),iSMSCofig.PersonName);
			AlarmObject.Format(_T("%s%sƷ���µ�%s"),iDeviceInfo.PlazaName,iDeviceInfo.BrandName,iDeviceInfo.DevName);
			strAlarmType.Format(_T("%s"),strAlarmTypeName);
			TemplateContent.Replace(_T("[owner]"),Owner);
			TemplateContent.Replace(_T("[object]"),AlarmObject);
			TemplateContent.Replace(_T("[alarmtype]"),strAlarmType);
			Insert2SMSRECORD(iSMSCofig,TemplateContent);
		}
	}

	ALARMDef.alarmLevel = 10;
	ALARMDef.alarmObjID = iDeviceInfo.iDevId;
	ALARMDef.stationID = iDeviceInfo.iStationId;
	memcpy(ALARMDef.alarmTypeName,strAlarmTypeName.GetBuffer(),sizeof(ALARMDef.alarmTypeName));
	memcpy(ALARMDef.alarmObjName,iDeviceInfo.BrandName,sizeof(ALARMDef.alarmObjName));

	ALARMDef.alarmObjType = 31;
	ALARMDef.devID = iDeviceInfo.iDevId;
	CTime curTime = CTime::GetCurrentTime();
	ALARMDef.contentTime = curTime.GetTime();
	curTime = CTime(curTime.GetYear(),curTime.GetMonth(),curTime.GetDay(),0,0,0);
	ALARMDef.alarmTime = curTime.GetTime();

	ALARMDef.status = 0;

	ALARMDef.projectID = 1;    //���̺�
	ALARMDef.systemID = 1;    //��ϵͳ��
	ALARMDef.rtAlarm = 0;    //����¼ʵʱ�澯
	memset(ALARMDef.stationName, 0, sizeof(ALARMDef.stationName));
	memset(ALARMDef.projectName, 0, sizeof(ALARMDef.projectName));
	//memset(ALARMDef.alarmSource, 0, sizeof(ALARMDef.alarmSource));
	//_tcsncpy_s(ALARMDef.alarmSource, _countof(ALARMDef.alarmSource), C_ALARMRESORCE, C_ALARMRESORCE.GetLength());
	memcpy(ALARMDef.alarmSource,_T("Ӧ�÷���澯"),sizeof(ALARMDef.alarmSource));
	ALARMDef.groupID1 = 1;
	ALARMDef.groupID2 = 2;
	//_tcsncpy_s(ALARMDef.alarmContent, _countof(ALARMDef.alarmContent), cstrWarnMsg, cstrWarnMsg.GetLength());
	//memcpy(ALARMDef.alarmContent,_T("�����澯"),sizeof(ALARMDef.alarmContent));
}

void CAlarmTask::Insert2SMSRECORD(SMSConfigDef iSMSCofig,CString TemplateContent)
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
	ReguTrace(SQL,"���ż�¼���SQL:<%s>",strSqlFinal);
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

void CAlarmTask::BackupData()
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
	COleDateTimeSpan DaysSpan;
	DaysSpan.SetDateTimeSpan(m_iValidDays,0,0,0);

	COleDateTime NowTime = COleDateTime::GetCurrentTime();
	COleDateTime DelTime = NowTime - DaysSpan;
	CString cstrBakSql = _T("");
	//�ȱ���Ҫɾ��������
	cstrBakSql.Format(_T("insert into TE_ALARM_INDEX_BAK select * FROM TE_ALARM_INDEX WHERE ALARMTIME <= '%d-%d-%d'"),DelTime.GetYear(),DelTime.GetMonth(),DelTime.GetDay());
	pRead = (BYTE*)GetMessage_RecordOfSql_Ext(hPipe, cstrBakSql);
	if (pRead)
	{
		delete[] pRead;
		pRead = NULL;
	}
	CString cstrDelSql = _T("");
	cstrDelSql.Format(_T("DELETE FROM TE_ALARM_INDEX WHERE ALARMTIME <= '%d-%d-%d'"),DelTime.GetYear(),DelTime.GetMonth(),DelTime.GetDay());
	pRead = (BYTE*)GetMessage_RecordOfSql_Ext(hPipe, cstrDelSql);
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
void CAlarmTask::AddAlarm2DB(AlarmItemDef ALARMDef)
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
	CString ctrContentTime = _T("");
	ctrContentTime.Format(_T("%d-%d-%d %d:%d:%d"),dt2.GetYear(),
		dt2.GetMonth(),
		dt2.GetDay(),
		dt2.GetHour(),
		dt2.GetMinute(),
		dt2.GetSecond());
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
	strSqlFinal.Format(C_SQL_INSERT_ALARMINDEX,ALARMDef.alarmType,ALARMDef.alarmObjID,ALARMDef.alarmObjType,strSql,strAlarmContent,ctrContentTime,
		ALARMDef.alarmType,ALARMDef.alarmObjID,ALARMDef.alarmObjType);
	ReguTrace(SQL,"�澯��¼���SQL:<%s>",strSqlFinal);
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

	//AddRealAlarm(ALARMDef);
}

void CAlarmTask::UpdateAlarmStatus()
{
	SYSTEMTIME sys_time;
	GetLocalTime(&sys_time);
	CString ctrContentTime = _T("");
	ctrContentTime.Format(_T("%d-%d-%d 0:00:00"),sys_time.wYear,sys_time.wMonth,sys_time.wDay);

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
	CString strSqlFinal = _T("");
	strSqlFinal.Format(_T("UPDATE TE_ALARM_INDEX SET STATUS=2 WHERE GROUP2=2 AND CONTENTTIME<'%s';"),ctrContentTime);
	//ReguTrace(SQL,"�澯��¼���SQL:<%s>",strSqlFinal);
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
BOOL CAlarmTask::LoadDayValue(CString strTime,DataType iDataType)
{
	int iFdCount = 2;	//	��ѯ���ֶ���
	int iMaxRsCount = -1;	// ���Ʋ�ѯ�õ�������¼������-1ָ������

	// �����ݹܵ�����SQL���
	BYTE* pRead = NULL;

	HANDLE hPipe =OpenRealDataPipe();

	if (hPipe == NULL) 
	{
		MYERROR(_T("LoadDayValue hPipe null"));
		return FALSE;
	}

	int tempId = 0;
	while (1)
	{
		CString strQuery;
		if (iDataType==SUMCOUNT)
		{
			strQuery.Format(_T("select top(10000) DEVICEID, SUMCOUNT1 from TE_PAYMENT_BILL_DAY where RECDATE='%s' and DEVICEID>%d order by DEVICEID;"),strTime,tempId);
		} 
		else if(iDataType==MEANELEC)
		{
			strQuery.Format(_T("select top(10000) DEVICEID, MEANELEC from TE_PAYMENT_BILL_DAY where RECDATE='%s' and DEVICEID>%d order by DEVICEID;"),strTime,tempId);
		}
		
		pRead = (BYTE*)GetMessage_RecordOfSql(hPipe,strQuery);

		if (NULL == pRead)	// δ��ѯ����¼
		{
			CloseHandle(hPipe);
			MYERROR(_T("LoadDayValue pRead null"));
			return FALSE;
		}
		// �������صĽ��
		NetMessageHead *pNetMessageHead = (NetMessageHead*)pRead;
		if (pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB) {
			//	MYWARN(_T("���ر����쳣��%s"),bufToHexString(pRead,pNetMessageHead->Length).c_str());
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			MYERROR(_T("LoadDayValue pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB null"));
			return FALSE;
		}	

		if (pNetMessageHead->Length < 1)	// �ձ��ģ��޲�ѯ���
		{
			//MYWARN(_T("���յ��ջظ����ģ��޷�������ѯ���"));
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			MYERROR(_T("LoadDayValue pNetMessageHead->Length < 1 null"));
			return FALSE;
		}

		// ��ȡ��ѯ���ֶ���
		WORD* pWord = (WORD*)&pRead[sizeof(NetMessageHead)];
		int nFieldNum = (int)*pWord;
		if (nFieldNum != iFdCount)	// ��ѯ�ֶ�����������ȣ�����ѯ����쳣�˳�
		{
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			MYERROR(_T("LoadDayValue nFieldNum != iFdCount null"));
			return FALSE;
		}
		pWord++;

		// ��¼������
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

		// ��¼��
		DWORD dwRecordNum = *((int*)pByte);
		if ((iMaxRsCount > 0) && (dwRecordNum != iMaxRsCount))
		{
			delete[] pRead;
			pRead = NULL;
			CloseHandle(hPipe);
			return FALSE;
		}
		pByte += sizeof(DWORD);

		//TCHAR *tValue = new TCHAR[1024+1];
		//if (tValue == NULL)
		//{
		//	//MYERROR(_T("�����ڴ���󣬴����룺%d"),GetLastError());
		//	MYERROR(_T("LoadDayValue tValue == NULL"));
		//	delete[] pRead;
		//	pRead = NULL;
		//	CloseHandle(hPipe);
		//	return FALSE;
		//}

		// ����ѯ���д�뵽����
		LZGRECORDSET rs;
		for (int i=0;i<(int)dwRecordNum;i++)
		{
			DevDayValue iDevDayValue;
			memset(&iDevDayValue,0,sizeof(DevDayValue));
			rs.clear();
			memcpy(rs.buf,pByte, pHead->DataLen);
			int DevId = rs.iRs;
			pByte += pHead->DataLen;

			rs.clear();
			memcpy(rs.buf,pByte, (pHead+1)->DataLen);
			float DayValue = rs.fRs;
			pByte += (pHead+1)->DataLen;

			if (DevId>tempId)
			{
				tempId = DevId;
			}
			if (iDataType==SUMCOUNT)
			{
				iDevDayValue.iDevId = DevId;
				iDevDayValue.DayValue = DayValue;
			}
			else if (iDataType==MEANELEC)
			{
				if (m_DevDayValueMap.Lookup(DevId,iDevDayValue))
				{
					iDevDayValue.last7Value = DayValue;
				}
				else
					continue;
			}
			
			m_DevDayValueMap.SetAt(DevId,iDevDayValue);
		}
		if (dwRecordNum<10000)
		{
			break;
		}
	}
	delete[] pRead;
	pRead = NULL;
	CloseHandle(hPipe);
	return TRUE;
}

int CAlarmTask::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CAlarmTask, CWinThread)
END_MESSAGE_MAP()


// CAlarmTask ��Ϣ�������
