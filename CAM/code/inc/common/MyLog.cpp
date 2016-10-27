/**
 *
 * �� �� �� : MyLog.cpp
 * �������� : 2012-10-03
 * ��    �� : ½�ι�
 * �޸����� : 2013-04-19
 * ��ǰ�汾 : $Revision: 1.0 $
 * �������� : ��־��¼����ϢԪ���У�
 *						1 ģ����
 *						2 ��־���ͣ�1:debug/2:warn/3:error/4:fault
 *						3 ����ʱ��
 *						4 Դ�ļ���
 *						5 Դ�ļ��к�
 *						6 ��־��������
 * �޸ļ�¼ :	2014-07-19 ½�ι� �ĳ�MFC�棬����lib��
 **/

#include "stdafx.h"
#include "MyLog.h"
#include <sys/timeb.h>
#include "MyLock.h"

#include "CrossMultibytesAndUnicode.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static MyLog g_log;
MyLock g_LogLock;//��־��
short g_CurrLogLevel;//��ǰ��־���������

// BYTE����ת��ʮ�����Ƹ�ʽ���ַ���
std::tstring bufToHexString(unsigned char *pBuffer, int iLen)
{
	std::tstring str=_T("");
	TCHAR buf[4];
	memset(buf,0,4*sizeof(TCHAR));
	bool b=false;
	if(iLen>1000)
	{
		b = true;
		iLen = 1000;
	}
	for(int i=0;i<iLen;i++)
	{
		stringprintf(buf,4*sizeof(TCHAR),_T("%02X "),pBuffer[i]);
		str += buf;
	}
	if(b)
		str += _T(" ...... ");
	return str;
}

MyLog::MyLog()
{
	m_hUdpSocket = NULL;
	m_hTcpSocket = NULL;
	memset(&m_log,0,sizeof(stuLog));
	m_iLogLevel = LOG_ERROR;
	m_iMaxFileSize = 1024 * 1024;//��󱣴�1M����־�ļ�
	init();
}

MyLog::~MyLog()
{
}

MyLog* MyLog::getLog()
{
	return &g_log;
}

// ָ����־����
void MyLog::setLogLevel(MyLog::LOG_LEVEL l)
{
	getLog()->m_iLogLevel = l;
}

// ָ����־�ļ������ߴ�
void MyLog::setLogFileMaxSize(int size)
{
	getLog()->m_iMaxFileSize = size;
}

// ָ������ģ����
void MyLog::setModuleName(TCHAR* name)
{
	getLog()->m_cModuleName = name;
	int p=getLog()->m_cModuleName.find_last_of(_T('\\'),getLog()->m_cModuleName.length());
	if(p>=0)
		getLog()->m_cModuleName = getLog()->m_cModuleName.substr(p+1,getLog()->m_cModuleName.length()-p-1);
	p=getLog()->m_cModuleName.find_last_of(_T('/'),getLog()->m_cModuleName.length());
	if(p>=0)
		getLog()->m_cModuleName = getLog()->m_cModuleName.substr(p+1,getLog()->m_cModuleName.length()-p-1);
	if(getLog()->m_cModuleName.length()>4 && getLog()->m_cModuleName.substr(getLog()->m_cModuleName.length()-4,4) == _T(".exe"))
		getLog()->m_cModuleName = getLog()->m_cModuleName.substr(0,getLog()->m_cModuleName.length()-4);
}

// ��ȡ��ǰ����ģ����
TCHAR* MyLog::getModuleName()
{
	return (TCHAR*)getLog()->m_cModuleName.c_str();
}


