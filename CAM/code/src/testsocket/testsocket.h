
// testsocket.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "SendThread.h"
#include "ClientSocket.h"

// CtestsocketApp:
// �йش����ʵ�֣������ testsocket.cpp
//

class CClientSocket;
class CSendThread;
class CtestsocketApp : public CWinAppEx
{
public:
	CtestsocketApp();
	~CtestsocketApp();

	CClientSocket* m_pWdSocket;
	CSendThread* m_pSendThread;
// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CtestsocketApp theApp;