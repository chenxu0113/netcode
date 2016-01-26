
// TSSampleDataSvrDlg.h : 头文件
//

#pragma once
#include "RedisRecvSample.h"
#include "CSendP2pTask.h"
#include "ProcessThread.h"
#include "SaveYmDataThread.h"
#include "UpDateRTDB.h"
#include "TrayIcon.h"

// CTSSampleDataSvrDlg 对话框
class CTSSampleDataSvrDlg : public CDialog
{
// 构造
public:
	CTSSampleDataSvrDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TSSAMPLEDATASVR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	CRedisRecvSample *m_pCRedisRecvSample;
	CCSendP2pTask *m_pCCSendP2pTask;
	CProcessThread *m_pCProcessThread[10];
	CSaveYmDataThread *m_pCSaveYmDataThread;
	CUpDateRTDB *m_pCUpdateRTDBThread;
// 实现
protected:
	LRESULT OnTrayNotification(WPARAM wParam,LPARAM lParam);
	HICON m_hIcon;
	CTrayIcon m_TrayIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//virtual void OnClose();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAbout1();
	afx_msg void OnExit();
	afx_msg void OnShow();
	afx_msg void OnClose();
};