// �����־��Ϣ��UDP�����
bool MyLog::startLogToUDPSvr(TCHAR* ip,int port)
{
	MyLog *p = getLog();
	if(p == NULL)
		return false;

	WSADATA WSAData;
	WSAStartup( MAKEWORD( 1, 1 ), &WSAData );
	p->m_hUdpSocket = ::socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(p->m_hUdpSocket == INVALID_SOCKET)
	{
		p->m_hUdpSocket = NULL;
		return false;
	}
	memset(&p->m_udp_dest_sin, 0 ,sizeof(p->m_udp_dest_sin));


	char *m_ip = (char *)malloc(16);
	if(m_ip == NULL)
	{
		return false;
	}
	memset(m_ip,0,16);

	WideCharToMultiByte (CP_OEMCP,NULL,ip,-1,(LPSTR)m_ip,16,NULL,FALSE);

	p->m_udp_dest_sin.sin_family		= AF_INET;
	p->m_udp_dest_sin.sin_addr.s_addr	= inet_addr(m_ip);
	p->m_udp_dest_sin.sin_port			= htons(port); 
	p->m_bToLogUDPSvr=true;

	free(m_ip);
	return true;
}

// �����־��Ϣ��TCP�����
bool MyLog::startLogToTCPSvr(TCHAR* ip,int port)
{
	MyLog *p = getLog();
	if(p == NULL)
		return false;
	WSADATA WSAData;
	WSAStartup( MAKEWORD( 1, 1 ), &WSAData );
	p->m_hTcpSocket = ::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	outprintf(_T("log tcp af=%d,type=%d,protocol=%d,handle=%X"),AF_INET,SOCK_STREAM,IPPROTO_TCP,p->m_hTcpSocket);
	if(p->m_hTcpSocket == INVALID_SOCKET)
	{
		p->m_hTcpSocket = NULL;
		return false;
	}
	int bNodelay = 1;  
	int err = setsockopt(p->m_hTcpSocket,IPPROTO_TCP,TCP_NODELAY,(char *)&bNodelay,sizeof(bNodelay));//��������ʱ�㷨 
	if ( err == SOCKET_ERROR ) 
	{
		stopLogToTCPSvr();
		return false;       
	} 		
	int ret;
	sockaddr_in dest_sin;
	memset(&dest_sin, 0 ,sizeof(dest_sin));

	char *m_ip = (char *)malloc(16);
	if(m_ip == NULL)
	{
		return false;
	}
	memset(m_ip,0,16);

	WideCharToMultiByte (CP_OEMCP,NULL,ip,-1,(LPSTR)m_ip,16,NULL,FALSE);

	dest_sin.sin_family 		= AF_INET;
	dest_sin.sin_addr.s_addr = inet_addr(m_ip);
	dest_sin.sin_port 			= htons(port); 
	ret = ::connect(p->m_hTcpSocket, (sockaddr *)&dest_sin, sizeof(dest_sin));
	if( ret < 0)
	{
		stopLogToTCPSvr();
		return false;
	}

	p->m_bToLogTCPSvr=true;
	return true;
}

// �����־��Ϣ��ָ���ļ�
bool MyLog::startLogToFile(TCHAR* file)	
{
	getLog()->m_cLogFile = file;
	getLog()->m_bToFile=true;
	return true;
}

// �����־������̨
bool MyLog::startLogToConsole()	
{
	getLog()->m_bToConsole = true;
	return true;
}

// ֹͣ��־�����UDP������
bool MyLog::stopLogToUDPSvr()
{
	MyLog *p = getLog();
	if(p->m_hUdpSocket != NULL)
	{
		shutdown(p->m_hUdpSocket, 2);
		closesocket(p->m_hUdpSocket);
	}
	p->m_bToLogUDPSvr=false;
	return true;
}

// ֹͣ��־�����TCP������
bool MyLog::stopLogToTCPSvr()
{
	MyLog *p = getLog();
	if(p->m_hTcpSocket != NULL)
	{
		shutdown(p->m_hTcpSocket, 2);
		closesocket(p->m_hTcpSocket);
	}
	p->m_bToLogTCPSvr=false;
	return true;
}

// ֹͣ��־������ļ�
bool MyLog::stopLogToFile()
{
	getLog()->m_bToFile=false;
	return true;
}

