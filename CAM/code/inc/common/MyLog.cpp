/**
 *
 * 文 件 名 : MyLog.cpp
 * 创建日期 : 2012-10-03
 * 作    者 : 陆治国
 * 修改日期 : 2013-04-19
 * 当前版本 : $Revision: 1.0 $
 * 功能描述 : 日志记录的信息元素有：
 *						1 模块名
 *						2 日志类型：1:debug/2:warn/3:error/4:fault
 *						3 发生时间
 *						4 源文件名
 *						5 源文件行号
 *						6 日志内容描述
 * 修改记录 :	2014-07-19 陆治国 改成MFC版，生成lib用
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
MyLock g_LogLock;//日志锁
short g_CurrLogLevel;//当前日志的输出级别

// BYTE报文转成十六进制格式的字符串
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
	m_iMaxFileSize = 1024 * 1024;//最大保存1M的日志文件
	init();
}

MyLog::~MyLog()
{
}

MyLog* MyLog::getLog()
{
	return &g_log;
}

// 指定日志级别
void MyLog::setLogLevel(MyLog::LOG_LEVEL l)
{
	getLog()->m_iLogLevel = l;
}

// 指定日志文件的最大尺寸
void MyLog::setLogFileMaxSize(int size)
{
	getLog()->m_iMaxFileSize = size;
}

// 指定程序模块名
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

// 获取当前程序模块名
TCHAR* MyLog::getModuleName()
{
	return (TCHAR*)getLog()->m_cModuleName.c_str();
}


// 输出日志信息到UDP服务端
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

// 输出日志信息到TCP服务端
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
	int err = setsockopt(p->m_hTcpSocket,IPPROTO_TCP,TCP_NODELAY,(char *)&bNodelay,sizeof(bNodelay));//不采用延时算法 
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

// 输出日志信息到指定文件
bool MyLog::startLogToFile(TCHAR* file)	
{
	getLog()->m_cLogFile = file;
	getLog()->m_bToFile=true;
	return true;
}

// 输出日志到控制台
bool MyLog::startLogToConsole()	
{
	getLog()->m_bToConsole = true;
	return true;
}

// 停止日志输出到UDP服务器
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

// 停止日志输出到TCP服务器
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

// 停止日志输出到文件
bool MyLog::stopLogToFile()
{
	getLog()->m_bToFile=false;
	return true;
}

// 停止日志输出到控制台
bool MyLog::stopLogToConsole()
{
	getLog()->m_bToConsole=false;
	return true;
}

// 初始化
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

// 处理日志头
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
	if(str.length()>=sizeof(m_log.filename))	// 防止数组越界
		str = str.substr(0,stringlen(m_log.filename));
	stringcpy(m_log.filename,str.c_str());
	m_log.line = line;
	str = m_cModuleName;
	if(str.length()>=sizeof(m_log.modulename))	// 防止数组越界
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

// 处理日志体
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

	//去除无效的回车换行
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
			stopLogToUDPSvr();//发送失败不再发送到UDP
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
			stopLogToTCPSvr();//发送失败不再发送到TCP
		}
	}
	g_LogLock.unlock();
}

// 将日志记录到文件中
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
			// LUZG 20121128 自动创建日志目录 ↓
			TCHAR path[_MAX_PATH];
            memset(path,0,sizeof(path));
			tgetcwd(path,_MAX_PATH); // 缓存原路径防止递归死循环
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
			makeMultiDirs(dirpath.c_str()); // 创建多级目录
            _chdrive(iDrive);
			tchdir(path);	// 回复原工作路径
			// LUZG 20121128 自动创建日志目录 ↑
		}
		else
			fclose(tryfp);
		FILE *fp = 0;
		fp=FileOpen(pathfile.c_str(),_T("ab+"));
		if(fp == NULL)
			return;
		if (iscreate)
			fwrite("\xff\xfe",1, 2, fp); // 设置Unicode编码文件头
		fseek(fp,0,SEEK_END);
		fwrite(buff,sizeof(TCHAR),stringlen(buff),fp);
		long len = ftell(fp);
		fclose(fp);
		//文件是否超过最大长度?
		if(len > m_iMaxFileSize)
		{
			//将文件复制到另一处
			tstring tmpfile = pathfile+_T(".back");
			FILE *fptmp=FileOpen(tmpfile.c_str(),_T("wb"));
			if(fptmp == NULL)
			{
				outprintf(_T("@@@@@@@@@@ 创建交换文件失败，清除日志文件!"));
				::tremove(pathfile.c_str());
				return;
			}
			fp=FileOpen(pathfile.c_str(),L"rb");
			if(fp == NULL)
			{
				outprintf(_T("@@@@@@@@@@ 再次打开日志文件失败，清除日志文件!"));
				::tremove(pathfile.c_str());
				return;
			}
			TCHAR buf[1024];
			int ret;
			//不减小文件 fseek(fp,(m_iMaxFileSize/10),SEEK_SET);
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
			//文件复制完后，不改名，而是直接删除原文件，这样由下次创建文件，就不会需要经常复制文件了
			//::rename(tmpfile.c_str(),pathfile.c_str());			
		}
	}
}

// 追加方式写入文件行
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