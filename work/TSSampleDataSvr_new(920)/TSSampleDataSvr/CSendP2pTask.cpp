// CSendP2pTask.cpp : ʵ���ļ�
//
#pragma once
#include "stdafx.h"
#include "TSSampleDataSvr.h"
#include "CSendP2pTask.h"
#include "TSSampleDataSvr.h"
#include "TSSampleDataSvrDlg.h"

extern CLog *g_log ;
// CCSendP2pTask
#pragma pack(1)
typedef struct YmUnit_{
	WORD DevId;
	WORD Id;
	DWORD  Value;
	BYTE flag;
}YmUnit;

typedef struct MmiYmFrame_ {
	NetMessageHead head;
	SocketMsgHead mmi_head;
	BYTE srcCode;
	BYTE DstCode;
	BYTE btRtuNo;
	WORD wdYxNum;
	WORD wdYcNum;
	WORD wdYmNum;

	BYTE 	wYear;
	BYTE 	bMonth;
	BYTE 	bDay;
	BYTE 	bHour;
	BYTE 	bMinite;
	BYTE	bSecond;
	//YmUnit YmUnit1[1];
	SocketGroupYmUnitEx YmUnitEx[1];
}MmiYmFrame;

#pragma pack()

IMPLEMENT_DYNCREATE(CCSendP2pTask, CWinThread)

CCSendP2pTask::CCSendP2pTask()
{
	m_SoftBus = new CRedisBus;
	m_ProcessDataEvent = NULL;
	m_hExitEvent = NULL;
	m_hExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_ProcessDataEvent = CreateEvent(NULL,TRUE, FALSE, NULL);
	m_AlarmEvent = CreateEvent(NULL,TRUE, FALSE, NULL);
	InitializeCriticalSection(&m_csRecordList);
	InitializeCriticalSection(&m_csAlarmInfoList);
	if(g_log==NULL)
	{
		g_log = new CLog(_T("\\TSSampleDataSvr"));
	}
}

CCSendP2pTask::~CCSendP2pTask()
{
	
}

