// MessageTask.cpp : ʵ���ļ�
//

#include "stdafx.h"
//#include "soapStub.h"
#include "BasicHttpBinding_USCOREIWandaMsg.nsmap"
#include "soapH.h"
#include "TSAlarmServer_WD.h"
#include "MessageTask.h"


// MessageTask
extern CLog *g_log ;
IMPLEMENT_DYNCREATE(CMessageTask, CWinThread)

CMessageTask::CMessageTask()
{
	if(NULL==g_log)
	{
		g_log = new CLog(_T("\\TSAlarmServer"));
	}
}

CMessageTask::~CMessageTask()
{
}

BOOL CMessageTask::InitMesInfo()
{
	m_vecMesInfo.clear();
	//m_MesInfoMap.RemoveAll();
	int iFdCount = 4;	    //��ѯ���ֶ��� 
	int iMaxRsCount = -1;	// ���Ʋ�ѯ�õ�������¼������-1ָ������


	// �����ݹܵ�����SQL���
	BYTE* pRead = NULL;

	HANDLE hPipe =OpenRealDataPipe();

	if (hPipe == NULL) 
	{
		MYERROR(_T("InitMesInfo hPipe null"));
		return FALSE;
	}


	CString strQuery = _T("");
	strQuery.Format(C_SQL_QUERY_MESINFO);
	pRead = (BYTE*)GetMessage_RecordOfSql(hPipe,strQuery);
	ReguTrace(SQL,"����server�ӿ�sql=%s!",strQuery);
	if (NULL == pRead)	// δ��ѯ����¼
	{
		CloseHandle(hPipe);
		MYERROR(_T("InitMesInfo pRead null"));
		return FALSE;
	}
	// �������صĽ��
	NetMessageHead *pNetMessageHead = (NetMessageHead*)pRead;
	if (pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB) {
		//	MYWARN(_T("���ر����쳣��%s"),bufToHexString(pRead,pNetMessageHead->Length).c_str());
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("InitMesInfo pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB null"));
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
	if (nFieldNum != iFdCount)	// ��ѯ�ֶ�����������ȣ���ѯ����쳣�˳�
	{
		//	MYWARN(_T("��ѯ�ֶ�����������ȣ���ѯ����쳣�˳�"));
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("InitMesInfo nFieldNum != iFdCount null"));
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
		MYERROR(_T("InitMesInfo tValue == NULL"));
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		return FALSE;
	}

	// ����ѯ���д�뵽����

	//LZGRECORDSET rs;
	for (int i=0;i<(int)dwRecordNum;i++)
	{
		MesInfoData iMessageInfo;
		memset(&iMessageInfo,0,sizeof(MesInfoData));
		
        const TCHAR* sysCode=_T("zndbglpt");
		memcpy(iMessageInfo.fromSys,sysCode,sizeof(TCHAR)*8);
         
		ZeroMemory(tValue,1024+1);
		memcpy(tValue, pByte, (pHead)->DataLen);
		iMessageInfo.Dataid = (int)*((DWORD *)tValue);
		pByte += pHead->DataLen;
         
	   // target��*��ϢĿ������˵�RTXNO���߽������ֻ���
		ZeroMemory(tValue,1024+1);
		
		CString CTxStr = _T("");
		iMessageInfo.bCtxFlag=1;
       //const TCHAR* ctxNum=_T("v_zhanglin");
	   //memcpy(iMessageInfo.target_Ctx,ctxNum,sizeof(TCHAR)*10);
		memcpy(tValue, pByte, (pHead+1)->DataLen);
		CTxStr.Format(_T("%s"),tValue);           //��CTX��
		pByte += (pHead+1)->DataLen;
		
		CString PhoneNumStr = _T("");
		iMessageInfo.bCtxFlag=0;
		memcpy(tValue, pByte, (pHead+2)->DataLen);
		PhoneNumStr.Format(_T("%s"),tValue);      //���ֻ���
        pByte += (pHead+2)->DataLen;
		
		if (!CTxStr.IsEmpty())
		{
			iMessageInfo.bCtxFlag = TRUE;
			memcpy(iMessageInfo.target_Ctx,CTxStr,CTxStr.GetLength()*sizeof(TCHAR));
		}

		if (!PhoneNumStr.IsEmpty())
		{
			iMessageInfo.bPhoneFlag = TRUE;
			memcpy(iMessageInfo.target_Phone,PhoneNumStr.GetBuffer(),PhoneNumStr.GetLength()*sizeof(TCHAR));
		}
     
       // msTitle :*������Ϣ�ı��⣬�����Ϊ""
        const TCHAR* msTitle=_T("���ܵ�����ƽ̨");
		memcpy(iMessageInfo.msTitle,msTitle,sizeof(TCHAR)*20);

	   //TCHAR msContent[1024];   *�������ݣ���Ϣ���ݲ��ܳ���500����
		ZeroMemory(tValue,1024+1);
		memcpy(tValue, pByte, (pHead+3)->DataLen);
		memcpy(iMessageInfo.msContent,tValue,sizeof(TCHAR)*500);
		pByte += (pHead+3)->DataLen;

        const TCHAR* priority=_T("1");
		memcpy(iMessageInfo.priority,priority,sizeof(TCHAR)*1);
	

        BOOL nResult = SendMessageProcess(iMessageInfo);
		if (nResult)
		{
            MYERROR(_T("���ŷ��ͳɹ�"));
			iMessageInfo.bFlag = TRUE;		
		}
		else
		{
			MYERROR(_T("���ŷ���ʧ��"));
			iMessageInfo.bFlag = FALSE;		
		}     
		m_vecMesInfo.push_back(iMessageInfo);
	}
	delete []tValue;
	tValue = NULL;

	delete[] pRead;
	pRead = NULL;
	CloseHandle(hPipe);
	return TRUE;
}

