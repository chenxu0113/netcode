#include "stdafx.h"
#include "Log.h"

#include <shlwapi.h>

#define LOG_FILE_MAX_SIZE (50*1024*1024)    //50M

class CAutoLock
{
public:
    CAutoLock(CRITICAL_SECTION* pCritical)
    {
        ASSERT(NULL != pCritical);

        m_pCritical = pCritical;

        ::EnterCriticalSection(m_pCritical);
    }

    virtual ~CAutoLock()
    {
        ::LeaveCriticalSection(m_pCritical);
    }

protected:
    CRITICAL_SECTION *m_pCritical;
};

CLog::CLog()
{

}

CLog::CLog(LPCTSTR strFile)
{
	::InitializeCriticalSection(&m_cs);

	CString sPath, sPath1(_T("\\log"));

	GetModuleFileName(NULL, sPath.GetBuffer(MAX_PATH), MAX_PATH);
	sPath.ReleaseBuffer();
	sPath = sPath.Left(sPath.ReverseFind('\\'));

	if (!PathFileExists(sPath + sPath1))
	{
		if (!CreateDirectory(sPath + sPath1, NULL))
		{
            ASSERT(FALSE); //创建目录失败
			return;
		}
	}

	m_sPath.Format(_T("%s%s%s_"), sPath, sPath1, strFile);	

    m_nLogFileTime = 0;
}

BOOL CLog::InitLog(LPCTSTR strFile)
{
	::InitializeCriticalSection(&m_cs);
	CTime tCur = CTime::GetCurrentTime();
	CString sPath = _T(""), sPath1 = _T("");

	sPath1 = _T("\\log");

	GetModuleFileName(NULL, sPath.GetBuffer(MAX_PATH), MAX_PATH);
	sPath.ReleaseBuffer();
	sPath = sPath.Left(sPath.ReverseFind('\\'));

	if (!PathFileExists(sPath+sPath1))
	{
		if (!CreateDirectory(sPath+sPath1, NULL))
		{
			return FALSE;
		}
	}

	m_sPath.Format(_T("%s%s\\%s_HeartBeat_"), sPath, sPath1, strFile);
// 	sPath.Format(_T("%s%d-%02d-%02d.log"), m_sPath, tCur.GetYear(), tCur.GetMonth(), tCur.GetDay());
// 
// 	if(m_fLog.Open(sPath, CFile::modeCreate|CFile::shareDenyNone | CFile::modeNoTruncate | CFile::modeReadWrite))
// 	{
// 		m_fLog.SeekToEnd();
// 	}

	return TRUE;
}

CLog::~CLog(void)
{
	if (m_fLog.m_hFile != CFile::hFileNull)
    {
		m_fLog.Close();
    }

	::DeleteCriticalSection(&m_cs);
}