// ֹͣ��־���������̨
bool MyLog::stopLogToConsole()
{
	getLog()->m_bToConsole=false;
	return true;
}

// ��ʼ��
void MyLog::init()
{
	m_bToConsole = true;
	m_bToFile	 = false;
	m_bToLogUDPSvr  = false;
	m_bToLogTCPSvr  = false;
	TCHAR buf[1205];
	memset(buf,0,1205*sizeof(TCHAR));
	GetModuleFileName(NULL,(TCHAR *)buf,1024*sizeof(TCHAR));
	setModuleName(buf);
}

// ������־ͷ
void MyLog::logHead(short logtype,const TCHAR* file,int line,const TCHAR* func)
{
	g_LogLock.lock();
	if(logtype<m_iLogLevel)
	{
		m_log.logtype = 100;
		return;
	}
	tstring str;
	m_log.threadid = (long)GetCurrentThreadId();
	m_log.logtype = (char)logtype;
	str = file;
	int p;
	p=str.find_last_of(_T('\\'),str.length());
	if(p>=0)
	{
		p++;
		str = str.substr(p,str.length()-p);
	}
	p=str.find_last_of(_T('/'),str.length());
	if(p>=0)
	{
		p++;
		str = str.substr(p,str.length()-p);
	}
	if(str.length()>=sizeof(m_log.filename))	// ��ֹ����Խ��
		str = str.substr(0,stringlen(m_log.filename));
	stringcpy(m_log.filename,str.c_str());
	m_log.line = line;
	str = m_cModuleName;
	if(str.length()>=sizeof(m_log.modulename))	// ��ֹ����Խ��
		str = str.substr(0,stringlen(m_log.modulename));
	stringcpy(m_log.modulename,str.c_str());
	

	//MSW 150323 MOD
	// 	struct  tm *t;
	// 	time_t now;
	// 	time(&now);
	// 	t = localtime(&now);
	struct timeb timebuffer;
	struct tm *t;
	unsigned short millitm1 = 0;

	ftime(&timebuffer);
	t = localtime(&timebuffer.time);
	millitm1 = timebuffer.millitm;
	//MSW 150323 MOD END ////////////////////////

	m_log.datetime.year = t->tm_year;
	m_log.datetime.month = t->tm_mon;
	m_log.datetime.day = t->tm_mday;
	m_log.datetime.hour = t->tm_hour;
	m_log.datetime.minute = t->tm_min;
	m_log.datetime.second = t->tm_sec;

	g_CurrLogLevel = logtype;
	if(m_bToConsole||m_bToFile)
	{
		tstring type;
		switch(logtype)
		{
		case LOG_DEBUG:
			type=_T("debug");
			break;
		case LOG_WARN:
			type=_T("warn ");
			break;
		case LOG_ERROR:
			type=_T("error");
			break;
		}
		TCHAR buf[1024];
		memset(buf,0,1024 * sizeof(TCHAR));
		stringprintf(buf,1024*sizeof(TCHAR),_T("%04d-%02d-%02d %02d:%02d:%02d_%03d[%ld] %s at %s(%s line:%d):\t"),
            t->tm_year+1900,t->tm_mon+1,t->tm_mday,
			t->tm_hour,t->tm_min,t->tm_sec,millitm1, m_log.threadid, type.c_str(),
			m_log.filename,func,m_log.line);
		if(m_bToConsole)
			outprintf(_T("{%s}%s"),m_log.modulename,buf);
		if(m_bToFile)
			logFile(buf);
	}
}

