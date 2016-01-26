
// TSAlarmServer_WD.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CTSAlarmServer_WDApp:
// 有关此类的实现，请参阅 TSAlarmServer_WD.cpp
//

class CTSAlarmServer_WDApp : public CWinAppEx
{
public:
	CTSAlarmServer_WDApp();

// 重写
	public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CTSAlarmServer_WDApp theApp;