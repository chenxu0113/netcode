
// TSAlarmServer_WD.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTSAlarmServer_WDApp:
// �йش����ʵ�֣������ TSAlarmServer_WD.cpp
//

class CTSAlarmServer_WDApp : public CWinAppEx
{
public:
	CTSAlarmServer_WDApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTSAlarmServer_WDApp theApp;