// ������־��
void MyLog::logBody(const TCHAR * cformat, ...)
{
	if(m_log.logtype == 100)
	{
		g_LogLock.unlock();
		return;
	}
	memset(m_log.msg,0,sizeof(m_log.msg));
	va_list ap;
	va_start(ap, cformat);
	if(m_bToConsole)
		tvprintf(cformat,ap);
	vswprintf(m_log.msg,sizeof(m_log.msg),cformat,ap);
	va_end(ap);
	if(m_bToConsole && (stringlen(cformat) > 0) && (cformat[stringlen(cformat)-1] != _T('\n')))
		outprintf(_T("\n"));
	m_log.msg[stringlen(m_log.msg)] = _T('\0');

	//ȥ����Ч�Ļس�����
	while(
		(stringlen(m_log.msg)>0) && 
		((m_log.msg[stringlen(m_log.msg)-1] == _T('\r')) || (m_log.msg[stringlen(m_log.msg)-1] == _T('\n')))
		)
		m_log.msg[stringlen(m_log.msg)-1] = _T('\0');

	if(m_bToFile)
	{
		logFile(m_log.msg);
		if((stringlen(cformat) > 0) && (cformat[stringlen(cformat)-1] != _T('\n')))
			logFile(_T("\r\n"));
	}
	if(m_bToLogUDPSvr)
	{
		m_log.size = (sizeof(m_log) - sizeof(m_log.msg))+stringlen(m_log.msg)*sizeof(TCHAR);
		m_log.size = htonl(m_log.size);
		int ret = sendto(m_hUdpSocket, (char *)&m_log, m_log.size, 0, (sockaddr*)&m_udp_dest_sin, sizeof(m_udp_dest_sin));
		if(ret != sizeof(stuLog))
		{
			stopLogToUDPSvr();//����ʧ�ܲ��ٷ��͵�UDP
		}
	}
	if(m_bToLogTCPSvr)
	{
		m_log.size = (sizeof(m_log) - sizeof(m_log.msg))+stringlen(m_log.msg)*sizeof(TCHAR);
		int size = m_log.size;
		m_log.size = htonl(size);

		int ret=::send(m_hTcpSocket, (char *)&m_log, size, 0);
		if(ret != (size))
		{
			stopLogToTCPSvr();//����ʧ�ܲ��ٷ��͵�TCP
		}
	}
	g_LogLock.unlock();
}

// ����־��¼���ļ���
void MyLog::logFile(TCHAR *buff)
{
	bool iscreate=false;
	for(int i=0;i<=1;i++)
	{
		tstring pathfile = m_cLogFile;
		if(i==0)
		{
			tstring type;
			switch(g_CurrLogLevel)
			{
			case LOG_DEBUG:
				type=_T(".debug");
				break;
			case LOG_WARN:
				type=_T(".warn ");
				break;
			case LOG_ERROR:
				type=_T(".error");
				break;
			}
			pathfile += type;
		}

		FILE *tryfp = 0;
		tryfp=FileOpen(pathfile.c_str(),_T("r"));
		if(tryfp == NULL)
		{
			iscreate=true;
			// LUZG 20121128 �Զ�������־Ŀ¼ ��
			TCHAR path[_MAX_PATH];
            memset(path,0,sizeof(path));
			tgetcwd(path,_MAX_PATH); // ����ԭ·����ֹ�ݹ���ѭ��
            int iDrive = _getdrive();
			tstring str = m_cLogFile;
			int p;
			p=str.find_last_of(_T('\\'),str.length());
			if(p>=0)
			{
				p++;
				str = str.substr(p,str.length()-p);
			}
			p=str.find_last_of(_T('/'),str.length());
			if(p>=0)
			{
				p++;
				str = str.substr(p,str.length()-p);
			}
			tstring dirpath = m_cLogFile;
			dirpath = dirpath.substr(0,dirpath.length()-str.length());
			makeMultiDirs(dirpath.c_str()); // �����༶Ŀ¼
            _chdrive(iDrive);
			tchdir(path);	// �ظ�ԭ����·��
			// LUZG 20121128 �Զ�������־Ŀ¼ ��
		}
		else
			fclose(tryfp);
		FILE *fp = 0;
		fp=FileOpen(pathfile.c_str(),_T("ab+"));
		if(fp == NULL)
			return;
		if (iscreate)
			fwrite("\xff\xfe",1, 2, fp); // ����Unicode�����ļ�ͷ
		fseek(fp,0,SEEK_END);
		fwrite(buff,sizeof(TCHAR),stringlen(buff),fp);
		long len = ftell(fp);
		fclose(fp);
		//�ļ��Ƿ񳬹���󳤶�?
		if(len > m_iMaxFileSize)
		{
			//���ļ����Ƶ���һ��
			tstring tmpfile = pathfile+_T(".back");
			FILE *fptmp=FileOpen(tmpfile.c_str(),_T("wb"));
			if(fptmp == NULL)
			{
				outprintf(_T("@@@@@@@@@@ ���������ļ�ʧ�ܣ������־�ļ�!"));
				::tremove(pathfile.c_str());
				return;
			}
			fp=FileOpen(pathfile.c_str(),L"rb");
			if(fp == NULL)
			{
				outprintf(_T("@@@@@@@@@@ �ٴδ���־�ļ�ʧ�ܣ������־�ļ�!"));
				::tremove(pathfile.c_str());
				return;
			}
			TCHAR buf[1024];
			int ret;
			//����С�ļ� fseek(fp,(m_iMaxFileSize/10),SEEK_SET);
			while(1)
			{
				ret = fread(buf,1,1024,fp);
				if(ret <= 0)
					break;
				fwrite(buf,1,ret,fptmp);
			}
			fclose(fp);
			fclose(fptmp);
			::tremove(pathfile.c_str());
			//�ļ�������󣬲�����������ֱ��ɾ��ԭ�ļ����������´δ����ļ����Ͳ�����Ҫ���������ļ���
			//::rename(tmpfile.c_str(),pathfile.c_str());			
		}
	}
}