void CLog::Log(LPCTSTR strLog)
{
    if (NULL == strLog)
    {
        return;
    }

//	CAutoLock auto_lock(&m_cs);
	EnterCriticalSection(&m_cs);//MSW 160628 ADD,上面的CAutoLock经常会导致指针被覆盖而崩溃

    SYSTEMTIME st;
    GetLocalTime(&st);

    CTime tmNow(st);
    unsigned int nNowLogFileTime = tmNow.GetYear() * 10000 + tmNow.GetMonth() * 100 + tmNow.GetDay();

    if (m_fLog.m_hFile == CFile::hFileNull)
    {
        //文件还没有打开
        CString strLogFileFullPath;
        if (!GetCurentIndex(tmNow, strLogFileFullPath))
        {
            m_nCurrentIndex = 0;
            strLogFileFullPath = GetLogFileFullPath(tmNow, m_nCurrentIndex);
        }

        if (!m_fLog.Open(strLogFileFullPath, CFile::modeCreate|CFile::shareDenyNone | CFile::modeNoTruncate | CFile::modeReadWrite))
        {
            ASSERT(FALSE); 
			LeaveCriticalSection(&m_cs);//MSW 160628 ADD,上面的CAutoLock经常会导致指针被覆盖而崩溃
            return;            
        }   

        m_nLogFileTime = nNowLogFileTime;
    }

    if (nNowLogFileTime != m_nLogFileTime)
    {
        m_fLog.Flush();
        m_fLog.Close();        

        m_nCurrentIndex = 0;    
        CString strLogFileFullPath = GetLogFileFullPath(tmNow, m_nCurrentIndex);

        if (!m_fLog.Open(strLogFileFullPath, CFile::modeCreate|CFile::shareDenyNone | CFile::modeNoTruncate | CFile::modeReadWrite))
        {
            ASSERT(FALSE);
			LeaveCriticalSection(&m_cs);//MSW 160628 ADD,上面的CAutoLock经常会导致指针被覆盖而崩溃
            return;
        }

        m_nLogFileTime = nNowLogFileTime;
        m_fLog.SeekToEnd();
    }

    ULONGLONG ullLogFileSize = m_fLog.SeekToEnd();
    if (ullLogFileSize > LOG_FILE_MAX_SIZE)
    {        
        m_fLog.Flush();
        m_fLog.Close();        

        m_nCurrentIndex++;    
        CString strLogFileFullPath = GetLogFileFullPath(tmNow, m_nCurrentIndex);

        if (!m_fLog.Open(strLogFileFullPath, CFile::modeCreate|CFile::shareDenyNone | CFile::modeNoTruncate | CFile::modeReadWrite))
        {
            ASSERT(FALSE);
			LeaveCriticalSection(&m_cs);//MSW 160628 ADD,上面的CAutoLock经常会导致指针被覆盖而崩溃
            return;
        }

        m_nLogFileTime = m_nLogFileTime;
        m_fLog.SeekToEnd();
    }

	CString strTxt;	
	strTxt.Format(_T("%04d-%02d-%02d %02d:%02d:%02d:%03d\t%s\r\n"), 
					st.wYear, st.wMonth, st.wDay,
					st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, 
					strLog);

	int nLen = (strTxt.GetLength() * sizeof(TCHAR)) + 1;
	char* pLog = new(std::nothrow) char[nLen];
	if (NULL == pLog)
	{		
		LeaveCriticalSection(&m_cs);//MSW 160628 ADD,上面的CAutoLock经常会导致指针被覆盖而崩溃
		return;
	}
	memset(pLog, 0, nLen);

#ifdef _UNICODE
	USES_CONVERSION;
	strcpy(pLog, W2A(strTxt));
#else
	strcpy(pLog, strTxt);
#endif

	m_fLog.Write(pLog, strlen(pLog));
	delete[] pLog;

	LeaveCriticalSection(&m_cs);//MSW 160628 ADD,上面的CAutoLock经常会导致指针被覆盖而崩溃
}

BOOL CLog::GetCurentIndex(const CTime &tm, CString &strFileFullPath)
{
    CFileFind finder;

    m_nCurrentIndex = -1;
    strFileFullPath = _T("");

    CString strTime;
    strTime.Format(_T("%04d%02d%02d_"), tm.GetYear(), tm.GetMonth(), tm.GetDay());
    for (int i=999; i>=0; i--)
    {
        CString strTemp;
        strTemp.Format(_T("%03d.log"), i);
        CString strWildcard = m_sPath + strTime + strTemp;
        if (finder.FindFile(strWildcard))
        {
            CString strFileName = strWildcard;

            strFileName.Replace(m_sPath, _T(""));
            strFileName.Replace(strTime, _T(""));
            strFileName.Replace(_T(".log"), _T(""));

            if (strFileName.GetLength() > 0)
            {
                m_nCurrentIndex = i;
                strFileFullPath = strWildcard;
                break;
            }
        }
    }   
    finder.Close();

    if (m_nCurrentIndex == -1)
    {
        return FALSE;
    }

    return TRUE;
}

CString CLog::GetLogFileFullPath(const CTime &tm, int nIndex)
{
    CString strTime;
    strTime.Format(_T("%04d%02d%02d_"), tm.GetYear(), tm.GetMonth(), tm.GetDay());

    CString strTemp;
    strTemp.Format(_T("%03d"), nIndex);

    return m_sPath + strTime + strTemp + _T(".log");
}