BOOL CCSendP2pTask::InitInstance()
{
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	redisContext *con = NULL;
	int i = 0;

	if(!ConnectRedisServer())
	{
		return FALSE;
	}

	while(1)
	{
		/*WORD wDevNo = 0;
		int nDevId = 0;
		if (i%3==0)
		{
			wDevNo = 1;
			nDevId = 13;
		}
		else if (i%3==1)
		{
			wDevNo = 4;
			nDevId = 24;
		}
		else if (i%3==2)
		{
			wDevNo = 5;
			nDevId = 25;
		}*/
		 CTime ttCurTime = CTime::GetCurrentTime();
		 ProtocolCallItem iItem;
		 memset(&iItem,0,sizeof(ProtocolCallItem));
		 ////******************���ֵ��¼����for test************************
		 //iItem.m_nType = 0xE1;
		 //iItem.m_nDeviceNo = 21;   //�豸���һ�� Id
		 //iItem.m_nStationNum = 3;

		 //SocketMsgPrepayRecord iPrepayRecord ;
		 //memset(&iPrepayRecord,0,sizeof(SocketMsgPrepayRecord));
		 //iPrepayRecord.btRtuNo = 3;   //==m_nStationNum
		 //iPrepayRecord.btEndNo = 0xff;
		 //iPrepayRecord.btForeNo = 0x20;
		 //iPrepayRecord.nDevNo = 2;  //�豸��ڶ��� deviceNo
		 //iPrepayRecord.wdYjNum = 2;
		 //memcpy(iItem.m_nBuffer,&iPrepayRecord,sizeof(SocketMsgPrepayRecord));
		 ////******************���ֵ��¼����END************************

		 ////******************����б���for test************************
		 //iItem.m_nType = 0xB3;
		 //iItem.m_nDeviceNo = nDevId;  //24 25
		 //iItem.m_nStationNum = 2;

		 //SocketMsgHuRecord iHuPrepayRecord ;
		 //memset(&iHuPrepayRecord,0,sizeof(SocketMsgHuRecord));
		 //iHuPrepayRecord.btRtuNo = 3;
		 //iHuPrepayRecord.btEndNo = 0xff;
		 //iHuPrepayRecord.btForeNo = 0x20;
		 //iHuPrepayRecord.nDevNo = wDevNo;   //4  5
		 //iHuPrepayRecord.btAFN = 0x0C;
		 //iHuPrepayRecord.btFn = 167;
		 //iHuPrepayRecord.dwProcessId = 1001;
		 //iHuPrepayRecord.dwUseId = 11;
		 //memcpy(iItem.m_nBuffer,&iHuPrepayRecord,sizeof(SocketMsgHuRecord));
		 ////******************����б���END************************

		 //**************************�г�ֵ��¼*********************************
		 HANDLE hEvents[3];
		 hEvents[0] = m_hExitEvent; 
		 ResetEvent(hEvents[0]);
		 hEvents[1] = m_ProcessDataEvent;
		 ResetEvent(hEvents[1]);
		 hEvents[2] = m_AlarmEvent;
		 ResetEvent(hEvents[2]);
		 DWORD dwWait = WaitForMultipleObjects(3, hEvents, FALSE, INFINITE);
		 switch (dwWait)
		 {
		 case WAIT_TIMEOUT:
			 break;
		 case WAIT_OBJECT_0:	  
			 break;
		 case WAIT_OBJECT_0+1:   
			 ResetEvent(hEvents[1]);
				{
					EnterCriticalSection(&m_csRecordList);
					POSITION pos = NULL;
					pos = m_RecordList.GetHeadPosition();
					ReguTrace(Config,"��ʼ�����г�ֵ��¼����");
					while (pos) 
					{
						ProtocolCallItem * pItem = NULL;
						POSITION pos1 = pos;
						pItem = (ProtocolCallItem*)m_RecordList.GetNext(pos);
						m_RecordList.RemoveAt(pos1);
						if (pItem)
						{
							int Totallength=sizeof(ProtocolCallItem);
							unsigned char * buffer = (unsigned char *)pItem;
							ReguTrace(Config,"��ʼ�����г�ֵ��¼����:%s",bufToHexString(buffer,Totallength).c_str());
							if(m_SoftBus->SendMessageToMsmq("DownMessage",buffer,Totallength)== REDIS_ERR)
							{
								LeaveCriticalSection(&m_csRecordList);//MSW 150828 ADD,��ֹredis����ʧ�ܣ����¶�������
								m_SoftBus->UnRegisterSoftBus();
								ConnectRedisServer();
								EnterCriticalSection(&m_csRecordList);//MSW 150828 ADD,��ֹredis����ʧ�ܣ����¶�������
							}

							delete pItem;
							pItem = NULL;
						}
						
					}

					LeaveCriticalSection(&m_csRecordList);

				}
				break;
		 case WAIT_OBJECT_0+2:
			 ResetEvent(hEvents[2]);
			{
				EnterCriticalSection(&m_csAlarmInfoList);
				POSITION pos = NULL;
				pos = m_AlarmInfoList.GetHeadPosition();
				ReguTrace(Config,"��ʼ���͸澯��Ϣ����");
				while (pos) 
				{
					AlarmInfo * pAlarmInfo = NULL;
					POSITION pos1 = pos;
					pAlarmInfo = (AlarmInfo*)m_AlarmInfoList.GetNext(pos);
					if (pAlarmInfo)
					{
						int Totallength=sizeof(AlarmInfo) + sizeof(AlarmMsgHead);
						AlarmMsgHead iAlarmHeadMsg;
						iAlarmHeadMsg.dAlarmType = pAlarmInfo->AlarmType;
						iAlarmHeadMsg.MsgBegin = 0x7e;
						iAlarmHeadMsg.MsgEnd = 0x7e;
						unsigned char * buffer = new unsigned char[Totallength];
						memcpy(buffer,&iAlarmHeadMsg,sizeof(AlarmMsgHead));
						memcpy(buffer+sizeof(AlarmMsgHead),pAlarmInfo,sizeof(AlarmInfo));
						
						ReguTrace(Config,"��ʼ���͸澯��Ϣ����:�澯����:%d,������վ:%d,״̬:%d",pAlarmInfo->ObjectId,pAlarmInfo->reserve,pAlarmInfo->Status);
						if(m_SoftBus->SendMessageToMsmq("AlarmInfoData",buffer,Totallength)== REDIS_ERR)
						{
							LeaveCriticalSection(&m_csAlarmInfoList);//MSW 150828 ADD,��ֹredis����ʧ�ܣ����¶�������
							m_SoftBus->UnRegisterSoftBus();
							ConnectRedisServer();
							EnterCriticalSection(&m_csAlarmInfoList);//MSW 150828 ADD,��ֹredis����ʧ�ܣ����¶�������
						}

						delete pAlarmInfo;
						pAlarmInfo = NULL;
						if (buffer!=NULL)
						{
							delete [] buffer;
							buffer = NULL;
						}
					}
					m_AlarmInfoList.RemoveAt(pos1);
				}

				LeaveCriticalSection(&m_csAlarmInfoList);

			}
			break;
		 default:
			 break;
		 }
		 //**************************�г�ֵ��¼END*********************************
		/* int Totallength=sizeof(ProtocolCallItem);
		 unsigned char * buffer = (unsigned char *)&iItem;


		if(m_SoftBus->SendMessageToMsmq("DownMessage",buffer,Totallength)== REDIS_ERR)
		{
			m_SoftBus->UnRegisterSoftBus();
			ConnectRedisServer();
		}

		i++;
		Sleep(30000);*/
	}
	
	return TRUE;
}


