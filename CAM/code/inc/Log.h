#pragma once

class CLog
{
public:
	CLog(LPCTSTR strFile);
	CLog();
	virtual ~CLog(void);

	void Log(LPCTSTR strLog);
	BOOL InitLog(LPCTSTR strFile);

private:
    BOOL GetCurentIndex(const CTime &tm, CString &strFileFullPath);
    CString GetLogFileFullPath(const CTime &tm, int nIndex);

	CRITICAL_SECTION m_cs;

	CFile m_fLog;
	CString m_sPath;
    CString m_sModuleFilePath;

    int m_nCurrentIndex;
    unsigned int m_nLogFileTime;
};