BOOL CMessageTask::InitInstance()
{
	// TODO: �ڴ�ִ���������̳߳�ʼ��
	int retimes=0;
	while(1)
	{
		if (!InitMesInfo())
		{

			if (retimes>=3)    //  edit by  list  �������κ��Ա����˳�
			{
				ReguTrace(ERRO,"InitMesInfo ERRO return");
				exit(EXIT_FAILURE);  //�������˳������Ź�����������
				//return FALSE;
			}
			retimes++;
			Sleep(1000);
		}	
		if(m_vecMesInfo.size()>0)  
		{
			UpdateMsToDB();
		}
		Sleep(20000);  

	}	
	return TRUE;


	//while(1)
	//{
	//	if (!InitMesInfo())
	//	{
	//		ReguTrace(ERRO,"InitMesInfo ERRO return!");
	//		return FALSE;
	//	}	
	//	if(m_vecMesInfo.size()>0)  //GUOJ MOD 151114
	//	{
	//		UpdateMsToDB();
	//	}
	//	Sleep(20000);  //GUOJ ADD 151114
	//	 
	//}	
	//return TRUE;
}

int CMessageTask::ExitInstance()
{
	// TODO: �ڴ�ִ���������߳�����
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CMessageTask, CWinThread)
END_MESSAGE_MAP()

