// UpDateRTDB.cpp : 实现文件
//

#include "stdafx.h"
#include "TSSampleDataSvr.h"
#include "UpDateRTDB.h"

extern CLog *g_log;
// CUpDateRTDB

IMPLEMENT_DYNCREATE(CUpDateRTDB, CWinThread)

CUpDateRTDB::CUpDateRTDB()
{
	m_hExitEvent = NULL;
	m_hExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_ProcessDataEvent = CreateEvent(NULL,TRUE, FALSE, NULL);
	InitializeCriticalSection(&m_csYxYmDataList);
	if(NULL==g_log)
	{
		g_log = new CLog(_T("\\TSSampleDataSvr"));
	}
}

CUpDateRTDB::~CUpDateRTDB()
{
}

BOOL CUpDateRTDB::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	while(1)
	{
		HANDLE hEvents[2];
		hEvents[0] = m_hExitEvent; 
		ResetEvent(hEvents[0]);
		hEvents[1] = m_ProcessDataEvent;
		ResetEvent(hEvents[1]);
		DWORD dwWait = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);
		switch (dwWait)
		{
		case WAIT_TIMEOUT:
			break;
		case WAIT_OBJECT_0:	  
			break;
		case WAIT_OBJECT_0+1:   
			ResetEvent(hEvents[1]);
			POSITION pos = NULL;
			HANDLE hPipe=NULL;	 
			hPipe = OpenRealDataPipe();
			if (hPipe==NULL)
				continue;
			CString UpdateSql = _T("");
			int icount = 0;
			while(1)
			{
				EnterCriticalSection(&m_csYxYmDataList);
				pos = m_YxYmDataList.GetHeadPosition();
				if (!pos)
				{
					LeaveCriticalSection(&m_csYxYmDataList);
					if (!UpdateSql.IsEmpty())
					{
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
					break;
				}
				POSITION pos1 = pos;
				BYTE *pBuf = (BYTE*)m_YxYmDataList.GetNext(pos);
				m_YxYmDataList.RemoveAt(pos1);
				LeaveCriticalSection(&m_csYxYmDataList);
				YmYxData *pYmYxData = (YmYxData*)pBuf;
				if (pYmYxData==NULL)
				{
					if (pYmYxData!=NULL)
					{
						delete pYmYxData;
						pYmYxData = NULL;
					}
					continue;
				}
				if (pYmYxData->iType==1)	//Ym
				{
					YmConfigDef *pYmDefData = (YmConfigDef*)pYmYxData->data;
					if (pYmDefData==NULL)
					{
						if (pYmYxData!=NULL)
						{
							delete pYmYxData;
							pYmYxData = NULL;
						}
						continue;
					}
					CString TempSql = _T("");
					TempSql.Format(_T("UPDATE TB_PULSE SET RAWVALUE=%lld,YMVALUE=%f WHERE ID=%d ;"),pYmDefData->nYmRaw,pYmDefData->dYmValue,pYmDefData->nYmIndex);
					UpdateSql += TempSql;
					icount++;
				}
				else if (pYmYxData->iType==2)   //Yx
				{
					YxConfigDef *pYxDefData = (YxConfigDef*)pYmYxData->data;
					if (pYxDefData==NULL)
					{
						if (pYmYxData!=NULL)
						{
							delete pYmYxData;
							pYmYxData = NULL;
						}
						continue;
					}
					CString TempSql = _T("");
					TempSql.Format(_T("UPDATE TB_DI SET RAWVALUE=%d,YXVALUE=%d WHERE ID=%d ;"),pYxDefData->bYxRaw,pYxDefData->bYxValue,pYxDefData->nYxIndex);
					UpdateSql += TempSql;
					icount++;
				}

				if (pYmYxData->data!=NULL)
				{
					delete pYmYxData->data;
					pYmYxData->data = NULL;
				}
				if (pYmYxData!=NULL)
				{
					delete pYmYxData;
					pYmYxData = NULL;
				}

				if (icount%100!=0)
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
		}
	}
	return TRUE;
}

int CUpDateRTDB::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CUpDateRTDB, CWinThread)
END_MESSAGE_MAP()


// CUpDateRTDB 消息处理程序
