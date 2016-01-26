// MessageTask.cpp : 实现文件
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
	int iFdCount = 4;	    //查询的字段数 
	int iMaxRsCount = -1;	// 限制查询得到的最大记录个数，-1指不限制


	// 向数据管道发送SQL语句
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
	ReguTrace(SQL,"调用server接口sql=%s!",strQuery);
	if (NULL == pRead)	// 未查询到记录
	{
		CloseHandle(hPipe);
		MYERROR(_T("InitMesInfo pRead null"));
		return FALSE;
	}
	// 解析返回的结果
	NetMessageHead *pNetMessageHead = (NetMessageHead*)pRead;
	if (pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB) {
		//	MYWARN(_T("返回报文异常，%s"),bufToHexString(pRead,pNetMessageHead->Length).c_str());
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("InitMesInfo pNetMessageHead->MessageType != NET_MESSAGE_RETRECORDOFTB null"));
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
	if (nFieldNum != iFdCount)	// 查询字段数量，不相等，查询结果异常退出
	{
		//	MYWARN(_T("查询字段数量，不相等，查询结果异常退出"));
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		MYERROR(_T("InitMesInfo nFieldNum != iFdCount null"));
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
		MYERROR(_T("InitMesInfo tValue == NULL"));
		delete[] pRead;
		pRead = NULL;
		CloseHandle(hPipe);
		return FALSE;
	}

	// 将查询结果写入到缓存

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
         
	   // target：*信息目标接收人的RTXNO或者接收人手机号
		ZeroMemory(tValue,1024+1);
		
		CString CTxStr = _T("");
		iMessageInfo.bCtxFlag=1;
       //const TCHAR* ctxNum=_T("v_zhanglin");
	   //memcpy(iMessageInfo.target_Ctx,ctxNum,sizeof(TCHAR)*10);
		memcpy(tValue, pByte, (pHead+1)->DataLen);
		CTxStr.Format(_T("%s"),tValue);           //存CTX号
		pByte += (pHead+1)->DataLen;
		
		CString PhoneNumStr = _T("");
		iMessageInfo.bCtxFlag=0;
		memcpy(tValue, pByte, (pHead+2)->DataLen);
		PhoneNumStr.Format(_T("%s"),tValue);      //存手机号
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
     
       // msTitle :*发送信息的标题，标题可为""
        const TCHAR* msTitle=_T("智能电表管理平台");
		memcpy(iMessageInfo.msTitle,msTitle,sizeof(TCHAR)*20);

	   //TCHAR msContent[1024];   *发送内容，信息内容不能超过500个字
		ZeroMemory(tValue,1024+1);
		memcpy(tValue, pByte, (pHead+3)->DataLen);
		memcpy(iMessageInfo.msContent,tValue,sizeof(TCHAR)*500);
		pByte += (pHead+3)->DataLen;

        const TCHAR* priority=_T("1");
		memcpy(iMessageInfo.priority,priority,sizeof(TCHAR)*1);
	

        BOOL nResult = SendMessageProcess(iMessageInfo);
		if (nResult)
		{
            MYERROR(_T("短信发送成功"));
			iMessageInfo.bFlag = TRUE;		
		}
		else
		{
			MYERROR(_T("短信发送失败"));
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
	// TODO: 在此执行任意逐线程初始化
	int retimes=0;
	while(1)
	{
		if (!InitMesInfo())
		{

			if (retimes>=3)    //  edit by  list  重试三次后仍报错，退出
			{
				ReguTrace(ERRO,"InitMesInfo ERRO return");
				exit(EXIT_FAILURE);  //无条件退出，看门狗会重启程序
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
	// TODO: 在此执行任意逐线程清理
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
// MessageTask 消息处理程序
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
     //短信
	_ns1__SetSMSInfoC _ns1__SetSMSinfoC;
	_ns1__SetSMSInfoCResponse _ns1__SetSMSinfoCResponse;	
	_ns1__SetSMSinfoC.fromSys=&sFromSys;
	_ns1__SetSMSinfoC.target=&sPhoneTarget;
	_ns1__SetSMSinfoC.msTitle=&sTitle;
	_ns1__SetSMSinfoC.msContent=&smsContent;
	_ns1__SetSMSinfoC.targetTime=& stargetTime;
	_ns1__SetSMSinfoC.priority=&spriority;

   //CTX短信
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
		//发CTX
       soap_call___ns1__SetRTXInfoC(&add_soap2,NULL,NULL,&_ns1__SetRTXinfoC,&_ns1__SetRTXinfoCResponse);
	   std::string sRetRTX;
	   sRetRTX=_ns1__SetRTXinfoCResponse.SetRTXInfoCResult->c_str();
	   soap_end(&add_soap2);
	   soap_done(&add_soap2);
	   //错误码判断，失败要重发
	   if(add_soap2.error)
	   {
		   soap_end(&add_soap2);
		   soap_done(&add_soap2);
		   ReguTrace(Config,"调用CTX短信发送接口失败");
		   return false;
	   }
	   //解析错误码
	   if (strcmp(sRetRTX.c_str(),"OK") == 0)
	   {
		   ReguTrace(Config,"CTX短信发送成功");
		   return TRUE;
	   }
	   else
	   {
		   ReguTrace(Config,"CTX短信发送失败");
		   return FALSE;
	   }      
	}
	if(iMessageInfo.bPhoneFlag ==TRUE && iMessageInfo.bCtxFlag == FALSE)
	{
		//发送短信息
	  soap_call___ns1__SetSMSInfoC(&add_soap2,NULL,NULL,&_ns1__SetSMSinfoC,&_ns1__SetSMSinfoCResponse);
	  std::string sRetSMS;
	  sRetSMS=_ns1__SetSMSinfoCResponse.SetSMSInfoCResult->c_str();
	  soap_end(&add_soap2);
	  soap_done(&add_soap2);

	  //错误码判断，失败要重发
	  if(add_soap2.error)
	  {
		  soap_end(&add_soap2);
		  soap_done(&add_soap2);
		  ReguTrace(Config,"调用短信发送接口失败");
		  return false;
	  }
	  //解析错误码
	  if (strcmp(sRetSMS.c_str(),"OK") == 0)
	  {
		  ReguTrace(Config,"短信发送成功");
		  return TRUE;
	  }
	  else
	  {
		  ReguTrace(Config,"短信发送失败");
		  return FALSE;
	  }
	}
	if (iMessageInfo.bPhoneFlag==TRUE && iMessageInfo.bCtxFlag==TRUE)
	{		
		//发送短信息
		soap_call___ns1__SetSMSInfoC(&add_soap2,NULL,NULL,&_ns1__SetSMSinfoC,&_ns1__SetSMSinfoCResponse);
		//发CTX信息
		soap_call___ns1__SetRTXInfoC(&add_soap2,NULL,NULL,&_ns1__SetRTXinfoC,&_ns1__SetRTXinfoCResponse);
		//错误码判断，失败要重发
		if(add_soap2.error)
		{
			soap_end(&add_soap2);
			soap_done(&add_soap2);
			ReguTrace(Config,"调用短信发送接口失败");
			return false;
		}
		std::string sRetSMS;
		sRetSMS=_ns1__SetSMSinfoCResponse.SetSMSInfoCResult->c_str();

		std::string sRetRTX;
		sRetRTX=_ns1__SetRTXinfoCResponse.SetRTXInfoCResult->c_str();

		soap_end(&add_soap2);
		soap_done(&add_soap2);

		//解析错误码
		if (strcmp(sRetSMS.c_str(),"OK") == 0||strcmp(sRetRTX.c_str(),"OK") == 0)
		{
			ReguTrace(Config,"短信发送成功");
			return TRUE;
		}
		else
		{
			ReguTrace(Config,"短信发送失败");
			return FALSE;
		}
	}
	if (iMessageInfo.bPhoneFlag==FALSE&&iMessageInfo.bCtxFlag==FALSE)
	{
		//记录不存在
        ReguTrace(Config,"数据库中记录不存在");
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
	//将所有发送成功的ID逐个放入IdList列表
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
	//更新数据库中已经发送成功的信息
	strSqlFinal.Format(_T("UPDATE TE_SMSRECORD SET RESULT=1 WHERE id in(%s);"),IdList);
	ReguTrace(SQL,"短信发送确认入库SQL:<%s>",strSqlFinal);
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