int CCSendP2pTask::GetTestBuffer(unsigned char ** buffer)
{
	ProtocolCallItem iItem;
	memset(&iItem,0,sizeof(ProtocolCallItem));
	iItem.m_nType = 0xB3;
	iItem.m_nDeviceNo = 2;
	iItem.m_nStationNum = 1;
	
	SocketMsgPrepayRecord iPrepayRecord ;
	memset(&iPrepayRecord,0,sizeof(SocketMsgPrepayRecord));
	iPrepayRecord.btRtuNo = 2;
	iPrepayRecord.btEndNo = 0xff;
	iPrepayRecord.btForeNo = 0x20;
	iPrepayRecord.nDevNo = 1;
	iPrepayRecord.wdYjNum = 2;
	memcpy(iItem.m_nBuffer,&iPrepayRecord,sizeof(SocketMsgPrepayRecord));

	int Totallength=sizeof(ProtocolCallItem);

	SocketMsgGroupYmDataEx_DATE* pSocketMsgYmData = new SocketMsgGroupYmDataEx_DATE;
	CTime ttCurTime = CTime::GetCurrentTime();
	pSocketMsgYmData->btForeNo=1;  //Դ������
	pSocketMsgYmData->btEndNo=0xff;   //Ŀ�ػ�����
	pSocketMsgYmData->btRtuNo=1;   //RTU��
	pSocketMsgYmData->wdYxNum=0; //YX ���� 
	pSocketMsgYmData->wdYcNum=0;   //YC ����
	pSocketMsgYmData->wdYmNum=1;   //YM ����
	pSocketMsgYmData->bYear=15;
	pSocketMsgYmData->bMonth=ttCurTime.GetMonth();
	pSocketMsgYmData->bDay=ttCurTime.GetDay();
	pSocketMsgYmData->bHour=ttCurTime.GetHour();
	pSocketMsgYmData->bMinite=ttCurTime.GetMinute();
	pSocketMsgYmData->bSecond = 0;
	pSocketMsgYmData->YmUnitEx[0].bFlag = 1;
	pSocketMsgYmData->YmUnitEx[0].sgy.DevId = 709;
	pSocketMsgYmData->YmUnitEx[0].sgy.Id = 15;
	pSocketMsgYmData->YmUnitEx[0].sgy.Value= 123456;

	SocketGroupYmUnitEx *pYmData = pSocketMsgYmData->YmUnitEx;
	int ByteOffset = 0;
	int i = 0;
	/*����Totallength ʱ���ټ���һ��YmUnit����ΪMmiYmFrame�в��ܶ���YmUnit YmUnit[0];*/
	int si = sizeof(MmiYmFrame);
	int sa = sizeof(SocketGroupYmUnitEx);
	int sb = sizeof(unsigned char);
	int sc = sizeof(SocketGroupYmUnit);


	
	unsigned char* SndBuf = new unsigned char[Totallength];

	memset(SndBuf, 0, Totallength);

	MmiYmFrame *pyds = (MmiYmFrame*)SndBuf;

	pyds->head.MessageType = NET_MESSAGE_RTDB;
	int nn = sizeof(NetMessageHead);
	pyds->head.Length = Totallength-sizeof(NetMessageHead);
	pyds->mmi_head.wdLen = pyds->head.Length - sizeof(SocketMsgHead);
	pyds->mmi_head.dwSyncCode = 0xeb90eb90;

	pyds->mmi_head.btType = SOCKET_MSG_GROUP_YMDATA_EX_DATE;
	pyds->srcCode = pSocketMsgYmData->btForeNo;
	pyds->DstCode = 0xff;
	pyds->btRtuNo = pSocketMsgYmData->btRtuNo;
	pyds->wdYmNum = pSocketMsgYmData->wdYmNum;
	pyds->wYear = pSocketMsgYmData->bYear;
	pyds->bMonth = pSocketMsgYmData->bMonth;
	pyds->bDay = pSocketMsgYmData->bDay;
	pyds->bHour = pSocketMsgYmData->bHour;
	pyds->bMinite = pSocketMsgYmData->bMinite;
	pyds->bSecond = pSocketMsgYmData->bSecond;

	ByteOffset += (sizeof(MmiYmFrame)-sizeof(SocketGroupYmUnitEx));
	for (i=0; i<pSocketMsgYmData->wdYmNum; i++, pYmData++)
	{
		SocketGroupYmUnitEx *pyu = ((SocketGroupYmUnitEx*)&SndBuf[ByteOffset]);
		pyu->sgy.DevId = pYmData->sgy.DevId;
		pyu->sgy.Id = pYmData->sgy.Id;
		pyu->sgy.Value = pYmData->sgy.Value;
		pyu->bFlag = pYmData->bFlag;
		ByteOffset+=sizeof(SocketGroupYmUnitEx);
	}
	printf("build data(len=%d):",Totallength);
	for( i=0;i<Totallength;i++){
		printf("%s%02x",(i%4!=0)?" ":((i%16==0)?"\n":" "),SndBuf[i]);
	}
	printf("\n");
	*buffer = (unsigned char *)SndBuf;
	delete pSocketMsgYmData;
	return Totallength;
}


BOOL CCSendP2pTask::ConnectRedisServer()
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
		if (m_SoftBus->RegisterSoftBus(szIP, pDlg->m_pCRedisRecvSample->RedisPort) == REDIS_OK)
		{
			break;
		}
		else
		{
			m_SoftBus->UnRegisterSoftBus();
			m_SoftBus->RegisterSoftBus(szIP, pDlg->m_pCRedisRecvSample->RedisPort);
			Sleep(3000);
		}
	}

	return TRUE;
}

int CCSendP2pTask::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CCSendP2pTask, CWinThread)
END_MESSAGE_MAP()


// CCSendP2pTask ��Ϣ�������
