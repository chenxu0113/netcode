/**
 *
 * 文 件 名 : MyLog.h
 * 创建日期 : 2012-10-03
 * 作    者 : 陆治国
 * 修改日期 : 2013-04-19
 * 当前版本 : $Revision: 1.0 $
 * 功能描述 : 日志记录的信息元素有：
 *						1 模块名
 *						2 日志类型：1:debug/2:warn/3:error
 *						3 发生时间
 *						4 源文件名
 *						5 源文件行号
 *						6 日志内容描述
 * 修改记录 :	2014-07-19 陆治国 改成MFC版，生成lib用
 **/

#ifndef MyLogGER_H_
#define MyLogGER_H_

#pragma warning(disable:4996)
#pragma pack(4) 

#define WIN32_LEAN_AND_MEAN 
#include <winsock2.h> 
#define socklen_t int
#pragma comment(lib,"ws2_32.lib")
#include <direct.h>


#include <stdio.h> 
#include <stdlib.h> 
#include <string>
#include <stdarg.h>
#include <time.h>

using namespace std;


#ifdef _UNICODE
#define tstring wstring
#define outprintf(x,...) wprintf(x,__VA_ARGS__)
#define tchdir _wchdir
#define tmkdir _wmkdir
#define tremove _wremove
#define tgetcwd _wgetcwd
#define tvprintf(x,...) vwprintf(x,__VA_ARGS__)
#else
#define tstring string
#define outprintf(x,...) printf(x,__VA_ARGS__)
#define tchdir _chdir
#define tmkdir _mkdir
#define tremove remove
#define tgetcwd _getcwd
#define tvprintf(x,...) vprintf(x,__VA_ARGS__)
#endif


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef _MAX_PATH
#define _MAX_PATH 260
#endif

// define
#ifndef __FUNCTION__
#define __FUNCTION__ ""
#endif


#ifdef UNICODE
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)
#define __WFUNCTION__ WIDEN(__FUNCTION__)

#define ____FILE__ __WFILE__
#define ____FUNCTION__ __WFUNCTION__
#else
#define ____FILE__ __FILE__
#define ____FUNCTION__ __FUNCTION__
#endif


// 调试信息输出
#ifdef _MY_WITHOUT_DEBUG
	#define MYDEBUG 
#else
	#define MYDEBUG MyLog::getLog()->logHead(1,____FILE__,__LINE__,____FUNCTION__);MyLog::getLog()->logBody
#endif

// 警告信息输出
#ifdef _MY_WITHOUT_WARN
	#define MYWARN  
#else
	#define MYWARN  MyLog::getLog()->logHead(2,____FILE__,__LINE__,____FUNCTION__);MyLog::getLog()->logBody
#endif

// 错误信息输出
#ifdef _MY_WITHOUT_ERROR
	#define MYERROR 
#else
	#define MYERROR MyLog::getLog()->logHead(3,____FILE__,__LINE__,____FUNCTION__);MyLog::getLog()->logBody
#endif

//////////////////////////////////////////////////////////////////////////

#ifndef SOCKET 
#define SOCKET int
#endif

#ifndef NULL 
#define NULL 0
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET -1
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR -1
#endif

#ifndef BYTE
#define BYTE unsigned char
#endif

#define MAX_PACKAGE_SIZE 1020
// 转成十六进制字符串
std::tstring bufToHexString(unsigned char *pBuffer, int iLen);
inline std::tstring bufToHexString(void *pBuffer, int iLen)
{
	return bufToHexString((unsigned char *)pBuffer, iLen);
}

//////////////////////////////////////////////////////////////////////////

//#pragma pack(1)
class MyLog  
{
public:
	// 定义日志级别
	enum LOG_LEVEL
	{
		LOG_DEBUG=1,
		LOG_WARN=2,
		LOG_ERROR=3,
		LOG_NONE=0
	};

	// 存放日志时间的结构体
	struct stuDateTime
	{
		TCHAR year;
		TCHAR month;
		TCHAR day;
		TCHAR hour;
		TCHAR minute;
		TCHAR second;
	};


	// 存放日志命令的结构体
	struct stuLog
	{
		int	size;
		long	threadid;
		TCHAR	logtype;
		stuDateTime		datetime;
		TCHAR	modulename[20];
		TCHAR	filename[40];
		int		line;
		TCHAR	msg[10240];//attr:m=xxx;f=xxx;t=xxx;
	};

	// 存放日志详细信息的结构体
	struct stuLogDetail
	{
	public:
		stuLogDetail()
		{
			pNext = NULL;
		}
		tstring	clientip;
		tstring	modulename;
		long	threadid;
		tstring	datetime;
		short	logtype;
		tstring	filename;
		tstring	field1;
		tstring	field2;
		tstring	field3;
		tstring	field4;
		tstring	field5;
		int		line;
		tstring	msg;
		stuLogDetail *pNext;
	};

	MyLog();
	virtual ~MyLog();
	static MyLog* getLog();

//protected:
	void logHead(short logtype,const TCHAR* file,int line,const TCHAR* func);

	void logBody(const TCHAR * cformat, ...);

	inline void logFile(TCHAR *buff);

	static void writeFileLine(TCHAR * filename,TCHAR *buff);

	static void setLogLevel(LOG_LEVEL l);

	static void setLogFileMaxSize(int size);

	static void setModuleName(TCHAR* name);

	static TCHAR* getModuleName();

	static bool startLogToUDPSvr(TCHAR* ip,int port=7001);

	static bool startLogToTCPSvr(TCHAR* ip,int port=7001);

	static bool startLogToFile(TCHAR* file);

	static bool startLogToConsole();
	static bool stopLogToUDPSvr();
	static bool stopLogToTCPSvr();
	static bool stopLogToFile();
	static bool stopLogToConsole();

protected:

	void init();

	void makeMultiDirs(const TCHAR *_Path);

private:
	SOCKET m_hTcpSocket;
	SOCKET m_hUdpSocket;
	sockaddr_in m_dest_sin;//LOG服务器的地址

	tstring m_cModuleName;//模块名

	tstring m_cLogFile;//LOG文件名

	bool m_bToConsole;//是否输出到控制台
	bool m_bToFile;//是否输出到文件
	bool m_bToLogUDPSvr;//是否输出到UDP服务器
	bool m_bToLogTCPSvr;//是否输出到TCP服务器

	sockaddr_in m_udp_dest_sin;

	int m_iLogLevel;//日志级别
	long m_iMaxFileSize;//最大的日志文件大小
	stuLog m_log;
};
//#pragma pack()

#endif  // !MyLogGER_H_