BOOL CMessageTask::TChar2Char(const TCHAR* pTchar, char* pChar)
{
	int iASCIILen = 0;
	iASCIILen = WideCharToMultiByte(CP_ACP, 0, pTchar, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, pTchar, -1, pChar, iASCIILen, NULL, NULL);
	return TRUE;
}
// MessageTask ��Ϣ�������
BOOL CMessageTask::SendMessageProcess(MesInfoData& iMessageInfo)
{	
	struct soap add_soap2;
	char szContent[2048];
	memset(szContent,0,2048);
	char* pTmp = szContent;
	TChar2Char(iMessageInfo.fromSys,pTmp);
	std::string sFromSys(pTmp);
	TChar2Char(iMessageInfo.target_Ctx,pTmp);
	std::string sCtxTarget(pTmp);
	TChar2Char(iMessageInfo.target_Phone,pTmp);
	std::string sPhoneTarget(pTmp);
	TChar2Char(iMessageInfo.msTitle,pTmp);
	std::string sTitle(pTmp);
	TChar2Char(iMessageInfo.msContent,pTmp);
	std::string smsContent(pTmp);
	TChar2Char(iMessageInfo.targetTime,pTmp);
	std::string stargetTime(pTmp);
	TChar2Char(iMessageInfo.priority,pTmp);
	std::string spriority(pTmp);
     //����
	_ns1__SetSMSInfoC _ns1__SetSMSinfoC;
	_ns1__SetSMSInfoCResponse _ns1__SetSMSinfoCResponse;	
	_ns1__SetSMSinfoC.fromSys=&sFromSys;
	_ns1__SetSMSinfoC.target=&sPhoneTarget;
	_ns1__SetSMSinfoC.msTitle=&sTitle;
	_ns1__SetSMSinfoC.msContent=&smsContent;
	_ns1__SetSMSinfoC.targetTime=& stargetTime;
	_ns1__SetSMSinfoC.priority=&spriority;

   //CTX����
	_ns1__SetRTXInfoC _ns1__SetRTXinfoC;
	_ns1__SetRTXInfoCResponse _ns1__SetRTXinfoCResponse;
	_ns1__SetRTXinfoC.fromSys=&sFromSys;
	_ns1__SetRTXinfoC.target=&sCtxTarget;
	_ns1__SetRTXinfoC.msTitle=&sTitle;
	_ns1__SetRTXinfoC.msContent=&smsContent;
	_ns1__SetRTXinfoC.targetTime=& stargetTime;
	_ns1__SetRTXinfoC.priority=&spriority;

	soap_init(&add_soap2);
	soap_set_mode(&add_soap2,SOAP_C_MBSTRING);
	add_soap2.mode|=SOAP_C_MBSTRING;
    
	if(iMessageInfo.bCtxFlag==TRUE && iMessageInfo.bPhoneFlag==FALSE)
	{
		//��CTX
       soap_call___ns1__SetRTXInfoC(&add_soap2,NULL,NULL,&_ns1__SetRTXinfoC,&_ns1__SetRTXinfoCResponse);
	   std::string sRetRTX;
	   sRetRTX=_ns1__SetRTXinfoCResponse.SetRTXInfoCResult->c_str();
	   soap_end(&add_soap2);
	   soap_done(&add_soap2);
	   //�������жϣ�ʧ��Ҫ�ط�
	   if(add_soap2.error)
	   {
		   soap_end(&add_soap2);
		   soap_done(&add_soap2);
		   ReguTrace(Config,"����CTX���ŷ��ͽӿ�ʧ��");
		   return false;
	   }
	   //����������
	   if (strcmp(sRetRTX.c_str(),"OK") == 0)
	   {
		   ReguTrace(Config,"CTX���ŷ��ͳɹ�");
		   return TRUE;
	   }
	   else
	   {
		   ReguTrace(Config,"CTX���ŷ���ʧ��");
		   return FALSE;
	   }      
	}
	if(iMessageInfo.bPhoneFlag ==TRUE && iMessageInfo.bCtxFlag == FALSE)
	{
		//���Ͷ���Ϣ
	  soap_call___ns1__SetSMSInfoC(&add_soap2,NULL,NULL,&_ns1__SetSMSinfoC,&_ns1__SetSMSinfoCResponse);
	  std::string sRetSMS;
	  sRetSMS=_ns1__SetSMSinfoCResponse.SetSMSInfoCResult->c_str();
	  soap_end(&add_soap2);
	  soap_done(&add_soap2);

	  //�������жϣ�ʧ��Ҫ�ط�
	  if(add_soap2.error)
	  {
		  soap_end(&add_soap2);
		  soap_done(&add_soap2);
		  ReguTrace(Config,"���ö��ŷ��ͽӿ�ʧ��");
		  return false;
	  }
	  //����������
	  if (strcmp(sRetSMS.c_str(),"OK") == 0)
	  {
		  ReguTrace(Config,"���ŷ��ͳɹ�");
		  return TRUE;
	  }
	  else
	  {
		  ReguTrace(Config,"���ŷ���ʧ��");
		  return FALSE;
	  }
	}
	if (iMessageInfo.bPhoneFlag==TRUE && iMessageInfo.bCtxFlag==TRUE)
	{		
		//���Ͷ���Ϣ
		soap_call___ns1__SetSMSInfoC(&add_soap2,NULL,NULL,&_ns1__SetSMSinfoC,&_ns1__SetSMSinfoCResponse);
		//��CTX��Ϣ
		soap_call___ns1__SetRTXInfoC(&add_soap2,NULL,NULL,&_ns1__SetRTXinfoC,&_ns1__SetRTXinfoCResponse);
		//�������жϣ�ʧ��Ҫ�ط�
		if(add_soap2.error)
		{
			soap_end(&add_soap2);
			soap_done(&add_soap2);
			ReguTrace(Config,"���ö��ŷ��ͽӿ�ʧ��");
			return false;
		}
		std::string sRetSMS;
		sRetSMS=_ns1__SetSMSinfoCResponse.SetSMSInfoCResult->c_str();

		std::string sRetRTX;
		sRetRTX=_ns1__SetRTXinfoCResponse.SetRTXInfoCResult->c_str();

		soap_end(&add_soap2);
		soap_done(&add_soap2);

		//����������
		if (strcmp(sRetSMS.c_str(),"OK") == 0||strcmp(sRetRTX.c_str(),"OK") == 0)
		{
			ReguTrace(Config,"���ŷ��ͳɹ�");
			return TRUE;
		}
		else
		{
			ReguTrace(Config,"���ŷ���ʧ��");
			return FALSE;
		}
	}
	if (iMessageInfo.bPhoneFlag==FALSE&&iMessageInfo.bCtxFlag==FALSE)
	{
		//��¼������
        ReguTrace(Config,"���ݿ��м�¼������");
		return FALSE;
	}	
	return TRUE;
}
void CMessageTask::UpdateMsToDB()
{
	//HANDLE hPipe = OpenServerPipe();
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
	//�����з��ͳɹ���ID�������IdList�б�
	CString IdList = _T("");
	vector<MesInfoData>::iterator iterMesInfoB = m_vecMesInfo.begin();
	vector<MesInfoData>::iterator iterMesInfoE = m_vecMesInfo.end();
	for (; iterMesInfoB != iterMesInfoE; iterMesInfoB++)
	{
		MesInfoData& iMsgInfo = *iterMesInfoB;
		if (iMsgInfo.bFlag==TRUE)
		{
			CString strId = _T("");
			strId.Format(_T("%d,"),iMsgInfo.Dataid);
			IdList += strId;
		}
	}
	IdList = IdList.Left(IdList.GetLength() - 1);
	if (IdList.IsEmpty())	
	{
		if (hPipe != NULL)
		{
			CloseHandle(hPipe);
			hPipe = NULL;
		}
		return;
	}
	BYTE* pRead = NULL;
	//*****************************
	CString strSqlFinal = _T("");
	//�������ݿ����Ѿ����ͳɹ�����Ϣ
	strSqlFinal.Format(_T("UPDATE TE_SMSRECORD SET RESULT=1 WHERE id in(%s);"),IdList);
	ReguTrace(SQL,"���ŷ���ȷ�����SQL:<%s>",strSqlFinal);
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