// ׷�ӷ�ʽд���ļ���
void MyLog::writeFileLine(TCHAR * filename,TCHAR *buff)
{
	FILE *fp = 0;
	fp=FileOpen(filename,_T("ab+"));
	if(fp == NULL)
		return;
	fseek(fp,0,SEEK_END);
	fwrite(buff,sizeof(TCHAR),stringlen(buff),fp);
	fclose(fp);
}

void MyLog::makeMultiDirs(const TCHAR *_Path)
{
	tstring str;
	tstring rightstr;
	tstring searchString(_T("/"));
	tstring replaceString(_T("\\"));
	str.assign(_Path);
	tstring::size_type pos = 0;
	while ( (pos = str.find(searchString, pos)) != tstring::npos ) {
		str.replace( pos, searchString.size(), replaceString );
		pos++;
	}
    int n = str.find_first_of(_T(':'));
    if (n>0)
    {
        tstring sdrive = str.substr(0,n);
        if ((sdrive.compare(_T("C"))==0) || (sdrive.compare(_T("c"))==0))
        {
            _chdrive(3);
        }
        else if ((sdrive.compare(_T("D"))==0) || (sdrive.compare(_T("d"))==0))
        {
            _chdrive(4);
        }
        else if ((sdrive.compare(_T("E"))==0) || (sdrive.compare(_T("e"))==0))
        {
            _chdrive(5);
        }
        else if ((sdrive.compare(_T("F"))==0) || (sdrive.compare(_T("f"))==0))
        {
            _chdrive(6);
        }
        else if ((sdrive.compare(_T("G"))==0) || (sdrive.compare(_T("g"))==0))
        {
            _chdrive(7);
        }
    }
	int p = str.find_first_of(_T('\\'));
	if (p>=0)
	{
		p++;
		rightstr = str.substr(p,str.length()-p);
		str = str.substr(0,p-1);
        tstring tmpstr = str.substr(1,str.length());
        if (tmpstr.compare(_T(":"))==0)
        {
            tchdir(_T("\\"));
        }
		tmkdir(str.c_str());
		int ifanhui = tchdir(str.c_str());
		if (rightstr.empty())
		{
			return;
		}
		makeMultiDirs(rightstr.c_str());
	}
	return /*_wmkdir(_Path)*/;
}