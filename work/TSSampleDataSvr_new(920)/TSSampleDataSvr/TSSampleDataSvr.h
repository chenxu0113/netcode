
// TSSampleDataSvr.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

// CTSSampleDataSvrApp:
// �йش����ʵ�֣������ TSSampleDataSvr.cpp
//

class CTSSampleDataSvrApp : public CWinAppEx
{
public:
	CTSSampleDataSvrApp();

// ��д
public:
	virtual BOOL InitInstance();
	BOOL bExitFlag;
// ʵ��
public:
	DECLARE_MESSAGE_MAP()
};

extern CTSSampleDataSvrApp theApp;